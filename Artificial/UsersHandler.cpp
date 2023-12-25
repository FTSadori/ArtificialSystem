#include "UsersHandler.h"

namespace Commands
{
	UsersHandler::UsersHandler(const std::vector<UserPassword>& users)
	{
		for (const auto& user_data : users)
		{
			m_users.emplace(user_data.user.name(), user_data);
		}
	}

	void UsersHandler::add_user(const std::string& name, User user, hash_t pass_hash)
	{
		if (m_users.contains(name))
			throw UserNameAlreadyExistsException("(UsersHandler::add_user) Name " + name + " already exists");
		m_users.emplace(name, UserPassword{ user, pass_hash });
		if (m_users.size() == 1)
			m_current_user = m_users.begin()->second.user;
	}

	User UsersHandler::get_current_user()
	{
		return m_current_user;
	}

	void UsersHandler::try_change_user(const std::string& name, const std::string& pass)
	{
		std::hash<std::string> hasher;
		hash_t input_hash = hasher(pass);

		if (!m_users.contains(name))
			throw UserNameDoesNotExistException("(UsersHandler::try_change_user) User with name " + name + " doesn't exist");
		if (m_users[name].pass_hash != 0 && input_hash != m_users[name].pass_hash)
			throw WrongUserPasswordException("(UsersHandler::try_change_user) Wrong password to user " + name);
		
		m_current_user = m_users[name].user;
	}
	const std::unordered_map<std::string, UserPassword>& UsersHandler::get_map()
	{
		return m_users;
	}
	
	void UsersHandler::remove_user(const std::string& name)
	{
		m_users.erase(name);
	}
}
