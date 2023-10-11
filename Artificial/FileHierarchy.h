#pragma once
#include "FolderInfo.h"
#include "HierarchyFileInfo.h"
#include <unordered_map>

namespace Core::Memory
{
	class FileHierarchy final : public ILoadable
	{
	public:
		FileHierarchy(uintptr_t _root_dir);

		void add_to_hierarchy(uintptr_t _dir, uintptr_t _start, bool is_folder);
		void remove_from_hierarchy(uintptr_t _start);
		void change_ptr_for(uintptr_t _from, uintptr_t _to);

		virtual DataQueue get_as_data() const override final;
		virtual void load_from_data(DataQueue& _data) override final;

		const std::unordered_map<uintptr_t, std::vector<HierarchyFileInfo>>& get_file_hierarchy();
		const std::unordered_map<uintptr_t, uintptr_t>& get_higher_dir();
	
	private:
		std::vector<HierarchyFileInfo>::iterator find_data_about(uintptr_t _start);

		std::unordered_map<uintptr_t, std::vector<HierarchyFileInfo>> m_file_hierarchy;
		std::unordered_map<uintptr_t, uintptr_t> m_higher_dir;

		uintptr_t m_root_dir;
	};
}
