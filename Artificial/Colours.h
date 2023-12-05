#pragma once
#include <stdint.h>

namespace GUI
{
	enum class Colours
	{
		BLACK,
		BLUE,
		GREEN,
		TURQUOISE,
		RED,
		VIOLET,
		YELLOW,
		LIGHT_GRAY,
		GRAY,
		LIGHT_BLUE,
		LIGHT_GREEN,
		LIGHT_TURQUOISE,
		LIGHT_RED,
		LIGHT_VIOLET,
		LIGHT_YELLOW,
		WHITE,
	};

	struct ColoursCombiner
	{
		ColoursCombiner(Colours _foreground, Colours _background)
			: m_foreground(_foreground), m_background(_background)
			, m_code((uint8_t)_background * 16 + (uint8_t)_foreground) 
		{}

		const Colours m_foreground;
		const Colours m_background;
		const uint8_t m_code;
	};
}