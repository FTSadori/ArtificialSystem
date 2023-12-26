#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "MemoryCreateOption.h"
#include "ShowFileListOption.h"
#include "ChangeDirectoryOption.h"
#include "MemoryDeleteOption.h"
#include "MemoryWriteOption.h"

namespace Commands
{
	class MemoryOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("mk", std::make_unique<MemoryCreateOption>(core));
			controller.add_option("ls", std::make_unique<ShowFileListOption>(core));
			controller.add_option("cd", std::make_unique<ChangeDirectoryOption>(core));
			controller.add_option("rm", std::make_unique<MemoryDeleteOption>(core));
			controller.add_option("write", std::make_unique<MemoryWriteOption>(core));
			return controller;
		}
	};
}