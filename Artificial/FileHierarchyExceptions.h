#pragma once
#include "BaseException.h"

namespace Memory
{
	struct DirectoryDoesntExist final : public Exception
	{
		DirectoryDoesntExist(const std::string& _message) : Exception(_message) {}
		DirectoryDoesntExist(const char* _message) : Exception(_message) {}
	};

	struct CantDeleteRootDirectory final : public Exception
	{
		CantDeleteRootDirectory(const std::string& _message) : Exception(_message) {}
		CantDeleteRootDirectory(const char* _message) : Exception(_message) {}
	};

	struct PointerAlreadyUsed final : public Exception
	{
		PointerAlreadyUsed(const std::string& _message) : Exception(_message) {}
		PointerAlreadyUsed(const char* _message) : Exception(_message) {}
	};

	struct RootAlreadyExists final : public Exception
	{
		RootAlreadyExists(const std::string& _message) : Exception(_message) {}
		RootAlreadyExists(const char* _message) : Exception(_message) {}
	};

	struct RootIsTheHighestDirectory final : public Exception
	{
		RootIsTheHighestDirectory(const std::string& _message) : Exception(_message) {}
		RootIsTheHighestDirectory(const char* _message) : Exception(_message) {}
	};

	struct RecursiveDirectoryMove final : public Exception
	{
		RecursiveDirectoryMove(const std::string& _message) : Exception(_message) {}
		RecursiveDirectoryMove(const char* _message) : Exception(_message) {}
	};
}