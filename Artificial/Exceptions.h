#pragma once
#include "BaseException.h"

namespace Memory
{
	struct NotEnoughFreeSpace final : public Exception
	{
		NotEnoughFreeSpace(const std::string& _message) : Exception(_message) {}
		NotEnoughFreeSpace(const char* _message) : Exception(_message) {}
	};

	struct WrongFileDeletion final : public Exception
	{
		WrongFileDeletion(const std::string& _message) : Exception(_message) {}
		WrongFileDeletion(const char* _message) : Exception(_message) {}
	};

	struct WrongFileReading final : public Exception
	{
		WrongFileReading(const std::string& _message) : Exception(_message) {}
		WrongFileReading(const char* _message) : Exception(_message) {}
	};

	struct FileIsBlocked final : public Exception
	{
		FileIsBlocked(const std::string& _message) : Exception(_message) {}
		FileIsBlocked(const char* _message) : Exception(_message) {}
	};

	struct FileAlreadyExists final : public Exception
	{
		FileAlreadyExists(const std::string& _message) : Exception(_message) {}
		FileAlreadyExists(const char* _message) : Exception(_message) {}
	};

	struct FileDoesNotExist final : public Exception
	{
		FileDoesNotExist(const std::string& _message) : Exception(_message) {}
		FileDoesNotExist(const char* _message) : Exception(_message) {}
	};

	struct SectorFileIsFull final : public Exception
	{
		SectorFileIsFull(const std::string& _message) : Exception(_message) {}
		SectorFileIsFull(const char* _message) : Exception(_message) {}
	};
}