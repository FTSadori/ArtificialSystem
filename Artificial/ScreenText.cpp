#include "ScreenText.h"
#include "Separator.h"
#include "Parser.h"

namespace GUI
{
	void ScreenText::push_text(const std::string& text, TextAttributes attributes)
	{
		if (text.empty()) return;
		
		auto lines = Commands::Separator::split(text, '\n');
		size_t i = 0;

		size_t cur_lines = m_text.size();
		if (cur_lines == 0 || m_text.back().size() == c_size.columns)
		{
			m_point_map.emplace(cur_lines, std::vector<AttributePoint>({ {0, attributes} }));
		}
		else
		{
			size_t last_size = m_text.back().size();

			m_point_map[cur_lines - 1];
			m_point_map[cur_lines - 1].push_back({ last_size, attributes });

			size_t remains = c_size.columns - last_size;
			if (lines[0].size() <= remains)
			{
				m_text.back() += lines[0];
				++i;
			}
			else
			{
				m_text.back() += lines[0].substr(0, remains);
				lines[0] = lines[0].substr(remains);
			}
		}

		for (; i < lines.size(); ++i)
		{
			while (lines[i].size() > c_size.columns)
			{
				m_text.push_back(lines[i].substr(0, c_size.columns));
				lines[i] = lines[i].substr(c_size.columns);
			}
			m_text.push_back(lines[i]);
		}
	}

	void ScreenText::push_text(const std::vector<TextInfo>& info_array)
	{
		for (const auto& info : info_array)
			push_text(info.text, info.attributes);
	}

	void ScreenText::push_text(char c, TextAttributes attributes)
	{
		push_text(std::string(1, c), attributes);
	}

	void ScreenText::render_text_from(ScreenPoint absolute, size_t line_num)
	{
		TextAttributes attribute(0);

		if (m_point_map.contains(line_num) && m_point_map[line_num].front().symbol_num == 0)
		{
			attribute = m_point_map[line_num].front().attribute;
		}
		else for (auto it = m_point_map.rbegin(); it != m_point_map.rend(); it++)
		{
			if (it->first < line_num)
				attribute = it->second.back().attribute;
		}

		size_t rows_used = 0;
		ConsoleWindow::set_text_attributes(attribute);

		for (size_t i = line_num; i < min(m_text.size(), line_num + c_size.rows); ++i)
		{
			ConsoleWindow::set_cursor_position(absolute);
			
			if (m_point_map.contains(i))
			{
				size_t prev = 0;
				for (const auto& point : m_point_map[i])
				{
					std::cout << m_text[i].substr(prev, point.symbol_num - prev);
					prev = point.symbol_num;
					ConsoleWindow::set_text_attributes(point.attribute);
				}
				std::cout << m_text[i].substr(prev);
			}
			else std::cout << m_text[i];
			
			absolute.x += 1;
			rows_used += 1;
		}
	}
	
	size_t ScreenText::get_lines_num()
	{
		return m_text.size();
	}

	void ScreenText::render_text_end(ScreenPoint absolute)
	{
		if (m_text.size() <= c_size.rows)
			render_text_from(absolute, 0);
		else
			render_text_from(absolute, m_text.size() - c_size.rows);
	}
}
