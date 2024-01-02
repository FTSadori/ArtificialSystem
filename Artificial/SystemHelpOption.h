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
	// helpsystem
	class SystemHelpOption final : public AbstractControllerOption
	{
	public:
		SystemHelpOption(ICore& _core)
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
			ptr->print_third("- adddisk\n");
			ptr->print_third("- cleardisks\n");
			ptr->print_third("- startsystem\n");
			ptr->print_third("- disklist\n");
			
			return;
		}
	};
}