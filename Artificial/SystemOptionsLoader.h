#pragma once
#include "BaseController.h"
#include "SystemCreationStartOption.h"
#include "SystemDiskAddOption.h"
#include "SystemDiskListOption.h"
#include "SystemDiskClearOption.h"

namespace Commands
{
	class SystemOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("syscreateinfo", std::make_unique<SystemCreationStartOption>(core));
			controller.add_option("diskadd", std::make_unique<SystemDiskAddOption>(core));
			controller.add_option("disklist", std::make_unique<SystemDiskListOption>(core));
			controller.add_option("diskclear", std::make_unique<SystemDiskClearOption>(core));
			return controller;
		}
	};
}