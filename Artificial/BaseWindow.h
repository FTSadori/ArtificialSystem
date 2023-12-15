#pragma once
#include "ConsoleWindow.h"
#include "ScreenText.h"
#include "TextInfo.h"

namespace GUI
{
	class BaseWindow
	{
	public:
		BaseWindow(Size _size, ScreenPoint _position, const std::string& _title = "Window", Colours _background = Colours::BLACK);

		void set_size(Size _size);
		void set_title(std::string _title);
		void set_position(ScreenPoint _position);

		Size get_size() const;
		const std::string& get_title() const;
		ScreenPoint get_position() const;

		virtual void render_border(TextColours colours) const;
		virtual void render_text_end() const;
		virtual void render_text_line() const;
		virtual void render_background() const;
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
		Colours m_background;
		std::string m_title;
		Size m_size;
		ScreenPoint m_position;

		std::vector<TextInfo> m_text_parts;
		size_t m_line_num = 0;
	};
}
