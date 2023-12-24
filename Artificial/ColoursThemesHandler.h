#pragma once
#include "ILoadable.h"
#include "ColourTheme.h"
#include <unordered_map>

namespace GUI
{
	class ColoursThemesHandler final : public Memory::ILoadable
	{
	public:
		virtual Memory::DataQueue get_as_data() const;
		virtual void load_from_data(Memory::DataQueue& _data);

		std::unordered_map<std::string, SystemColourTheme> m_system_themes;
		std::unordered_map<std::string, TextColourTheme> m_text_themes;
	};
}