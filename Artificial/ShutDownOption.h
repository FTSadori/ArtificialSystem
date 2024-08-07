#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"
#include "Core.h"
#include <filesystem>

namespace Commands
{
	// shutdown
	class ShutDownOption final : public AbstractControllerOption
	{
	public:
		ShutDownOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("0 or 255 permission lvl needed\n");
				ptr->print_main("Shuts down system now or after several seconds.\n");
				ptr->print_secondary("shutdown [:after num] [::DELETESYSTEM]\n");
				ptr->print_main("  :after num - (flag + int) freezes system for 'num' seconds;\n");
				ptr->print_main("  ::DELETESYSTEM - (flag) needs 255 lvl, removes all content from Inner directory;\n");

				return;
			}

			if (_command.has(":after"))
				std::this_thread::sleep_for(std::chrono::seconds(Parser::from_string<size_t>(_command.get(":after"))));
			
			if (_command.has("::DELETESYSTEM"))
			{
				std::filesystem::remove_all("Inner");
			}

			exit(0);
		}
	};
}
