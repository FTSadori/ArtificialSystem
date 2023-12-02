#pragma once
#include "BaseException.h"

namespace Commands
{
	struct PermissionException final : public Exception
	{
		PermissionException(const std::string& _message) : Exception(_message) {}
		PermissionException(const char* _message) : Exception(_message) {}
	};

	struct CommandException final : public Exception
	{
		CommandException(const std::string& _message) : Exception(_message) {}
		CommandException(const char* _message) : Exception(_message) {}
	};
}