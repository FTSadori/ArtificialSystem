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
				ptr->print_secondary("write {path} {base64_text} [:preenter password] [::nobase64] [::append]\n");
				ptr->print_main("  path - (string) absolute or relative path;\n");
				ptr->print_main("  base64_text - (string) text in base64 format;\n");
				ptr->print_main("  :preenter password - (flag + string) you can enter password here if command needs it;\n");
				ptr->print_main("  ::nobase64 - (flag) allows to input text in decoded variant;\n");
				ptr->print_main("  ::append - (flag) appends new text to the end of the file;\n");

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

			if (_command.has(":preenter"))
				m_core.passwords().check_password(perm.password_hash, _command.get(":preenter"));
			else
				m_core.passwords().check_password(ptr, perm.password_hash);

			std::string str = (_command.has("::nobase64") ? _command.get("2") : b64decode(_command.get("2")));
			Memory::DataQueue data;
			for (char c : str) data.push_char(c);
			if (_command.has("::append"))
			{
				Memory::DataQueue dataf = disk.read(path.disk_path(), sender.system());
				dataf.concat(data);
				disk.write(path.disk_path(), dataf, sender.system());
			}
			else
				disk.write(path.disk_path(), data, sender.system());
			return;
		}
	};
}