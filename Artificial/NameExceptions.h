#pragma once
#include "BaseException.h"

namespace Core::Memory
{
	struct NameIsNotValid final : public Exception
	{
		NameIsNotValid(const std::string& _message) : Exception(_message) {}
		NameIsNotValid(const char* _message) : Exception(_message) {}
	};

	struct FileWithThisNameDoesNotExist final : public Exception
	{
		FileWithThisNameDoesNotExist(const std::string& _message) : Exception(_message) {}
		FileWithThisNameDoesNotExist(const char* _message) : Exception(_message) {}
	};

	struct PathIsNotValid final : public Exception
	{
		PathIsNotValid(const std::string& _message) : Exception(_message) {}
		PathIsNotValid(const char* _message) : Exception(_message) {}
	};
}
