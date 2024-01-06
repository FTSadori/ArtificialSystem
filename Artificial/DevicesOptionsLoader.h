#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "LoadFileFromPortOption.h"
#include "OpenPortOption.h"

namespace Commands
{
	class DevicesOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("fromport", std::make_unique<LoadFileFromPortOption>(core));
			controller.add_option("openport", std::make_unique<OpenPortOption>(core));
			return controller;
		}
	};
}