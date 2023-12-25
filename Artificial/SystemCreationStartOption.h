#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"

namespace Commands
{
	// syscreateinfo
	class SystemCreationStartOption final : public AbstractControllerOption
	{
	public:
		SystemCreationStartOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("AMOGUS\n");
				return;
			}

			if (sender.lvl() < 255)
				throw PermissionException("(SystemCreationStartOption) Sender has low permission lvl");

			ptr->print_secondary("=============================\n");
			ptr->print_secondary("WELCOME TO ARTIFICIAL SYSTEM!\n");
			ptr->print_secondary("=============================\n");
			ptr->print_main("Now you should start setting up you future system.\n");
			
			ptr->print_main("Use next commands:\n");
			ptr->print_secondary("- adddisk {mark} {capacity} {max_sec_num} [::main]");
			ptr->print_main(" - add new disk partition\n");
			ptr->print_secondary("- cleardisks");
			ptr->print_main(" - clear partition info\n");
			ptr->print_secondary("- disklist");
			ptr->print_main(" - show partition info\n");

			ptr->print_third("- adduser {name} {lvl} [:p password] [::root]");
			ptr->print_main(" - add new user\n");
			ptr->print_third("- clearuser {name}");
			ptr->print_main(" - clear user by name\n");
			ptr->print_third("- userlist");
			ptr->print_main(" - show users info\n");

			ptr->print_secondary("- startsystem");
			ptr->print_main(" - end preparation\n");

			ptr->print_main("Use ::help flag in any command to see more information about it.\n\n");

			return;
		}
	};
}
