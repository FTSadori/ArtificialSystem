#pragma once
#include "CommandExecutor.h"
#include "DiskSystem.h"
#include "GUIHandler.h"
#include "ColoursThemesHandler.h"

namespace Commands
{
	class ICore
	{
	public:
		ICore() = default;

		virtual UsersHandler& users() = 0;
		virtual GUI::GUIHandler& gui() = 0;
		virtual GUI::ColoursThemesHandler& themes() = 0;
		virtual Memory::DiskSystem& memory() = 0;
		virtual Memory::DiskSystemInfo& memory_info() = 0;
	};
}