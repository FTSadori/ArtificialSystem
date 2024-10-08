#pragma once
#include <string>

namespace Commands 
{
	class User
	{
	public:
		User(const std::string& _name = "", bool _sudo = false, uint8_t _lvl = 0)
			: m_name(_name), m_sudo(_sudo), m_lvl(_lvl) 
		{}

		bool sudo() const { return m_sudo; }
		uint8_t lvl() const { return m_lvl; }
		std::string name() const { return m_name; }
		bool system() const { return m_lvl == m_system_lvl; }
		void set_lvl(uint8_t lvl) { m_lvl = (m_lvl > lvl ? m_lvl : lvl); }

	protected:
		bool m_sudo;
		uint8_t m_lvl;
		std::string m_name;

		uint8_t m_system_lvl = 255;
	};
}