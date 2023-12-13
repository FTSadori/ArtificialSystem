#pragma once
#include "TerminalWindow.h"

namespace GUI
{
	class GUIHandler final
	{
	public:
		GUIHandler(Colours _background, Colours _window, Colours _border);

	private:
		Colours m_background;
		Colours m_window;
		Colours m_border;

		TerminalWindow m_terminal;
		int m_current_window = 0;

		void start_resize_thread();
		void render_current_window();
		void set_size_to_all(Size new_size);
	};
}
