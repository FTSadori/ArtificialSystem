#pragma once
#include "User.h"

namespace Commands
{
	class SystemUser final : public User
	{
	public:
		SystemUser() : User("system", true, 255) {}
	};
}