#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"

namespace Commands
{
	class NetOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			//controller.add_option("mk", std::make_unique<MemoryCreateOption>(core));
			return controller;
		}
	};
}