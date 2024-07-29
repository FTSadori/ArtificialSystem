#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include <map>
#include <fstream>
#include <filesystem>

namespace Commands
{
	// allfromport
	class LoadAllFromPortOption final : public AbstractControllerOption
	{
	public:
		LoadAllFromPortOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Loads all files from port with entered number to memory.\n");
				ptr->print_secondary("fromport {port_number} {external_dir}\n");
				ptr->print_main("  port_number - (int) port number;\n");
				ptr->print_main("  external_dir - (string) name of the dir in port;\n");
				return;
			}

			if (!std::filesystem::exists(_command.get("1") + "\\" + _command.get("2")))
				throw CommandException("(LoadAllFromPortOption) Can't find " + _command.get("1") + "\\" + _command.get("2") + " dir");

			for (const auto& entry : std::filesystem::directory_iterator(_command.get("1") + "\\" + _command.get("2")))
			{
				Command create_command("\"" + _command.get("path") + "\" mk \"" + entry.path().filename().string() + "\"");
				m_core.execute(create_command, sender);
			
				std::ifstream in(entry.path());
				if (!in.is_open())
					throw CommandException("(LoadAllFromPortOption) Could not open the file");

				std::string str = "";

				while (!in.eof())
				{
					std::string line;
					std::getline(in, line);
					str += line + "\n";
				}
				str.substr(0, str.size() - 1);

				Command write_command("\"" + _command.get("path") + "\" write \"" + entry.path().filename().string() + "\" " + b64encode(str));
				m_core.execute(write_command, sender);
			}

			return;
		}
	};
}