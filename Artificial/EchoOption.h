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
	// e
	class EchoOption final : public AbstractControllerOption
	{
	public:
		EchoOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Prints text to terminal\n");
				ptr->print_secondary("e {line} [:color {1|2|3}]\n");
				ptr->print_main("  line - (string) line to output;\n");
				ptr->print_main("  :color 1|2|3 - (flag + integer) color console to apply to text (1 - main, 2 - secondary, 3 - third);\n");
				return;
			}
			
			if (_command.has(":color") && _command.get(":color") == "2")
				ptr->print_secondary(_command.get("1") + "\n");
			else if (_command.has(":color") && _command.get(":color") == "3")
				ptr->print_third(_command.get("1") + "\n");
			else
				ptr->print_main(_command.get("1") + "\n");

			return;
		}
	};
}