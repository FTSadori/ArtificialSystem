#pragma once
#include "TerminalWindow.h"
#include "TextEditorWindow.h"
#include "ConsoleInputHandler.h"
#include "ImageTextWindow.h"
#include "ICommandExecutor.h"
#include "UsersHandler.h"
#include "ColourTheme.h"
#include "Command.h"
#include "Base64.h"
#include <memory>

namespace GUI
{
	class GUIHandler final
	{
	public:
		GUIHandler() = default;
		GUIHandler(Commands::ICommandExecutor* _core, Commands::UsersHandler* _users_handler,
			SystemColourTheme theme);

		void open_editor(const Memory::FullPath& path, const std::string& data, TextColourTheme theme, bool readonly);
		void open_image(const std::string& name, const std::string& data, TextColourTheme theme);
		void change_colours(SystemColourTheme theme);

		void connect_to_core(Commands::ICommandExecutor* _core);
		void connect_to_users(Commands::UsersHandler* _users);

		TerminalWindow* get_terminal_ptr();

	private:
		const size_t c_full_tab_size_divider = 2;
		const size_t c_mini_tab_size_divider = 6;
		const size_t c_min_tab_size = 6;

		const int16_t c_horizontal_margin = 2;
		const int16_t c_vertical_margin = 1;
		const int16_t c_tabs_height = 1;

		SystemColourTheme m_theme;
		
		Size m_window_size{ 0, 0 };
		std::mutex m_window_size_mutex;

		Commands::ICommandExecutor* m_core = nullptr;
		Commands::UsersHandler* m_users_handler = nullptr;

		std::vector<std::unique_ptr<BaseWindow>> m_windows;
		size_t m_current_window = 0;

		size_t m_tabs_from = 0;
		size_t m_tabs_to = 0;

		ScreenPoint get_windows_start();
		Size get_windows_size(Size screen_size);

		bool try_key_combination(KEY_EVENT_RECORD key);

		void doCtrlS();
		void doCtrlZ();
		void doCtrlX();
		void doCtrlQ();

		void start_input_thread();
		void start_resize_thread();
		void rerender();
		void render(size_t window_num);
		void render_header(Size window_size, size_t window_num);
		void render_header_background(Size window_size);
		void render_header_border(Size window_size);
		void render_window(size_t window_num);
		void set_size_to_all(Size new_size);
	};
}
