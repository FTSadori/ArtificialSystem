#include "DataQueue.h"

namespace Memory
{
	void DataQueue::push_char(char c)
	{
		m_data.push_back(c);
	}

	void DataQueue::fix_last_char()
	{
		m_data.pop_back();
	}

	const char* DataQueue::get_data() const
	{
		return m_data.data();
	}

	size_t DataQueue::size() const
	{
		return m_data.size();
	}

	void DataQueue::concat(const DataQueue& _other)
	{
		m_data.insert(m_data.end(), _other.m_data.begin(), _other.m_data.end());
	}

	char DataQueue::operator[](size_t index) const
	{
		return m_data[index];
	}

	DataQueue::DataQueue(const std::vector<char>& vec)
	{
		concat_vector(vec);
	}

	void DataQueue::concat_vector(const std::vector<char>& vec)
	{
		m_data.insert(m_data.end(), vec.begin(), vec.end());
	}
}