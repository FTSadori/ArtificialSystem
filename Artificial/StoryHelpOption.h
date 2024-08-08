#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"

namespace Commands
{
	// helpstory
	class StoryHelpOption final : public AbstractControllerOption
	{
	public:
		StoryHelpOption(ICore& _core)
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
			ptr->print_third("- go\n");
			ptr->print_third("- talkto\n");
			ptr->print_third("- pick\n");
			ptr->print_third("- inv\n");
			ptr->print_third("- useitem\n");
			ptr->print_third("- d\n");
			ptr->print_third("- setnewdialog\n");
			ptr->print_third("- setnewpath\n");
			ptr->print_third("- mkitem\n");
			ptr->print_third("- choice\n");
			ptr->print_third("- lookaround\n");
			ptr->print_third("- spawnnova\n");
			ptr->print_third("- callnova\n");
			ptr->print_third("- showstats\n");
			ptr->print_third("- choosecore\n");
			ptr->print_third("- killcore\n");

			return;
		}
	};
}