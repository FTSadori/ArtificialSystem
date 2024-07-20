#include "Core.h"

namespace Commands
{
	void Core::load()
	{
		try
		{
			m_memory_info = Memory::DiskSystem::try_load_boot();
			m_memory.load(m_memory_info);
			// vse dobre
			std::string mark = m_memory.get_start_path().mark();
			Memory::DataQueue data = m_memory.get_disk(mark).read(Memory::DiskPath("\\users.txt"), true);
			Memory::UsersData users_data;
			users_data.load_from_data(data);
			m_users = UsersHandler(users_data);
		
			m_gui.connect_to_core(this);
			m_gui.connect_to_users(&m_users);
			m_gui.get_terminal_ptr()->set_user_name(m_users.get_current_user().name());
			m_gui.get_terminal_ptr()->set_path(m_memory.get_start_path());

			Command cmd("none:\\ preload");
			execute(cmd, User("system", true, 255));
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
					execute(Command("\"" + ptr->get_path().full_path_str() + "\" " + input), m_users.get_current_user());
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

	void Core::add_controller(const BaseController& controller)
	{
		m_controllers.emplace_back(controller);
	}

	void Core::execute(const ICommand& command, const User& sender)
	{
		for (auto& controller : m_controllers)
		{
			if (controller.has_option(command.get("name")))
				return controller.execute(command, sender);
		}
		throw CommandException("(Core) Unknown command '" + command.get("name") + "'");
	}
}
