#include "DiskFileManager.h"
#include "FileHierarchyExceptions.h"
#include <chrono>
#include <iostream>
#include "FileInfo.h"

namespace Memory
{
	DiskFileManager::DiskFileManager(const std::string& _folder, size_t _capacity, const std::string& _mark, uint32_t _max_sec_num)
		: m_sectors(_folder, _capacity, _mark, _max_sec_num), capacity(_capacity)
	{
		try {
			{	
				DataQueue data = m_sectors.get_raw_file(m_sectors.c_sector_file_max_size, true);
				m_hierarchy_data_ptr = data.pop<uintptr_t>();
				DataQueue h_data = m_sectors.get_raw_file(m_hierarchy_data_ptr, true);
				m_file_hierarchy.load_from_data(h_data);
			}
			{
				DataQueue data_names = m_sectors.get_raw_file(m_sectors.c_sector_file_max_size + sizeof(uintptr_t), true);
				m_names_data_ptr = data_names.pop<uintptr_t>();
				DataQueue n_data = m_sectors.get_raw_file(m_names_data_ptr, true);
				load_from_data(n_data);
			}
		}
		catch (const std::exception&)
		{
			m_sectors.write_in_system_sector(m_sectors.c_sector_file_max_size, sizeof(uintptr_t));
			m_sectors.write_in_system_sector(m_sectors.c_sector_file_max_size + sizeof(uintptr_t), sizeof(uintptr_t));
			create(DiskPath("", ""), Permissions(true, 0, 0, 0, 0), FileT::DIR, true);
		}
	}

	void DiskFileManager::save_hierarchy_data()
	{
		if (m_hierarchy_data_ptr != 0)
		{
			m_sectors.delete_file(m_hierarchy_data_ptr, true);
		}
		m_hierarchy_data_ptr = m_sectors.put_raw_file(m_file_hierarchy.get_as_data(), true);
		DataQueue data;
		data.push(m_hierarchy_data_ptr);
		m_sectors.write_in_system_sector(m_sectors.c_sector_file_max_size, sizeof(uintptr_t), data);
	}

	uintptr_t DiskFileManager::get_root_dir()
	{
		return m_file_hierarchy.get_root_dir();
	}

	size_t DiskFileManager::get_content_size(const DiskPath& _path)
	{
		if (m_names_to_ptr.find(_path.full_name()) == m_names_to_ptr.end())
			throw FileDoesNotExist("(DiskFileManager::get_content_size) File or directory " + _path.full_name() + " does not exist");
		
		size_t size = m_sectors.get_file_size(m_names_to_ptr[_path.full_name()]);
		return size - 2 * sizeof(time_t) - sizeof(Permissions);
	}

	FileInfo DiskFileManager::get_info(const DiskPath& _path, bool system)
	{
		if (m_names_to_ptr.find(_path.full_name()) == m_names_to_ptr.end())
			throw FileDoesNotExist("(DiskFileManager::get_info) File or directory " + _path.full_name() + " does not exist");

		DataQueue data = m_sectors.get_raw_file(m_names_to_ptr[_path.full_name()], system);
		FileInfo info;
		info.load_from_data(data);

		return info;
	}

	void DiskFileManager::change_info(const DiskPath& _path, Permissions _per, bool system)
	{
		if (m_names_to_ptr.find(_path.full_name()) == m_names_to_ptr.end())
			throw FileDoesNotExist("(DiskFileManager::change_info) File or directory (" + _path.full_name() + ") does not exist");

		uintptr_t ptr = m_names_to_ptr[_path.full_name()];
		DataQueue data = m_sectors.get_raw_file(ptr, system);
		data.pop<Permissions>();
		time_t a = data.pop<time_t>();
		time_t b = data.pop<time_t>();
		m_sectors.delete_file(ptr, system);

		FileInfo info = FileInfo(_per, a, b);
		DataQueue fileData = info.get_as_data();
		fileData.concat(data);

		uintptr_t new_ptr = m_sectors.put_raw_file(fileData, system);

		m_file_hierarchy.change_ptr_for(ptr, new_ptr);

		m_ptr_to_names.erase(ptr);
		m_ptr_to_names.emplace(new_ptr, _path.full_name());
		m_names_to_ptr[_path.full_name()] = new_ptr;
	}

	void DiskFileManager::reload()
	{
		save_hierarchy_data();

		if (m_names_data_ptr != 0)
		{
			m_sectors.delete_file(m_names_data_ptr, true);
		}
		m_names_data_ptr = m_sectors.put_raw_file(get_as_data(), true);
		DataQueue data;
		data.push(m_names_data_ptr);
		m_sectors.write_in_system_sector(m_sectors.c_sector_file_max_size + sizeof(uintptr_t), sizeof(uintptr_t), data);
	}

	DataQueue DiskFileManager::get_as_data() const
	{
		DataQueue data;
		data.push(m_ptr_to_names.size());
		for (const auto& pair : m_ptr_to_names)
		{
			data.push(pair.first);
			data.push(pair.second.size());
			for (char c : pair.second)
				data.push(c);
		}
		return data;
	}

	void DiskFileManager::load_from_data(DataQueue& _data)
	{
		m_ptr_to_names.clear();
		m_names_to_ptr.clear();
		
		size_t size = _data.pop<size_t>();
		for (size_t i = 0; i < size; ++i)
		{
			uintptr_t ptr = _data.pop<uintptr_t>();
			size_t line_size = _data.pop<size_t>();
			std::string line;
			for (size_t j = 0; j < line_size; ++j)
				line += _data.pop<char>();
			m_ptr_to_names.emplace(ptr, line);
			m_names_to_ptr.emplace(std::move(line), ptr);
		}
	}

	std::string DiskFileManager::check_names(const DiskPath& _path, FileT _type)
	{
		const std::string& name = _path.file();
		const std::string& dir = _path.dir();

		if (name == "" && dir == "")
			return _path.full_name();

		if (name == "")
			throw NameIsNotValid("(DiskFileManager::check_names) Name must not be empty");

		if (!NameValidator::is_name_valid(name))
			throw NameIsNotValid("(DiskFileManager::check_names) Name must not include / \\ : * ? \" < > |");
		
		if (m_names_to_ptr.find(dir) == m_names_to_ptr.end())
			throw FileWithThisNameDoesNotExist("(DiskFileManager::check_names) Directory with this name (" + dir + ") does not exist");
		
		if (!m_file_hierarchy.is_directory(m_names_to_ptr[dir]))
			throw FileWithThisNameDoesNotExist("(DiskFileManager::check_names) Directory with this name (" + dir + ") does not exist");

		if (m_names_to_ptr.find(_path.full_name()) != m_names_to_ptr.end())
			throw FileAlreadyExists("(DiskFileManager::check_names) File with this name (" + _path.full_name() + ") already exists");
	
		return _path.full_name();
	}

	void DiskFileManager::update_file_links(uintptr_t _old, uintptr_t _new)
	{
		std::string name = m_ptr_to_names[_old];

		m_file_hierarchy.change_ptr_for(_old, _new);

		m_names_to_ptr[name] = _new;
		m_ptr_to_names.erase(_old);
		m_ptr_to_names.emplace(_new, name);
	}

	void DiskFileManager::update_directory(const DiskPath& _path)
	{
		if (m_names_to_ptr.find(_path.full_name()) == m_names_to_ptr.end())
			throw FileDoesNotExist("(DiskFileManager::update_directory) File or directory " + _path.full_name() + " does not exist");

		if (_path.full_name() == "") return;

		uintptr_t old_ptr = m_names_to_ptr[_path.full_name()];
		bool if_system = m_sectors.is_system(m_names_to_ptr[_path.full_name()]);

		auto info = get_info(_path, if_system);
		info.changing_time = Time::now();
		DataQueue data = info.get_as_data();

		m_sectors.delete_file(old_ptr, if_system);
		uintptr_t new_ptr = m_sectors.put_raw_file(data, if_system);

		m_file_hierarchy.change_ptr_for(old_ptr, new_ptr);

		m_names_to_ptr[_path.full_name()] = new_ptr;
		m_ptr_to_names.erase(old_ptr);
		m_ptr_to_names.emplace(new_ptr, _path.full_name());

		if (_path.dir() != "") // check for root
			update_directory(DiskPath(_path.dir()));
	}

	void DiskFileManager::write(const DiskPath& _path, DataQueue& _new_data, bool system)
	{
		if (m_names_to_ptr.find(_path.full_name()) == m_names_to_ptr.end())
		{
			create(_path, Permissions(false, 0, 0, 0, 0), FileT::FILE, system);
		}
		else if (get_type(_path) == FileT::DIR)
		{
			throw FileIsBlocked("(DiskFileManager::write) You can't write into " + _path.full_name() + " directory");
		}
		
		uintptr_t old_file_ptr = m_names_to_ptr[_path.full_name()];
		
		size_t old_size = get_content_size(_path);
		if (old_size < _new_data.size())
		{
			size_t needs = _new_data.size() - old_size;
			if (m_sectors.get_free_space_size() < needs)
				throw NotEnoughFreeSpace("(DiskFileManager::write_file) Can't write into " + _path.full_name() + " file. Lack of memory");
		}

		FileInfo file_info = get_info(_path, system);
		file_info.changing_time = Time::now();
		DataQueue all_data = file_info.get_as_data();
		all_data.concat(_new_data);

		m_sectors.delete_file(old_file_ptr, system);
		uintptr_t new_file_ptr = m_sectors.put_raw_file(all_data, system);
		
		update_file_links(old_file_ptr, new_file_ptr);
		update_directory(_path.dir());

		reload();
	}

	DataQueue DiskFileManager::read(const DiskPath& _path, bool system)
	{
		if (m_names_to_ptr.find(_path.full_name()) == m_names_to_ptr.end())
			throw FileDoesNotExist("(DiskFileManager::read) File or directory (" + _path.full_name() + ") does not exist");
		if (get_type(_path) == FileT::DIR)
			throw FileIsBlocked("(DiskFileManager::read) You can't read from " + _path.full_name() + " directory");

		DataQueue data = m_sectors.get_raw_file(m_names_to_ptr[_path.full_name()], system);
		data.pop<Permissions>();
		data.pop<time_t>();
		data.pop<time_t>();
		
		return data;
	}

	FileT DiskFileManager::get_type(const DiskPath& _path)
	{
		if (m_names_to_ptr.find(_path.full_name()) == m_names_to_ptr.end())
			throw FileDoesNotExist("(DiskFileManager::get_type) File or directory (" + _path.full_name() + ") does not exist");

		return (m_file_hierarchy.is_directory(m_names_to_ptr[_path.full_name()]) ? FileT::DIR : FileT::FILE);
	}

	void DiskFileManager::remove(const DiskPath& _path, bool system, bool _first)
	{
		if (_first)
		{
			if (m_names_to_ptr.find(_path.full_name()) == m_names_to_ptr.end())
				throw FileDoesNotExist("(DiskFileManager::remove) File or directory (" + _path.full_name() + ") does not exist");
		}
		
		FileT file_type = get_type(_path);
		uintptr_t ptr = m_names_to_ptr[_path.full_name()];
		m_sectors.delete_file(ptr, system);

		std::string name = m_ptr_to_names[ptr];

		m_ptr_to_names.erase(ptr);
		m_names_to_ptr.erase(name);

		if (file_type == FileT::DIR)
		{
			const std::vector<HierarchyFileInfo>& vec = m_file_hierarchy.get_file_hierarchy().at(ptr);
			for (size_t i = 0; i < vec.size(); ++i)
			{
				try
				{
					remove(m_ptr_to_names[vec[i].start], system, false);
				}
				catch (const Exception& ex)
				{
					std::cout << ex.what() << std::endl;
				}
			}
		}

		if (_first)
		{
			m_file_hierarchy.remove_from_hierarchy(ptr);
			reload();
		}
	}

	void DiskFileManager::rename(const DiskPath& _path, const DiskPath& new_path, bool system, bool _first)
	{
		if (_first)
		{
			if (m_names_to_ptr.find(_path.full_name()) == m_names_to_ptr.end())
				throw FileDoesNotExist("(DiskFileManager::rename) File or directory (" + _path.full_name() + ") does not exist");

			if (!NameValidator::is_name_valid(new_path.file()))
				throw NameIsNotValid("(DiskFileManager::rename) Name (" + new_path.file() + ") must not include / \\ : * ? \" < > |");

			if (m_names_to_ptr.find(new_path.full_name()) != m_names_to_ptr.end())
				throw FileAlreadyExists("(DiskFileManager::rename) File or directory (" + new_path.full_name() + ") already exists");

			if (m_names_to_ptr.find(new_path.dir()) == m_names_to_ptr.end())
				throw DirectoryDoesntExist("(DiskFileManager::rename) Destination directory (" + _path.dir() + ") does not exist");
		}

		uintptr_t ptr = m_names_to_ptr[_path.full_name()];
		m_names_to_ptr.erase(_path.full_name());
		m_names_to_ptr.emplace(new_path.full_name(), ptr);
		m_ptr_to_names[ptr] = new_path.full_name();

		if (get_type(new_path) == FileT::DIR)
		{
			const std::vector<HierarchyFileInfo>& vec = m_file_hierarchy.get_file_hierarchy().at(ptr);
			for (size_t i = 0; i < vec.size(); ++i)
			{
				try
				{
					DiskPath old_sub_path(m_ptr_to_names[vec[i].start]);
					DiskPath new_sub_path(new_path.full_name(), old_sub_path.file());
					rename(m_ptr_to_names[vec[i].start], new_sub_path, system, false);
				}
				catch (const Exception& ex)
				{
					std::cout << ex.what() << std::endl;
				}
			}
		}

		if (_first)
		{
			reload();
		}
	}

	bool DiskFileManager::is_system(const DiskPath& _file)
	{
		return m_sectors.is_system(m_names_to_ptr[_file.full_name()]);
	}

	bool DiskFileManager::is_exists(const DiskPath& _file)
	{
		return (m_names_to_ptr.find(_file.full_name()) != m_names_to_ptr.end());
	}

	void DiskFileManager::copy(const DiskPath& _src, const DiskPath& _dst, bool system, bool _first)
	{
		if (_first)
		{
			if (m_names_to_ptr.find(_src.full_name()) == m_names_to_ptr.end())
				throw FileDoesNotExist("(DiskFileManager::move) File or directory (" + _src.full_name() + ") does not exist");

			if (m_names_to_ptr.find(_dst.dir()) == m_names_to_ptr.end())
				throw DirectoryDoesntExist("(DiskFileManager::move) Destination directory (" + _dst.full_name() + ") does not exist");

			if (get_type(_src) == FileT::DIR && _dst.full_name().starts_with(_src.full_name() + "\\"))
				throw RecursiveDirectoryMove("(DiskFileManager::mome) Source directory (" + _src.full_name() + ") contains destination directory (" + _dst.full_name() + ")");
		}

		FileInfo info = get_info(_src, system);
		create(_dst, info.permissions, get_type(_src), is_system(_src));
		
		if (get_type(_src) == FileT::DIR)
		{
			uintptr_t ptr = m_names_to_ptr[_src.full_name()];
			const std::vector<HierarchyFileInfo>& vec = m_file_hierarchy.get_file_hierarchy().at(ptr);
			for (size_t i = 0; i < vec.size(); ++i)
			{
				try
				{
					DiskPath path = DiskPath(m_ptr_to_names[vec[i].start]);
					copy(path, DiskPath(_dst.full_name(), path.file()), system, false);
				}
				catch (const Exception&) { }
			}
		}
		else
		{
			DataQueue data = read(_src, system);
			write(_dst, data, system);
		}
	}

	std::vector<std::string> DiskFileManager::list(const DiskPath& _dir, bool system)
	{
		if (m_names_to_ptr.find(_dir.full_name()) == m_names_to_ptr.end())
			throw FileDoesNotExist("(DiskFileManager::list) File or directory (" + _dir.full_name() + ") does not exist");

		if (get_type(_dir) != FileT::DIR)
			throw DirectoryDoesntExist("(DiskFileManager::list) " + _dir.full_name() + " isn't a directory");

		std::vector<std::string> list;

		uintptr_t ptr = m_names_to_ptr[_dir.full_name()];
		
		for (const auto& info : m_file_hierarchy.get_file_hierarchy().at(ptr))
			list.push_back(DiskPath(m_ptr_to_names[info.start]).file());
		
		return list;
	}

	
	void DiskFileManager::move(const DiskPath& _path, const DiskPath& new_dir, bool system, bool _first)
	{
		std::string new_dir_name = new_dir.full_name();

		if (m_names_to_ptr.find(_path.full_name()) == m_names_to_ptr.end())
			throw FileDoesNotExist("(DiskFileManager::move) File or directory (" + _path.full_name() + ") does not exist");

		if (m_names_to_ptr.find(new_dir_name) == m_names_to_ptr.end())
			throw DirectoryDoesntExist("(DiskFileManager::move) Destination directory (" + new_dir_name + ") does not exist");

		if (get_type(_path) == FileT::DIR && new_dir_name.starts_with(_path.full_name()))
			throw RecursiveDirectoryMove("(DiskFileManager::mome) Source directory (" + _path.full_name() + ") contains destination directory (" + new_dir_name + ")");

		uintptr_t ptr = m_names_to_ptr[_path.full_name()];
		uintptr_t dir_ptr = m_names_to_ptr[new_dir_name];

		std::string old_dir = _path.dir();

		m_file_hierarchy.move(ptr, dir_ptr);
		
		rename(_path, DiskPath(new_dir.full_name(), _path.file()), system);
	}

	void DiskFileManager::create(const DiskPath& _path, const Permissions& _per, FileT _type, bool system)
	{
		std::string new_name = check_names(_path, _type);

		time_t time = Time::now();

		FileInfo info(_per, time, time);
		DataQueue data = info.get_as_data();

		uintptr_t ptr = m_sectors.put_raw_file(data, system);
		
		if (_path.file() == "" && _type == FileT::DIR)
		{
			m_file_hierarchy.init(ptr);
		}
		else
		{
			uintptr_t higher_dir = m_names_to_ptr[_path.dir()];
			m_file_hierarchy.add_to_hierarchy(higher_dir, ptr, _type);
		}
		m_names_to_ptr.emplace(new_name, ptr);
		m_ptr_to_names.emplace(ptr, new_name);

		update_directory(_path.dir());
		reload();
	}
}