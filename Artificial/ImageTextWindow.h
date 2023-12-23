#pragma once
#include "BaseWindow.h"
#include "Separator.h"

namespace GUI
{
	class ImageTextWindow final : public BaseWindow
	{
	public:
		ImageTextWindow(Size _size, ScreenPoint _position, const std::string& _title = "Window");
		
		virtual void render_text() override;
		void set_text(const std::string image);

		virtual void on_printable(SHORT code) override {}
		virtual void on_backspace() override {}
		virtual void on_enter() override {}
		virtual void on_left() override {}
		virtual void on_right() override {}
		virtual void on_up() override;
		virtual void on_down() override;
	};
}