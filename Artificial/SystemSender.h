#pragma once
#include "ISender.h"

namespace Commands
{
	class SystemSender : public ISender
	{
	public:
		virtual bool sudo() const { return true; }
		virtual uint8_t get_lvl() const { return 255; }
	};
}