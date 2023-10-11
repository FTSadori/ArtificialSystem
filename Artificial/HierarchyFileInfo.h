#pragma once
#include <cstdint>
#include "ILoadable.h"

namespace Core::Memory
{
	struct HierarchyFileInfo final : ILoadable
	{
		uintptr_t start = 0;
		bool is_directory = false;

		HierarchyFileInfo() {}
		HierarchyFileInfo(uintptr_t _start, bool _is_directory)
			: start(_start), is_directory(_is_directory) {}

		virtual DataQueue get_as_data() const override;
		virtual void load_from_data(DataQueue& _data) override;
	};
}
