#pragma once
#include "User.h"

namespace Commands
{
	class SystemUser final : public User
	{
	public:
		SystemUser() : User(true, 255) {}
	};
}