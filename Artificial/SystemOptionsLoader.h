#pragma once
#include "BaseController.h"
#include "SystemCreationStartOption.h"

namespace Commands
{
	class SystemOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("syscreateinfo", std::make_unique<SystemCreationStartOption>(core));
			return controller;
		}
	};
}