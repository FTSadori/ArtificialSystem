#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Separator.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "ItemUsingHandler.h"
#include "DialogHandler.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// talkto
	class TalkToOption final : public AbstractControllerOption
	{
	public:
		TalkToOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Makes Nova speak with someone\n");
				ptr->print_main("Nova must be in current directory\n");
				ptr->print_secondary("talkto {name}\n");
				ptr->print_main("  name - (string) name of the file\\dir in current directory;\n");
				return;
			}

			using namespace Memory;

			FullPath nova_path = RelativePathCreator::combine(_command.get("path"), "nova.girl");
			auto& disk = m_core.memory().get_disk(nova_path.mark());
			if (!disk.is_exists(nova_path.disk_path()))
				throw CommandException("(TalkToOption) Nova is not here");

			std::string mainmark = m_core.memory_info().get_main_disk_info().mark;
			auto& maindisk = m_core.memory().get_disk(mainmark);
			if (!maindisk.is_exists(DiskPath("\\system")))
				maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.dialogs")))
				maindisk.create(DiskPath("\\system", ".dialogs"), Permissions(true, 255, 255, 255, 0), FileT::FILE, true);

			if (!Story::DialogHandler::is_loaded())
			{
				auto data = maindisk.read(DiskPath("\\system\\.dialogs"), true);
				Story::DialogHandler::load(std::string(data.get_data(), data.size()));
			}

			FullPath str_path = RelativePathCreator::combine(_command.get("path"), _command.get("1"));
			const auto& str = Story::DialogHandler::get(str_path.full_path_str());
			if (str == "")
				ptr->print_main("She was unable to start a dialog...\n");
			else
			{
				Command cmd("\"" + ptr->get_path().full_path_str() + "\" run \"" + str + "\"");
				m_core.execute(cmd, User("amogus", true, 255));
			}

			return;
		}
	};
}