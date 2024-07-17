#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "StringToColour.h"
#include <map>

namespace Commands
{
	// d
	class DialogLineOption final : public AbstractControllerOption
	{
	public:
		DialogLineOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Prints text to terminal\n");
				ptr->print_main("You can set timer between symbols and set if user need to press enter after line\n");
				ptr->print_secondary("d {line} [:color {1|2|3}] [:sl ms] [::noenter] [::bysymbol\n");
				ptr->print_main("  line - (string) line to output;\n");
				ptr->print_main("  :color 1|2|3 - (flag + integer) color console to apply to text (1 - main, 2 - secondary, 3 - third);\n");
				ptr->print_main("  :sl ms - (flag + integer) time in milliseconds between symbols printing;\n");
				ptr->print_main("  ::noenter - (flag) add if line must end immediately after all line is printed;\n");
				ptr->print_main("  ::bysymbol - (flag) prints line by symbols (by words by default);\n");

				return;
			}

			int ms = (_command.has(":sl") ? Parser::from_string<int>(_command.get(":sl")) : 50);
			
			if (_command.has("::bysymbol"))
			{
				for (char c : _command.get("1"))
				{
					if (_command.has(":color") && _command.get(":color") == "2")
						ptr->print_secondary(std::string(1, c));
					else if (_command.has(":color") && _command.get(":color") == "3")
						ptr->print_third(std::string(1, c));
					else
						ptr->print_main(std::string(1, c));
					std::this_thread::sleep_for(std::chrono::milliseconds(ms));
					ptr->rerender();
				}
				ptr->print_main(" ");
			}
			else
			{
				for (const auto& s : Separator::split(_command.get("1"), ' '))
				{
					if (_command.has(":color") && _command.get(":color") == "2")
						ptr->print_secondary(s + " ");
					else if (_command.has(":color") && _command.get(":color") == "3")
						ptr->print_third(s + " ");
					else
						ptr->print_main(s + " ");
					std::this_thread::sleep_for(std::chrono::milliseconds(ms) * s.size());

					ptr->rerender();
				}
			}
			ptr->rerender();

			if (_command.has("::noenter"))
			{
				ptr->print_main("\n");
				return;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(ms * 3));
			ptr->print_third("(*)");
			ptr->rerender();
			ptr->wait_for_input(GUI::TerminalInputType::TEXT);
			ptr->get_last_input();
			return;
		}
	};
}