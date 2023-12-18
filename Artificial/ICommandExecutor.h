#pragma once
#include "ICommand.h"
#include "User.h"

namespace Commands
{
	class ICommandExecutor
	{
	public:
		virtual std::string execute(const ICommand& command, const User& sender) = 0;
		virtual ~ICommandExecutor() = default;
	};
}