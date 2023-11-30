#include "DiskPath.h"
#include "NameExceptions.h"
#include <iostream>

namespace Memory
{
	DiskPath::DiskPath(const std::string& _dir, const std::string& _file) 
		: m_dir(_dir), m_file(_file)
	{
		m_full_name = m_dir + c_div + m_file;
		if (m_full_name.size() == 1) // check for root directory
			m_full_name = "";
	}

	DiskPath::DiskPath(const std::string& _path)
	{
		if (_path == "") // check for root directory
		{
			m_full_name = "";
			m_dir = "";
			m_file = "";
			return;
		}

		if (_path.find(c_div) == std::string::npos)
			throw NameIsNotValid("(DiskPath::DiskPath) " + _path + " is not valid path");

		size_t i = _path.size();

		while (_path[--i] != c_div);

		m_dir = _path.substr(0, i);
		m_file = _path.substr(i + 1);
		if (m_file.back() == c_div)
			m_file = m_file.substr(0, m_file.size() - 1);

		m_full_name = m_dir + c_div + m_file;
	}

	const std::string& DiskPath::full_name() const
	{
		return m_full_name;
	}

	const std::string& DiskPath::dir() const
	{
		return m_dir;
	}

	const std::string& DiskPath::file() const
	{
		return m_file;
	}
}
