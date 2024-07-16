#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "SpawnNovaOption.h"
#include "GoNovaOption.h"
#include "CreateItemOption.h"
#include "PickItemOption.h"

namespace Commands
{
	class StoryOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("spawnnova", std::make_unique<SpawnNovaOption>(core));
			controller.add_option("go", std::make_unique<GoNovaOption>(core));
			controller.add_option("mkitem", std::make_unique<CreateItemOption>(core));
			controller.add_option("pick", std::make_unique<PickItemOption>(core));
			return controller;
		}
	};
}