#pragma once
#include "BaseController.h"
#include "SystemCreationStartOption.h"
#include "UsersHandler.h"

namespace Commands
{
	class SystemOptionsLoader final
	{
	public:
		static BaseController Load(ICommandExecutor& core, GUI::GUIHandler& gui, Memory::DiskSystem& system, UsersHandler& users)
		{
			BaseController controller;
			controller.add_option("syscreateinfo", std::make_unique<SystemCreationStartOption>(core, gui));
			return controller;
		}
	};
}