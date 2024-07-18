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
	// addpack
	class AddPackOption final : public AbstractControllerOption
	{
	public:
		AddPackOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Adds chosen package to package list for future installation\n");
				ptr->print_main("You need to have nova.girl file in current directory\n");
				ptr->print_main("Works only with *.pack files\n");
				ptr->print_secondary("addpack {packfile}\n");
				ptr->print_main("  packfile - (string) name of file to pick up;\n");
				return;
			}

			using namespace Memory;

			FullPath nova_path = RelativePathCreator::combine(_command.get("path"), "nova.girl");
			auto& disk = m_core.memory().get_disk(nova_path.mark());
			if (!disk.is_exists(nova_path.disk_path()))
				throw CommandException("(AddPackOption) Nova is not here");

			if (_command.get("1").find('\\') != std::string::npos)
				throw CommandException("(AddPackOption) You can use only files in current directory");

			FullPath new_path = RelativePathCreator::combine(_command.get("path"), _command.get("1"));
			if (disk.get_type(new_path.disk_path()) != FileT::FILE || !_command.get("1").ends_with(".pack"))
				throw CommandException("(AddPackOption) " + _command.get("1") + " is not a *.pack file");

			std::string mainmark = m_core.memory_info().get_main_disk_info().mark;
			auto& maindisk = m_core.memory().get_disk(mainmark);
			if (!maindisk.is_exists(DiskPath("\\system")))
				maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.availablepacks")))
				maindisk.create(DiskPath("\\system", ".availablepacks"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);

			Command cmd("\"" + _command.get("path") + "\" move \"" + new_path.full_path_str() + "\" \"" +
				mainmark + ":\\system\\.availablepacks\"");
			m_core.execute(cmd, User("hand of god", true, 255));

			return;
		}
	};
}