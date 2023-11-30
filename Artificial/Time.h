#pragma once
#include <chrono>

namespace Memory
{
	struct Time final
	{
		static time_t now()
		{
			const auto p1 = std::chrono::system_clock::now();
			return std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
		}
	};
}
