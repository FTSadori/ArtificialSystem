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
	// download
	class DownloadOption final : public AbstractControllerOption
	{
	public:
		DownloadOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("35 or server permission lvl needed\n");
				ptr->print_main("Checks connection to server by address and tries to download data from it\n");
				ptr->print_secondary("download {address}\n");
				ptr->print_main("  address - (string) address line;\n");
				return;
			}

			if (sender.lvl() < 35)
				throw PermissionException("(DownloadOption) Sender has low permission lvl");

			Command command("\"" + _command.get("path") + "\"" + " ping " + "\"" + _command.get("1") + "\"");
			m_core.execute(command, sender);
			
			const NetData& netData = m_core.net().get(_command.get("1"));
			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), netData.name);

			auto& disk = m_core.memory().get_disk(path.mark());

			while (disk.is_exists(path.disk_path()))
			{
				path = path.full_path_str() + "_";
			}

			ptr->print_main("Saving file as " + path.disk_path().file() + ".");

			Command command_mk("\"" + _command.get("path") + "\"" + " mk " + "\"" + path.disk_path().file() + "\"");
			m_core.execute(command_mk, sender);

			ptr->print_main(".");

			Command command_write("\"" + _command.get("path") + "\"" + " write " + "\"" + path.disk_path().file() + "\" " + netData.b64data);
			m_core.execute(command_write, sender);
			
			ptr->print_main(".\n");

			return;
		}
	};
}