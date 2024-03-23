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

// 默认模式中，检测快捷键
int check_normal_keys()
{
	static bool last_stat = false;
	for (int i = 0; i < 6; ++i)
	{
		int n = keys[i].size();
		// 遍历组合中所有键
		for (int j = 0; j < n; ++j)
		{
			if (!(GetAsyncKeyState(keys[i][j]) & 0x8000))
				j = 0xffff;	// 退出该轮循环

			if (j == n - 1)
			{
				// 避免长按返回多次true
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

// 组管理模式中，检测快捷键
int check_group_keys()
{
	static bool last_stat = false;
	int s = group_keys.size();
	for (int i = 0; i < s; ++i)
	{
		int n = group_keys[i].size();
		// 遍历组合中所有键
		for (int j = 0; j < n; ++j)
		{
			if (!(GetAsyncKeyState(group_keys[i][j]) & 0x8000))
				j = 0xffff;	// 退出该轮循环

			if (j == n - 1)
			{
				// 避免长按返回多次true
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

// 默认模式 线程
void normal_mode(void)
{
	HWND now = NULL;
	LONG exStyle = NULL;
	BYTE alpha = 0;
	bool hide_flag = false;

	COLORREF crKey;
	DWORD dwFlags;
	// 自身默认隐藏
	hidden_windows.insert(GetConsoleWindow());
	while (normal_flag)
	{
		switch (check_normal_keys())
		{
		case -1:
			break;
		case 0:	// 窗口置顶
			now = GetForegroundWindow();
			SetWindowPos(now, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			break;
		case 1: // 取消置顶
			now = GetForegroundWindow();
			SetWindowPos(now, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			break;
		case 2: // 增大透明度
			now = GetForegroundWindow();
			exStyle = GetWindowLong(now, GWL_EXSTYLE);
			if (!(exStyle & WS_EX_LAYERED))	// 窗口属性改为分层，支持修改透明度
			{
				SetWindowLong(now, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
				SetLayeredWindowAttributes(now, 0, 204, LWA_ALPHA);
			}
			else
			{
				GetLayeredWindowAttributes(now, &crKey, &alpha, &dwFlags);
				alpha = (alpha > 51 ? alpha : 51) - 51;	// 避免整形溢出
				SetLayeredWindowAttributes(now, 0, alpha, LWA_ALPHA);
			}
			break;
		case 3: // 减小透明度
			now = GetForegroundWindow();
			GetLayeredWindowAttributes(now, &crKey, &alpha, &dwFlags);
			alpha = (alpha < 204 ? alpha : 204) + 51;	// 避免整形溢出
			SetLayeredWindowAttributes(now, 0, alpha, LWA_ALPHA);
			break;
		case 4: // 修改隐藏列表
			now = GetForegroundWindow();
			if (hidden_windows.count(now))
			{
				if (IsWindowVisible(now)) 	// 避免将隐藏的窗口移除列表，无法恢复
					hidden_windows.erase(now);
			}
			else
				hidden_windows.insert(now);
			break;
		case 5: // 隐藏窗口
			for (auto it = hidden_windows.begin(); it != hidden_windows.end();)
			{
				HWND hwnd = *it;
				if (IsWindow(hwnd)) // 窗口未被手动关掉
				{
					ShowWindow(hwnd, hide_flag ? SW_NORMAL : SW_HIDE);
					++it;
				}
				else
					it = hidden_windows.erase(it); // 删除元素并更新迭代器
			}
			hide_flag ^= 1;
		}
		Sleep(50);
	}
}

// 组管理模式 线程
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
			// 确定当前是一个有效的组
			if (current_group >= 0 && current_group < group_keys.size() - 2)
			{
				now = GetForegroundWindow();
				if (groups[current_group].count(now))
				{
					if (IsWindowVisible(now)) 	// 避免将隐藏的窗口移除列表，无法恢复
						groups[current_group].erase(now);
				}
				else
					groups[current_group].insert(now);
			}
			break;
		case 1:
			if (current_group >= 0 && current_group < group_keys.size() - 2)
			{
				// 把快捷键和组vector都删除
				group_keys.erase(group_keys.begin() + current_group + 2);
				groups.erase(groups.begin() + current_group);
				current_group = -1;
			}
			break;
		default:
			// 返回的值一定是vector的index，貌似不用检查合法性？
			// 隐藏上一组内容
			if (current_group >= 0 && current_group < group_keys.size() - 2)
			{
				for (auto it = groups[current_group].begin(); it != groups[current_group].end();)
				{
					HWND hwnd = *it;
					if (IsWindow(hwnd)) // 窗口未被手动关掉
					{
						ShowWindow(hwnd, SW_HIDE);
						++it;
					}
					else
						it = groups[current_group].erase(it); // 删除元素并更新迭代器
				}
			}
			// 显示当前组内容
			current_group = choice - 2;
			for (auto it = groups[current_group].begin(); it != groups[current_group].end();)
			{
				HWND hwnd = *it;
				if (IsWindow(hwnd)) // 窗口未被手动关掉
				{
					ShowWindow(hwnd, SW_NORMAL);
					++it;
				}
				else
					it = groups[current_group].erase(it); // 删除元素并更新迭代器
			}
		}
		Sleep(50);
	}
}

// 遍历窗口的回调函数，将handle加入hidden_windows
BOOL CALLBACK insert_handle(HWND hwnd, LPARAM lParam)
{
	if (IsWindowVisible(hwnd))	// 检测用户是否可见，避免显示本不可见的系统进程
		hidden_windows.insert(hwnd);
	return 1;
}

// 将所有窗口加入隐藏列表
void add_win()
{
	EnumWindows(insert_handle, reinterpret_cast<LPARAM>(nullptr));
}

// 将所有窗口移除隐藏列表
void delete_win()
{
	for (auto hwnd : hidden_windows)	// 先把所有窗口显示出来，防止之后无法恢复该窗口
		ShowWindow(hwnd, SW_NORMAL);
	hidden_windows.clear();
}

// 控制台退出时恢复所有窗口
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