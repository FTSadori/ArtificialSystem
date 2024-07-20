#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// addlvls
	class AddLvlsToUserOption final : public AbstractControllerOption
	{
	public:
		AddLvlsToUserOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n");
				ptr->print_main("Adds lvls to user\n");
				ptr->print_secondary("addlvls {name} {lvls}\n");
				ptr->print_main("  name - (string) user name;\n");
				ptr->print_main("  lvls - (int) extra levels;\n");

				return;
			}

			if (!sender.system())
				throw PermissionException("(AddLvlsToUserOption) Sender has low permission lvl");

			auto& data = m_core.users().get_data().m_data;
			if (!data.contains(_command.get("1")))
				throw CommandException("(AddLvlsToUserOption) Wrong user name");
			data.at(_command.get("1")).user.set_lvl(
				data.at(_command.get("1")).user.lvl() + Parser::from_string<int>(_command.get("2"))
			);
		}
	};
}
