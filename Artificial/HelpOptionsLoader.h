#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "BaseHelpOption.h"
#include "MemoryHelpOption.h"
#include "UserHelpOption.h"
#include "SystemHelpOption.h"
#include "WindowsHelpOption.h"

namespace Commands
{
	class HelpOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("help", std::make_unique<BaseHelpOption>(core));
			controller.add_option("helpmemory", std::make_unique<MemoryHelpOption>(core));
			controller.add_option("helpsystem", std::make_unique<SystemHelpOption>(core));
			controller.add_option("helpuser", std::make_unique<UserHelpOption>(core));
			controller.add_option("helpwindows", std::make_unique<WindowsHelpOption>(core));
			return controller;
		}
	};
}