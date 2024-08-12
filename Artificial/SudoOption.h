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
	// sudo
	class SudoOption final : public AbstractControllerOption
	{
	public:
		SudoOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Executes commands like system user\n");
				ptr->print_secondary("sudo {command...}\n");
				ptr->print_main("  command... - (string) full command string to execute;\n");
				return;
			}

			std::hash<std::string> hasher;
			m_core.passwords().check_password(ptr, hasher("69"));

			std::string bigbrainmove = _command.getwholeline().substr(_command.get("path").size());

			int w = bigbrainmove.find_first_of('o');
			Command cmd("\"" + _command.get("path") + "\" " + bigbrainmove.substr(w + 1));
			m_core.execute(cmd, User("hand of god", true, 255));

			return;
		}
	};
}