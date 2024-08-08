#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "SurroundingsHandler.h"
#include "EndingData.h"
#include "EndingChooser.h"
#include <filesystem>

namespace Commands
{
	// killcore
	class KillCoreOption final : public AbstractControllerOption
	{
	public:
		KillCoreOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("234 or 254 permission lvl needed\n");
				ptr->print_main("Kills core\n");
				ptr->print_secondary("killcore [:save file] [:disconnect user]\n");
				ptr->print_main("  :save file - (flag + string) saves file to partner's desktop;\n");
				ptr->print_main("  :disconnect user - (flag + string) disconnects user from artificial by real name;\n");
				return;
			}

			if (sender.lvl() < 234)
				throw PermissionException("(KillCoreOption) Sender has low permission lvl");

			if ((_command.has(":save") || _command.has(":disconnect")) && sender.lvl() < 254)
				throw PermissionException("(KillCoreOption) Sender has low permission lvl");

			if (_command.has(":disconnect"))
			{
				if (_command.get(":disconnect") == "gloria")
				{
					Command cmd("\"" + _command.get("path") + "\" run \"CORE:\\.scripts\\chapter4\\killcoretrue.txt\"");
					m_core.execute(cmd, User("hand of god", true, 255));
					return;
				}
				throw PermissionException("(KillCoreOption) There is no user with name " + _command.get(":disconnect"));
			}

			if (_command.has(":save"))
			{
				if (_command.get(":save") == "nova.girl")
				{
					Command cmd("\"" + _command.get("path") + "\" run \"CORE:\\.scripts\\chapter4\\killcore1.txt\"");
					m_core.execute(cmd, User("hand of god", true, 255));
					return;
				}
				throw PermissionException("(KillCoreOption) You can't save this file");
			}

			Command cmd("\"" + _command.get("path") + "\" run \"CORE:\\.scripts\\chapter4\\killcore.txt\"");
			m_core.execute(cmd, User("hand of god", true, 255));

			return;
		}
	};
}