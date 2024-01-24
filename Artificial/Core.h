#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "CommandExceptions.h"

namespace Commands
{
	class Core final : public ICore
	{
	public:
		Core() = default;

		void load();

		virtual UsersHandler& users()                 override { std::lock_guard lock(m_execution_mutex); return m_users; }
		virtual GUI::GUIHandler& gui()                override { std::lock_guard lock(m_execution_mutex); return m_gui; }
		virtual GUI::ColoursThemesHandler& themes()   override { std::lock_guard lock(m_execution_mutex); return m_themes; }
		virtual Memory::DiskSystem& memory()          override { std::lock_guard lock(m_execution_mutex); return m_memory; }
		virtual Memory::DiskSystemInfo& memory_info() override { std::lock_guard lock(m_execution_mutex); return m_memory_info; }
		virtual PasswordHandler& passwords()          override { std::lock_guard lock(m_execution_mutex); return m_passwords; }
		virtual NetContainer& net()                   override { std::lock_guard lock(m_execution_mutex); return m_net; }
		virtual Mova::Processor& processor()          override { std::lock_guard lock(m_execution_mutex); return m_processor; }


		void add_controller(const BaseController& controller);
		virtual void execute(const ICommand& command, const User& sender) override;

	private:
		UsersHandler m_users;
		GUI::GUIHandler m_gui;
		GUI::ColoursThemesHandler m_themes;
		Memory::DiskSystem m_memory;
		Memory::DiskSystemInfo m_memory_info;
		PasswordHandler m_passwords;
		NetContainer m_net;
		Mova::Processor m_processor

		std::vector<BaseController> m_controllers;
		std::mutex m_execution_mutex;

		void start_input_thread();
	};
}