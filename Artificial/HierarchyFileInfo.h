#pragma once
#include <cstdint>
#include "ILoadable.h"
#include "FileType.h"

namespace Core::Memory
{
	struct HierarchyFileInfo final : public ILoadable
	{
		uintptr_t start = 0;
		FileT type = FileT::FILE;

		HierarchyFileInfo() {}
		HierarchyFileInfo(uintptr_t _start, FileT _type)
			: start(_start), type(_type) {}

		virtual DataQueue get_as_data() const override;
		virtual void load_from_data(DataQueue& _data) override;
	};
}
