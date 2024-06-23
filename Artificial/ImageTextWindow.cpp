#include "ImageTextWindow.h"

namespace GUI
{
    ImageTextWindow::ImageTextWindow(Size _size, ScreenPoint _position, const std::string& _title)
        : BaseWindow(_size, _position, _title)
    {

    }

    void ImageTextWindow::render_text()
    {
        ScreenText text(m_size, m_main, m_secondary, m_third, m_selection);
        text.push_text(m_text_parts);
        text.render_text_from(m_position, m_render_from_line);
    }

    void ImageTextWindow::set_text(const std::string image)
    {
        m_text_parts.clear();
        m_text_parts.push_back(TextInfo{image, TextType::MAIN, m_main});
    }
    
    void ImageTextWindow::on_up()
    {
        m_render_from_line = max(1, m_render_from_line) - 1;
    }

    void ImageTextWindow::on_down()
    {
        m_render_from_line++;
    }
}
