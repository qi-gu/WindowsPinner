#include <iostream>
#include <Windows.h>
#include <unordered_set>
#include <vector>
#include <thread>
#include <conio.h>
#include "all.h"

bool flag = true;

// ����û�����Ļس�
void clear_input()
{
	while (getchar() != 10);
}

void print_key()
{
	printf("��ǰ��ݼ���\n\n");

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
	printf("\n\n");
}

void menu()
{
	system("cls");
	std::cout << qg << std::endl;
	printf("\n\n");
	print_key();
	printf("���� h ��ʾ����\n\n> ");
}

void help()
{
	system("cls");

	printf("���ܽ��ܣ�\n");
	printf("1�������ö�������ǰ����Ĵ����ö�\n");
	printf("2��ȡ���ö�������ǰ����Ĵ���ȡ���ö�\n");
	printf("3������͸���ȣ�����ǰ����Ĵ���͸�������󣨸����ɼ���\n");
	printf("4����С͸���ȣ�����ǰ����Ĵ���͸���ȼ�С�����ɼ���\n");
	printf("5�����б�������/ɾ�����ڣ�����ǰ����Ĵ��ڼ��������б������Ѿ��������б������Ƴ������б�\n");
	printf("6�������б������д��ڣ��������б��е����д�������\n\n");

	printf("ע��������Ĭ�����������б��У��������ش��ڵĿ�ݼ���ֱ�����ر�����\n\n");

	printf("e���༭��ݼ�\n");
	printf("h���鿴�˰���\n\n");

	printf("�� Enter ������ҳ\n");
	clear_input();
	getchar();
}

void edit()
{
	char opt;
	flag = false;	// ��ֹ���̣߳���ֹ����
	clear_input();

	while (1)
	{
		system("cls");
		print_key();

		printf("��ѡ����Ҫ�޸Ŀ�ݼ��Ĺ��ܣ����� e ������ҳ����\n");
		opt = getchar();
		clear_input();
		if (opt == 'e')
		{
			write_key();
			flag = true;
			std::thread(background).detach();	// �������߳�
			return;
		}
		else if (opt < '1' || opt > '6')
			continue;

		opt -= '1';
		keys[opt].clear();
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
				printf("\n\n���� Enter ����");
				clear_input();	// ������Ŀɼ��ַ����
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
	init_key();	//�����key.config�����޸Ŀ�ݼ�

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