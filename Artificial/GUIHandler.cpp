#include "GUIHandler.h"

namespace GUI
{
	GUIHandler::GUIHandler(Colours _background, Colours _window, Colours _border)
		: m_terminal(TerminalWindow({ 5, 10 }, { 2, 2 }, "Terminal", _window)),
		m_background(_background), m_window(_window), m_border(_border)
	{
		start_resize_thread();
	}
	
	void GUIHandler::start_resize_thread()
	{
		std::thread th([&]() {
			Size console_size = { 0, 0 };
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				Size new_console_size = ConsoleWindow::get_console_size();
				if (new_console_size != console_size)
				{
					ConsoleWindow::fill_screen(m_background);
					console_size = new_console_size;
					set_size_to_all({ uint16_t(new_console_size.rows - 3), uint16_t(new_console_size.columns - 4) });
					render_current_window();
					ConsoleWindow::set_cursor_visibility(false);
				}
			}
		});
		th.detach();
	}

	void GUIHandler::render_current_window()
	{
		if (m_current_window == 0)
		{
			m_terminal.render_background();
			m_terminal.render_border(TextColours(m_border, m_background));
			m_terminal.render_text();
		}
	}
	
	void GUIHandler::set_size_to_all(Size new_size)
	{
		m_terminal.set_size(new_size);
	}
}
