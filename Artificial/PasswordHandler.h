#pragma once
#include "Hash.h"
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

	private:
		std::set<hash_t> m_set;
	};
}