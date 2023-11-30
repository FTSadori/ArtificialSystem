#pragma once
#include <string>
#include "FileType.h"

namespace Memory
{
	class DiskPath final
	{
	public:
		static const char c_div = '\\';

		DiskPath(const std::string& _dir, const std::string& _file);
		DiskPath(const std::string& _path);
		DiskPath() : DiskPath("") {}

		const std::string& full_name() const;
		const std::string& dir() const;
		const std::string& file() const;

	protected:
		std::string m_dir;
		std::string m_file;

		std::string m_full_name;
	};
}