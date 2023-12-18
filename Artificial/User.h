#pragma once
#include "ISender.h"

namespace Commands 
{
	class User : public ISender
	{
	public:
		User(bool _sudo, uint8_t _lvl)
			: c_sudo(_sudo), c_lvl(_lvl) 
		{}

		virtual bool sudo() const { return c_sudo; }
		virtual uint8_t get_lvl() const { return c_lvl; }
	protected:
		const bool c_sudo;
		const uint8_t c_lvl;
	};
}