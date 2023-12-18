#pragma once
#include "User.h"
#include "Hash.h"
#include <string>
#include <unordered_map>
#include "UserExceptions.h"

namespace Commands
{
	struct UserPassword
	{
		User user;
		hash_t pass_hash;
	};

	class UsersHandler final
	{
	public:
		UsersHandler() = default;
		UsersHandler(const std::vector<UserPassword>& users);

		void add_user(const std::string& name, User user, hash_t pass_hash);
		User get_current_user();
		void try_change_user(const std::string& name, const std::string& pass);

	private:
		User m_current_user{ "none", false, 0 };
		std::unordered_map<std::string, UserPassword> m_users;
	};
}