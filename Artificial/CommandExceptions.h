#pragma once
#include "BaseException.h"

namespace Commands
{
	struct SenderHasLowPermissionLvl final : public Exception
	{
		SenderHasLowPermissionLvl(const std::string& _message) : Exception(_message) {}
		SenderHasLowPermissionLvl(const char* _message) : Exception(_message) {}
	};

	struct PasswordAlreadyExists final : public Exception
	{
		PasswordAlreadyExists(const std::string& _message) : Exception(_message) {}
		PasswordAlreadyExists(const char* _message) : Exception(_message) {}
	};
}