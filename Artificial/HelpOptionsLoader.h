#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "BaseHelpOption.h"
#include "MemoryHelpOption.h"
#include "UserHelpOption.h"
#include "SystemHelpOption.h"
#include "WindowsHelpOption.h"
#include "DevicesHelpOption.h"
#include "NetHelpOption.h"
#include "MovaHelpOption.h"

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
			controller.add_option("helpdevices", std::make_unique<DevicesHelpOption>(core));
			controller.add_option("helpnet", std::make_unique<NetHelpOption>(core));
			controller.add_option("helpmova", std::make_unique<MovaHelpOption>(core));
			return controller;
		}
	};
}