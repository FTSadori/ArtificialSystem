#pragma once
#pragma once
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// systemstart
	class SystemStartOption final : public AbstractControllerOption
	{
	public:
		SystemStartOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n");
				ptr->print_main("Ends DiskSystemInfo preparation and starts system memory.\n");

				return;
			}

			if (sender.lvl() < 255)
				throw PermissionException("(SystemStartOption) Sender has low permission lvl");

			if (m_core.memory_info().disks_info.empty())
				throw MemoryIsNotLoadedException("(SystemStartOption) You need to have at least one disk partition");

			m_core.memory().load(m_core.memory_info());
			std::string mark = m_core.memory().get_start_path().mark();
			Memory::DataQueue data = m_core.users().get_data().get_as_data();
			m_core.memory().get_disk(mark).create(Memory::DiskPath("\\users.txt"), Memory::Permissions(true, 255, 255, 0, 0), Memory::FileT::FILE, true);

			ptr->print_main("Memory is now loaded!\n");
			ptr->print_main("Now you can create your user\n");
			ptr->print_third("- adduser {name} {lvl} [:p password] [::root]");
			ptr->print_main(" - add new user\n");
			ptr->print_third("- deleteuser {name}");
			ptr->print_main(" - delete user by name\n");
			ptr->print_third("- userlist");
			ptr->print_main(" - show users info\n");
			ptr->print_third("- changeuser {name}");
			ptr->print_main(" - change to another user\n");
		}
	};
}
