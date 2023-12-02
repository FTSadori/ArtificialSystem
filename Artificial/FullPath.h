#pragma once
#include "ILoadable.h"
#include "DiskFileManager.h"
#include "DiskInfo.h"
#include "DiskSystemInfo.h"
#include "DiskException.h"
#include "DiskPath.h"

namespace Memory
{
	class FullPath final
	{
	public:
		static const char c_disk_div = ':';

		FullPath(const std::string& _mark, const DiskPath& _path);
		FullPath(const std::string& _path);

		const std::string& full_disk_name() const;
		const DiskPath& disk_path() const;
		const std::string& full_dir_name() const;
		const std::string& mark() const;

	private:
		std::string m_mark;
		DiskPath m_path;

		std::string m_full_dir_name;
		std::string m_full_disk_name;
	};
}