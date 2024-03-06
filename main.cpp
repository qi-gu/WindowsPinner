#include <iostream>
#include <Windows.h>
#include <unordered_set>
#include <vector>
#include <thread>
#include <conio.h>
#include "all.h"

bool flag = true;

// 清空用户输入的回车
void clear_input()
{
	while (getchar() != 10);
}

void print_key()
{
	printf("当前快捷键：\n\n");

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
	printf("\n\n");
}

void menu()
{
	system("cls");
	std::cout << qg << std::endl;
	printf("\n\n");
	print_key();
	printf("输入 h 显示帮助\n\n> ");
}

void help()
{
	system("cls");

	printf("功能介绍：\n");
	printf("1、窗口置顶：将当前最顶部的窗口置顶\n");
	printf("2、取消置顶：将当前最顶部的窗口取消置顶\n");
	printf("3、增大透明度：将当前最顶部的窗口透明度增大（更不可见）\n");
	printf("4、减小透明度：将当前最顶部的窗口透明度减小（更可见）\n");
	printf("5、从列表中增加/删除窗口：将当前最顶部的窗口加入隐藏列表，若其已经在隐藏列表中则移除隐藏列表\n");
	printf("6、隐藏列表中所有窗口：将隐藏列表中的所有窗口隐藏\n\n");

	printf("注：本程序默认已在隐藏列表中，按下隐藏窗口的快捷键可直接隐藏本窗口\n\n");

	printf("e：编辑快捷键\n");
	printf("h：查看此帮助\n\n");

	printf("按 Enter 返回首页\n");
	clear_input();
	getchar();
}

void edit()
{
	char opt;
	flag = false;	// 终止子线程，防止竞争
	clear_input();

	while (1)
	{
		system("cls");
		print_key();

		printf("请选择需要修改快捷键的功能（输入 e 返回首页）：\n");
		opt = getchar();
		clear_input();
		if (opt == 'e')
		{
			write_key();
			flag = true;
			std::thread(background).detach();	// 重启子线程
			return;
		}
		else if (opt < '1' || opt > '6')
			continue;

		opt -= '1';
		keys[opt].clear();
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
			for (int i = 0x08; i < 0xDE; ++i)
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

						keys[opt].push_back(i);
						printf("%s ", key_map.at(i).c_str());
					}
				}
			}
			if (already_set && null_input)
			{
				printf("\n\n按下 Enter 继续");
				clear_input();	// 将输入的可见字符清除
				SetConsoleMode(in_handle, mode);
				break;
			}
			Sleep(50);
		}
	}
}

int main()
{
	SetConsoleTitle(L"WindowsPinner");
	init_key();	//如果有key.config，就修改快捷键

	std::thread (background).detach();
	while (1)
	{
		menu();
		
		switch (getchar())
		{
		case 'h':
			help();
			break;
		case 'e':
			edit();
			break;
		}
	}
	return 0;
}