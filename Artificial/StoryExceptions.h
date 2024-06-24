#pragma once
#include "BaseException.h"

namespace Story
{
	struct TaskNameException final : public Exception
	{
		TaskNameException(const std::string& _message) : Exception(_message) {}
		TaskNameException(const char* _message) : Exception(_message) {}
	};
}