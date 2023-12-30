#pragma once
#include "Hash.h"
#include "TerminalWindow.h"
#include <set>

namespace Commands
{
	class PasswordHandler final
	{
	public:
		bool is_password_entered(hash_t pass_hash)
		{
			return m_set.contains(pass_hash);
		}

		void add_hash(hash_t pass_hash)
		{
			m_set.insert(pass_hash);
		}

		void check_password(GUI::TerminalWindow* ptr, hash_t needed, const std::string& text = "Input password: ")
		{
			if (needed != 0 && !is_password_entered(needed))
			{
				ptr->print_main(text);
				ptr->wait_for_input(GUI::TerminalInputType::PASSWORD);
				hash_t hash = std::hash<std::string>()(ptr->get_last_input());
				if (needed != hash)
					throw WrongPasswordException("(PasswordHandler) Wrong password");
				add_hash(needed);
			}
		}

	private:
		std::set<hash_t> m_set;
	};
}