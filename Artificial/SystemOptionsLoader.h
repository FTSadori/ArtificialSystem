#pragma once
#include "BaseController.h"
#include "SystemCreationStartOption.h"
#include "SystemAddDiskOption.h"
#include "SystemDiskListOption.h"
#include "SystemStartOption.h"
#include "SystemClearDisksOption.h"
#include "SystemPreloadOption.h"

namespace Commands
{
	class SystemOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("syscreateinfo", std::make_unique<SystemCreationStartOption>(core));
			controller.add_option("adddisk", std::make_unique<SystemAddDiskOption>(core));
			controller.add_option("disklist", std::make_unique<SystemDiskListOption>(core));
			controller.add_option("cleardisks", std::make_unique<SystemClearDisksOption>(core));
			controller.add_option("startsystem", std::make_unique<SystemStartOption>(core));
			controller.add_option("preload", std::make_unique<SystemPreloadOption>(core));
			return controller;
		}
	};
}