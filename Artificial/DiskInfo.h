#pragma once
#include "ILoadable.h"

namespace Memory
{
	struct DiskInfo final : public ILoadable
	{
		DiskInfo() : mark(""), capacity(0), max_sec_num(0) {}
		DiskInfo(const std::string& _mark, size_t _capacity, uint32_t _max_sec_num)
			: mark(_mark), capacity(_capacity), max_sec_num(_max_sec_num) {}

		std::string mark;
		size_t capacity;
		uint32_t max_sec_num;

		virtual DataQueue get_as_data() const override
		{
			DataQueue data;
			data.push<size_t>(mark.size());
			for (char c : mark)
				data.push_char(c);
			data.push<size_t>(capacity);
			data.push<uint32_t>(max_sec_num);
			return data;
		}

		virtual void load_from_data(DataQueue& _data) override
		{
			size_t mark_size = _data.pop<size_t>();
			mark = "";
			for (int i = 0; i < mark_size; ++i)
				mark += _data.pop<char>();
			capacity = _data.pop<size_t>();
			max_sec_num = _data.pop<uint32_t>();
		}
	};
}
