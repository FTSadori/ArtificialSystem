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

		uintptr_t put_raw_file(const std::vector<uint8_t>& _data, bool system);
		std::vector<uint8_t> get_raw_file(uintptr_t _start, bool system);
		void delete_file(uintptr_t _start, bool system); // only have to delete all sectors from _start and while next != 0

		virtual uintptr_t get_load_file_ptr() override final;
		virtual std::vector<uint8_t> get_as_data() override final;
		virtual void load_from_data(const std::vector<uint8_t>& _data) override final;

	private:
		FreeSectorInfo find_next_free_sector();

		void reload();

		void add_sector(const SectorInfo& _sector, const std::vector<uint8_t>& _data);

		static RealFileManager s_real_file_manager;
		std::map<uintptr_t, SectorInfo> m_sectors;
	};
}
