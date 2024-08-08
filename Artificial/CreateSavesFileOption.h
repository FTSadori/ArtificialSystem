#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include <fstream>
#include <map>
#include <filesystem>

namespace Commands
{
	// createsavesfile
	class CreateSavesFileOption final : public AbstractControllerOption
	{
	public:
		CreateSavesFileOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n");
				ptr->print_main("Creates file on the users real desktop\n");
				ptr->print_secondary("createsavesfile {name} {base64content}\n");
				ptr->print_main("  name - (string) file name;\n");
				ptr->print_main("  base64content - (string) content of the created file (must be encoded in base64);\n");
				return;
			}

			if (!sender.system())
				throw PermissionException("(CreateDesktopFileOption) Sender has low permission lvl");

			if (!std::filesystem::exists("___SAVES___"))
				std::filesystem::create_directory("___SAVES___");

			std::ofstream out("___SAVES___\\" + _command.get("1") + "");
			out << b64decode(_command.get("2"));
			out.close();

			return;
		}
	};
}