#pragma once
#include "DiskSystem.h"
#include "GUIHandler.h"
#include "ColoursThemesHandler.h"
#include "ICommandExecutor.h"
#include "PasswordHandler.h"
#include "NetContainer.h"

namespace Commands
{
	class ICore : public ICommandExecutor
	{
	public:
		virtual UsersHandler& users() = 0;
		virtual GUI::GUIHandler& gui() = 0;
		virtual GUI::ColoursThemesHandler& themes() = 0;
		virtual Memory::DiskSystem& memory() = 0;
		virtual Memory::DiskSystemInfo& memory_info() = 0;
		virtual PasswordHandler& passwords() = 0;
		virtual NetContainer& net() = 0;

		virtual ~ICore() = default;
	};
}