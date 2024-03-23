#include <iostream>
#include <Windows.h>
#include <unordered_set>
#include <vector>
#include <thread>
#include <conio.h>
#include <cstdlib>
#include "all.h"

// 清空用户输入的回车
void clear_input()
{
	while (getchar() != 10);
}

void print_key(bool mode)
{
	if (mode)
	{
		printf("（默认模式）当前快捷键：\n\n");
		printf("1：窗口置顶：\t\t\t");
		for (int k : keys[0])
			printf("%s ", key_map.at(k).c_str());

		printf("\n2：取消置顶：\t\t\t");
		for (int k : keys[1])
			printf("%s ", key_map.at(k).c_str());

		printf("\n3：增大透明度：\t\t\t");
		for (int k : keys[2])
			printf("%s ", key_map.at(k).c_str());

		printf("\n4：减小透明度：\t\t\t");
		for (int k : keys[3])
			printf("%s ", key_map.at(k).c_str());

		printf("\n5：从列表中增加/删除窗口：\t");
		for (int k : keys[4])
			printf("%s ", key_map.at(k).c_str());

		printf("\n6：隐藏列表中所有窗口：\t\t");
		for (int k : keys[5])
			printf("%s ", key_map.at(k).c_str());
	}
	else
	{
		printf("（组管理模式）当前快捷键：\n\n");
		int n = group_keys.size();
		printf("1：编辑组：\t\t\t");
		for (int k : group_keys[0])
			printf("%s ", key_map.at(k).c_str());
		printf("\n2：删除组：\t\t\t");
		for (int k : group_keys[1])
			printf("%s ", key_map.at(k).c_str());
		for (int i = 2; i < n; ++i)
		{
			printf("\n显示组%d：\t\t\t", i - 1);
			for (int k : group_keys[i])
				printf("%s ", key_map.at(k).c_str());
		}
	}
	printf("\n\n");
}

void edit_key(std::vector<int> &key)
{
	key.clear();
	bool already_set = false;
	system("cls");
	printf("将该快捷键设置为：\n");
	bool check[223];
	memset(check, 0, 223);	// 判断按键是否已push

	HANDLE in_handle = GetStdHandle(STD_INPUT_HANDLE);	// 禁用用户输入，防止按下可见字符时打印到屏幕上
	DWORD mode = 0;
	GetConsoleMode(in_handle, &mode);
	SetConsoleMode(in_handle, mode & (~ENABLE_ECHO_INPUT));

	while (GetAsyncKeyState(13) & 0x8000)
		Sleep(50);	// 等待用户松开之前的Enter
	while (1)
	{
		bool null_input = true;
		for (int i = 0x08; i < 0xDF; ++i)
		{
			if (GetKeyState(i) & 0x8000)
			{
				null_input = false;
				already_set = true;
				if (!check[i])
				{
					check[i] = true;
					if (key_map.find(i) == key_map.end())
						continue;

					key.push_back(i);
					printf("%s ", key_map.at(i).c_str());
				}
			}
		}
		if (already_set && null_input)
		{
			printf("\n\n按下 Enter 继续");
			clear_input();	// 将输入的可见字符清除
			SetConsoleMode(in_handle, mode);
			return;
		}
		Sleep(50);
	}
}

void menu()
{
	system("cls");
	std::cout << qg << std::endl;
	printf("\n\n");
	print_key(normal_flag);
	printf("输入 h 显示帮助\n\n> ");
}

void help()
{
	system("cls");

	printf("功能介绍：\n\n");
	printf("默认模式：\n");
	printf("\t1、窗口置顶：将当前最顶部的窗口置顶\n");
	printf("\t2、取消置顶：将当前最顶部的窗口取消置顶\n");
	printf("\t3、增大透明度：将当前最顶部的窗口透明度增大（更不可见）\n");
	printf("\t4、减小透明度：将当前最顶部的窗口透明度减小（更可见）\n");
	printf("\t5、从列表中增加/删除窗口：将当前最顶部的窗口加入隐藏列表，若其已经在隐藏列表中则将其从隐藏列表中移除\n");
	printf("\t6、隐藏/显示列表中所有窗口：将隐藏列表中的所有窗口隐藏，若已隐藏则将其全部恢复\n");
	printf("注：本程序默认已在隐藏列表中，按下隐藏窗口的快捷键可直接隐藏本窗口\n\n");

	printf("组管理模式：\n");
	printf("\t1、切换组：根据快捷键，将对应任务组中所有窗口显示，所有其它任务组中的窗口隐藏，未加入任务组的窗口维持不变\n");
	printf("\t2、编辑组：将当前最顶部的窗口加入当前任务组；若其已经在当前任务组中，则将其从任务组中移除\n");
	printf("\t3、删除组：将当前显示的任务组删除\n\n");

	printf("首页可输入字符：\n");
	printf("\ta：默认模式下，将所有窗口加入隐藏列表；组管理模式下，创建一个新的任务组\n");
	printf("\td：默认模式下，清空隐藏列表\n");
	printf("\te：编辑快捷键\n");
	printf("\tg：切换至组管理模式\n");
	printf("\th：查看此帮助\n");
	printf("\tn：切换至默认模式\n\n");

	printf("按 Enter 返回首页\n");
	clear_input();
	getchar();
}

void edit()
{
	int opt;
	clear_input();
	if (normal_flag)
	{
		normal_flag = false;	// 终止子线程，防止竞争

		while (1)
		{
			system("cls");
			print_key(true);

			printf("请选择需要修改的快捷键（输入 -1 返回首页）：\n");
			scanf("%d", &opt);
			clear_input();
			if (opt == -1)
			{
				write_key();
				normal_flag = true;
				std::thread(normal_mode).detach();	// 重启子线程
				return;
			}
			else if (opt < 1 || opt > 6)
				continue;

			edit_key(keys[opt - 1]);
		}
	}
	else
	{
		group_flag = false;	// 终止子线程，防止竞争

		while (1)
		{
			system("cls");
			printf("当前快捷键：\n\n");
			printf("1：编辑组：\t\t\t");
			for (int k : group_keys[0])
				printf("%s ", key_map.at(k).c_str());
			printf("\n2：删除组：\t\t\t");
			for (int k : group_keys[1])
				printf("%s ", key_map.at(k).c_str());
			printf("\n\n");

			printf("请选择需要修改的快捷键（输入 -1 返回首页）：\n");
			scanf("%d", &opt);
			clear_input();
			if (opt == -1)
			{
				write_key();
				group_flag = true;
				std::thread(group_mode).detach();	// 重启子线程
				return;
			}
			else if (opt == 1 || opt == 2)
				edit_key(group_keys[opt - 1]);
		}
	}
}

void add_group()
{
	group_flag = false;
	clear_input();

	group_keys.emplace_back();
	groups.emplace_back();
	edit_key(group_keys[group_keys.size() - 1]);

	group_flag = true;
	std::thread(group_mode).detach();
}

int main()
{
	SetConsoleTitle(L"WindowsPinner");
	//如果有key.config，就修改快捷键
	init_key();
	// 控制台退出时恢复所有窗口
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)restore_windows, true);

	std::thread (normal_mode).detach();
	while (1)
	{
		menu();
		
		switch (getchar())
		{
		case 'a':
			if (normal_flag)
				add_win();
			else
				add_group();
			break;
		case 'd':
			if (normal_flag)
				delete_win();
			break;
		case 'e':
			edit();
			break;
		case 'g':
			if (normal_flag)
			{
				normal_flag = false;
				group_flag = true;
				std::thread(group_mode).detach();
			}
			break;
		case 'h':
			help();
			break;
		case 'n':
			if (group_flag)
			{
				normal_flag = true;
				group_flag = false;
				std::thread(normal_mode).detach();
			}
		}
	}
	return 0;
}