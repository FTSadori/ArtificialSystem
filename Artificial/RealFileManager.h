#pragma once
#include "SectorName.h"
#include <string>
#include <vector>

namespace Core::Memory
{
	class RealFileManager final
	{
	public:
		RealFileManager(uint8_t _key) : m_key(_key) {}

		void write_into_real_file(SectorName _name, const std::vector<uint8_t>& _raw);
		std::vector<uint8_t> read_from_real_file(SectorName _name);
		void delete_real_file(SectorName _name);

	private:
		uint8_t m_key;
	};
}