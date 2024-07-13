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
	// changecolours
	class ChangeColours final : public AbstractControllerOption
	{
	public:
		ChangeColours(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Changes background, console and border colours.\n");
				ptr->print_secondary("changecolours {background} {console} {border}\n");
				ptr->print_main("  background - (string) colour for background;\n");
				ptr->print_main("  console - (string) colour for console background;\n");
				ptr->print_main("  border - (string) colour for console border;\n");
				ptr->print_secondary("List of colours: \n");
				ptr->print_main("  black, blue, gray, green, lightblue, lightgray, lightgreen, lightred, lightturquoise, lightviolet, lightyellow, red, turquoise, violet, white, yellow\n");
				return;
			}

			if (!sender.system())
				throw PermissionException("(OpenTextEditorOption) Sender has low permission lvl");

			GUI::SystemColourTheme newColourTheme = GUI::SystemColourTheme(
				GUI::StringToColour::ToColour(_command.get("1")),
				GUI::StringToColour::ToColour(_command.get("2")),
				GUI::StringToColour::ToColour(_command.get("3"))
			);

			m_core.gui().change_colours_all_windows(newColourTheme);

			using namespace Memory;

			auto& maindisk = m_core.memory().get_disk(m_core.memory_info().get_main_disk_info().mark);
			if (!maindisk.is_exists(DiskPath("\\system")))
				maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.colours")))
				maindisk.create(DiskPath("\\system", ".colours"), Permissions(true, 255, 255, 255, 0), FileT::FILE, true);
			
			std::string strdata = _command.get("1") + " " + _command.get("2") + " " + _command.get("3");
			DataQueue data = DataQueue(std::vector<char>(strdata.begin(), strdata.end()));

			maindisk.write(DiskPath("\\system", ".colours"), data, true);

			return;
		}
	};
}