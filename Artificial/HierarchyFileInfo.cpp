#include "HierarchyFileInfo.h"

namespace Memory
{
	DataQueue HierarchyFileInfo::get_as_data() const
	{
		DataQueue data;
		data.push(start);
		data.push(type);
		return data;
	}

	void HierarchyFileInfo::load_from_data(DataQueue& _data)
	{
		start = _data.pop<uintptr_t>();
		type = _data.pop<FileT>();
	}
}
