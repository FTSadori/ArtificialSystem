#pragma once
#include "AbstractController.h"
#include "GUIHandler.h"
#include "MemoryCreateOption.h"

namespace Commands
{
	class MemoryOptionsLoader final
	{
	public:
		static AbstractController Load(Memory::DiskSystem& system, ICommandExecutor& core, GUI::GUIHandler& gui)
		{
			AbstractController controller;
			controller.add_option("mk", std::make_unique<MemoryCreateOption>(system, core, gui));
			return controller;
		}
	};
}