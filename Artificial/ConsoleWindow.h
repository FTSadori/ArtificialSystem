#pragma once
#include "GuiExceptions.h"
#include "TextAttributes.h"
#include "Size.h"
#include <stdint.h>
#include <windows.h>
#include <iostream>

namespace GUI
{
	struct ScreenPoint
	{
		int16_t x = 0;
		int16_t y = 0;
	};

	class ConsoleWindow final
	{
	public:
		static void init();
		static void set_text_colours(const TextColours& _color);
		static void set_text_borders(const TextBorders& _borders);
		static void set_text_attributes(const TextAttributes& _attributes);
		static Size get_console_size();
		static void set_cursor_visibility(bool visibility);
		static void set_cursor_position(const ScreenPoint point);
		static void fill_screen(Colours background);
		static void delta_cursor_position(const ScreenPoint delta);
	private:
		static HANDLE m_console_handler;
		static uint16_t m_last_text_attributes;
	};
}