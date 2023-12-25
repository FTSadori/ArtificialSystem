#pragma once
#include "BaseController.h"
#include "SystemCreationStartOption.h"
#include "SystemAddDiskOption.h"
#include "SystemDiskListOption.h"
#include "SystemClearDisksOption.h"

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
			return controller;
		}
	};
}