#include "Path.h"
#include "NameExceptions.h"
#include <iostream>

namespace Core::Memory
{
	const std::string DiskPath::c_div_str = "\\";

	DiskPath::DiskPath(const std::string& _dir, const std::string& _file) 
		: m_dir(_dir), m_file(_file)
	{
		m_full_name = m_dir + m_file;
	}

	DiskPath::DiskPath(const std::string& _path)
	{
		if (_path[0] != c_div)
		{
			throw PathIsNotValid("(DiskPath::DiskPath) Path " + _path + " is not valid");
		}
		if (_path.back() == c_div)
			m_full_name = _path.substr(0, _path.size() - 1);
		else
			m_full_name = _path;

		size_t i = m_full_name.size();

		if (i == 0)
		{
			m_dir = "";
			m_file = "";
		}
		else
		{
			while (_path[--i] != c_div);

			m_dir = _path.substr(0, i + 1);
			m_file = _path.substr(i + 1);
		}
	}

	std::string DiskPath::full_name(FileT _type) const
	{
		return ((_type == FileT::FOLDER) ? m_full_name + c_div : m_full_name);
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
