#pragma once
#include "BaseWindow.h"
#include "FullPath.h"
#include "Parser.h"

namespace GUI
{
	class TextEditorWindow final : public BaseWindow
	{
	public:
		TextEditorWindow(const Memory::FullPath& _path, Size _size, ScreenPoint _position, const std::string& _title = "Window", Colours _background = Colours::BLACK);

		virtual void render_text() override;

		virtual void on_printable(SHORT code) override;
		virtual void on_backspace() override;
		virtual void on_enter() override;
		virtual void on_left() override;
		virtual void on_right() override;
		virtual void on_up() override;
		virtual void on_down() override;

		Memory::FullPath get_file_path();

		void set_colours(Colours background, Colours main, Colours secondary);
	private:
		void next_line_num();
		void prev_line_num();

		const uint16_t c_max_num_size = 6;

		size_t m_line_num = 0;
		size_t m_cursor_in_line = 0;

		TextAttributes m_main = 15;
		TextAttributes m_secondary = 6;
		TextAttributes m_selection = 15 * 16;

		Memory::FullPath m_file_path;

		std::vector<std::string> m_lines;
	};
}