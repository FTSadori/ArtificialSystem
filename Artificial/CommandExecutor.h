#pragma once
#include "ICommandExecutor.h"
#include "BaseController.h"
#include "CommandExceptions.h"
#include <memory>

namespace Commands
{
	class CommandExecutor : public ICommandExecutor
	{
	public:
		virtual void add_controller(const BaseController& controller)
		{
			m_controllers.emplace_back(controller);
		}

		virtual std::string execute(const ICommand& command, const User& sender) override
		{
			std::lock_guard lock(m_execution_mutex);
			for (auto& controller : m_controllers)
			{
				if (controller.has_option(command.get("name")))
					return controller.execute(command, sender);
			}
			throw CommandException("(CommandExecutor) Unknown command '" + command.get("name") + "'");
		}

	protected:
		std::vector<BaseController> m_controllers;
		std::mutex m_execution_mutex;
	};
}