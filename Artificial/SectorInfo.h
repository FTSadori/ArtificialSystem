#pragma once
#include <cstdint>

namespace Core::Memory
{
	struct SectorInfo final
	{
		uintptr_t start;
		size_t size;
		bool system;
		bool first_in_chain;
		uintptr_t next;
	};

	struct FreeSectorInfo final
	{
		uintptr_t start;
		size_t capacity;
	};
}
