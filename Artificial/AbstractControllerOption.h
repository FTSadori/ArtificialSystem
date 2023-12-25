#pragma once
#include "ICommand.h"
#include "User.h"
#include "DiskSystem.h"
#include "SystemUser.h"
#include "ICommandExecutor.h"
#include "GUIHandler.h"

namespace Commands
{
	class AbstractControllerOption
	{
	public:
		AbstractControllerOption(ICommandExecutor& _core, GUI::GUIHandler& _gui)
			: m_core(_core), m_gui(_gui) {}

		virtual void execute(const ICommand& command, const User& sender) = 0;

		virtual ~AbstractControllerOption() = default;

	protected:
		ICommandExecutor& m_core;
		GUI::GUIHandler& m_gui;
	};
}