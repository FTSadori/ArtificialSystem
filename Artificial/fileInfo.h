#pragma once
#include <vector>
#include <string>

struct FileInfo final
{
	std::string dir_name;
	std::string file_name;
	std::vector<char> file_content;

	size_t size;
	time_t creation_time;
	time_t changing_time;

	bool hidden;
	bool sudo;

	uint8_t read_perm_lvl;
	uint8_t write_perm_lvl;
	uint8_t exec_perm_lvl;
};