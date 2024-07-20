#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Separator.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "PathEventsHandler.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// setnewpath
	class SetPathPairOption final : public AbstractControllerOption
	{
	public:
		SetPathPairOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("System permission lvl needed\n");
				ptr->print_main("Sets\\Unsets 'go' script path\n");
				ptr->print_secondary("setnewpath {gopath} {scriptpath}\n");
				ptr->print_main("  gopath - (string) path to file that causes script running;\n");
				ptr->print_main("  scriptpath - (string) path to script file (\"\" if remove);\n");
				return;
			}

			using namespace Memory;

			if (!sender.system())
				throw PermissionException("(SetPathPairOption) Sender has low permission lvl");

			std::string mainmark = m_core.memory_info().get_main_disk_info().mark;
			auto& maindisk = m_core.memory().get_disk(mainmark);
			if (!maindisk.is_exists(DiskPath("\\system")))
				maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.goevent")))
				maindisk.create(DiskPath("\\system\\.goevent"), Permissions(true, 255, 255, 255, 0), FileT::FILE, true);

			if (!Story::PathEventsHandler::is_loaded())
			{
				auto data = maindisk.read(DiskPath("\\system\\.goevent"), true);
				Story::PathEventsHandler::load(std::string(data.get_data(), data.size()));
			}

			Memory::FullPath path1 = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));
			auto& disk1 = m_core.memory().get_disk(path1.mark());
			if (!disk1.is_exists(path1.disk_path()))
				throw CommandException("(SetNewPathOption) Wrong gopath");

			if (_command.get("2") != "")
			{
				Memory::FullPath path2 = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("2"));
				auto& disk2 = m_core.memory().get_disk(path2.mark());
				if (!disk2.is_exists(path2.disk_path()))
					throw CommandException("(SetNewPathOption) Wrong scriptpath");

				Story::PathEventsHandler::set_new_pair(path1.full_path_str(), path2.full_path_str());
			}
			else
				Story::PathEventsHandler::set_new_pair(path1.full_path_str(), "");

			auto new_data = Story::PathEventsHandler::to_data();

			Memory::DataQueue data;
			for (char c : new_data)
				data.push_char(c);

			maindisk.write(DiskPath("\\system\\.goevent"), data, true);
			return;
		}
	};
}