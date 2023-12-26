#pragma once
#include "ILoadable.h"
#include "DiskFileManager.h"
#include "DiskInfo.h"
#include "DiskSystemInfo.h"
#include "DiskException.h"
#include "FullPath.h"

namespace Memory
{
	class DiskSystem final
	{
	public:
		DiskSystem() = default;
		DiskSystem(const DiskSystemInfo& _info);
		
		void load(const DiskSystemInfo& _info);
		bool is_loaded();
		
		static DiskSystemInfo try_load_boot();
		
		DiskFileManager& get_disk(const std::string& _mark);

		void copy(const FullPath& _src, const FullPath& _dst, bool system);
		void move(const FullPath& _src, const FullPath& _dst, bool system);

		FullPath get_start_path();
	private:
		std::map<std::string, DiskFileManager> m_disks;
		std::string m_main_disk;
		bool m_is_loaded = false;
	};
}
