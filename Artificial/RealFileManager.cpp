#include "RealFileManager.h"
#include "Exceptions.h"
#include <fstream>
#include <iostream>

void RealFileManager::write_into_real_file(SectorName _name, const std::vector<uint8_t>& _raw)
{
	std::ofstream out(_name.get_name());
	for (uint8_t n : _raw)
		out << (char)(n ^ m_key);
	out.close();
}

std::vector<uint8_t> RealFileManager::read_from_real_file(SectorName _name)
{
	std::ifstream in(_name.get_name());
	if (in.bad())
		throw FileDoesNotExist("Sector file does not exist");

	std::vector<uint8_t> data;

	while (!in.eof())
		data.push_back((uint8_t)in.get() ^ m_key);
	
	data.pop_back();

	in.close();

	return data;
}

void RealFileManager::delete_real_file(SectorName _name)
{
	remove(_name.get_name().c_str());
}
