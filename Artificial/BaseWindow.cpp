#include "BaseWindow.h"

namespace GUI
{
	Size BaseWindow::s_max_size{ 1024, 1024 };
	Size BaseWindow::s_min_size{ 2, 2 };

	BaseWindow::BaseWindow(Size _size, ScreenPoint _position, const std::string& _title, Colours _background)
		: m_background(_background)
	{
		set_size(_size);
		set_position(_position);
		set_title(_title);
	}

	inline void BaseWindow::set_size(Size _size)
	{
		m_size.columns = min(max(_size.columns, s_min_size.columns), s_max_size.columns);
		m_size.rows = min(max(_size.rows, s_min_size.rows), s_max_size.rows);
	}
	
	inline void BaseWindow::set_title(std::string _title)
	{
		m_title = (_title.empty() ? "Window" : _title);
	}
	
	inline void BaseWindow::set_position(ScreenPoint _position)
	{
		m_position.x = max(1, _position.x);
		m_position.y = max(1, _position.y);
	}

	inline Size BaseWindow::get_size() const { return m_size; }
	inline const std::string& BaseWindow::get_title() const { return m_title; }
	inline ScreenPoint BaseWindow::get_position() const { return m_position; }
	
	void BaseWindow::render_border(TextColours colours)
	{
		ConsoleWindow::set_text_colours(colours);

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
}
