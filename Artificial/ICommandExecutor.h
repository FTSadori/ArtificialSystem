#pragma once
#include "ICommand.h"
#include "ISender.h"

namespace Commands
{
	class ICommandExecutor
	{
	public:
		virtual std::string execute(const ICommand& command, const ISender& sender) = 0;
		virtual ~ICommandExecutor() = default;
	};
}