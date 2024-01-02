#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include <map>

namespace Commands
{
	// helpmemory
	class MemoryHelpOption final : public AbstractControllerOption
	{
	public:
		MemoryHelpOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("\n");
				return;
			}

			ptr->print_main("Use next commands with ::help to get more info:\n");
			ptr->print_third("- cd\n");
			ptr->print_third("- chmod\n");
			ptr->print_third("- copy\n");
			ptr->print_third("- mk\n");
			ptr->print_third("- rm\n");
			ptr->print_third("- move\n");
			ptr->print_third("- write\n");
			ptr->print_third("- rename\n");
			ptr->print_third("- ls\n");

			return;
		}
	};
}