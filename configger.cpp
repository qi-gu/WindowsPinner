#include <iostream>
#include <fstream>
#include <sstream>
#include "all.h"


// 存在key.config就读取用户的快捷键
void init_key()
{
	std::ifstream file("key.config");
	if (!file.is_open())
		return;

	std::string line;
	for (int i = 0; i < 6; ++i)
	{
		keys[i].clear();
		getline(file, line);
		std::istringstream iss(line);
		int number;
		while (iss >> number)
		{
			if (key_map.find(number) != key_map.end())
				keys[i].push_back(number);
			else
			{
				printf("key.config 配置错误，请删除文件重试！");
				getchar();
				exit(0);
			}
		}
	}

	file.close();
}

// 将当前的快捷键写入文件
void write_key()
{
	std::ofstream file("key.config");

	for (std::vector<int> i : keys)
	{
		for (int j : i)
			file << j << " ";
		file << std::endl;
	}

	file.close();
}