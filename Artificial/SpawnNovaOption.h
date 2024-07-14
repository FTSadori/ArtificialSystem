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
	// spawnnova
	class SpawnNovaOption final : public AbstractControllerOption
	{
	public:
		SpawnNovaOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n");
				ptr->print_main("Creates Nova file in current directory\n");
				return;
			}

			if (!sender.system())
				throw PermissionException("(SpawnNovaOption) Sender has low permission lvl");

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), "nova.girl");
			auto& disk = m_core.memory().get_disk(path.mark());
			disk.create(path.disk_path(), Memory::Permissions(false, 10, 255, 0, 0), Memory::FileT::FILE, false);
			Command cmd("\"" + _command.get("path") + "\" write nova.girl T01PUg==");
			m_core.execute(cmd, sender);
			return;
		}
	};
}