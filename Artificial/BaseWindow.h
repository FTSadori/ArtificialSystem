#pragma once
#include "ConsoleWindow.h"
#include "ScreenText.h"
#include "ColourTheme.h"
#include "TextInfo.h"

namespace GUI
{
	class BaseWindow
	{
	public:
		BaseWindow(Size _size, ScreenPoint _position, const std::string& _title = "Window");

		void set_size(Size _size);
		void set_title(std::string _title);
		void set_position(ScreenPoint _position);

		void set_window_colours(Colours _background, Colours _border);
		void set_text_colours(TextColourTheme theme);
		
		Size get_size() const;
		const std::string& get_title() const;
		ScreenPoint get_position() const;

		virtual void render_border(Colours _console) const;
		virtual void render_background() const;
		virtual void render_text() = 0;
	
		virtual void key_pressed(KEY_EVENT_RECORD key_event);
		
		virtual void on_printable(SHORT code) = 0;
		virtual void on_backspace() = 0;
		virtual void on_enter() = 0;
		virtual void on_left() = 0;
		virtual void on_right() = 0;
		virtual void on_up() = 0;
		virtual void on_down() = 0;

		virtual ~BaseWindow() = default;

		static Size s_max_size;
		static Size s_min_size;

	protected:
		Colours m_background = Colours::BLACK;
		Colours m_border = Colours::WHITE;

		TextAttributes m_main = 15;
		TextAttributes m_secondary = 6;
		TextAttributes m_third = 14;
		TextAttributes m_selection = 15 * 16;

		std::string m_title;
		Size m_size;
		ScreenPoint m_position;

		std::vector<TextInfo> m_text_parts;
		size_t m_render_from_line = 0;
	};
}
