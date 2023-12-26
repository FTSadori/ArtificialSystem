#pragma once
#include "User.h"
#include "Hash.h"
#include <string>
#include <unordered_map>
#include "UserExceptions.h"
#include "UsersData.h"

namespace Commands
{
	class UsersHandler final
	public:
		UsersHandler() = default;
		UsersHandler(const Memory::UsersData& users);

		void add_user(const std::string& name, User user, hash_t pass_hash = 0);
		User get_current_user();
		void try_change_user(const std::string& name, const std::string& pass = "");
		const Memory::UsersData& get_data();
		void remove_user(const std::string& name);

	private:
		User m_current_user{ "none", false, 0 };
		Memory::UsersData m_users;
	};
}