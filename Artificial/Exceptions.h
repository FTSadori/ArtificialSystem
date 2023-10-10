#pragma once
#include <exception>

namespace Core::Memory
{
	class NotEnoughFreeSpace final : public std::exception
	{
	public:
		NotEnoughFreeSpace(const char* _message) : m_message(_message) {}
		const char* what() const { return m_message; }

	private:
		const char* m_message;
	};

	class WrongFileDeletion final : public std::exception
	{
	public:
		WrongFileDeletion(const char* _message) : m_message(_message) {}
		const char* what() const { return m_message; }

	private:
		const char* m_message;
	};

	class WrongFileReading final : public std::exception
	{
	public:
		WrongFileReading(const char* _message) : m_message(_message) {}
		const char* what() const { return m_message; }

	private:
		const char* m_message;
	};

	class FileIsBlocked final : public std::exception
	{
	public:
		FileIsBlocked(const char* _message) : m_message(_message) {}
		const char* what() const { return m_message; }

	private:
		const char* m_message;
	};

	class FileAlreadyExists final : public std::exception
	{
	public:
		FileAlreadyExists(const char* _message) : m_message(_message) {}
		const char* what() const { return m_message; }

	private:
		const char* m_message;
	};

	class FileDoesNotExist final : public std::exception
	{
	public:
		FileDoesNotExist(const char* _message) : m_message(_message) {}
		const char* what() const { return m_message; }

	private:
		const char* m_message;
	};

	class SectorFileIsFull final : public std::exception
	{
	public:
		SectorFileIsFull(const char* _message) : m_message(_message) {}
		const char* what() const { return m_message; }

	private:
		const char* m_message;
	};
}