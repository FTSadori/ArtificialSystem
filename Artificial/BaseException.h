#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(const std::string& _message) : m_message(_message) {}
	Exception(const char* _message) : m_message(_message) {}
	virtual const char* what() const { return m_message.c_str(); }

protected:
	std::string m_message;
};