#include "TerminalWindow.h"

namespace GUI
{
	TerminalWindow::TerminalWindow(const Memory::FullPath& _path, const std::string& _user, Size _size, ScreenPoint _position, const std::string& _title, Colours _background)
		: BaseWindow(_size, _position, _title, _background), m_path(_path), m_user(_user)
	{
		m_buffer.push_back("");
		m_command_line = 0;
		start_new_command();
	}

	void TerminalWindow::key_pressed(KEY_EVENT_RECORD key_event)
	{
		SHORT code = key_event.uChar.AsciiChar;
		
		if (!m_canprint)
		{
			if (code >= 32 && code <= 126)
			{
				m_buffer.back() = m_buffer.back().substr(0, m_cursor_in_buffer) 
					+ CHAR(code) 
					+ m_buffer.back().substr(m_cursor_in_buffer);
				++m_cursor_in_buffer;
				render_background();
				render_text_end();
				return;
			}

			ScreenText text(m_size);
			switch (key_event.wVirtualKeyCode)
			{
			case 8:
				if (m_buffer.back().size() != 0)
				{
					m_buffer.back() = m_buffer.back().substr(0, m_cursor_in_buffer - 1)
						+ m_buffer.back().substr(m_cursor_in_buffer);
					--m_cursor_in_buffer;
				}
				break;
			case 13:
				m_cursor_in_buffer = 0;
				m_buffer.back() += '\n';
				m_text_parts.emplace_back(m_buffer.back(), m_main);
				start_new_command();
				m_buffer.push_back("");
				m_command_line = m_buffer.size() - 1;
				break;
			case 37:
				m_cursor_in_buffer = max(1, m_cursor_in_buffer) - 1;
				break;
			case 39:
				m_cursor_in_buffer = min(m_cursor_in_buffer + 1, m_buffer.back().size());
				break;
			case 38:
				m_command_line = max(1, m_command_line) - 1;
				m_buffer.back() = m_buffer[m_command_line];
				m_cursor_in_buffer = m_buffer.back().size();
				if (m_command_line != m_buffer.size() - 1)
				{
					m_buffer.back().pop_back();
					m_cursor_in_buffer--;
				}
				break;
			case 40:
				m_command_line = min(m_command_line + 1, m_buffer.size() - 1);
				m_buffer.back() = m_buffer[m_command_line];
				m_cursor_in_buffer = m_buffer.back().size();
				if (m_command_line != m_buffer.size() - 1)
				{
					m_buffer.back().pop_back();
					m_cursor_in_buffer--;
				}
				break;
			default:
				break;
			}

			render_background();
			render_text_end();
		}
	}
	
	void TerminalWindow::render_text_end() const
	{
		load_screen_text().render_text_end(m_position);
	}

	void TerminalWindow::render_text_line() const
	{
		load_screen_text().render_text_from(m_position, m_line_num);
	}

	void TerminalWindow::set_colours(Colours background, Colours main, Colours secondary, Colours third)
	{
		m_background = background;
		m_main = TextAttributes(TextColours(main, background), TextBorders());
		m_secondary = TextAttributes(TextColours(secondary, background), TextBorders());
		m_third = TextAttributes(TextColours(third, background), TextBorders());
		m_selection = TextAttributes(TextColours(background, main), TextBorders());
	}
	
	ScreenText TerminalWindow::load_screen_text() const
	{
		ScreenText stext(m_size);
		stext.push_text(m_text_parts);
		stext.push_text(m_buffer.back().substr(0, m_cursor_in_buffer), m_main);
		if (m_cursor_in_buffer < m_buffer.back().size())
		{
			stext.push_text(m_buffer.back()[m_cursor_in_buffer], m_selection);
			stext.push_text(m_buffer.back().substr(m_cursor_in_buffer + 1), m_main);
		}
		else
			stext.push_text(" ", m_selection);
		return stext;
	}

	void TerminalWindow::start_new_command()
	{
		m_text_parts.emplace_back("(" + m_user + ") ", m_third);
		m_text_parts.emplace_back(m_path.full_disk_name() + " $ ", m_secondary);
	}
}
