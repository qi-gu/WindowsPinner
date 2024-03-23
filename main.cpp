#include <iostream>
#include <Windows.h>
#include <unordered_set>
#include <vector>
#include <thread>
#include <conio.h>
#include <cstdlib>
#include "all.h"

// ����û�����Ļس�
void clear_input()
{
	while (getchar() != 10);
}

void print_key(bool mode)
{
	if (mode)
	{
		printf("��Ĭ��ģʽ����ǰ��ݼ���\n\n");
		printf("1�������ö���\t\t\t");
		for (int k : keys[0])
			printf("%s ", key_map.at(k).c_str());

		printf("\n2��ȡ���ö���\t\t\t");
		for (int k : keys[1])
			printf("%s ", key_map.at(k).c_str());

		printf("\n3������͸���ȣ�\t\t\t");
		for (int k : keys[2])
			printf("%s ", key_map.at(k).c_str());

		printf("\n4����С͸���ȣ�\t\t\t");
		for (int k : keys[3])
			printf("%s ", key_map.at(k).c_str());

		printf("\n5�����б�������/ɾ�����ڣ�\t");
		for (int k : keys[4])
			printf("%s ", key_map.at(k).c_str());

		printf("\n6�������б������д��ڣ�\t\t");
		for (int k : keys[5])
			printf("%s ", key_map.at(k).c_str());
	}
	else
	{
		printf("�������ģʽ����ǰ��ݼ���\n\n");
		int n = group_keys.size();
		printf("1���༭�飺\t\t\t");
		for (int k : group_keys[0])
			printf("%s ", key_map.at(k).c_str());
		printf("\n2��ɾ���飺\t\t\t");
		for (int k : group_keys[1])
			printf("%s ", key_map.at(k).c_str());
		for (int i = 2; i < n; ++i)
		{
			printf("\n��ʾ��%d��\t\t\t", i - 1);
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
	printf("���ÿ�ݼ�����Ϊ��\n");
	bool check[223];
	memset(check, 0, 223);	// �жϰ����Ƿ���push

	HANDLE in_handle = GetStdHandle(STD_INPUT_HANDLE);	// �����û����룬��ֹ���¿ɼ��ַ�ʱ��ӡ����Ļ��
	DWORD mode = 0;
	GetConsoleMode(in_handle, &mode);
	SetConsoleMode(in_handle, mode & (~ENABLE_ECHO_INPUT));

	while (GetAsyncKeyState(13) & 0x8000)
		Sleep(50);	// �ȴ��û��ɿ�֮ǰ��Enter
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
			printf("\n\n���� Enter ����");
			clear_input();	// ������Ŀɼ��ַ����
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
	printf("���� h ��ʾ����\n\n> ");
}

void help()
{
	system("cls");

	printf("���ܽ��ܣ�\n\n");
	printf("Ĭ��ģʽ��\n");
	printf("\t1�������ö�������ǰ����Ĵ����ö�\n");
	printf("\t2��ȡ���ö�������ǰ����Ĵ���ȡ���ö�\n");
	printf("\t3������͸���ȣ�����ǰ����Ĵ���͸�������󣨸����ɼ���\n");
	printf("\t4����С͸���ȣ�����ǰ����Ĵ���͸���ȼ�С�����ɼ���\n");
	printf("\t5�����б�������/ɾ�����ڣ�����ǰ����Ĵ��ڼ��������б������Ѿ��������б�������������б����Ƴ�\n");
	printf("\t6������/��ʾ�б������д��ڣ��������б��е����д������أ�������������ȫ���ָ�\n");
	printf("ע��������Ĭ�����������б��У��������ش��ڵĿ�ݼ���ֱ�����ر�����\n\n");

	printf("�����ģʽ��\n");
	printf("\t1���л��飺���ݿ�ݼ�������Ӧ�����������д�����ʾ�����������������еĴ������أ�δ����������Ĵ���ά�ֲ���\n");
	printf("\t2���༭�飺����ǰ����Ĵ��ڼ��뵱ǰ�����飻�����Ѿ��ڵ�ǰ�������У���������������Ƴ�\n");
	printf("\t3��ɾ���飺����ǰ��ʾ��������ɾ��\n\n");

	printf("��ҳ�������ַ���\n");
	printf("\ta��Ĭ��ģʽ�£������д��ڼ��������б������ģʽ�£�����һ���µ�������\n");
	printf("\td��Ĭ��ģʽ�£���������б�\n");
	printf("\te���༭��ݼ�\n");
	printf("\tg���л��������ģʽ\n");
	printf("\th���鿴�˰���\n");
	printf("\tn���л���Ĭ��ģʽ\n\n");

	printf("�� Enter ������ҳ\n");
	clear_input();
	getchar();
}

void edit()
{
	int opt;
	clear_input();
	if (normal_flag)
	{
		normal_flag = false;	// ��ֹ���̣߳���ֹ����

		while (1)
		{
			system("cls");
			print_key(true);

			printf("��ѡ����Ҫ�޸ĵĿ�ݼ������� -1 ������ҳ����\n");
			scanf("%d", &opt);
			clear_input();
			if (opt == -1)
			{
				write_key();
				normal_flag = true;
				std::thread(normal_mode).detach();	// �������߳�
				return;
			}
			else if (opt < 1 || opt > 6)
				continue;

			edit_key(keys[opt - 1]);
		}
	}
	else
	{
		group_flag = false;	// ��ֹ���̣߳���ֹ����

		while (1)
		{
			system("cls");
			printf("��ǰ��ݼ���\n\n");
			printf("1���༭�飺\t\t\t");
			for (int k : group_keys[0])
				printf("%s ", key_map.at(k).c_str());
			printf("\n2��ɾ���飺\t\t\t");
			for (int k : group_keys[1])
				printf("%s ", key_map.at(k).c_str());
			printf("\n\n");

			printf("��ѡ����Ҫ�޸ĵĿ�ݼ������� -1 ������ҳ����\n");
			scanf("%d", &opt);
			clear_input();
			if (opt == -1)
			{
				write_key();
				group_flag = true;
				std::thread(group_mode).detach();	// �������߳�
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
	//�����key.config�����޸Ŀ�ݼ�
	init_key();
	// ����̨�˳�ʱ�ָ����д���
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