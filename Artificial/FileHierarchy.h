#pragma once
#include "HierarchyFileInfo.h"
#include "FileType.h"
#include <unordered_map>

namespace Core::Memory
{
	class FileHierarchy final : public ILoadable
	{
	public:
		FileHierarchy() = default;
		FileHierarchy(uintptr_t _root_dir);

		void init(uintptr_t _root_dir);
		bool is_inited() const;

		uintptr_t get_root_dir() const;

		bool is_directory(uintptr_t _ptr) const;

		void add_to_hierarchy(uintptr_t _dir, uintptr_t _start, FileT _type);
		void remove_from_hierarchy(uintptr_t _start);
		void change_ptr_for(uintptr_t _from, uintptr_t _to);

		void move(uintptr_t _start, uintptr_t _new_dir);

		virtual DataQueue get_as_data() const override final;
		virtual void load_from_data(DataQueue& _data) override final;

		const std::unordered_map<uintptr_t, std::vector<HierarchyFileInfo>>& get_file_hierarchy() const;
		const std::unordered_map<uintptr_t, uintptr_t>& get_higher_dir() const;

	private:
		std::vector<HierarchyFileInfo>::iterator find_data_about(uintptr_t _start);

		std::unordered_map<uintptr_t, std::vector<HierarchyFileInfo>> m_file_hierarchy;
		std::unordered_map<uintptr_t, uintptr_t> m_higher_dir;

		uintptr_t m_root_dir = 0;
	};
}
