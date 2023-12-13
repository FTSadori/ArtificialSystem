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

	void TerminalWindow::print_main(const std::string& line)
	{
		print(line, m_main);
	}

	void TerminalWindow::print_secondary(const std::string& line)
	{
		print(line, m_secondary);
	}

	void TerminalWindow::print_third(const std::string& line)
	{
		print(line, m_third);
	}

	void TerminalWindow::print(const std::string& line, TextAttributes attributes)
	{
		if (m_caninput) return;

		std::lock_guard lock(m_text_mutex);
		m_text_parts.emplace_back(line, attributes);
	}

	void TerminalWindow::wait_for_input(TerminalInputType type)
	{
		if (m_caninput) return;
	
		std::unique_lock lock(m_text_mutex);
		m_caninput = true;
		m_current_input_type = type;
		m_cv.wait(lock, [&] { return m_entered; });
	}

	std::string TerminalWindow::get_last_input()
	{
		std::lock_guard lock(m_text_mutex);
		m_entered = false;
		return m_last_input;
	}

	void TerminalWindow::key_pressed(KEY_EVENT_RECORD key_event)
	{
		if (!m_caninput) { return; }

		SHORT code = key_event.uChar.AsciiChar;

		// printable symbol
		if (code >= 32 && code <= 126)
		{
			m_last_input = m_last_input.substr(0, m_cursor_in_input)
				+ CHAR(code)
				+ m_last_input.substr(m_cursor_in_input);
			++m_cursor_in_input;
		}
		else
		{
			switch (key_event.wVirtualKeyCode)
			{
			// backspace
			case 8:
				if (m_last_input.size() != 0)
				{
					m_last_input = m_last_input.substr(0, m_cursor_in_input - 1)
						+ m_last_input.substr(m_cursor_in_input);
					--m_cursor_in_input;
				}
				break;
			// enter
			case 13:
				m_cursor_in_input = 0;
				m_buffer.push_back(m_last_input);
				m_last_input = "";
				m_text_parts.emplace_back(m_buffer.back() + '\n', m_main);
				start_new_command();
				m_command_line = m_buffer.size();
				m_entered = true;
				m_caninput = false;
				break;
			// left
			case 37:
				m_cursor_in_input = max(1, m_cursor_in_input) - 1;
				break;
			// right
			case 39:
				m_cursor_in_input = min(m_cursor_in_input + 1, m_last_input.size());
				break;
			// up
			case 38:
				if (m_current_input_type == TerminalInputType::COMMAND)
				{
					m_command_line = max(1, m_command_line) - 1;
					m_last_input = m_buffer[m_command_line];
					m_cursor_in_input = m_last_input.size();
				}
				break;
			// down
			case 40:
				if (m_current_input_type == TerminalInputType::COMMAND && m_command_line != m_buffer.size())
				{
					m_command_line = min(m_command_line + 1, m_buffer.size() - 1);
					m_last_input = m_buffer[m_command_line];
					m_cursor_in_input = m_last_input.size();
				}
				break;
			default:
				break;
			}
		}

		render_background();
		render_text_end();
	}
	
	void TerminalWindow::render_text_end() const
	{
		load_screen_text().render_text_end(m_position);
	}

	void TerminalWindow::render_text_line() const
	{
		load_screen_text().render_text_from(m_position, m_line_num);
	}

	void TerminalWindow::set_path(const Memory::FullPath& _path)
	{
		m_path = _path;
	}

	void TerminalWindow::set_user_name(const std::string& _name)
	{
		m_user = _name;
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
		
		std::string input;
		if (m_current_input_type == TerminalInputType::PASSWORD)
			input = std::string(m_last_input.size(), '*');
		else
			input = m_last_input;

		stext.push_text(m_last_input.substr(0, m_cursor_in_input), m_main);
		if (m_cursor_in_input < m_last_input.size())
		{
			stext.push_text(m_last_input[m_cursor_in_input], m_selection);
			stext.push_text(m_last_input.substr(m_cursor_in_input + 1), m_main);
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
