#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// adduser {name} {lvl} [:p password] [::root]
	class AddUserOption final : public AbstractControllerOption
	{
	public:
		AddUserOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Your entered permission lvl needed\n");
				ptr->print_main("Adds new system user\n");
				ptr->print_secondary("adduser {name} {lvl} [:p password] [::root]\n");
				ptr->print_main("  name - (string) new user name;\n");
				ptr->print_main("  lvl - (integer) new user permission level, needs to be lower or equal to yours and in [0, 254] range;\n");
				ptr->print_main("  :p \"password\" - (flag + string) password for new user;\n");
				ptr->print_main("  ::root - (flag) new user will be root if flag enabled;\n");

				return;
			}

			if (!m_core.memory().is_loaded())
				throw MemoryIsNotLoadedException("(AddUserOption) Memory is not loaded. Try using 'startsystem' command after setting up DiskSystemInfo file.");

			uint8_t perm_lvl = Parser::from_string<int>(_command.get("2"));
			if (sender.lvl() < perm_lvl)
				throw PermissionException("(AddUserOption) Sender has low permission lvl");

			if (!sender.sudo() && _command.has("::root"))
				throw PermissionException("(AddUserOption) You can't create a root user");

			User new_user = User(_command.get("1"), _command.has("::root"), perm_lvl);
			hash_t pass_hash = 0;
			if (_command.has(":p"))
			{
				pass_hash = std::hash<std::string>()(_command.get(":p"));
			}

			m_core.users().add_user(new_user.name(), new_user, pass_hash);

			std::string mark = m_core.memory().get_start_path().mark();
			Memory::DataQueue data = m_core.users().get_data().get_as_data();
			m_core.memory().get_disk(mark).write(Memory::DiskPath("\\users.txt"), data, true);
		}
	};
}
