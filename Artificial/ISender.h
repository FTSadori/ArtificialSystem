#pragma once
#include <stdint.h>

namespace Commands
{
	class ISender
	{
	public:
		virtual bool sudo() const = 0;
		virtual uint8_t get_lvl() const = 0;
	};
}