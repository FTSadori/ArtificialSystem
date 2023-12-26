#pragma once
#include "BaseException.h"

namespace Commands
{
	struct UserNameAlreadyExistsException final : public Exception
	{
		UserNameAlreadyExistsException(const std::string& _message) : Exception(_message) {}
		UserNameAlreadyExistsException(const char* _message) : Exception(_message) {}
	};

	struct WrongPasswordException final : public Exception
	{
		WrongPasswordException(const std::string& _message) : Exception(_message) {}
		WrongPasswordException(const char* _message) : Exception(_message) {}
	};

	struct UserNameDoesNotExistException final : public Exception
	{
		UserNameDoesNotExistException(const std::string& _message) : Exception(_message) {}
		UserNameDoesNotExistException(const char* _message) : Exception(_message) {}
	};
}