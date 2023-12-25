#pragma once
#include "BaseController.h"
#include "GUIHandler.h"
#include "MemoryCreateOption.h"

namespace Commands
{
	class MemoryOptionsLoader final
	{
	public:
		static BaseController Load(ICommandExecutor& core, GUI::GUIHandler& gui, Memory::DiskSystem& system)
		{
			BaseController controller;
			controller.add_option("mk", std::make_unique<MemoryCreateOption>(system, core, gui));
			return controller;
		}
	};
}