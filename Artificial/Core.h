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

		virtual UsersHandler& users() override { return m_users; }
		virtual GUI::GUIHandler& gui() override { return m_gui; }
		virtual GUI::ColoursThemesHandler& themes() override { return m_themes; }
		virtual Memory::DiskSystem& memory() override { return m_memory; }
		virtual Memory::DiskSystemInfo& memory_info() override { return m_memory_info; }
		virtual PasswordHandler& passwords() override { return m_passwords; }

		void add_controller(const BaseController& controller);
		virtual void execute(const ICommand& command, const User& sender) override;

	private:
		UsersHandler m_users;
		GUI::GUIHandler m_gui;
		GUI::ColoursThemesHandler m_themes;
		Memory::DiskSystem m_memory;
		Memory::DiskSystemInfo m_memory_info;
		PasswordHandler m_passwords;

		std::vector<BaseController> m_controllers;
		std::mutex m_execution_mutex;

		void start_input_thread();
	};
}