#pragma once
#include "ICommandExecutor.h"
#include "IController.h"
#include "CommandExceptions.h"
#include <memory>

namespace Commands
{
	class CommandExecutor : public ICommandExecutor
	{
	public:
		virtual void add_controller(std::unique_ptr<IController>&& controller)
		{
			m_controllers.emplace_back(controller.release());
		}

		virtual std::string execute(const ICommand& command, const ISender& sender) override
		{
			for (const auto& controller : m_controllers)
			{
				if (controller->has_option(command.get("name")))
					return controller->execute(command, sender);
			}
			throw CommandException("(CommandExecutor) Unknown command");
		}

	protected:
		std::vector<std::unique_ptr<IController>> m_controllers;
	};
}