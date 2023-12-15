#pragma once
#include "BaseWindow.h"
#include "FullPath.h"
#include "ScreenText.h"

namespace GUI
{
	enum class TerminalInputType
	{
		COMMAND,
		TEXT,
		PASSWORD,
	};

	class TerminalWindow final : public BaseWindow
	{
	public:
		TerminalWindow(const Memory::FullPath& _path, const std::string& _user, Size _size, ScreenPoint _position, const std::string& _title = "Window", Colours _background = Colours::BLACK);
		TerminalWindow(Size _size, ScreenPoint _position, const std::string& _title = "Window", Colours _background = Colours::BLACK);

		void print_main(const std::string& line);
		void print_secondary(const std::string& line);
		void print_third(const std::string& line);
		void print(const std::string& line, TextAttributes attributes);

		void wait_for_input(TerminalInputType type);
		std::string get_last_input();
		virtual void key_pressed(KEY_EVENT_RECORD key_event) override;
		virtual void render_text_end() const override;
		virtual void render_text_line() const override;

		void set_path(const Memory::FullPath& _path);
		void set_user_name(const std::string& _name);

		Memory::FullPath get_path();
		std::string get_user_name();

		void set_colours(Colours background, Colours main, Colours secondary, Colours third);
	private:
		ScreenText load_screen_text() const;
		void start_new_command();

		std::string m_last_input;
		bool m_entered = false;

		std::string m_path_line;
		std::string m_user;

		TerminalInputType m_current_input_type = TerminalInputType::COMMAND;
		bool m_caninput = false;

		std::vector<std::string> m_buffer;
		size_t m_cursor_in_input = 0;
		size_t m_command_line = 0;

		TextAttributes m_main = 15;
		TextAttributes m_secondary = 6;
		TextAttributes m_third = 14;
		TextAttributes m_selection = 15 * 16;

		std::mutex m_text_mutex;
		std::condition_variable m_cv;
	};
}
