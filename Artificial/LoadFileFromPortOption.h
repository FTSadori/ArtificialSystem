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
	// fromport
	class LoadFileFromPortOption final : public AbstractControllerOption
	{
	public:
		LoadFileFromPortOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Loads file from port with entered number to memory.\n");
				ptr->print_secondary("fromport {port_number} {external_file_name} {new_file_name}\n");
				ptr->print_main("  port_number - (int) port number;\n");
				ptr->print_main("  external_file_name - (string) name of the file in port;\n");
				ptr->print_main("  new_file_name - (string) name of created file;\n");
				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("3"));

			if (!std::filesystem::exists(_command.get("1")))
				throw CommandException("(LoadFileFromPortOption) Can't find " + _command.get("1") + " port");

			Command create_command("\"" + _command.get("path") + "\" mk \"" + _command.get("3") + "\"");
			m_core.execute(create_command, sender);

			Memory::RealFileManager fm;
			auto data = fm.read_from_real_file(_command.get("1"), _command.get("2"));
			std::string str = std::string(data.get_data(), data.size());

			Command write_command("\"" + _command.get("path") + "\" write \"" + _command.get("3") + "\" " + b64encode(str));
			m_core.execute(write_command, sender);

			return;
		}
	};
}