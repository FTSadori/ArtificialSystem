#pragma once
#include "TerminalWindow.h"
#include "TextEditorWindow.h"
#include "ConsoleInputHandler.h"
#include "ICommandExecutor.h"
#include "UsersHandler.h"
#include "Command.h"
#include "Base64.h"

namespace GUI
{
	class GUIHandler final
	{
	public:
		GUIHandler(Commands::ICommandExecutor& _core, Commands::UsersHandler& _users_handler,
			Colours _background, Colours _window, Colours _border);

		void open_editor(const Memory::FullPath& path, const std::string& data);

	private:
		const size_t c_full_tab_size_divider = 2;
		const size_t c_mini_tab_size_divider = 6;
		const size_t c_min_tab_size = 6;

		const int16_t c_horizontal_margin = 2;
		const int16_t c_vertical_margin = 1;
		const int16_t c_tabs_height = 1;

		Colours m_background;
		Colours m_window;
		Colours m_border;

		Size m_window_size{ 0, 0 };
		std::mutex m_window_size_mutex;

		Commands::ICommandExecutor& m_core;
		Commands::UsersHandler& m_users_handler;

		TerminalWindow m_terminal;
		std::vector<TextEditorWindow> m_text_editors;
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
		void render(size_t window_num);
		void render_header(Size window_size, size_t window_num);
		void render_header_background(Size window_size);
		void render_header_border(Size window_size);
		void render_window(size_t window_num);
		void set_size_to_all(Size new_size);
	};
}
