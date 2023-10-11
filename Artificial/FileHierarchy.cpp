#include "FileHierarchy.h"
#include "FileHierarchyExceptions.h"
#include "Exceptions.h"
#include <iostream>

namespace Core::Memory
{
	FileHierarchy::FileHierarchy(uintptr_t _root_dir) : m_root_dir(_root_dir)
	{ 
		m_file_hierarchy.emplace(_root_dir, std::vector<HierarchyFileInfo>{}); 
	}

	void FileHierarchy::add_to_hierarchy(uintptr_t _dir, uintptr_t _start, bool is_folder)
	{
		if (m_file_hierarchy.find(_dir) == m_file_hierarchy.end())
			throw DirectoryDoesntExist("There is no directory in this sector");
		m_higher_dir.emplace(_start, _dir);
		m_file_hierarchy[_dir].emplace_back(_start, is_folder);
		if (is_folder)
			m_file_hierarchy.emplace(_start, std::vector<HierarchyFileInfo>());
	}

	void Core::Memory::FileHierarchy::remove_from_hierarchy(uintptr_t _start)
	{
		if (_start == m_root_dir)
			throw CantDeleteRootDirectory("Can't delete root directory");

		if (m_file_hierarchy.find(_start) != m_file_hierarchy.end())
		{
			// is folder
			while(!m_file_hierarchy[_start].empty())
			{
				remove_from_hierarchy(m_file_hierarchy[_start].begin()->start);
			}

			m_file_hierarchy.erase(_start);
		}

		auto iter = find_data_about(_start);
		auto& higher_dir = m_file_hierarchy[m_higher_dir[_start]];
		higher_dir.erase(iter);
		
		m_higher_dir.erase(_start);
	}

	DataQueue Core::Memory::FileHierarchy::get_as_data() const
	{
		DataQueue data;
		data.push(m_file_hierarchy.size()); // 8 bytes (size of tree)
		for (const auto& pair : m_file_hierarchy)
		{
			data.push(pair.first); // 8 bytes (dir)
			data.push(pair.second.size()); // 8 bytes (subdirs num in dir)

			for (const HierarchyFileInfo& i : pair.second)
				data.concat(i.get_as_data()); // 9 bytes (subdirs info)
		}

		return data;
	}

	void Core::Memory::FileHierarchy::load_from_data(DataQueue& _data)
	{
		m_file_hierarchy.clear();
		m_higher_dir.clear();

		size_t size = _data.pop<size_t>();

		for (size_t i = 0; i < size; ++i)
		{
			uintptr_t dir = _data.pop<uintptr_t>();
			m_file_hierarchy.emplace(dir, std::vector<HierarchyFileInfo>());
			size_t subdirs_num = _data.pop<uintptr_t>();

			for (size_t j = 0; j < subdirs_num; ++j)
			{
				uintptr_t subdir = _data.pop<uintptr_t>();
				m_file_hierarchy[dir].emplace_back(subdir, _data.pop<bool>());
				m_higher_dir[subdir] = dir;
			}
		}
	}

	const std::unordered_map<uintptr_t, std::vector<HierarchyFileInfo>>& FileHierarchy::get_file_hierarchy()
	{
		return m_file_hierarchy;
	}

	const std::unordered_map<uintptr_t, uintptr_t>& FileHierarchy::get_higher_dir()
	{
		return m_higher_dir;
	}

	std::vector<HierarchyFileInfo>::iterator FileHierarchy::find_data_about(uintptr_t _start)
	{
		auto& higher_dir = m_file_hierarchy[m_higher_dir[_start]];

		for (auto beg = higher_dir.begin(); beg != higher_dir.end(); beg++)
		{
			if (beg->start == _start)
			{
				return beg;
			}
		}
		throw FileDoesNotExist("File does not exist");
	}

	void FileHierarchy::change_ptr_for(uintptr_t _from, uintptr_t _to)
	{
		if (m_higher_dir.find(_to) != m_higher_dir.end())
			throw PointerAlreadyUsed("This pointer is already used");

		auto iter = find_data_about(_from);
		auto& higher_dir = m_file_hierarchy[m_higher_dir[_from]];

		m_higher_dir.emplace(_to, m_higher_dir[_from]);
		m_higher_dir.erase(_from);

		bool b = iter->is_directory;
		higher_dir.erase(iter);
		higher_dir.emplace_back(_to, b);

		m_file_hierarchy.emplace(_to, m_file_hierarchy[_from]);
		m_file_hierarchy.erase(_from);

		for (const HierarchyFileInfo& info : m_file_hierarchy[_to])
			m_higher_dir[info.start] = _to;
	}
}