#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "MemoryCreateOption.h"
#include "ShowFileListOption.h"

namespace Commands
{
	class MemoryOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			//controller.add_option("mk", std::make_unique<MemoryCreateOption>(core));
			controller.add_option("ls", std::make_unique<ShowFileListOption>(core));
			return controller;
		}
	};
}