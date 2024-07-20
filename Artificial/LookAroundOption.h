#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "SurroundingsHandler.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// lookaround
	class LookAroundOption final : public AbstractControllerOption
	{
	public:
		LookAroundOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("1 permission lvl needed\n");
				ptr->print_main("Makes Nova to look around\n");
				ptr->print_main("You need to have nova.girl file in current directory\n");
				return;
			}

			using namespace Memory;

			if (sender.lvl() < 1)
				throw PermissionException("(LookAroundOption) Sender has low permission lvl");

			FullPath nova_path = RelativePathCreator::combine(_command.get("path"), "nova.girl");
			auto& disk = m_core.memory().get_disk(nova_path.mark());
			if (!disk.is_exists(nova_path.disk_path()))
				throw CommandException("(LookAroundOption) Nova is not here");

			if (Story::SurroundingsHandler::s_map.contains(ptr->get_path().full_path_str()))
			{
				Command cmd("\"" + _command.get("path") + "\" lookat \"" + Story::SurroundingsHandler::s_map.at(ptr->get_path().full_path_str()) + "\"");
				m_core.execute(cmd, User("hand of god", true, 255));
			}
			else
				ptr->print_main("Nova didn't find anything interesting...\n");

			return;
		}
	};
}