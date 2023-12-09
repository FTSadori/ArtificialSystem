#pragma once
#include "BaseWindow.h"
#include "FullPath.h"
#include "ScreenText.h"

namespace GUI
{
	class TerminalWindow final : public BaseWindow
	{
	public:
		TerminalWindow(const Memory::FullPath& _path, const std::string& _user, Size _size, ScreenPoint _position, const std::string& _title = "Window", Colours _background = Colours::BLACK);

		virtual void key_pressed(KEY_EVENT_RECORD key_event) override;
		virtual void render_text_end() const override;
		virtual void render_text_line() const override;
	
		void set_colours(Colours background, Colours main, Colours secondary, Colours third);
	private:
		ScreenText load_screen_text() const;
		void start_new_command();

		bool m_canprint = false;
		std::vector<std::string> m_buffer;
		Memory::FullPath m_path;
		std::string m_user;
		size_t m_cursor_in_buffer = 0;

		TextAttributes m_main = 15;
		TextAttributes m_secondary = 6;
		TextAttributes m_third = 14;
		TextAttributes m_selection = 15 * 16;
	};
}
