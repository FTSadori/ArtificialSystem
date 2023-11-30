#include "Sectors.h"
#include "Exceptions.h"
#include <iostream>
#include <sstream>

namespace Memory
{
	RealFileManager Sectors::s_real_file_manager;

	Sectors::Sectors(const std::string& _folder, size_t _capacity, const std::string& _disk_mark, uint32_t _max_sector_num)
		: c_capacity(_capacity), c_disk_mark(_disk_mark), c_max_sector_num(_max_sector_num),
		c_sector_file_max_size(_max_sector_num * sizeof(SectorInfo) + sizeof(uint32_t)),
		c_folder(_folder)
	{
		size_t _first_sector_capacity = c_sector_file_max_size;
		try {
			auto data = s_real_file_manager.read_from_real_file(c_folder, SectorName(_disk_mark, c_sector_file));
			load_from_data(data);
		}
		catch (const std::exception&)
		{
			add_sector(SectorInfo(c_sector_file, _first_sector_capacity, true, true, 0), {});
			reload();
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

	void Sectors::write_in_system_sector(uintptr_t _start, uintptr_t _size, const DataQueue& _data)
	{
		SectorInfo info = SectorInfo(_start, _size, true, true, 0);

		if (m_sectors.find(_start) == m_sectors.end())
			m_sectors.emplace(_start, info);
		s_real_file_manager.write_into_real_file(c_folder, SectorName{ c_disk_mark, _start }, _data);
		reload();
	}

	void Sectors::add_sector(const SectorInfo& _sector, const DataQueue& _data)
	{
		s_real_file_manager.write_into_real_file(c_folder, SectorName{ c_disk_mark, _sector.start }, _data);
		m_sectors.emplace(_sector.start, _sector);
	}

	size_t Sectors::get_free_space_size()
	{
		size_t space = c_capacity;

		for (const auto& pair : m_sectors)
			space -= std::min(pair.second.size, space);
		return space;
	}

	uintptr_t Sectors::put_raw_file(const DataQueue& _data, bool system)
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
			}
		}
		catch (const NotEnoughFreeSpace& ex)
		{
			for (const auto& sec : new_sectors)
				m_sectors.erase(sec.start);
			throw ex;
		}

		if (m_sectors.size() > c_max_sector_num)
			throw SectorFileIsFull("(Sectors::put_raw_file) You can't add more sectors");

		uintptr_t ret = new_sectors[0].start;
		size_t beg = 0;
		size_t end = 0;
		for (const auto& sec : new_sectors)
		{
			m_sectors.erase(sec.start);
			end = beg + sec.size;
			add_sector(sec, DataQueue(std::vector<char>(_data.get_data() + beg, _data.get_data() + end)));
			beg = end;
		}
		reload();

		return ret;
	}

	DataQueue Sectors::get_raw_file(uintptr_t _start, bool system)
	{

		DataQueue result;
		do
		{
			if (m_sectors.find(_start) == m_sectors.end())
				throw WrongFileReading("(Sectors::get_raw_file) File was not read properly");

			SectorInfo s = m_sectors[_start];
			if (s.first_in_chain && s.system && !system)
				throw FileIsBlocked("(Sectors::get_raw_file) File is a system file. Failed to read it");
			auto file = s_real_file_manager.read_from_real_file(c_folder, SectorName{ c_disk_mark, _start });
			result.concat(file);
			_start = s.next;
		} while (_start != 0);

		return result;
	}

	size_t Memory::Sectors::get_file_size(uintptr_t _start)
	{
		size_t size = 0;

		do
		{
			if (m_sectors.find(_start) == m_sectors.end())
				throw WrongFileReading("(Sectors::get_file_size) File was not read properly");

			SectorInfo s = m_sectors[_start];
			size += s.size;
			_start = s.next;
		} while (_start != 0);

		return size;
	}

	void Sectors::delete_file(uintptr_t _start, bool system)
	{
		if (m_sectors.find(_start) == m_sectors.end())
			throw WrongFileDeletion("(Sectors::delete_file) File was not deleted properly");
		if (!m_sectors[_start].first_in_chain)
			throw WrongFileDeletion("(Sectors::delete_file) File was not deleted properly");
		do
		{
			if (m_sectors.find(_start) == m_sectors.end())
				throw WrongFileDeletion("(Sectors::delete_file) File was not deleted properly");
			SectorInfo s = m_sectors[_start];
			if (s.first_in_chain && s.system && !system)
				throw FileIsBlocked("(Sectors::delete_file) File is a system file. Failed to delete it");
			m_sectors.erase(_start);
			s_real_file_manager.delete_real_file(c_folder, SectorName{ c_disk_mark, _start });
			_start = s.next;
		} while (_start != 0);
		
		reload();
	}

	DataQueue Sectors::get_as_data() const
	{
		DataQueue data;

		uint32_t size = m_sectors.size();
		data.push(size);

		for (const auto& pair : m_sectors)
		{
			data.concat(pair.second.get_as_data());
		}

		return data;
	}
	
	void Sectors::load_from_data(DataQueue& _data)
	{
		if (_data.size() == 0) return;
		
		uint32_t size = _data.pop<uint32_t>();
		
		m_sectors.clear();

		for (uint32_t i = 0; i < size; ++i)
		{
			SectorInfo sec;
			sec.load_from_data(_data);
			m_sectors.emplace(sec.start, sec);
		}
	}

	void Sectors::reload()
	{
		s_real_file_manager.write_into_real_file(c_folder, SectorName(c_disk_mark, c_sector_file), get_as_data());
	}

	bool Sectors::is_system(uintptr_t _start)
	{
		return m_sectors[_start].system;
	}
}
