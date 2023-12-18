#include "TextEditorWindow.h"

namespace GUI
{
	TextEditorWindow::TextEditorWindow(const Memory::FullPath& _path, Size _size, ScreenPoint _position, const std::string& _title, Colours _background)
	: BaseWindow(_size, _position, _title, _background), m_file_path(_path)
	{
		m_lines.push_back("");
	}

	void TextEditorWindow::render_text()
	{
		ScreenText text(Size{ m_size.rows, uint16_t(m_size.columns - c_max_num_size) });
		std::vector<size_t> nums;
		for (size_t i = 0; i < m_lines.size(); ++i)
		{
			nums.push_back(text.get_lines_num() - 1);
			if (m_line_num == i)
			{
				text.push_text(m_lines[i].substr(0, m_cursor_in_line), m_main);
				if (m_cursor_in_line < m_lines[i].size())
				{
					text.push_text(m_lines[i][m_cursor_in_line], m_selection);
					text.push_text(m_lines[i].substr(m_cursor_in_line + 1), m_main);
				}
				else
					text.push_text(" ", m_selection);

				if (text.get_lines_num() - 1 < m_render_from_line)
					--m_render_from_line;
				else if (text.get_lines_num() - 1 >= m_render_from_line + m_size.rows)
					++m_render_from_line;
			}
			else
				text.push_text(m_lines[i], m_main);
			text.push_text("\n", m_main);
		}

		ScreenText numbers(Size{ m_size.rows, c_max_num_size });
		numbers.push_text(" 1", m_secondary);
		nums[0] = 0;
		for (size_t i = 1; i < nums.size(); ++i)
		{
			numbers.push_text(std::string(nums[i] - nums[i - 1], '\n') + " " + Commands::Parser::to_string(i + 1), m_secondary);
		}

		text.render_text_from({ m_position.x, int16_t(m_position.y + c_max_num_size) }, m_render_from_line);
		numbers.render_text_from(m_position, m_render_from_line);
	}

	void TextEditorWindow::on_printable(SHORT code)
	{
		m_lines[m_line_num] = m_lines[m_line_num].substr(0, m_cursor_in_line)
			+ CHAR(code)
			+ m_lines[m_line_num].substr(m_cursor_in_line);
		++m_cursor_in_line;
	}

	void TextEditorWindow::on_backspace()
	{
		if (m_cursor_in_line == 0 && m_line_num > 0)
		{
			m_cursor_in_line = m_lines[m_line_num - 1].size();
			m_lines[m_line_num - 1] += m_lines[m_line_num];
			m_lines.erase(m_lines.begin() + m_line_num);
			prev_line_num();
		}
		else if (m_cursor_in_line > 0)
		{
			m_lines[m_line_num] = m_lines[m_line_num].substr(0, m_cursor_in_line - 1)
				+ m_lines[m_line_num].substr(m_cursor_in_line);
			--m_cursor_in_line;
		}
	}

	void TextEditorWindow::on_enter()
	{
		m_lines.insert(m_lines.begin() + m_line_num + 1, m_lines[m_line_num].substr(m_cursor_in_line));
		m_lines[m_line_num] = m_lines[m_line_num].substr(0, m_cursor_in_line);
		next_line_num();
		m_cursor_in_line = 0;
	}

	void TextEditorWindow::on_left()
	{
		if (m_cursor_in_line == 0 && m_line_num > 0)
		{
			m_cursor_in_line = m_lines[m_line_num - 1].size();
			prev_line_num();
		}
		else if (m_cursor_in_line > 0)
		{
			--m_cursor_in_line;
		}
	}

	void TextEditorWindow::on_right()
	{
		if (m_cursor_in_line == m_lines[m_line_num].size() && m_line_num < m_lines.size() - 1)
		{
			m_cursor_in_line = 0;
			next_line_num();
		}
		else if (m_cursor_in_line < m_lines[m_line_num].size())
		{
			++m_cursor_in_line;
		}
	}

	void TextEditorWindow::on_up()
	{
		if (m_line_num > 0)
		{
			prev_line_num();
			m_cursor_in_line = min(m_lines[m_line_num].size(), m_cursor_in_line);
		}
	}

	void TextEditorWindow::on_down()
	{
		if (m_line_num < m_lines.size() - 1)
		{
			next_line_num();
			m_cursor_in_line = min(m_lines[m_line_num].size(), m_cursor_in_line);
		}
	}

	void TextEditorWindow::set_colours(Colours background, Colours main, Colours secondary)
	{
		m_background = background;
		m_main = TextAttributes(TextColours(main, background), TextBorders());
		m_secondary = TextAttributes(TextColours(secondary, background), TextBorders());
		m_selection = TextAttributes(TextColours(background, main), TextBorders());
	}

	void TextEditorWindow::next_line_num()
	{
		++m_line_num;
		if (m_render_from_line + m_size.rows == m_line_num)
		{
			++m_render_from_line;
		}
	}

	void TextEditorWindow::prev_line_num()
	{
		--m_line_num;
		if (m_render_from_line > m_line_num)
		{
			--m_render_from_line;
		}
	}

	Memory::FullPath TextEditorWindow::get_file_path()
	{
		return m_file_path;
	}

	std::string TextEditorWindow::get_text()
	{
		return Commands::Separator::join(m_lines, '\n');
	}
}
