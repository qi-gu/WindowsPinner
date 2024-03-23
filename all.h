#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <Windows.h>

extern void normal_mode();
extern void group_mode();
extern void init_key();
extern void write_key();
extern void add_win();
extern void delete_win();
extern void restore_windows(DWORD fdwctrltype);

extern std::vector<std::vector<int>> keys;
extern std::vector<std::vector<int>> group_keys;
extern std::vector<std::unordered_set<HWND>> groups;
extern const std::map<int, std::string> key_map;
extern const std::string qg;
extern bool normal_flag;
extern bool group_flag;