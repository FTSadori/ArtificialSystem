#include "ScreenText.h"
#include "Separator.h"
#include "Parser.h"

namespace GUI
{
	void ScreenText::change_theme(TextAttributes _main, TextAttributes _secondary, TextAttributes _third, TextAttributes _selection)
	{
		m_main = _main;
		m_secondary = _secondary;
		m_third = _third;
		m_selection = _selection;
	}

	void ScreenText::push_text(const std::string& text, TextType type, TextAttributes attributes)
	{
		if (text.empty()) return;

		switch (type)
		{
		case GUI::TextType::MAIN:      attributes = m_main;      break;
		case GUI::TextType::SECONDARY: attributes = m_secondary; break;
		case GUI::TextType::THIRD:     attributes = m_third;     break;
		case GUI::TextType::SELECTION: attributes = m_selection; break;
		}
		
		auto lines = Separator::split(text, '\n');
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
			
			//
			if (lines.size() == 0)
				lines.push_back("");
			//
			
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
			push_text(info.text, info.type, info.attributes);
	}

	void ScreenText::push_text(char c, TextType type, TextAttributes attributes)
	{
		push_text(std::string(1, c), type, attributes);
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
				printf(m_text[i].substr(prev).c_str());
			}
			else printf(m_text[i].c_str());
			
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
