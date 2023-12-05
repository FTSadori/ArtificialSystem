#include "Console.h"

namespace GUI
{
	HANDLE Console::m_console_handler = NULL;
	uint16_t Console::m_last_text_attributes = 15;

	void Console::init()
	{
		m_console_handler = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void Console::set_text_colours(const TextColours& _color)
	{
		m_last_text_attributes &= 0xFFFF0000;
		m_last_text_attributes |= _color.m_code;

		SetConsoleTextAttribute(m_console_handler, m_last_text_attributes);
	}

	void Console::set_text_borders(const TextBorders& _borders)
	{
		m_last_text_attributes &= ~COMMON_LVB_GRID_HORIZONTAL;
		m_last_text_attributes &= ~COMMON_LVB_GRID_LVERTICAL;
		m_last_text_attributes &= ~COMMON_LVB_GRID_RVERTICAL;
		m_last_text_attributes &= ~COMMON_LVB_UNDERSCORE;
		m_last_text_attributes |= _borders.m_code;

		SetConsoleTextAttribute(m_console_handler, m_last_text_attributes);
	}

	void Console::set_text_attributes(const TextAttributes& _attributes)
	{
		m_last_text_attributes = _attributes.m_code;

		SetConsoleTextAttribute(m_console_handler, m_last_text_attributes);
	}
}
