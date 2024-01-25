#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "RunProgramOption.h"

namespace Commands
{
	class MovaOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("mova", std::make_unique<RunProgramOption>(core));
			return controller;
		}
	};
}