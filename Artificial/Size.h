#pragma once
#include <cstdint>

namespace GUI
{
	struct Size
	{
		uint16_t rows = 0;
		uint16_t columns = 0;
	};

	bool operator!=(const Size& a, const Size& b);
}