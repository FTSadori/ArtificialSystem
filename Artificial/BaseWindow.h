#pragma once
#include "ConsoleWindow.h"
#include "ScreenText.h"

namespace GUI
{
	class BaseWindow
	{
	public:
		BaseWindow(Size _size, ScreenPoint _position, const std::string& _title = "Window", Colours _background = Colours::BLACK);

		inline void set_size(Size _size);
		inline void set_title(std::string _title);
		inline void set_position(ScreenPoint _position);

		inline Size get_size() const;
		inline const std::string& get_title() const;
		inline ScreenPoint get_position() const;

		void render_border(TextColours colours);

		virtual ~BaseWindow() = default;

		static Size s_max_size;
		static Size s_min_size;
	protected:
		Colours m_background;
		std::string m_title;
		Size m_size;
		ScreenPoint m_position;
	};
}
