#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// deleteuser
	class DeleteUserOption final : public AbstractControllerOption
	{
	public:
		DeleteUserOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("20 permission lvl needed\n");
				ptr->print_main("Deletes system user by name\n");
				ptr->print_secondary("deleteuser {name}\n");
				ptr->print_main("  name - (string) user name;\n");

				return;
			}

			if (sender.lvl() < 20)
				throw PermissionException("(DeleteUserOption) Sender has low permission lvl");

			if (ptr->get_user_name() == _command.get("1"))
				throw CannotDeleteCurrentUserException("(DeleteUserOption) You can't delete current user, try switch to another one");

			const auto& userpass = m_core.users().get_map().at(_command.get("1"));
			if (userpass.pass_hash != 0)
			{
				ptr->print_main("Input password for " + _command.get("1") + ": ");
				ptr->wait_for_input(GUI::TerminalInputType::PASSWORD);
				std::string input = ptr->get_last_input();
			}
			m_core.users().remove_user(_command.get("1"));
			ptr->set_user_name(m_core.users().get_current_user().name());
		}
	};
}
