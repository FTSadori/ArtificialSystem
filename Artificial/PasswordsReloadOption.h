#pragma once
#include "CommandExceptions.h"
#include "Core.h"
#include "TaskStateHandler.h"

namespace Commands
{
	// preload
	class PasswordsReloadOption final : public AbstractControllerOption
	{
	public:
		PasswordsReloadOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n");
				ptr->print_main("Makes PasswordHandler forget about entered passwords.\n");

				return;
			}

			if (!sender.system())
				throw PermissionException("(SystemPreloadOption) Sender has low permission lvl");

			m_core.passwords().reset();
		}
	};
}
