#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "PingOption.h"

namespace Commands
{
	class NetOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("ping", std::make_unique<PingOption>(core));
			return controller;
		}
	};
}