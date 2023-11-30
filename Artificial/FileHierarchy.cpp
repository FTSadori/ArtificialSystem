#include "FileHierarchy.h"
#include "FileHierarchyExceptions.h"
#include "Exceptions.h"
#include <iostream>

namespace Memory
{
	FileHierarchy::FileHierarchy(uintptr_t _root_dir) : m_root_dir(_root_dir)
	{ 
		m_file_hierarchy.emplace(_root_dir, std::vector<HierarchyFileInfo>{}); 
	}

	void FileHierarchy::init(uintptr_t _root_dir)
	{
		m_root_dir = _root_dir;
		m_file_hierarchy.clear();
		m_file_hierarchy.emplace(_root_dir, std::vector<HierarchyFileInfo>{}); 
	}

	bool FileHierarchy::is_inited() const
	{
		return m_root_dir != 0;
	}

	uintptr_t FileHierarchy::get_root_dir() const
	{
		return m_root_dir;
	}

	bool FileHierarchy::is_directory(uintptr_t _ptr) const
	{
		return m_file_hierarchy.find(_ptr) != m_file_hierarchy.end();
	}

	void FileHierarchy::add_to_hierarchy(uintptr_t _dir, uintptr_t _start, FileT _type)
	{
		if (m_file_hierarchy.find(_dir) == m_file_hierarchy.end())
			throw DirectoryDoesntExist("(FileHierarchy::add_to_hierarchy) There is no directory in this sector");
		m_higher_dir.emplace(_start, _dir);
		m_file_hierarchy[_dir].emplace_back(_start, _type);
		if (_type == FileT::DIR)
			m_file_hierarchy.emplace(_start, std::vector<HierarchyFileInfo>());
	}


	void FileHierarchy::remove_from_hierarchy(uintptr_t _start)
	{
		if (_start == m_root_dir)
			throw CantDeleteRootDirectory("(FileHierarchy::remove_from_hierarchy) Can't delete root directory");

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

	DataQueue Memory::FileHierarchy::get_as_data() const
	{
		DataQueue data;
		data.push(m_root_dir);
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

	void FileHierarchy::load_from_data(DataQueue& _data)
	{
		m_file_hierarchy.clear();
		m_higher_dir.clear();

		m_root_dir = _data.pop<uintptr_t>();
		
		size_t size = _data.pop<size_t>();

		for (size_t i = 0; i < size; ++i)
		{
			uintptr_t dir = _data.pop<uintptr_t>();
			m_file_hierarchy.emplace(dir, std::vector<HierarchyFileInfo>());
			size_t subdirs_num = _data.pop<uintptr_t>();

			for (size_t j = 0; j < subdirs_num; ++j)
			{
				uintptr_t subdir = _data.pop<uintptr_t>();
				m_file_hierarchy[dir].emplace_back(subdir, _data.pop<FileT>());
				m_higher_dir[subdir] = dir;
			}
		}
	}

	const std::unordered_map<uintptr_t, std::vector<HierarchyFileInfo>>& FileHierarchy::get_file_hierarchy() const
	{
		return m_file_hierarchy;
	}

	const std::unordered_map<uintptr_t, uintptr_t>& FileHierarchy::get_higher_dir() const
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
		throw FileDoesNotExist("(FileHierarchy::find_data_about) File does not exist");
	}

	void FileHierarchy::change_ptr_for(uintptr_t _from, uintptr_t _to)
	{
		if (m_higher_dir.find(_to) != m_higher_dir.end())
			throw PointerAlreadyUsed("(FileHierarchy::change_ptr_for) Pointer is already used");

		auto iter = find_data_about(_from);
		auto& higher_dir = m_file_hierarchy[m_higher_dir[_from]];

		m_higher_dir.emplace(_to, m_higher_dir[_from]);
		m_higher_dir.erase(_from);

		FileT b = iter->type;
		higher_dir.erase(iter);
		higher_dir.emplace_back(_to, b);

		if (b == FileT::DIR)
		{
			m_file_hierarchy.emplace(_to, m_file_hierarchy[_from]);
			m_file_hierarchy.erase(_from);

			for (const HierarchyFileInfo& info : m_file_hierarchy[_to])
				m_higher_dir[info.start] = _to;
		}
	}
	

	void FileHierarchy::move(uintptr_t _start, uintptr_t _new_dir)
	{
		auto iter = find_data_about(_start);
		HierarchyFileInfo data = *iter;

		uintptr_t _higher = m_higher_dir[_start];
		m_file_hierarchy[_higher].erase(iter);
		m_higher_dir[_start] = _new_dir;
		m_file_hierarchy[_new_dir].push_back(data);
	}
}