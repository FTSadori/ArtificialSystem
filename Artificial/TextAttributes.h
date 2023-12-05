#pragma once
#include "Colours.h"
#include <Windows.h>

namespace GUI
{
	struct TextBorders
	{
		TextBorders(bool left, bool top, bool right, bool bottom)
			: m_code((left * COMMON_LVB_GRID_LVERTICAL) | (top * COMMON_LVB_GRID_HORIZONTAL)
					| (right * COMMON_LVB_GRID_RVERTICAL) | (bottom * COMMON_LVB_UNDERSCORE))
		{}

		const uint16_t m_code;
	};

	struct TextColours
	{
		TextColours(Colours _foreground, Colours _background)
			: m_code((uint8_t)_background * 16 + (uint8_t)_foreground)
		{}

		const uint16_t m_code;
	};

	struct TextAttributes
	{
		TextAttributes(TextColours _colours, TextBorders _borders)
			: m_code(_colours.m_code | _borders.m_code)
		{}
		
		const uint16_t m_code;
	};
}