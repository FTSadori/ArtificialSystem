#pragma once
#include <vector>
#include "Path.h"

namespace Core::Memory
{
	class ILoadable
	{
	public:
		virtual uintptr_t get_load_file_ptr() = 0;
		virtual std::vector<uint8_t> get_as_data() = 0;
		virtual void load_from_data(const std::vector<uint8_t>& _data) = 0;
	};
}
