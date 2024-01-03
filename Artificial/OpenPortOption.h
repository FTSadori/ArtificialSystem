#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// openport
	class OpenPortOption final : public AbstractControllerOption
	{
	public:
		OpenPortOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Opens port for external files.\n");
				ptr->print_secondary("openport {port_number}\n");
				ptr->print_main("  port_number - (int) port number;\n");
				return;
			}

			if (!std::filesystem::exists(_command.get("1")))
				std::filesystem::create_directory(_command.get("1"));

			return;
		}
	};
}