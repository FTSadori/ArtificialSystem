#pragma once
#include "GuiExceptions.h"
#include "TextAttributes.h"
#include <stdint.h>
#include <windows.h>
#include <iostream>

namespace GUI
{
	struct Point
	{
		uint16_t x;
		uint16_t y;
	};

	struct Size
	{
		uint16_t rows;
		uint16_t columns;
	};

	static class Console final
	{
	public:
		static void init();
		static void set_text_colours(const TextColours& _color);
		static void set_text_borders(const TextBorders& _borders);
		static void set_text_attributes(const TextAttributes& _attributes);
	private:
		static HANDLE m_console_handler;
		static uint16_t m_last_text_attributes;
	};
}