#pragma once
#include "BaseWindow.h"
#include "FullPath.h"
#include "Parser.h"
#include "Separator.h"

namespace GUI
{
	class TextEditorWindow final : public BaseWindow
	{
	public:
		TextEditorWindow(const Memory::FullPath& _path, Size _size, ScreenPoint _position, const std::string& _title = "Window", const std::string& _text = "");
		
		virtual void render_text() override;

		virtual void on_printable(SHORT code) override;
		virtual void on_backspace() override;
		virtual void on_enter() override;
		virtual void on_left() override;
		virtual void on_right() override;
		virtual void on_up() override;
		virtual void on_down() override;

		Memory::FullPath get_file_path();
		std::string get_text();

		void set_readonly(bool _readonly);
		bool get_readonly();

	private:
		void next_line_num();
		void prev_line_num();

		bool m_readonly = false;

		uint16_t c_max_num_size = 6;

		size_t m_line_num = 0;
		size_t m_cursor_in_line = 0;

		TextAttributes m_main = 15;
		TextAttributes m_secondary = 6;
		TextAttributes m_selection = 15 * 16;

		Memory::FullPath m_file_path;

		std::vector<std::string> m_lines;
	};
}