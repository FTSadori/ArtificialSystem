#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// userlist
	class UserListOption final : public AbstractControllerOption
	{
	public:
		UserListOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Shows a list with all system users\n");
				
				return;
			}

			if (sender.lvl() < 0)
				throw PermissionException("(UserListOption) Sender has low permission lvl");

			size_t i = 0;
			const auto& map = m_core.users().get_data().m_data;
			for (const auto& [name, user] : map)
			{
				ptr->print_main(Parser::to_string(i++) + ". ");
				ptr->print_secondary(name + " ");
				ptr->print_main(Parser::to_string((int) user.user.lvl()) + " lvl");
				if (user.user.sudo())
					ptr->print_secondary(" (root)");
				ptr->print_secondary("\n");
			}
		}
	};
}
