#include "GUIHandler.h"

namespace GUI
{
	GUIHandler::GUIHandler()
	{
		init();
	}

	GUIHandler::GUIHandler(Commands::ICommandExecutor* _core, Commands::UsersHandler* _users_handler, SystemColourTheme theme)
		: m_core(_core), m_users_handler(_users_handler), m_systemTheme(theme)
	{
		init();
	}

	void GUIHandler::open_editor(const Memory::FullPath& path, const std::string& data, bool readonly)
	{
		auto* ptr = new TextEditorWindow(path, get_windows_size(m_window_size), get_windows_start(), path.disk_path().file(), data);
		ptr->set_readonly(readonly);
		ptr->set_window_colours(m_systemTheme.window, m_systemTheme.border);
		ptr->set_text_colours(m_textTheme);
		m_windows.emplace_back(ptr);
		m_current_window = m_windows.size() - 1;
		m_windows[0]->m_block = true;

		render(m_current_window);
	}

	void GUIHandler::open_image(const std::string& name, const std::string& data)
	{
		m_windows.emplace_back(new ImageTextWindow(get_windows_size(m_window_size), get_windows_start(), name));
		m_windows.back()->set_window_colours(m_systemTheme.window, m_systemTheme.border);
		ImageTextWindow* ptr = dynamic_cast<ImageTextWindow*>(m_windows.back().get());
		ptr->set_text_colours(m_textTheme);
		ptr->set_text(data);
		m_current_window = m_windows.size() - 1;
		m_windows[0]->m_block = true;

		render(m_current_window);
	}

	void GUIHandler::change_colours(SystemColourTheme theme)
	{
		m_systemTheme = theme;
		rerender();
	}

	void GUIHandler::set_text_colours(TextColourTheme theme)
	{
		m_textTheme = theme;
	}

	void GUIHandler::change_colours_all_windows(SystemColourTheme newColourTheme)
	{
		for (int i = 0; i < m_windows.size(); ++i)
		{
			m_windows[i]->set_window_colours(newColourTheme.window, newColourTheme.border);
			//m_core.gui().set_text_colours()
			if (i != m_current_window) continue;
			m_windows[i]->render_border(newColourTheme.background);
			m_windows[i]->rerender();
		}
		change_colours(newColourTheme);
	}

	void GUIHandler::change_text_colours_all_windows(TextColourTheme newTextTheme)
	{
		for (int i = 0; i < m_windows.size(); ++i)
		{
			m_windows[i]->set_text_colours(newTextTheme);
			if (i != m_current_window) continue;
			m_windows[i]->rerender();
		}
		set_text_colours(newTextTheme);
	}

	void GUIHandler::connect_to_core(Commands::ICommandExecutor* _core)
	{
		m_core = _core;
	}

	void GUIHandler::connect_to_users(Commands::UsersHandler* _users)
	{
		m_users_handler = _users;
	}

	void GUIHandler::rerender()
	{
		{
			std::lock_guard lock(m_window_size_mutex);
			m_window_size = ConsoleWindow::get_console_size();
		}
		{
			std::lock_guard lock(BaseWindow::m_render_mutex);
			ConsoleWindow::fill_screen(m_systemTheme.background);
			set_size_to_all(get_windows_size(m_window_size));
		}
		render(m_current_window);
		ConsoleWindow::set_cursor_visibility(false);
	}

	TerminalWindow* GUIHandler::get_terminal_ptr()
	{
		return dynamic_cast<TerminalWindow*>(m_windows[0].get());
	}
	
	void GUIHandler::init()
	{
		m_windows.emplace_back(new TerminalWindow({ 0, 0 }, get_windows_start(), "Terminal"));
		m_windows[0]->set_window_colours(m_systemTheme.window, m_systemTheme.border);

		start_resize_thread();
		start_input_thread();
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
				+ " write \""
				+ te_ptr->get_file_path().disk_path().file()
				+ "\" "
				+ text);

			if (m_core && m_users_handler)
				m_core->execute(cmd, m_users_handler->get_current_user());
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
						if (key.wVirtualKeyCode != 17 && (key.wVirtualKeyCode < 37 || key.wVirtualKeyCode > 40) && !te_ptr->get_title().ends_with("*"))
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
					rerender();
				}
			}
		});
		th.detach();
	}

	void GUIHandler::render(size_t window_num)
	{
		m_windows[window_num]->m_block = false;

		Size size;
		{
			std::lock_guard lock(m_window_size_mutex);
			size = m_window_size;
		}

		std::lock_guard lock(BaseWindow::m_render_mutex);
		{
			render_window(window_num);
			render_header_border(size);
			render_header_background(size);
			render_header(size, window_num);
		}
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
				ConsoleWindow::set_text_colours(TextColours(m_systemTheme.window, m_systemTheme.border));
				std::cout << " " + titles[i] + " ";
			}
			else
			{
				ConsoleWindow::set_text_colours(TextColours(m_systemTheme.border, m_systemTheme.window));
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
		ConsoleWindow::set_text_colours(TextColours(m_systemTheme.background, m_systemTheme.background));
		std::cout << std::string(window_width, ' ');
	}

	void GUIHandler::render_header_border(Size window_size)
	{
		ConsoleWindow::set_text_colours(TextColours(m_systemTheme.border, m_systemTheme.background));

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
		window->render_border(m_systemTheme.background);
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
