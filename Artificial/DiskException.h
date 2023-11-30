#pragma once
#include "Exceptions.h"

namespace Memory
{
	struct DiskMarkDoesNotExist final : public Exception
	{
		DiskMarkDoesNotExist(const std::string& _message) : Exception(_message) {}
		DiskMarkDoesNotExist(const char* _message) : Exception(_message) {}
	};
}