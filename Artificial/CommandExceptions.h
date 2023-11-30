#pragma once
#include "BaseException.h"

namespace Commands
{
	struct CantCreateCommand final : public Exception
	{
		CantCreateCommand(const std::string& _message) : Exception(_message) {}
		CantCreateCommand(const char* _message) : Exception(_message) {}
	};
}