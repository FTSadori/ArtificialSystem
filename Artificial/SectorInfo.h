#pragma once
#include <cstdint>
#include "ILoadable.h"

namespace Memory
{
	struct SectorInfo final : ILoadable
	{
		uintptr_t start = 0;
		size_t size = 0;
		bool system = false;
		bool first_in_chain = true;
		uintptr_t next = 0;

		SectorInfo() {}
		SectorInfo(uintptr_t _start, size_t _size, bool _system, bool _first_in_chain, uintptr_t _next)
			: start(_start), size(_size), system(_system), first_in_chain(_first_in_chain), next(_next) {}

		virtual DataQueue get_as_data() const override final;
		virtual void load_from_data(DataQueue& _data) override final;
	};

	struct FreeSectorInfo final
	{
		uintptr_t start;
		size_t capacity;
	};
}
