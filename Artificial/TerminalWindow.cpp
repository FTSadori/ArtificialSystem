#include "TerminalWindow.h"

namespace GUI
{
	TerminalWindow::TerminalWindow(const Memory::FullPath& _path, const std::string& _user, Size _size, ScreenPoint _position, const std::string& _title)
		: BaseWindow(_size, _position, _title), m_path_line(_path.full_path_str()), m_user(_user)
	{
	}

	TerminalWindow::TerminalWindow(Size _size, ScreenPoint _position, const std::string& _title)
		: BaseWindow(_size, _position, _title)
	{
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

		{
			std::lock_guard lock(m_text_mutex);
			m_text_parts.emplace_back(line, attributes);
		}
	}

	void TerminalWindow::wait_for_input(TerminalInputType type)
	{
		std::unique_lock lock(m_text_mutex);
		m_caninput = true;
		m_current_input_type = type;

		if (type == TerminalInputType::COMMAND)
			start_new_command();

		m_cv.wait(lock, [&] { return m_entered; });
	}

	std::string TerminalWindow::get_last_input()
	{
		std::lock_guard lock(m_text_mutex);
		m_entered = false;
		std::string input = m_last_input;
		m_last_input = "";
		return input;
	}

	void TerminalWindow::render_text()
	{
		load_screen_text().render_text_end(m_position);
	}

	void TerminalWindow::on_printable(SHORT code)
	{
		m_last_input = m_last_input.substr(0, m_cursor_in_input)
			+ CHAR(code)
			+ m_last_input.substr(m_cursor_in_input);
		++m_cursor_in_input;
	}

	void TerminalWindow::on_backspace()
	{
		if (m_last_input.size() != 0)
		{
			m_last_input = m_last_input.substr(0, m_cursor_in_input - 1)
				+ m_last_input.substr(m_cursor_in_input);
			--m_cursor_in_input;
		}
	}

	void TerminalWindow::on_enter()
	{
		m_cursor_in_input = 0;
		m_buffer.push_back(m_last_input);
		m_text_parts.emplace_back(m_buffer.back() + '\n', m_main);
		m_command_line = m_buffer.size();
		m_entered = true;
		m_caninput = false;
		m_cv.notify_one();
	}

	void TerminalWindow::on_left()
	{
		m_cursor_in_input = max(1, m_cursor_in_input) - 1;
	}

	void TerminalWindow::on_right()
	{
		m_cursor_in_input = min(m_cursor_in_input + 1, m_last_input.size());
	}

	void TerminalWindow::on_up()
	{
		if (m_current_input_type == TerminalInputType::COMMAND)
		{
			m_command_line = max(1, m_command_line) - 1;
			if (m_buffer.size() > 0)
			{
				m_last_input = m_buffer[m_command_line];
				m_cursor_in_input = m_last_input.size();
			}
		}
	}

	void TerminalWindow::on_down()
	{
		if (m_current_input_type == TerminalInputType::COMMAND && m_command_line != m_buffer.size())
		{
			m_command_line = min(m_command_line + 1, m_buffer.size() - 1);
			m_last_input = m_buffer[m_command_line];
			m_cursor_in_input = m_last_input.size();
		}
	}

	void TerminalWindow::set_path(const Memory::FullPath& _path)
	{
		m_path_line = _path.full_path_str();
	}

	void TerminalWindow::set_user_name(const std::string& _name)
	{
		m_user = _name;
	}

	Memory::FullPath TerminalWindow::get_path()
	{
		return m_path_line;
	}

	std::string TerminalWindow::get_user_name()
	{
		return m_user;
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

		stext.push_text(input.substr(0, m_cursor_in_input), m_main);
		if (m_cursor_in_input < input.size())
		{
			stext.push_text(input[m_cursor_in_input], m_selection);
			stext.push_text(input.substr(m_cursor_in_input + 1), m_main);
		}
		else
			stext.push_text(" ", m_selection);
		return stext;
	}

	void TerminalWindow::start_new_command()
	{
		m_text_parts.emplace_back("(" + m_user + ") ", m_third);
		m_text_parts.emplace_back(m_path_line + " $ ", m_secondary);

		rerender();
	}
}
