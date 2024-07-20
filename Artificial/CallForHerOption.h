#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "SurroundingsHandler.h"
#include <filesystem>

namespace Commands
{
	// callnova
	class CallForHerOption final : public AbstractControllerOption
	{
	public:
		CallForHerOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Call Nova\n");
				return;
			}

			using namespace Memory;

			std::string mainmark = m_core.memory_info().get_main_disk_info().mark;
			auto& maindisk = m_core.memory().get_disk(mainmark);
			if (!maindisk.is_exists(DiskPath("\\system")))
				maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.call")))
				maindisk.create(DiskPath("\\system\\.call"), Permissions(true, 255, 255, 255, 0), FileT::FILE, true);
			
			auto data = maindisk.read(DiskPath("\\system\\.call"), true);
			for (int i = 0; i < m_call_map.size(); ++i)
			if (data.size() == i 
				&& i < m_call_map.size()
				&& ptr->get_path().full_path_str() == m_call_map[i].first)
			{
				Command cmd("\"" + ptr->get_path().full_path_str() + "\" run \"" + m_call_map[i].second + "\"");
				m_core.execute(cmd, User("amogus", true, 255));
				Command cmd2("\"" + ptr->get_path().full_path_str() + "\" write CORE:\\system\\.call 0 ::nobase64 ::append");
				m_core.execute(cmd2, User("amogus", true, 255));
				return;
			}

			return;
		}

	private:
		std::vector<std::pair<std::string, std::string>> m_call_map = {
			{"CORRIDOR:", "CORE:\\.scripts\\chapter1\\nova_0.txt"}
		};
	};
}