#pragma once
#include "Permissions.h"
#include <string>

namespace Core::Memory
{
	class SectorName final
	{
	private:
		std::string name;

	public:
		SectorName(const std::string& _mark, uintptr_t _sector);

		std::string get_name();
	};
}
