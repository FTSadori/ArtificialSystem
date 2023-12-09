#pragma once
#include "Colours.h"
#include <Windows.h>

namespace GUI
{
	struct TextBorders
	{
		TextBorders(bool left = false, bool top = false, bool right = false, bool bottom = false)
			: m_code((left * COMMON_LVB_GRID_LVERTICAL) | (top * COMMON_LVB_GRID_HORIZONTAL)
					| (right * COMMON_LVB_GRID_RVERTICAL) | (bottom * COMMON_LVB_UNDERSCORE))
		{}

		uint16_t code() const { return m_code; }

	private:
		uint16_t m_code;
	};

	struct TextColours
	{
		TextColours(Colours _foreground, Colours _background)
			: m_code((uint8_t)_background * 16 + (uint8_t)_foreground)
		{}

		uint16_t code() const { return m_code; }
	
	private:
		uint16_t m_code;
	};

	struct TextAttributes
	{
		TextAttributes(TextColours _colours, TextBorders _borders)
			: m_code(_colours.code() | _borders.code())
		{}

		TextAttributes(uint16_t _code)
			: m_code(_code)
		{}

		uint16_t code() const { return m_code; }

	private:
		uint16_t m_code;
	};
}