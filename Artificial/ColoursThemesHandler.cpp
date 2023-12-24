#include "ColoursThemesHandler.h"

namespace GUI
{
    Memory::DataQueue ColoursThemesHandler::get_as_data() const
    {
        Memory::DataQueue data;
        data.push(m_system_themes.size());
        for (const auto& [name, theme] : m_system_themes)
        {
            data.push(name.size());
            data.push(name);
            data.push((uint8_t)theme.background);
            data.push((uint8_t)theme.border);
            data.push((uint8_t)theme.window);
        }
        data.push(m_text_themes.size());
        for (const auto& [name, theme] : m_text_themes)
        {
            data.push(name.size());
            data.push(name);
            data.push((uint8_t)theme.main);
            data.push((uint8_t)theme.secondary);
            data.push((uint8_t)theme.third);
        }
        return data;
    }
    
    void ColoursThemesHandler::load_from_data(Memory::DataQueue& _data)
    {
        m_system_themes.clear();
        m_text_themes.clear();
        
        size_t system_themes_size = _data.pop<size_t>();
        for (size_t i = 0; i < system_themes_size; ++i)
        {
            size_t name_size = _data.pop<size_t>();
            std::string name = "";
            for (size_t j = 0; j < name_size; ++j)
                name += _data.pop<char>();
            SystemColourTheme theme;
            theme.background = (Colours)_data.pop<uint8_t>();
            theme.border = (Colours)_data.pop<uint8_t>();
            theme.window = (Colours)_data.pop<uint8_t>();
        }

        size_t text_themes_size = _data.pop<size_t>();
        for (size_t i = 0; i < text_themes_size; ++i)
        {
            size_t name_size = _data.pop<size_t>();
            std::string name = "";
            for (size_t j = 0; j < name_size; ++j)
                name += _data.pop<char>();
            TextColourTheme theme;
            theme.main = (Colours)_data.pop<uint8_t>();
            theme.secondary = (Colours)_data.pop<uint8_t>();
            theme.third = (Colours)_data.pop<uint8_t>();
        }
    }
}
