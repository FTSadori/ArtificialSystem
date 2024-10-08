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
	// lookat
	class OpenImageViewerOption final : public AbstractControllerOption
	{
	public:
		OpenImageViewerOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("11 permission lvl needed\n");
				ptr->print_main("Opens file in image viewer\n");
				ptr->print_secondary("lookat {path} [:preenter password]\n");
				ptr->print_main("  path - (string) absolute or relative path;\n");
				ptr->print_main("  :preenter password - (flag + string) you can enter password here if command needs it;\n");

				return;
			}

			if (sender.lvl() < 11)
				throw PermissionException("(OpenImageViewerOption) Sender has low permission lvl");

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.system()).permissions;

			if (disk.get_type(path.disk_path()) != Memory::FileT::FILE)
				throw CommandException("(OpenImageViewerOption) It's not a file");

			if (sender.lvl() < perm.read_perm_lvl)
				throw PermissionException("(OpenImageViewerOption) Sender has low permission lvl");

			if (perm.hidden && !sender.sudo())
				throw PermissionException("(OpenImageViewerOption) Sender has low permission lvl");

			if (_command.has(":preenter"))
				m_core.passwords().check_password(perm.password_hash, _command.get(":preenter"));
			else
				m_core.passwords().check_password(ptr, perm.password_hash);

			Memory::DataQueue data = disk.read(path.disk_path(), sender.system());

			m_core.gui().open_image(path.disk_path().file(), std::string(data.get_data(), data.size()));

			return;
		}
	};
}