#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// ping
	class PingOption final : public AbstractControllerOption
	{
	public:
		PingOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("35 or server permission lvl needed\n");
				ptr->print_main("Checks connection to server by address\n");
				ptr->print_secondary("ping {address} [:preenter password]\n");
				ptr->print_main("  address - (string) address line;\n");
				ptr->print_main("  :preenter password - (flag + string) you can enter password here if command needs it;\n");
				return;
			}

			if (sender.lvl() < 35)
				throw PermissionException("(PingOption) Sender has low permission lvl");

			if (!m_core.net().has(_command.get("1")))
			{
				throw CommandException("(PingOption) No connection with " + _command.get("1"));
			}

			const NetData& data = m_core.net().get(_command.get("1"));

			if (data.lvl > sender.lvl())
			{
				throw PermissionException("(PingOption) You need " + Parser::to_string((int)data.lvl) + " lvl to connect");
			}

			if (_command.has(":preenter"))
				m_core.passwords().check_password(data.password_hash, _command.get(":preenter"));
			else
				m_core.passwords().check_password(ptr, data.password_hash);

			ptr->print_third("Connection with " + _command.get("1") + " is successful" + '\n');

			return;
		}
	};
}