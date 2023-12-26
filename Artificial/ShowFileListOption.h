#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// ls
	class ShowFileListOption final : public AbstractControllerOption
	{
	public:
		ShowFileListOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Shows file list in current directory. If user is sudo shows hidden files too\n");
				ptr->print_secondary("ls [::info]\n");
				ptr->print_main("  ::info - (flag) shows files info;\n");

				return;
			}

			if (!m_core.memory().is_loaded())
				throw MemoryIsNotLoadedException("(ShowFileListOption) Memory is not loaded. Try using 'startsystem' command after setting up DiskSystemInfo file.");

			Memory::FullPath cur_path = _command.get("path");

			auto& disk = m_core.memory().get_disk(cur_path.mark());
			auto list = disk.list(cur_path.disk_path(), sender.sudo());

			if (_command.has("::info"))
			{
				for (size_t i = 0; i < list.size(); ++i)
				{
					auto path = Memory::DiskPath(cur_path.disk_path().full_name(), list[i]);
					auto perm = disk.get_info(path, true).permissions;

					if (!sender.sudo() && perm.hidden)
						continue;

					ptr->print_main(Parser::to_string(i) + ". ");
					if (disk.get_type(path) == Memory::FileT::DIR)
						ptr->print_secondary(list[i]);
					else
						ptr->print_main(list[i]);

					ptr->print_main(" r:" + Parser::to_string((int)perm.read_perm_lvl));
					ptr->print_main(" w:" + Parser::to_string((int)perm.write_perm_lvl));
					if (disk.get_type(path) == Memory::FileT::FILE)
						ptr->print_main(" e:" + Parser::to_string((int)perm.exec_perm_lvl));
					if (perm.password_hash != 0)
						ptr->print_third(" has password");

					if (sender.sudo() && disk.get_info(path, true).permissions.hidden)
						ptr->print_third(" (hidden)");
					ptr->print_main("\n");
				}
			}
			else
			{
				bool flag = false;
				for (const auto& name : list)
				{
					auto path = Memory::DiskPath(cur_path.disk_path().full_name(), name);

					if (!sender.sudo() && disk.get_info(path, true).permissions.hidden)
						continue;

					if (disk.get_type(path) == Memory::FileT::DIR)
						ptr->print_secondary(name);
					else
						ptr->print_main(name);

					if (disk.get_info(path, true).permissions.hidden)
						ptr->print_third(" (hidden)");
					ptr->print_third("   ");
					flag = true;
				}
				if (flag) ptr->print_main("\n");
			}
		}
	};
}