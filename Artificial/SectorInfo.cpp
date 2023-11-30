#include "SectorInfo.h"
#include "DataQueue.h"
#include <sstream>

namespace Memory 
{
	DataQueue SectorInfo::get_as_data() const
	{
		DataQueue data;
		data.push(start);
		data.push(size);
		data.push(system);
		data.push(first_in_chain);
		data.push(next);

		return data;
	}

	void SectorInfo::load_from_data(DataQueue& _data)
	{
		start = _data.pop<uintptr_t>();
		size = _data.pop<size_t>();
		system = _data.pop<bool>();
		first_in_chain = _data.pop<bool>();
		next = _data.pop<uintptr_t>();
	}
}
