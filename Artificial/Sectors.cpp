#include "Sectors.h"
#include "Exceptions.h"
#include <iostream>
#include <sstream>

namespace Core::Memory
{
	RealFileManager Sectors::s_real_file_manager = RealFileManager(42);

	Sectors::Sectors(size_t _capacity, const std::string& _disk_mark, uint32_t _max_sector_num)
		: c_capacity(_capacity), c_disk_mark(_disk_mark), c_max_sector_num(_max_sector_num) 
	{
		size_t _first_sector_capacity = _max_sector_num * sizeof(SectorInfo) + sizeof(uint32_t);
		try {
			load_from_data(s_real_file_manager.read_from_real_file(SectorName(_disk_mark, 0)));
		}
		catch (const std::exception&)
		{
			add_sector(SectorInfo(0, _first_sector_capacity, true, true, 0), {});
		}
	}


	FreeSectorInfo Sectors::find_next_free_sector()
	{
		uintptr_t start = 0;

		if (m_sectors.empty())
			return FreeSectorInfo{ 0, c_capacity };

		for (const auto& pair : m_sectors)
		{
			if (pair.first > start + c_min_data_in_sector)
				return FreeSectorInfo{ start, pair.first - start };
			start = pair.first + pair.second.size;
		}
		if (c_capacity > start + c_min_data_in_sector)
			return FreeSectorInfo{ start, c_capacity - start };

		throw NotEnoughFreeSpace("Can't find a free sector");

		return FreeSectorInfo{ 0, 0 };
	}

	void Sectors::add_sector(const SectorInfo& _sector, const std::vector<uint8_t>& _data)
	{
		s_real_file_manager.write_into_real_file(SectorName{ c_disk_mark, _sector.start }, _data);
		m_sectors.emplace(_sector.start, _sector);
	}

	size_t Sectors::get_free_space_size()
	{
		size_t space = c_capacity;

		for (const auto& pair : m_sectors)
			space -= pair.second.size;
		return space;
	}

	uintptr_t Sectors::put_raw_file(const std::vector<uint8_t>& _data, bool system)
	{
		size_t remains = _data.size();

		std::vector<SectorInfo> new_sectors;
		bool first = true;

		try
		{
			while (remains > 0)
			{
				FreeSectorInfo sec = find_next_free_sector();
				SectorInfo s(sec.start, 0, system, first, 0);
				first = false;

				if (remains < sec.capacity)
				{
					s.size += remains;
					remains = 0;
				}
				else
				{
					s.size = sec.capacity;
					remains -= sec.capacity;
				}

				if (!s.first_in_chain)
				{
				 	new_sectors.back().next = s.start;
				}
				new_sectors.push_back(s);
				m_sectors.emplace(s.start, s);

				//todo Delete this print stuff
				std::cout << s.first_in_chain << " " << s.start << " " << s.size << " " << s.next << '\n';
			}
		}
		catch (const NotEnoughFreeSpace& ex)
		{
			for (const auto& sec : new_sectors)
				m_sectors.erase(sec.start);
			throw ex;
		}

		if (m_sectors.size() > c_max_sector_num)
			throw SectorFileIsFull("You can't add more sectors");

		uintptr_t ret = new_sectors[0].start;
		size_t beg = 0;
		size_t end = 0;
		for (const auto& sec : new_sectors)
		{
			m_sectors.erase(sec.start);
			end = beg + sec.size;
			add_sector(sec, std::vector<uint8_t>(_data.data() + beg, _data.data() + end));
			beg = end;
		}
		reload();

		return ret;
	}

	std::vector<uint8_t> Sectors::get_raw_file(uintptr_t _start, bool system)
	{

		std::vector<uint8_t> result{ 0 };
		do
		{
			if (m_sectors.find(_start) == m_sectors.end())
				throw WrongFileReading("File was not read properly");

			SectorInfo s = m_sectors[_start];
			if (s.first_in_chain && s.system && !system)
				throw FileIsBlocked("File is a system file. Failed to read it");
			auto file = s_real_file_manager.read_from_real_file(SectorName{ c_disk_mark, _start });
			result.insert(result.end(), file.begin(), file.end());
			_start = s.next;
		} while (_start != 0);

		result.erase(result.begin());

		return result;
	}

	void Sectors::delete_file(uintptr_t _start, bool system)
	{
		do
		{
			if (m_sectors.find(_start) == m_sectors.end())
				throw WrongFileDeletion("File was not deleted properly");
			SectorInfo s = m_sectors[_start];
			if (s.first_in_chain && s.system && !system)
				throw FileIsBlocked("File is a system file. Failed to delete it");
			s_real_file_manager.delete_real_file(SectorName{ c_disk_mark, _start });
			_start = s.next;
		} while (_start != 0);

		reload();
	}

	uintptr_t Sectors::get_load_file_ptr()
	{
		return c_sector_file;
	}

	std::vector<uint8_t> Sectors::get_as_data()
	{
		std::vector<uint8_t> ss;

		uint32_t count = m_sectors.size();

		ss.insert(ss.end(), (uint8_t*)&count, (uint8_t*)&count + sizeof(uint32_t));

		for (const auto& pair : m_sectors)
			ss.insert(ss.end(), (uint8_t*)&pair.second, (uint8_t*)&pair.second + sizeof(SectorInfo));

		return ss;
	}
	
	void Sectors::load_from_data(const std::vector<uint8_t>& _data)
	{
		if (_data.size() == 0) return;

		const uint8_t* beg = _data.data();
		uint32_t size = *((uint32_t*)beg);

		m_sectors.clear();

		for (uint32_t i = 0; i < size; ++i)
		{
			SectorInfo* sec = (SectorInfo*)(beg + sizeof(size) + i * sizeof(SectorInfo));
			m_sectors.emplace(sec->start, *sec);
		}
	}

	void Sectors::reload()
	{
		auto data = get_as_data();
		s_real_file_manager.write_into_real_file(SectorName(c_disk_mark, 0), data);
	}
}
