#include "HierarchyFileInfo.h"

namespace Core::Memory
{
	DataQueue HierarchyFileInfo::get_as_data() const
	{
		DataQueue data;
		data.push(start);
		data.push(is_directory);
		return data;
	}

	void HierarchyFileInfo::load_from_data(DataQueue& _data)
	{
		start = _data.pop<uintptr_t>();
		is_directory = _data.pop<bool>();
	}
}
