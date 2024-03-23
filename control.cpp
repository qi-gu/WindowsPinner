#include <iostream>
#include <Windows.h>
#include <unordered_set>
#include <vector>
#include <thread>
#include <conio.h>
#include "all.h"


std::unordered_set<HWND> hidden_windows;
std::vector<std::unordered_set<HWND>> groups;
std::vector<std::vector<int>> keys =
	{{VK_CONTROL, VK_F1}, {VK_CONTROL, VK_F2},
	{VK_CONTROL, VK_F5}, {VK_CONTROL, VK_F6},
	{VK_CONTROL, VK_F11}, {VK_CONTROL, VK_F12}};
std::vector<std::vector<int>> group_keys = 
	{{VK_CONTROL, 0x45}, {VK_CONTROL, VK_DELETE}};

bool normal_flag = true;
bool group_flag = false;

// Ĭ��ģʽ�У�����ݼ�
int check_normal_keys()
{
	static bool last_stat = false;
	for (int i = 0; i < 6; ++i)
	{
		int n = keys[i].size();
		// ������������м�
		for (int j = 0; j < n; ++j)
		{
			if (!(GetAsyncKeyState(keys[i][j]) & 0x8000))
				j = 0xffff;	// �˳�����ѭ��

			if (j == n - 1)
			{
				// ���ⳤ�����ض��true
				if (last_stat)
					return -1;
				last_stat = true;
				return i;
			}
		}
	}
	last_stat = false;
	return -1;
}

// �����ģʽ�У�����ݼ�
int check_group_keys()
{
	static bool last_stat = false;
	int s = group_keys.size();
	for (int i = 0; i < s; ++i)
	{
		int n = group_keys[i].size();
		// ������������м�
		for (int j = 0; j < n; ++j)
		{
			if (!(GetAsyncKeyState(group_keys[i][j]) & 0x8000))
				j = 0xffff;	// �˳�����ѭ��

			if (j == n - 1)
			{
				// ���ⳤ�����ض��true
				if (last_stat)
					return -1;
				last_stat = true;
				return i;
			}
		}
	}
	last_stat = false;
	return -1;
}

// Ĭ��ģʽ �߳�
void normal_mode(void)
{
	HWND now = NULL;
	LONG exStyle = NULL;
	BYTE alpha = 0;
	bool hide_flag = false;

	COLORREF crKey;
	DWORD dwFlags;
	// ����Ĭ������
	hidden_windows.insert(GetConsoleWindow());
	while (normal_flag)
	{
		switch (check_normal_keys())
		{
		case -1:
			break;
		case 0:	// �����ö�
			now = GetForegroundWindow();
			SetWindowPos(now, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			break;
		case 1: // ȡ���ö�
			now = GetForegroundWindow();
			SetWindowPos(now, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			break;
		case 2: // ����͸����
			now = GetForegroundWindow();
			exStyle = GetWindowLong(now, GWL_EXSTYLE);
			if (!(exStyle & WS_EX_LAYERED))	// �������Ը�Ϊ�ֲ㣬֧���޸�͸����
			{
				SetWindowLong(now, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
				SetLayeredWindowAttributes(now, 0, 204, LWA_ALPHA);
			}
			else
			{
				GetLayeredWindowAttributes(now, &crKey, &alpha, &dwFlags);
				alpha = (alpha > 51 ? alpha : 51) - 51;	// �����������
				SetLayeredWindowAttributes(now, 0, alpha, LWA_ALPHA);
			}
			break;
		case 3: // ��С͸����
			now = GetForegroundWindow();
			GetLayeredWindowAttributes(now, &crKey, &alpha, &dwFlags);
			alpha = (alpha < 204 ? alpha : 204) + 51;	// �����������
			SetLayeredWindowAttributes(now, 0, alpha, LWA_ALPHA);
			break;
		case 4: // �޸������б�
			now = GetForegroundWindow();
			if (hidden_windows.count(now))
			{
				if (IsWindowVisible(now)) 	// ���⽫���صĴ����Ƴ��б��޷��ָ�
					hidden_windows.erase(now);
			}
			else
				hidden_windows.insert(now);
			break;
		case 5: // ���ش���
			for (auto it = hidden_windows.begin(); it != hidden_windows.end();)
			{
				HWND hwnd = *it;
				if (IsWindow(hwnd)) // ����δ���ֶ��ص�
				{
					ShowWindow(hwnd, hide_flag ? SW_NORMAL : SW_HIDE);
					++it;
				}
				else
					it = hidden_windows.erase(it); // ɾ��Ԫ�ز����µ�����
			}
			hide_flag ^= 1;
		}
		Sleep(50);
	}
}

// �����ģʽ �߳�
void group_mode(void)
{
	int current_group = -1, choice = -1;
	HWND now = NULL;

	while (group_flag)
	{
		choice = check_group_keys();
		switch (choice)
		{
		case -1:
			break;
		case 0:
			// ȷ����ǰ��һ����Ч����
			if (current_group >= 0 && current_group < group_keys.size() - 2)
			{
				now = GetForegroundWindow();
				if (groups[current_group].count(now))
				{
					if (IsWindowVisible(now)) 	// ���⽫���صĴ����Ƴ��б��޷��ָ�
						groups[current_group].erase(now);
				}
				else
					groups[current_group].insert(now);
			}
			break;
		case 1:
			if (current_group >= 0 && current_group < group_keys.size() - 2)
			{
				// �ѿ�ݼ�����vector��ɾ��
				group_keys.erase(group_keys.begin() + current_group + 2);
				groups.erase(groups.begin() + current_group);
				current_group = -1;
			}
			break;
		default:
			// ���ص�ֵһ����vector��index��ò�Ʋ��ü��Ϸ��ԣ�
			// ������һ������
			if (current_group >= 0 && current_group < group_keys.size() - 2)
			{
				for (auto it = groups[current_group].begin(); it != groups[current_group].end();)
				{
					HWND hwnd = *it;
					if (IsWindow(hwnd)) // ����δ���ֶ��ص�
					{
						ShowWindow(hwnd, SW_HIDE);
						++it;
					}
					else
						it = groups[current_group].erase(it); // ɾ��Ԫ�ز����µ�����
				}
			}
			// ��ʾ��ǰ������
			current_group = choice - 2;
			for (auto it = groups[current_group].begin(); it != groups[current_group].end();)
			{
				HWND hwnd = *it;
				if (IsWindow(hwnd)) // ����δ���ֶ��ص�
				{
					ShowWindow(hwnd, SW_NORMAL);
					++it;
				}
				else
					it = groups[current_group].erase(it); // ɾ��Ԫ�ز����µ�����
			}
		}
		Sleep(50);
	}
}

// �������ڵĻص���������handle����hidden_windows
BOOL CALLBACK insert_handle(HWND hwnd, LPARAM lParam)
{
	if (IsWindowVisible(hwnd))	// ����û��Ƿ�ɼ���������ʾ�����ɼ���ϵͳ����
		hidden_windows.insert(hwnd);
	return 1;
}

// �����д��ڼ��������б�
void add_win()
{
	EnumWindows(insert_handle, reinterpret_cast<LPARAM>(nullptr));
}

// �����д����Ƴ������б�
void delete_win()
{
	for (auto hwnd : hidden_windows)	// �Ȱ����д�����ʾ��������ֹ֮���޷��ָ��ô���
		ShowWindow(hwnd, SW_NORMAL);
	hidden_windows.clear();
}

// ����̨�˳�ʱ�ָ����д���
void restore_windows(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
	case CTRL_CLOSE_EVENT:
		for (auto hwnd : hidden_windows)
			ShowWindow(hwnd, SW_NORMAL);
		for (auto &group : groups)
		{
			for (HWND hwnd : group)
				ShowWindow(hwnd, SW_NORMAL);
		}
	}
}