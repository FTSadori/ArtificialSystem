#pragma once
#include <exception>

namespace Core::Memory
{
	class DirectoryDoesntExist final : public std::exception
	{
	public:
		DirectoryDoesntExist(const char* _message) : m_message(_message) {}
		const char* what() const { return m_message; }

	private:
		const char* m_message;
	};

	class CantDeleteRootDirectory final : public std::exception
	{
	public:
		CantDeleteRootDirectory(const char* _message) : m_message(_message) {}
		const char* what() const { return m_message; }

	private:
		const char* m_message;
	};

	class PointerAlreadyUsed final : public std::exception
	{
	public:
		PointerAlreadyUsed(const char* _message) : m_message(_message) {}
		const char* what() const { return m_message; }

	private:
		const char* m_message;
	};

}