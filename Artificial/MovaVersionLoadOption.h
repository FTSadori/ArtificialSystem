#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Separator.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "MovaVersionHandler.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// movavload
	class MovaVersionLoadOption final : public AbstractControllerOption
	{
	public:
		MovaVersionLoadOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Checks packages and calculates max mova version\n");
				return;
			}

			using namespace Memory;

			std::string mainmark = m_core.memory_info().get_main_disk_info().mark;
			auto& maindisk = m_core.memory().get_disk(mainmark);
			if (!maindisk.is_exists(DiskPath("\\system\\.installedpacks")))
			{
				return;
			}

			auto list = maindisk.list(DiskPath("\\system\\.installedpacks"), sender.sudo());

			for (size_t i = 0; i < list.size(); ++i)
			{
				Mova::MovaVersionHandler::add_package(list[i]);
			}
			
			return;
		}
	};
}