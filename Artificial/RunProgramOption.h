#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "HexModule.h"
#include "SymbolsModule.h"
#include "FinalModule.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// mova
	class RunProgramOption final : public AbstractControllerOption
	{
	public:
		RunProgramOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Read permission lvl needed\n");
				ptr->print_main("Runs program using MOVA processor\n");
				ptr->print_secondary("mova {path} [...] [:r register_show_num] [::show]\n");
				ptr->print_main("  path - (string) path to file with code;\n");
				ptr->print_main("  ... - (doubles) start values in registers;\n");
				ptr->print_main("  ::show - (flag) on 4th module shows half-compiled code;\n");
				ptr->print_main("  :r register_show_num - (flag + int) shows no less than entered number of registers\n");
				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.system()).permissions;

			if (disk.get_type(path.disk_path()) != Memory::FileT::FILE)
				throw CommandException("(RunProgramOption) It's not a file");

			if (sender.lvl() < perm.read_perm_lvl)
				throw PermissionException("(RunProgramOption) Sender has low permission lvl");

			m_core.passwords().check_password(ptr, perm.password_hash);

			auto data = disk.read(path.disk_path(), sender.system());

			auto lines = Separator::split(std::string(data.get_data(), data.size()), '\n');

			std::vector<double> args;
			size_t num = 2;
			while (_command.has(Parser::to_string(num)))
			{
				args.push_back(Parser::from_string<double>(_command.get(Parser::to_string(num))));
				num++;
			}

			// currently we have only first module
			Mova::Version ver = lines[0];
			m_core.processor().set_version(ver);
			lines.erase(lines.begin());

			auto num_size = m_core.processor().get_current_num_size();
			switch (ver.module_lvl)
			{
			case 2: lines = Mova::HexModule::compile(lines); break;
			case 3: lines = Mova::SymbolsModule::compile(lines, num_size); break;
			case 4:
				auto l = Mova::FinalModule::compile(lines, ver);
				if (_command.has("::show"))
					for (auto& line : l)
						ptr->print_third(line + "\n");
				lines = Mova::SymbolsModule::compile(l, num_size); 
				break;
			}

			auto& res = m_core.processor().process(lines, args);
			std::string str_res;
			ptr->print_third("Program returned " + Parser::to_string(res[0]) + "\n");
			
			size_t checker = 0;
			if (_command.has(":r"))
			{
				checker = Parser::from_string<size_t>(_command.get(":r"));
			}

			// full register output
			ptr->print_third("Registers [");
			for (size_t i = 0; i < res.size(); ++i)
				if (i < checker || res[i] != 0)
					ptr->print_third(Parser::to_string(res[i]) + ", ");
				else
					break;
			ptr->print_third("0]\n");

			// line starts from [res[0]] symbol
			for (size_t i = res[0]; i < res.size(); ++i)
				if (i < checker || res[i] != 0)
				{
					try
					{
						if (res[i] >= 32 && res[i] <= 255)
							str_res.push_back((char)res[i]);
					}
					catch (...) { }
				}
				else
					break;
			
			ptr->print_secondary("Or string [" + str_res + "]\n");

			return;
		}
	};
}