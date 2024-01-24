#pragma once
#include "BaseException.h"

namespace Mova
{
	struct ProcessorException final : public Exception
	{
		ProcessorException(const std::string& _message) : Exception(_message) {}
		ProcessorException(const char* _message) : Exception(_message) {}
	};
}