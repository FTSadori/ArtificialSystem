#pragma once
#include "DataQueue.h"

namespace Memory
{
	class ILoadable
	{
	public:
		virtual DataQueue get_as_data() const = 0;
		virtual void load_from_data(DataQueue& _data) = 0;

		virtual ~ILoadable() {}
	};
}
