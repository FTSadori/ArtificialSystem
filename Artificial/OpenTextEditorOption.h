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
	// nano
	class OpenTextEditorOption final : public AbstractControllerOption
	{
	public:
		OpenTextEditorOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Read/write file permission lvl needed\n");
				ptr->print_main("Opens file in text editor\n");
				ptr->print_secondary("nano {path}\n");
				ptr->print_main("  path - (string) absolute or relative path;\n");

				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.system()).permissions;

			if (disk.get_type(path.disk_path()) != Memory::FileT::FILE)
				throw CommandException("(OpenTextEditorOption) It's not a file");

			if (sender.lvl() < perm.read_perm_lvl)
				throw PermissionException("(OpenTextEditorOption) Sender has low permission lvl");

			if (perm.hidden && !sender.sudo())
				throw PermissionException("(OpenTextEditorOption) Sender has low permission lvl");

			m_core.passwords().check_password(ptr, perm.password_hash);

			bool readonly = sender.lvl() < perm.write_perm_lvl;
			Memory::DataQueue data = disk.read(path.disk_path(), sender.system());
			
			m_core.gui().open_editor(path, std::string(data.get_data(), data.size()), readonly);

			return;
		}
	};
}