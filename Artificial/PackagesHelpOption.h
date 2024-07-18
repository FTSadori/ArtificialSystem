#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"

namespace Commands
{
	// helppackages
	class PackagesHelpOption final : public AbstractControllerOption
	{
	public:
		PackagesHelpOption(ICore& _core)
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
			ptr->print_third("- spawnpackmi\n");
			ptr->print_third("- mkpack\n");
			ptr->print_third("- addpack\n");

			return;
		}
	};
}