#pragma once
#include "SectorInfo.h"
#include "ILoadable.h"
#include "RealFileManager.h"
#include <map>

namespace Core::Memory
{
	class Sectors final : public ILoadable
	{
	public:
		const size_t c_capacity;
		const std::string c_disk_mark;
		const size_t c_min_data_in_sector = 32;
		const uint32_t c_max_sector_num;
		const uintptr_t c_sector_file = 0;

		Sectors(size_t _capacity, const std::string& _disk_mark, uint32_t _max_sector_num);

		size_t get_free_space_size(); // only have to sum up all m_sectors[i].size

		uintptr_t put_raw_file(const DataQueue& _data, bool system);
		DataQueue get_raw_file(uintptr_t _start, bool system);
		void delete_file(uintptr_t _start, bool system); // only have to delete all sectors from _start and while next != 0

		virtual DataQueue get_as_data() const override final;
		virtual void load_from_data(DataQueue& _data) override final;

		std::map<uintptr_t, SectorInfo> m_sectors;
	private:
		FreeSectorInfo find_next_free_sector();

		void reload();

		void add_sector(const SectorInfo& _sector, const DataQueue& _data);

		static RealFileManager s_real_file_manager;
	};
}
