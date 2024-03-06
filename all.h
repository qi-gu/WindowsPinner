#pragma once
#include <iostream>
#include <vector>
#include <map>

extern void background();
extern void init_key();
extern void write_key();

extern std::vector<std::vector<int>> keys;
extern const std::map<int, std::string> key_map;
extern const std::string qg;
extern bool flag;