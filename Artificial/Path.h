#pragma once
#include <string>

namespace Core::Memory
{
	struct DiskPath final
	{
		std::string folder;
		std::string file;
	};

	struct FullPath final
	{
		std::string disk;
		std::string folder;
		std::string file;
	};

	struct DiskFolderPath final
	{
		std::string higher_folder;
		std::string folder_name;
	};

	struct FullFolderPath final
	{
		std::string disk;
		std::string higher_folder;
		std::string folder_name;
	};
}