#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// changeuser
	class ChangeUserOption final : public AbstractControllerOption
	{
	public:
		ChangeUserOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Switches you to another system user\n");
				ptr->print_secondary("changeuser {name}\n");
				ptr->print_main("  name - (string) user name;\n");
				
				return;
			}

			if (sender.lvl() < 0)
				throw PermissionException("(ChangeUserOption) Sender has low permission lvl");

			const auto& userpass = m_core.users().get_map().at(_command.get("1"));
			if (userpass.pass_hash != 0)
			{
				ptr->print_main("Input password: ");
				ptr->wait_for_input(GUI::TerminalInputType::PASSWORD);
				std::string input = ptr->get_last_input();
				m_core.users().try_change_user(_command.get("1"), input);
			}
			else
			{
				m_core.users().try_change_user(_command.get("1"));
			}
			ptr->set_user_name(m_core.users().get_current_user().name());
		}
	};
}
