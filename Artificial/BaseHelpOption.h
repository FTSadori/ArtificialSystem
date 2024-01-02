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
	// help
	class BaseHelpOption final : public AbstractControllerOption
	{
	public:
		BaseHelpOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Why?\n");
				return;
			}

			ptr->print_main("Use next commands to get more info:\n");
			ptr->print_third("- helpmemory\n");
			ptr->print_third("- helpsystem\n");
			ptr->print_third("- helpusers\n");
			ptr->print_third("- helpwindows\n");

			return;
		}
	};
}