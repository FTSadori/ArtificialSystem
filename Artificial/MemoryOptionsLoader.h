#pragma once
#include "MemoryController.h"
#include "MemoryCreateOption.h"

namespace Commands
{
	static class MemoryOptionsLoader final
	{
	public:
		static MemoryController Load(Memory::DiskSystem& system, ICommandExecutor& core)
		{
			MemoryController controller(system);
			controller.add_option("mk", std::make_unique<MemoryCreateOption>(system, core));
			return controller;
		}
	};
}