#include "RealFileManager.h"
#include "Exceptions.h"
#include "Base64.h"
#include <fstream>

namespace Memory
{
	void RealFileManager::write_into_real_file(const std::string& _folder, SectorName _name, const DataQueue& _raw)
	{
		auto line = b64encode(_raw.get_data(), _raw.size());

		std::ofstream out(_folder + "\\" + _name.get_name());
		out << line;
		out.close();
	}

	DataQueue RealFileManager::read_from_real_file(const std::string& _folder, SectorName _name)
	{
		std::ifstream in(_folder + "\\" + _name.get_name());
		if (!in.is_open())
			throw FileDoesNotExist("Sector file does not exist");

		DataQueue data;

		while (!in.eof())
			data.push_char((char)(in.get()));

		data.fix_last_char();

		auto line = b64decode(data.get_data(), data.size());

		in.close();

		return DataQueue(std::vector<char>(line.data(), line.data() + line.size()));
	}

	void RealFileManager::delete_real_file(const std::string& _folder, SectorName _name)
	{
		remove((_folder + "\\" + _name.get_name()).c_str());
	}
}
