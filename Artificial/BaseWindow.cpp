#include "BaseWindow.h"

namespace GUI
{
	std::mutex BaseWindow::m_render_mutex;

	Size BaseWindow::s_max_size{ 1024, 1024 };
	Size BaseWindow::s_min_size{ 2, 2 };

	BaseWindow::BaseWindow(Size _size, ScreenPoint _position, const std::string& _title)
	{
		set_size(_size);
		set_position(_position);
		set_title(_title);
	}

	void BaseWindow::set_size(Size _size)
	{
		m_size.columns = min(max(_size.columns, s_min_size.columns), s_max_size.columns);
		m_size.rows = min(max(_size.rows, s_min_size.rows), s_max_size.rows);
	}
	
	void BaseWindow::set_title(std::string _title)
	{
		m_title = (_title.empty() ? "Window" : _title);
	}
	
	void BaseWindow::set_position(ScreenPoint _position)
	{
		m_position.x = max(1, _position.x);
		m_position.y = max(1, _position.y);
	}

	void BaseWindow::set_window_colours(Colours _background, Colours _border)
	{
		m_background = _background;
		m_border = _border;
	}

	void BaseWindow::set_text_colours(TextColourTheme theme)
	{
		m_main = TextAttributes(TextColours(theme.main, m_background), TextBorders());
		m_secondary = TextAttributes(TextColours(theme.secondary, m_background), TextBorders());
		m_third = TextAttributes(TextColours(theme.third, m_background), TextBorders());
		m_selection = TextAttributes(TextColours(m_background, theme.main), TextBorders());
	}

	Size BaseWindow::get_size() const { return m_size; }
	const std::string& BaseWindow::get_title() const { return m_title; }
	ScreenPoint BaseWindow::get_position() const { return m_position; }
	
	void BaseWindow::render_border(Colours colours) const
	{
		ConsoleWindow::set_text_colours(TextColours(m_border, colours));

		ConsoleWindow::set_cursor_position({ (int16_t)(m_position.x - 1), m_position.y });
		ConsoleWindow::set_text_borders(TextBorders(false, false, false, true));
		std::cout << std::string(m_size.columns, ' ');
		
		ConsoleWindow::set_cursor_position({ (int16_t)(m_position.x + m_size.rows), m_position.y });
		ConsoleWindow::set_text_borders(TextBorders(false, true, false, false));
		std::cout << std::string(m_size.columns, ' ');

		ConsoleWindow::set_text_borders(TextBorders(false, false, true, false));
		for (int16_t i = 0; i < m_size.rows; ++i)
		{
			ConsoleWindow::set_cursor_position({ (int16_t)(m_position.x + i), (int16_t)(m_position.y - 1) });
			std::cout << " ";
		}

		ConsoleWindow::set_text_borders(TextBorders(true, false, false, false));
		for (int16_t i = 0; i < m_size.rows; ++i)
		{
			ConsoleWindow::set_cursor_position({ (int16_t)(m_position.x + i), (int16_t)(m_position.y + m_size.columns) });
			std::cout << " ";
		}

		ConsoleWindow::set_text_borders(TextBorders(false, false, false, false));
	}

	void BaseWindow::render_background() const
	{
		ConsoleWindow::set_text_attributes(TextAttributes(TextColours(m_background, m_background), TextBorders(false, false, false, false)));
		for (uint16_t i = 0; i < m_size.rows; ++i)
		{
			ConsoleWindow::set_cursor_position({ (SHORT)(m_position.x + i), m_position.y });
			std::cout << std::string(m_size.columns, ' ');
		}
	}

	void BaseWindow::rerender()
	{
		if (m_block) return;
		std::lock_guard lock(m_render_mutex);
		render_background();
		render_text();
	}

	void BaseWindow::key_pressed(KEY_EVENT_RECORD key_event)
	{
		SHORT code = key_event.uChar.AsciiChar;

		if (code >= 32 && code <= 126)
			on_printable(code);
		else
		{
			switch (key_event.wVirtualKeyCode)
			{
			case 8:  on_backspace(); break;
			case 13: on_enter();     break;
			case 37: on_left();      break;
			case 39: on_right();     break;
			case 38: on_up();        break;
			case 40: on_down();      break;

			default:
				break;
			}
		}

		rerender();
	}
}
