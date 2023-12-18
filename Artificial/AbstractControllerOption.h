#pragma once
#include "ICommand.h"
#include "ISender.h"
#include "DiskSystem.h"
#include "SystemUser.h"
#include "ICommandExecutor.h"

namespace Commands
{
	class AbstractControllerOption
	{
	public:
		AbstractControllerOption(ICommandExecutor& _core)
			: m_core(_core) {}

		virtual std::string execute(const ICommand& command, const ISender& sender) = 0;
		
		virtual std::string send(const ICommand& command)
		{
			return m_core.execute(command, SystemUser());
		}

		virtual ~AbstractControllerOption() = default;

	private:
		ICommandExecutor& m_core;
	};
}