#pragma once
#include "SectorName.h"
#include "DataQueue.h"
#include <string>
#include <vector>

namespace Core::Memory
{
	class RealFileManager final
	{
	public:
		RealFileManager() {}

		void write_into_real_file(SectorName _name, const DataQueue& _raw);
		DataQueue read_from_real_file(SectorName _name);
		void delete_real_file(SectorName _name);
	};
}