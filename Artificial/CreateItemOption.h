#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "BannedFileExtensionsHandler.h"
#include "Base64.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// mkitem
	class CreateItemOption final : public AbstractControllerOption
	{
	public:
		CreateItemOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n");
				ptr->print_main("Creates item file\n");
				ptr->print_secondary("mkitem {filename} {itemname} {itemdesc} [::sys] [::h] [:p password]\n");
				ptr->print_main("  filename - (string) name of the new file (without .item);\n");
				ptr->print_main("  itemname - (string) name of the new item;\n");
				ptr->print_main("  itemdesc - (string) description of the item;\n");
				ptr->print_main("  ::sys - (flag) create system file (needs 255 lvl);\n");
				ptr->print_main("  ::h - (flag) create hidden file (needs root rights);\n");
				ptr->print_main("  :p \"password\" - (flag + string) sets up password on item (works if dir doesn't have password yet);\n");
				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			Command command("\"" + _command.get("path") + "\" mk \"" + _command.get("1") + ".item\" :wp 255" 
				+ (_command.has("::sys") ? " ::sys" : "")
				+ (_command.has("::h") ? " ::h" : "") 
				+ (_command.has(":p") ? (" :p " + _command.get(":p")) : ""));
			m_core.execute(command, sender);

			Command command2("\"" + _command.get("path") + "\" write \"" + _command.get("1") + ".item\" " + b64encode("[" + _command.get("2") + "]\n\n" + _command.get("3")));
			m_core.execute(command2, sender);

			return;
		}
	};
}