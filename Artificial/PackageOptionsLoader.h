#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "SpawnPackmiOption.h"
#include "CreatePackageOption.h"
#include "AddPackOption.h"
#include "PackmiOption.h"

namespace Commands
{
	class PackageOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("spawnpackmi", std::make_unique<SpawnPackmiOption>(core));
			controller.add_option("mkpack", std::make_unique<CreatePackageOption>(core));
			controller.add_option("addpack", std::make_unique<AddPackOption>(core));
			controller.add_option("packmi", std::make_unique<PackmiOption>(core));
			return controller;
		}
	};
}