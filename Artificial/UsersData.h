#pragma once
#include "User.h"
#include "Hash.h"
#include "ILoadable.h"

namespace Memory
{
	struct UserPassword
	{
		Commands::User user;
		hash_t pass_hash;
	};

	class UsersData final : public ILoadable
	{
	public:
		std::unordered_map<std::string, UserPassword> m_data;

		virtual DataQueue get_as_data() const override
		{
			DataQueue data;
			data.push<size_t>(m_data.size());
			for (const auto& [name, userpass] : m_data)
			{
				data.push<size_t>(name.size());
				for (char c : name)
					data.push_char(c);
				data.push<uint8_t>(userpass.user.lvl());
				data.push<bool>(userpass.user.sudo());

				data.push<hash_t>(userpass.pass_hash);
			}
			return data;
		}

		virtual void load_from_data(DataQueue& _data) override
		{
			m_data.clear();

			size_t data_size = _data.pop<size_t>();
			for (size_t i = 0; i < data_size; ++i)
			{
				size_t name_size = _data.pop<size_t>();
				std::string name;
				for (size_t j = 0; j < name_size; ++j)
					name += _data.pop<char>();
				uint8_t lvl = _data.pop<uint8_t>();
				bool sudo = _data.pop<bool>();

				hash_t hash = _data.pop<hash_t>();

				Commands::User new_user(name, sudo, lvl);
				m_data.emplace(name, UserPassword{ new_user, hash });
			}
		}
	};
}