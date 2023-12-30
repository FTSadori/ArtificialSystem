#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include <map>

namespace Commands
{
	// write
	class MemoryWriteOption final : public AbstractControllerOption
	{
	public:
		MemoryWriteOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("File write permission lvl needed\n");
				ptr->print_main("Writes text in file\n");
				ptr->print_secondary("write {path} {base64_text}\n");
				ptr->print_main("  path - (string) absolute or relative path;\n");
				ptr->print_main("  base64_text - (string) text in base64 format;\n");

				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.system()).permissions;

			if (sender.lvl() < perm.write_perm_lvl)
				throw PermissionException("(MemoryWriteOption) Sender has low permission lvl");

			if (disk.get_type(path.disk_path()) != Memory::FileT::FILE)
				throw CommandException("(MemoryWriteOption) It's not a file");

			if (perm.hidden && !sender.sudo())
				throw PermissionException("(MemoryWriteOption) Sender has low permission lvl");

			m_core.passwords().check_password(ptr, perm.password_hash);

			std::string str = b64decode(_command.get("2"));
			Memory::DataQueue data;
			for (char c : str) data.push_char(c);
			disk.write(path.disk_path(), data, sender.system());
			return;
		}
	};
}