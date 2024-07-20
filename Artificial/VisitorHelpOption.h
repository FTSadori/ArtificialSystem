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
	// visitorhelp
	class VisitorHelpOption final : public AbstractControllerOption
	{
	public:
		VisitorHelpOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Why?\n");
				return;
			}

			ptr->print_main("Use next commands with ::help to get more info:\n");
			ptr->print_third("- ls (0 lvl)\n");
			ptr->print_third("- go (1 lvl)\n");
			ptr->print_third("- lookaround (1 lvl)\n");
			ptr->print_third("- pick (2 lvl)\n");
			ptr->print_third("- inv (2 lvl)\n");
			ptr->print_third("- useitem (2 lvl)\n");
			ptr->print_third("- talkto (3 lvl)\n");

			return;
		}
	};
}