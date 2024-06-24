#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "StringToColour.h"
#include <map>

namespace Commands
{
	// changetextcolours
	class ChangeTextColours final : public AbstractControllerOption
	{
	public:
		ChangeTextColours(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Changes main, secondary and third text colours.\n");
				ptr->print_secondary("changetextcolours {main} {secondary} {third}\n");
				ptr->print_main("  main - (string) colour for background;\n");
				ptr->print_main("  secondary - (string) colour for console background;\n");
				ptr->print_main("  third - (string) colour for console border;\n");
				return;
			}

			if (!sender.system())
				throw PermissionException("(OpenTextEditorOption) Sender has low permission lvl");

			GUI::TextColourTheme newColourTheme = GUI::TextColourTheme(
				GUI::StringToColour::ToColour(_command.get("1")),
				GUI::StringToColour::ToColour(_command.get("2")),
				GUI::StringToColour::ToColour(_command.get("3"))
			);

			m_core.gui().change_text_colours_all_windows(newColourTheme);

			return;
		}
	};
}