#pragma once
#include "CommandExecutor.h"
#include "DiskSystem.h"
#include "GUIHandler.h"
#include "ColoursThemesHandler.h"

namespace Commands
{
	class Core final : public CommandExecutor
	{
	public:
		Core() = default;

		void load();

		UsersHandler& users() { return m_users; }
		GUI::GUIHandler& gui() { return m_gui; }
		GUI::ColoursThemesHandler& themes() { return m_themes; }
		Memory::DiskSystem& memory() { return m_memory; }

	private:
		UsersHandler m_users;
		GUI::GUIHandler m_gui;
		GUI::ColoursThemesHandler m_themes;
		Memory::DiskSystem m_memory;
	};
}