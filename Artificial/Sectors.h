#pragma once
#include "SectorInfo.h"
#include "ILoadable.h"
#include "RealFileManager.h"
#include <map>

namespace Memory
{
	class Sectors final : public ILoadable
	{
	public:
		const size_t c_capacity;
		const std::string c_disk_mark;
		const size_t c_min_data_in_sector = 32;
		const size_t c_sector_file_max_size;
		const uint32_t c_max_sector_num;
		const uintptr_t c_sector_file = 0;

		Sectors(const std::string& _folder, size_t _capacity, const std::string& _disk_mark, uint32_t _max_sector_num);

		size_t get_free_space_size();

		uintptr_t put_raw_file(const DataQueue& _data, bool system);
		DataQueue get_raw_file(uintptr_t _start, bool system);
		size_t get_file_size(uintptr_t _start);

		void delete_file(uintptr_t _start, bool system);

		virtual DataQueue get_as_data() const override final;
		virtual void load_from_data(DataQueue& _data) override final;

		bool is_system(uintptr_t _start);
		// system method
		void write_in_system_sector(uintptr_t _start, uintptr_t _size, const DataQueue& _data = DataQueue());

	private:
		FreeSectorInfo find_next_free_sector();

		void reload();

		void add_sector(const SectorInfo& _sector, const DataQueue& _data);

		std::map<uintptr_t, SectorInfo> m_sectors;

		const std::string c_folder;

		static RealFileManager s_real_file_manager;
	};
}
