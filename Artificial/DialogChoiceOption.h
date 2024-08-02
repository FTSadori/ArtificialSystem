#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Separator.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "ItemUsingHandler.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// choice
	class DialogChoiceOption final : public AbstractControllerOption
	{
	public:
		DialogChoiceOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Gives the user a choice\n");
				ptr->print_secondary("choice [:op1 option1] [:sc1 script1] [:op2 option2] [:sc2 script2] ...\n");
				ptr->print_main("  :op1 option1 - (string) option to choose;\n");
				ptr->print_main("  :sc1 script1 - (string) script that activates after choice;\n");
				return;
			}

			using namespace Memory;

			std::vector<std::string> scripts;
			int i = 1;
			while (_command.has(":op" + Parser::to_string(i)) && _command.has(":sc" + Parser::to_string(i)))
			{
				FullPath str_path = RelativePathCreator::combine(_command.get("path"), _command.get(":sc" + Parser::to_string(i)));
				scripts.emplace_back(str_path.full_path_str());
				ptr->print_secondary(Parser::to_string(i) + ". -> " + _command.get(":op" + Parser::to_string(i)) + "\n");
				ptr->rerender();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				i++;
			}

			int number = 0;
			while (number == 0 || number > scripts.size())
			{
				ptr->print_main("Choose number -> ");
				ptr->rerender();
				ptr->wait_for_input(GUI::TerminalInputType::TEXT);
				number = Parser::from_string<int>(ptr->get_last_input());
			}

			Command cmd("\"" + ptr->get_path().full_path_str() + "\" run \"" + scripts[number - 1] + "\"");
			m_core.execute(cmd, User("amogus", true, 255));


			return;
		}
	};
}