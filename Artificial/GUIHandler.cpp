#include "GUIHandler.h"

namespace GUI
{
	GUIHandler::GUIHandler(Commands::ICommandExecutor& _core, Commands::UsersHandler& _users_handler, Colours _background, Colours _window, Colours _border)
		: m_core(_core), m_users_handler(_users_handler),
		m_background(_background), m_window(_window), m_border(_border)
	{
		m_windows.emplace_back(new TerminalWindow({ 0, 0 }, get_windows_start(), "Terminal"));
		m_windows[0]->set_window_colours(m_window, m_border);

		start_resize_thread();
		start_input_thread();
	}

	void GUIHandler::open_editor(const Memory::FullPath& path, const std::string& data, bool readonly)
	{
		auto* ptr = new TextEditorWindow(path, get_windows_size(m_window_size), get_windows_start(), path.disk_path().file(), data);
		ptr->set_readonly(readonly);
		m_windows.emplace_back(ptr);
		m_windows.back()->set_window_colours(m_window, m_border);
	}

	void GUIHandler::open_image(const std::string& name, const std::string& data)
	{
		m_windows.emplace_back(new ImageTextWindow(get_windows_size(m_window_size), get_windows_start(), name));
		m_windows.back()->set_window_colours(m_window, m_border);
		ImageTextWindow* ptr = dynamic_cast<ImageTextWindow*>(m_windows.back().get());
		ptr->set_text(data);
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
		render(m_current_window);
		return true;
	}

	void GUIHandler::doCtrlS()
	{
		TextEditorWindow* te_ptr = dynamic_cast<TextEditorWindow*>(m_windows[m_current_window].get());
		if (te_ptr)
		{
			const auto& title = te_ptr->get_title();

			if (title.ends_with("*"))
				te_ptr->set_title(title.substr(0, title.size() - 1));

			std::string text = b64encode(te_ptr->get_text());
			auto path = te_ptr->get_file_path();

			Commands::Command cmd(te_ptr->get_file_path().full_dir_name()
				+ " write "
				+ te_ptr->get_file_path().disk_path().file()
				+ " "
				+ text);

			m_core.execute(cmd, m_users_handler.get_current_user());
		}
	}

	void GUIHandler::doCtrlZ()
	{
		m_current_window = max(1, m_current_window) - 1;
	}

	void GUIHandler::doCtrlX()
	{
		m_current_window = min(m_current_window + 1, m_windows.size() - 1);
	}

	void GUIHandler::doCtrlQ()
	{
		if (m_current_window > 0)
		{
			m_windows.erase(m_windows.begin() + m_current_window);
			m_current_window = min(m_current_window, m_windows.size() - 1);
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
					m_windows[m_current_window]->key_pressed(key);

					TextEditorWindow* te_ptr = dynamic_cast<TextEditorWindow*>(m_windows[m_current_window].get());
					
					if (te_ptr && !te_ptr->get_readonly())
					{
						if ((key.wVirtualKeyCode < 37 || key.wVirtualKeyCode > 40) && !te_ptr->get_title().ends_with("*"))
						{
							te_ptr->set_title(te_ptr->get_title() + "*");
							render(m_current_window);
						}
					}
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
		Size size;
		{
			std::lock_guard lock(m_window_size_mutex);
			size = m_window_size;
		}

		render_window(window_num);
		render_header_border(size); 
		render_header_background(size);
		render_header(size, window_num);
	}

	void GUIHandler::render_header(Size window_size, size_t window_num)
	{
		const size_t double_margin = 2 * c_horizontal_margin;

		size_t window_width = max(double_margin, window_size.columns) - double_margin;

		size_t full_tab_size = max(window_width / c_full_tab_size_divider, c_min_tab_size);
		size_t mini_tab_size = max(window_width / c_mini_tab_size_divider, c_min_tab_size);

		size_t sum = 0;

		std::vector<std::string> titles;
		for (const auto& win : m_windows)
			titles.push_back(win->get_title());

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

			if (sum + titles[i].size() + 2 >= window_width)
			{
				if (i <= window_num)
				{
					m_tabs_from++;
					render_header_background(window_size);
					render_header(window_size, window_num);
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

	void GUIHandler::render_header_background(Size window_size)
	{
		const size_t double_margin = 2 * c_horizontal_margin;
		size_t window_width = max(double_margin, window_size.columns) - double_margin;

		ConsoleWindow::set_cursor_position({ c_vertical_margin, c_horizontal_margin });
		ConsoleWindow::set_text_colours(TextColours(m_background, m_background));
		std::cout << std::string(window_width, ' ');
	}

	void GUIHandler::render_header_border(Size window_size)
	{
		ConsoleWindow::set_text_colours(TextColours(m_border, m_background));

		ConsoleWindow::set_cursor_position({ SHORT(c_vertical_margin - 1), c_horizontal_margin });
		ConsoleWindow::set_text_borders(TextBorders(false, false, false, true));
		std::cout << std::string(window_size.columns - c_horizontal_margin * 2, ' ');

		ConsoleWindow::set_cursor_position({ c_vertical_margin, SHORT(c_horizontal_margin - 1) });
		ConsoleWindow::set_text_borders(TextBorders(false, false, true, false));
		std::cout << ' ';

		ConsoleWindow::set_cursor_position({ c_vertical_margin, SHORT(window_size.columns - c_horizontal_margin) });
		ConsoleWindow::set_text_borders(TextBorders(true, false, false, false));
		std::cout << ' ';
	}

	void GUIHandler::render_window(size_t window_num)
	{
		BaseWindow* window = m_windows[window_num].get();

		window->render_background();
		window->render_border(m_background);
		window->render_text();
	}
	
	void GUIHandler::set_size_to_all(Size new_size)
	{
		if (new_size.columns > m_window_size.columns)
			new_size.columns = 0;
		if (new_size.rows > m_window_size.rows)
			new_size.rows = 0;
		
		for (const auto& window : m_windows)
			window->set_size(new_size);
	}
}
