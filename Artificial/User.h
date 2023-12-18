#pragma once
#include <string>

namespace Commands 
{
	class User
	{
	public:
		User(const std::string& _name, bool _sudo, uint8_t _lvl)
			: m_name(_name), m_sudo(_sudo), m_lvl(_lvl) 
		{}

		bool sudo() const { return m_sudo; }
		uint8_t lvl() const { return m_lvl; }
		std::string name() const { return m_name; }

	private:
		bool m_sudo;
		uint8_t m_lvl;
		std::string m_name;
	};
}