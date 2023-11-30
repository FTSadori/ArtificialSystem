#pragma once
#include "FileHierarchy.h"
#include "Exceptions.h"
#include "FileHierarchyExceptions.h"
#include "Sectors.h"
#include "FileInfo.h"
#include "DiskPath.h"
#include "Time.h"
#include "NameValidator.h"
#include <iostream>

namespace Memory
{
	class DiskFileManager final : public ILoadable
	{
	public:
		const size_t capacity;

		DiskFileManager(const std::string& _folder, size_t _capacity, const std::string& _mark, uint32_t _max_sec_num);

		void save_hierarchy_data();

		uintptr_t get_root_dir();

		FileT get_type(const DiskPath& _path);
		size_t get_content_size(const DiskPath& _path);
		FileInfo get_info(const DiskPath& _path, bool system);
		
		void change_info(const DiskPath& _path, Permissions _per, bool system);
		
		void create(const DiskPath& _path, const Permissions& _per, FileT _type, bool system);
		void remove(const DiskPath& _path, bool system, bool _first = true);
		void rename(const DiskPath& _path, const DiskPath& new_path, bool system, bool _first = true);
		void move(const DiskPath& _path, const DiskPath& new_dir, bool system, bool _first = true);
		void write(const DiskPath& _path, DataQueue& _new_data, bool system);
		DataQueue read(const DiskPath& _path, bool system);
		void copy(const DiskPath& _src, const DiskPath& _dst, bool system, bool _first = true);
		bool is_system(const DiskPath& _file);
		bool is_exists(const DiskPath& _file);

		std::vector<std::string> list(const DiskPath& _dir, bool system);

		void reload();
		virtual DataQueue get_as_data() const override;
		virtual void load_from_data(DataQueue& _data) override;

	private:
		void update_file_links(uintptr_t _old, uintptr_t _new);
		void update_directory(const DiskPath& _path);

		std::string check_names(const DiskPath& _path, FileT _type);
		
		std::unordered_map<std::string, uintptr_t> m_names_to_ptr;
		FileHierarchy m_file_hierarchy;
		Sectors m_sectors;

		uintptr_t m_hierarchy_data_ptr = 0;
		uintptr_t m_names_data_ptr = 0;

		std::unordered_map<uintptr_t, std::string> m_ptr_to_names;
	};
}