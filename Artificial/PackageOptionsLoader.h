#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "SpawnPackmiOption.h"

namespace Commands
{
	class PackageOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("spawnpackmi", std::make_unique<SpawnPackmiOption>(core));

			return controller;
		}
	};
}