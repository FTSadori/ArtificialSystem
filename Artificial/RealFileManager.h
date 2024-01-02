#pragma once
#include "SectorName.h"
#include "DataQueue.h"
#include <string>
#include <vector>

namespace Memory
{
	class RealFileManager final
	{
	public:
		RealFileManager() {}

		void write_into_real_file(const std::string& _folder, SectorName _name, const DataQueue& _raw);
		DataQueue read_from_real_file(const std::string& _folder, SectorName _name);
		DataQueue read_from_real_file(const std::string& _folder, const std::string& _name);
		void delete_real_file(const std::string& _folder, SectorName _name);
	};
}