#include "UsersHandler.h"

namespace Commands
{
	UsersHandler::UsersHandler(const Memory::UsersData& users)
	{
		m_current_user = users.m_data.begin()->second.user;
		for (const auto& [name, user_data] : users.m_data)
		{
			m_users.m_data.emplace(name, user_data);
		}
	}

	void UsersHandler::add_user(const std::string& name, User user, hash_t pass_hash)
	{
		if (m_users.m_data.contains(name))
			throw UserNameAlreadyExistsException("(UsersHandler::add_user) Name " + name + " already exists");
		m_users.m_data.emplace(name, Memory::UserPassword{ user, pass_hash });
		if (m_users.m_data.size() == 1)
			m_current_user = m_users.m_data.begin()->second.user;
	}

	User UsersHandler::get_current_user()
	{
		return m_current_user;
	}

	void UsersHandler::try_change_user(const std::string& name, const std::string& pass)
	{
		std::hash<std::string> hasher;
		hash_t input_hash = hasher(pass);

		if (!m_users.m_data.contains(name))
			throw UserNameDoesNotExistException("(UsersHandler::try_change_user) User with name " + name + " doesn't exist");
		if (m_users.m_data[name].pass_hash != 0 && input_hash != m_users.m_data[name].pass_hash)
			throw WrongPasswordException("(UsersHandler::try_change_user) Wrong password to user " + name);
		
		m_current_user = m_users.m_data[name].user;
	}

	void UsersHandler::reload_current_user()
	{
		m_current_user = m_users.m_data[m_current_user.name()].user;
	}

	Memory::UsersData& UsersHandler::get_data()
	{
		return m_users;
	}
	
	void UsersHandler::remove_user(const std::string& name)
	{
		m_users.m_data.erase(name);
	}
}
