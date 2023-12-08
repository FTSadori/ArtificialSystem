#pragma once
#include "ConsoleWindow.h"
#include "TextAttributes.h"
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
		ScreenText(Size _size)
			: c_size(_size),
			  c_default_back(Colours::BLACK), c_default_fore(Colours::WHITE),
			  c_secondary_fore(Colours::GRAY)
		{}

		ScreenText(Size _size, Colours _back, Colours _fore, Colours _secondary)
			: c_size(_size),
			c_default_back(_back), c_default_fore(_secondary),
			c_secondary_fore(_fore)
		{}

		void push_text(const std::string& text, TextAttributes attributes);
		void render_text_from(ScreenPoint absolute, size_t line_num);

	private:

		const Colours c_default_back;
		const Colours c_default_fore;
		const Colours c_secondary_fore;
		const Size c_size;
		std::vector<std::string> m_text;
		std::map<size_t, std::vector<AttributePoint>> m_point_map;
	};
}