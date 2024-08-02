#pragma once
#include "Core.h"

namespace Commands
{
	class NetFilesLoader final
	{
	public:
		static void load(Core& core)
		{
			std::hash<std::string> hasher;

			core.net().add_new("www.mova.com", NetData("Ylc5MllRPT0=", "___password___.txt", 0, 0));
		}
	};
}