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
			m_gui.get_terminal_ptr()->set_path(Memory::FullPath("none:\\"));
			Command cmd("none:\\ syscreateinfo");
			execute(cmd, m_users.get_current_user());
		}
		start_input_thread();
	}
	
	void Core::start_input_thread()
	{
		std::thread th([&]() {
			auto ptr = m_gui.get_terminal_ptr();
			while (true)
			{
				ptr->wait_for_input(GUI::TerminalInputType::COMMAND);
				auto input = ptr->get_last_input();
				if (input.empty()) continue;
				try {
					execute(Command(ptr->get_path().full_path_str() + " " + input), m_users.get_current_user());
				}
				catch (const Exception& ex)
				{
					ptr->print_secondary(ex.what());
					ptr->print_secondary("\n");
				}
				catch (...)
				{
					ptr->print_secondary("Unknown error");
					ptr->print_secondary("\n");
				}
			}
		});
		th.join();

	}

	
}
