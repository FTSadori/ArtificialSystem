#pragma once
#include "BaseException.h"

namespace Memory
{
	struct NotEnoughDataInStream final : public Exception
	{
		NotEnoughDataInStream(const std::string& _message) : Exception(_message) {}
		NotEnoughDataInStream(const char* _message) : Exception(_message) {}
	};
}