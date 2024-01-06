#include "NetContainer.h"

namespace Commands
{
	void NetContainer::add_new(const std::string& address, const NetData& data)
	{
		if (!m_data.contains(address))
			m_data[address] = data;
	}

	bool NetContainer::has(const std::string& address)
	{
		return m_data.contains(address);
	}

	const NetData& NetContainer::get(const std::string& address)
	{
		return m_data.at(address);
	}
}