#pragma once
#include "ConsoleWindow.h"
#include "TextAttributes.h"
#include "TextInfo.h"
#include "ColourTheme.h"
#include <string>
#include <vector>
#include <map>

namespace GUI 
{
	struct AttributePoint
	{
		size_t symbol_num = 0;
		TextAttributes attribute = 0;
	};

	class ScreenText final
	{
	public:
		ScreenText(Size _size, TextAttributes _main, TextAttributes _secondary, TextAttributes _third, TextAttributes _selection)
			: c_size(_size), m_main(_main), m_secondary(_secondary), m_third(_third), m_selection(_selection)
		{}

		void change_theme(TextAttributes _main, TextAttributes _secondary, TextAttributes _third, TextAttributes _selection);
		void push_text(const std::string& text, TextType type, TextAttributes attributes);
		void push_text(const std::vector<TextInfo>& info_array);
		void push_text(char c, TextType type, TextAttributes attributes);
		void render_text_from(ScreenPoint absolute, size_t line_num);
		size_t get_lines_num();
		void render_text_end(ScreenPoint absolute);

	private:
		const Size c_size;
		TextAttributes m_main;
		TextAttributes m_secondary;
		TextAttributes m_third;
		TextAttributes m_selection;
		std::vector<std::string> m_text;
		std::map<size_t, std::vector<AttributePoint>> m_point_map;
	};
}