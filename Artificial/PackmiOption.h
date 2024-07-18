#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Separator.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "PackagesHandler.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// packmi
	class PackmiOption final : public AbstractControllerOption
	{
	public:
		PackmiOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Asks Packmi to install packages or show lists\n");
				ptr->print_main("You need to have nova.girl and packmi.girl files in current directory\n");
				ptr->print_secondary("packmi [::list] [::list_ins] [:install name]\n");
				ptr->print_main("  ::list - (flag) shows available packages;\n");
				ptr->print_main("  ::list_ins - (flag) shows installed packages;\n");
				ptr->print_main("  :install name - (flag + string) installs package (name without .pack);\n");

				return;
			}

			using namespace Memory;

			FullPath nova_path = RelativePathCreator::combine(_command.get("path"), "nova.girl");
			auto& disk = m_core.memory().get_disk(nova_path.mark());
			if (!disk.is_exists(nova_path.disk_path()))
				throw CommandException("(PackmiOption) Nova is not here");

			FullPath packmi_path = RelativePathCreator::combine(_command.get("path"), "packmi.girl");
			if (!disk.is_exists(packmi_path.disk_path()))
				throw CommandException("(PackmiOption) Packmi is not here");

			std::string dir = "";

			if (_command.has("::list"))
				dir = "\\system\\.availablepacks";
			else if (_command.has("::list_ins"))
				dir = "\\system\\.installedpacks";

			if (dir != "")
			{
				std::string mainmark = m_core.memory_info().get_main_disk_info().mark;
				auto& maindisk = m_core.memory().get_disk(mainmark);
				if (!maindisk.is_exists(DiskPath("\\system")))
					maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
				if (!maindisk.is_exists(DiskPath(dir)))
					maindisk.create(DiskPath(dir), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);

				auto list = maindisk.list(DiskPath(dir), sender.sudo());

				for (size_t i = 0; i < list.size(); ++i)
				{
					auto path = Memory::DiskPath(dir, list[i]);

					ptr->print_main(Parser::to_string(i) + ". " + path.file() + "\n");

					DataQueue data = maindisk.read(path, true);
					std::string strdata(data.get_data(), data.size());
					
					ptr->print_secondary(b64decode(strdata) + "\n");
				}
				return;
			}

			if (_command.has(":install"))
			{
				std::string mainmark = m_core.memory_info().get_main_disk_info().mark;
				auto& maindisk = m_core.memory().get_disk(mainmark);
				if (!maindisk.is_exists(DiskPath("\\system")))
					maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
				if (!maindisk.is_exists(DiskPath("\\system\\.installedpacks")))
					maindisk.create(DiskPath("\\system\\.installedpacks"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);

				Command cmd("\"" + _command.get("path") + "\" move \"" + mainmark + ":\\system\\.availablepacks\\" + _command.get(":install")
					+ "\" \"" +
					mainmark + ":\\system\\.installedpacks\"");
				m_core.execute(cmd, User("hand of god", true, 255));

				if (Story::PackagesHandler::s_map.contains(_command.get(":install")))
				{
					Command cmd("\"" + ptr->get_path().full_path_str() + "\" run \"" + Story::PackagesHandler::s_map.at(_command.get(":install")) + "\"");
					m_core.execute(cmd, User("amogus", true, 255));
				}
			}

			return;
		}
	};
}