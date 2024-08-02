#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "RunProgramOption.h"
#include "ShowOpenedTasksOption.h"
#include "ShowTaskDetailsOption.h"
#include "RunTestsOption.h"
#include "MovaVersionLoadOption.h"
#include "OpenTaskOption.h"
#include "SkipTaskOption.h"

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
			controller.add_option("runtests", std::make_unique<RunTestsOption>(core));
			controller.add_option("movavload", std::make_unique<MovaVersionLoadOption>(core));
			controller.add_option("opentask", std::make_unique<OpenTaskOption>(core));
			controller.add_option("opentask", std::make_unique<OpenTaskOption>(core));
			controller.add_option("skiptask", std::make_unique<SkipTaskOption>(core));
			return controller;
		}
	};
}