#include "FullPath.h"

namespace Memory
{
	FullPath::FullPath(const std::string& _mark, const DiskPath& _path)
		: m_path(_path), m_mark(_mark)
	{
		m_full_disk_name = _mark + c_disk_div + _path.full_name();
		m_full_dir_name = _mark + c_disk_div + _path.dir();
	}

	FullPath::FullPath(const std::string& _path)
	{
		if (_path.find(c_disk_div) == std::string::npos)
			throw NameIsNotValid("(FullPath::FullPath) " + _path + " is not valid path");

		size_t i = _path.size();

		while (_path[--i] != c_disk_div);

		m_mark = _path.substr(0, i);
		m_path = DiskPath(_path.substr(i + 1));
		
		m_full_disk_name = m_mark + c_disk_div + m_path.full_name();
		m_full_dir_name = m_mark + c_disk_div + m_path.dir();
	}

	const std::string& FullPath::full_disk_name() const
	{
		return m_full_disk_name;
	}

	const DiskPath& FullPath::disk_path() const
	{
		return m_path;
	}

	const std::string& FullPath::full_dir_name() const
	{
		return m_full_dir_name;
	}

	const std::string& FullPath::mark() const
	{
		return m_mark;
	}

}