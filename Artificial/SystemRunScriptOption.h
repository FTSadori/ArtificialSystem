#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Separator.h"
#include "Core.h"
#include <fstream>

namespace Commands
{
	// run
	class SystemRunScriptOption final : public AbstractControllerOption
	{
	public:
		SystemRunScriptOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Script file execute (as minimum) permission lvl needed\n");
				ptr->print_main("Runs script with system commands. May be from real file or from file in Artificial.\n");
				ptr->print_secondary("run {path} [::real]\n");
				ptr->print_main("  path - (string) path to file, either real or not;\n");
				ptr->print_main("  ::real - (flag) is used to indicate that real file will be used for script;\n");

				return;
			}

			if (_command.has("::real"))
			{
				std::ifstream in(_command.get("1"));
				if (!in.is_open())
					throw CommandException("(SystemRunScriptOption) Could not open the file");

				while (!in.eof())
				{
					std::string line;
					std::getline(in, line);
					if (line == "") continue;
					Command cmd("\"" + ptr->get_path().full_path_str() + "\" " + line);
					m_core.execute(cmd, sender);
				}

				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.system()).permissions;

			if (disk.get_type(path.disk_path()) != Memory::FileT::FILE)
				throw CommandException("(SystemRunScriptOption) It's not a file");

			if (sender.lvl() < perm.exec_perm_lvl)
				throw PermissionException("(SystemRunScriptOption) Sender has low permission lvl");

			if (perm.hidden && !sender.sudo())
				throw PermissionException("(SystemRunScriptOption) Sender has low permission lvl");

			m_core.passwords().check_password(ptr, perm.password_hash);

			Memory::DataQueue data = disk.read(path.disk_path(), sender.system());
			std::string strdata(data.get_data(), data.size());

			for (const auto& command : Separator::split(strdata, '\n'))
			{
				if (command == "") continue;
				Command cmd("\"" + ptr->get_path().full_path_str() + "\" " + command);
				m_core.execute(cmd, sender);
			}
		}
	};
}
