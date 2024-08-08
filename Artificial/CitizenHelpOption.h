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
	// citizenhelp
	class CitizenHelpOption final : public AbstractControllerOption
	{
	public:
		CitizenHelpOption(ICore& _core)
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
			ptr->print_third("- nano (11 lvl)\n");
			ptr->print_third("- mk (12 lvl)\n");
			ptr->print_third("- mova (13 lvl)\n");
			ptr->print_third("- tasks (13 lvl)\n");
			ptr->print_third("- showtask (13 lvl)\n");
			ptr->print_third("- runtests (13 lvl)\n");
			ptr->print_third("- skiptask (13 lvl)\n");
			ptr->print_third("- rm (99 lvl)\n");
			
			return;
		}
	};
}