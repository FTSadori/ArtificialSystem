#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "RunProgramOption.h"
#include "ShowOpenedTasksOption.h"
#include "ShowTaskDetailsOption.h"

namespace Commands
{
	class MovaOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("mova", std::make_unique<RunProgramOption>(core));
			controller.add_option("tasks", std::make_unique<ShowOpenedTasksOption>(core));
			controller.add_option("showtask", std::make_unique<ShowTaskDetailsOption>(core));
			return controller;
		}
	};
}