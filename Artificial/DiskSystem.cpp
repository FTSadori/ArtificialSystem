#include "DiskSystem.h"
#include <filesystem>

namespace Memory
{
	DiskSystem::DiskSystem(const DiskSystemInfo& _info)
	{
		if (!std::filesystem::exists("Inner"))
		{
			std::filesystem::create_directory("Inner");
			DiskFileManager boot_disk("Inner", 2000, "boot", 20);
			DataQueue boot_data = _info.get_as_data();
			boot_disk.write(DiskPath("", "bfile"), boot_data, true);
			std::cout << boot_disk.list(DiskPath(""), true)[0];
		}
		for (const auto& [mark, info] : _info.disks_info)
			m_disks.emplace(mark, DiskFileManager("Inner", info.capacity, info.mark, info.max_sec_num));
	}

	DiskSystemInfo DiskSystem::try_load_boot()
	{
		DiskFileManager boot_disk("Inner", 2000, "boot", 20);
		if (!boot_disk.is_exists(DiskPath("", "bfile")))
			throw FileDoesNotExist("(DiskSystem::try_load_boot) Failed to load data");
		DataQueue boot_data = boot_disk.read(DiskPath("", "bfile"), true);
		DiskSystemInfo dsi;
		dsi.load_from_data(boot_data);
		return dsi;
	}

	DiskFileManager& DiskSystem::get_disk(const std::string& _mark)
	{
		if (m_disks.find(_mark) == m_disks.end())
			throw DiskMarkDoesNotExist("(DiskSystem::get_disk) Disk with mark \"" + _mark + "\"");

		return m_disks.at(_mark);
	}

	void DiskSystem::copy(const FullPath& _src, const FullPath& _dst, bool system)
	{
		DiskFileManager& src_disk = get_disk(_src.mark());
		DiskFileManager& dst_disk = get_disk(_dst.mark());

		const DiskPath& src = _src.disk_path();
		const DiskPath& dst = _dst.disk_path();

		FileInfo info = src_disk.get_info(src, system);
		dst_disk.create(dst, info.permissions, src_disk.get_type(src), src_disk.is_system(src));

		if (src_disk.get_type(src) == FileT::DIR)
		{
			const auto list = src_disk.list(src, system);
			for (const std::string& file : list)
			{
				try
				{
					FullPath new_src = FullPath(_src.mark(), DiskPath(_src.disk_path().full_name(), file));
					FullPath new_dst = FullPath(_dst.mark(), DiskPath(_dst.disk_path().full_name(), file));
					copy(new_src, new_dst, system);
				}
				catch (const Exception&) { }
			}
		}
		else
		{
			DataQueue data = src_disk.read(src, system);
			dst_disk.write(dst, data, system);
		}
	}

	void DiskSystem::move(const FullPath& _src, const FullPath& _dst, bool system)
	{
		copy(_src, _dst, system);

		get_disk(_src.mark()).remove(_src.disk_path(), system);
	}

}
