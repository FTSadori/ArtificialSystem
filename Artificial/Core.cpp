#include "Core.h"

namespace Commands
{
	void Core::load()
	{
		try
		{
			auto info = Memory::DiskSystem::try_load_boot();
			m_memory.load(info);
			// vse dobre
		}
		catch (const Exception&)
		{
			m_users.add_user("system", Commands::User("system", true, 255));
			m_gui.connect_to_core(this);
			m_gui.connect_to_users(&m_users);
			m_gui.get_terminal_ptr()->set_user_name("system");
			Command cmd("none:\\ syscreateinfo");
			execute(cmd, m_users.get_current_user());

			while (true)
			{
				Sleep(1000);
			}
		}
	}
}
