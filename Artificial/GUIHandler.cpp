#include "GUIHandler.h"

namespace GUI
{
	GUIHandler::GUIHandler(Commands::ICommandExecutor& _core, Commands::UsersHandler& _users_handler, Colours _background, Colours _window, Colours _border)
		: m_core(_core), m_users_handler(_users_handler),
		m_terminal(TerminalWindow({ 0, 0 }, get_windows_start(), "Terminal", _window)),
		m_background(_background), m_window(_window), m_border(_border)
	{
		start_resize_thread();
		start_input_thread();
	}

	void GUIHandler::open_editor(const Memory::FullPath& path, const std::string& data)
	{
		TextEditorWindow window(path, get_windows_size(m_window_size), get_windows_start(), path.disk_path().file(), m_window);
		m_text_editors.push_back(window);
		// todo add data to window
	}
	
	ScreenPoint GUIHandler::get_windows_start()
	{
		return { SHORT(c_vertical_margin + c_tabs_height), c_horizontal_margin };
	}

	Size GUIHandler::get_windows_size(Size screen_size)
	{
		return Size({
			uint16_t(m_window_size.rows - 2 * c_vertical_margin - c_tabs_height),
			uint16_t(m_window_size.columns - 2 * c_horizontal_margin)
		});
	}

	bool GUIHandler::try_key_combination(KEY_EVENT_RECORD key)
	{
		CHAR code = key.uChar.AsciiChar;

		switch (code)
		{
		case 19: doCtrlS(); break;
		case 26: doCtrlZ(); break;
		case 24: doCtrlX(); break;
		case 17: doCtrlQ(); break;
		default:
			return false;
		}
		return true;
	}

	void GUIHandler::doCtrlS()
	{
		if (m_current_window != 0)
		{
			auto& editor = m_text_editors[m_current_window - 1];

			std::string text = b64encode(editor.get_text());
			auto path = editor.get_file_path();
			Commands::Command cmd(editor.get_file_path().full_dir_name()
				+ " write "
				+ editor.get_file_path().disk_path().file()
				+ " "
				+ text);

			m_core.execute(cmd, m_users_handler.get_current_user());
		}
	}

	void GUIHandler::doCtrlZ()
	{
		m_current_window = max(1, m_current_window) - 1;
		render(m_current_window);
	}

	void GUIHandler::doCtrlX()
	{
		m_current_window = min(m_current_window + 1, m_text_editors.size());
		render(m_current_window);
	}

	void GUIHandler::doCtrlQ()
	{
		if (m_current_window > 0)
		{
			m_text_editors.erase(m_text_editors.begin() + (m_current_window - 1));
			doCtrlX();
		}
	}

	void GUIHandler::start_input_thread()
	{
		ConsoleInputHandler::start();

		std::thread th([&]() {
			while (true)
			{
				ConsoleInputHandler::wait_for_events();
				auto key = ConsoleInputHandler::get_last_key_event();
				if (!key.bKeyDown) continue;
				if (!try_key_combination(key))
				{
					if (m_current_window == 0)
						m_terminal.key_pressed(key);
					else
						m_text_editors[m_current_window - 1].key_pressed(key);
				}
			}
		});
		th.detach();
	}

	void GUIHandler::start_resize_thread()
	{
		std::thread th([&]() {
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				Size new_size = ConsoleWindow::get_console_size();
				if (m_window_size != new_size)
				{
					ConsoleWindow::fill_screen(m_background);
					{
						std::lock_guard lock(m_window_size_mutex);
						m_window_size = ConsoleWindow::get_console_size();
					}
					set_size_to_all(get_windows_size(m_window_size));
					render(m_current_window);
					ConsoleWindow::set_cursor_visibility(false);
				}
			}
		});
		th.detach();
	}

	void GUIHandler::render(size_t window_num)
	{
		render_window(window_num);
		render_header_border(); 
		render_header(window_num);
	}

	void GUIHandler::render_header(size_t window_num)
	{
		const size_t double_margin = 2 * c_horizontal_margin;

		size_t window_width = 0;
		{
			std::lock_guard lock(m_window_size_mutex);
			window_width = max(double_margin, m_window_size.columns) - double_margin;
		}

		size_t full_tab_size = max(window_width / c_full_tab_size_divider, c_min_tab_size);
		size_t mini_tab_size = max(window_width / c_mini_tab_size_divider, c_min_tab_size);

		size_t sum = 0;

		std::vector<std::string> titles;
		titles.push_back("Terminal");
		for (const auto& editor : m_text_editors)
			titles.push_back(editor.get_title());

		m_tabs_from = min(m_tabs_from, window_num);

		ConsoleWindow::set_cursor_position({ c_vertical_margin, c_horizontal_margin });
		for (size_t i = m_tabs_from; i < titles.size(); ++i)
		{
			size_t max_tab_size = (i == window_num) ? full_tab_size : mini_tab_size;

			if (titles[i].size() > max_tab_size)
			{
				titles[i].resize(max_tab_size - 3);
				titles[i] += "...";
			}

			if (sum + titles.size() + 2 >= window_width)
			{
				if (i <= window_num)
				{
					m_tabs_from++;
					render_header(window_num);
				}
				break;
			}

			if (i == window_num)
			{
				ConsoleWindow::set_text_colours(TextColours(m_window, m_border));
				std::cout << " " + titles[i] + " ";
			}
			else
			{
				ConsoleWindow::set_text_colours(TextColours(m_border, m_window));
				ConsoleWindow::set_text_borders(TextBorders(true, false, false, false));
				std::cout << " ";
				ConsoleWindow::set_text_borders(TextBorders(false, false, false, false));
				std::cout << titles[i];
				ConsoleWindow::set_text_borders(TextBorders(false, false, true, false));
				std::cout << " ";
			}
			sum += titles[i].size() + 2;
		}
	}

	void GUIHandler::render_header_border()
	{
		Size size;
		{
			std::lock_guard lock(m_window_size_mutex);
			size = m_window_size;
		}
		ConsoleWindow::set_text_colours(TextColours(m_border, m_background));

		ConsoleWindow::set_cursor_position({ SHORT(c_vertical_margin - 1), c_horizontal_margin });
		ConsoleWindow::set_text_borders(TextBorders(false, false, false, true));
		std::cout << std::string(size.columns - c_horizontal_margin * 2, ' ');

		ConsoleWindow::set_cursor_position({ c_vertical_margin, SHORT(c_horizontal_margin - 1) });
		ConsoleWindow::set_text_borders(TextBorders(false, false, true, false));
		std::cout << ' ';

		ConsoleWindow::set_cursor_position({ c_vertical_margin, SHORT(size.columns - c_horizontal_margin) });
		ConsoleWindow::set_text_borders(TextBorders(true, false, false, false));
		std::cout << ' ';
	}

	void GUIHandler::render_window(size_t window_num)
	{
		BaseWindow* window;
		
		if (window_num == 0)
			window = &m_terminal;
		else 
			window = &m_text_editors[window_num - 1];
		
		window->render_background();
		window->render_border(TextColours(m_border, m_background));
		window->render_text();
	}
	
	void GUIHandler::set_size_to_all(Size new_size)
	{
		if (new_size.columns > m_window_size.columns)
			new_size.columns = 0;
		if (new_size.rows > m_window_size.rows)
			new_size.rows = 0;

		m_terminal.set_size(new_size);
		for (auto& editor : m_text_editors)
			editor.set_size(new_size);
	}
}
