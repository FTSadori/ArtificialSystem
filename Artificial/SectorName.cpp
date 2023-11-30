#include "SectorName.h"
#include <sstream>

namespace Memory
{
	SectorName::SectorName(const std::string& _mark, uintptr_t _sector)
	{
		std::hash<uintptr_t> h_sector;
		std::hash<std::string> h_mark;

		std::stringstream ss;
		ss << std::hex << h_mark(_mark) << h_sector(_sector) << ".sec";

		name = ss.str();
	}

	std::string SectorName::get_name()
	{
		return name;
	}
}