#pragma once
#include "DataQueueExceptions.h"
#include <vector>

namespace Memory
{
	class DataQueue final
	{
	public:
		DataQueue() {}
		DataQueue(const std::vector<char>& vec);

		void concat_vector(const std::vector<char>& vec);

		void push_char(char c);
		void fix_last_char();
		
		const char* get_data() const;

		char operator[](size_t index) const;
		size_t size() const;

		void concat(const DataQueue& _other);

		template<typename value_t> void push(const value_t& val)
		{
			m_data.insert(m_data.end(), (char*)&val, (char*)&val + sizeof(value_t));
		}

		template<typename value_t> value_t pop()
		{
			if (m_data.size() < sizeof(value_t))
				throw NotEnoughDataInStream("Data in stream is not enough to fill value");
			std::vector<char> first_part(m_data.begin(), m_data.begin() + sizeof(value_t));
			value_t val = *((value_t*)first_part.data());
			m_data.erase(m_data.begin(), m_data.begin() + sizeof(value_t));
			return val;
		}

	private:
		std::vector<char> m_data;
	};
}