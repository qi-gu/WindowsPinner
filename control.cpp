#include <iostream>
#include <Windows.h>
#include <unordered_set>
#include <vector>
#include <thread>
#include <conio.h>
#include "all.h"


std::unordered_set<HWND> hidden_windows;
std::vector<std::vector<int>> keys =
	{{VK_CONTROL, VK_F1}, {VK_CONTROL, VK_F2},
	{VK_CONTROL, VK_F5}, {VK_CONTROL, VK_F6},
	{VK_CONTROL, VK_F11}, {VK_CONTROL, VK_F12}};

int check_keys()
{
	static bool last_stat = false;
	for (int i = 0; i < 6; ++i)
	{
		int n = keys[i].size();
		for (int j = 0; j < n; ++j)	// ������������м�
		{
			if (!(GetAsyncKeyState(keys[i][j]) & 0x8000))
			{
				j = 0xffff;	// �˳�����ѭ��
			}
			if (j == n - 1)
			{
				if (last_stat)	// ���ⳤ�����ض��true
					return -1;
				last_stat = true;
				return i;
			}
		}
	}
	last_stat = false;
	return -1;
}

void background(void)
{
	HWND now = NULL;
	LONG exStyle = NULL;
	BYTE alpha = 0;
	bool hide_flag = false;

	COLORREF crKey;
	DWORD dwFlags;
	hidden_windows.insert(GetConsoleWindow());	// ����Ĭ������
	while (flag)
	{
		switch (check_keys())
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
				hidden_windows.erase(now);
			else
				hidden_windows.insert(now);
			break;
		case 5: // ���ش���
			for (auto hwnd : hidden_windows)
				ShowWindow(hwnd, hide_flag ? SW_NORMAL : SW_HIDE);

			hide_flag ^= 1;
		}
		Sleep(50);
	}
}