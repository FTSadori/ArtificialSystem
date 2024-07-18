#pragma once
#include "Core.h"
#include "MemoryOptionsLoader.h"
#include "SystemOptionsLoader.h"
#include "UsersOptionsLoader.h"
#include "WindowOptionsLoader.h"
#include "HelpOptionsLoader.h"
#include "DevicesOptionsLoader.h"
#include "NetOptionsLoader.h"
#include "NetFilesLoader.h"
#include "MovaOptionsLoader.h"
#include "StoryOptionsLoader.h"
#include "PackageOptionsLoader.h"

namespace Commands
{
	class CoreLoader final
	{
	public:
		static void load(Core& core)
		{
			core.add_controller(SystemOptionsLoader::Load(core));
			core.add_controller(UsersOptionsLoader::Load(core));
			core.add_controller(MemoryOptionsLoader::Load(core));
			core.add_controller(WindowOptionsLoader::Load(core));
			core.add_controller(HelpOptionsLoader::Load(core));
			core.add_controller(DevicesOptionsLoader::Load(core));
			core.add_controller(NetOptionsLoader::Load(core));
			core.add_controller(MovaOptionsLoader::Load(core));
			core.add_controller(StoryOptionsLoader::Load(core));
			core.add_controller(PackageOptionsLoader::Load(core));
			NetFilesLoader::load(core);

			core.load();
		}
	};
}