#pragma once
#include "AbstractControllerOption.h"
#include <string>
#include <memory>

namespace Commands
{
	class BaseController
	{
	public:
		virtual bool has_option(const std::string& name)
		{
			return m_options.find(name) != m_options.end();
		}

		virtual void add_option(const std::string& name, std::unique_ptr<AbstractControllerOption>&& option)
		{
			m_options.emplace(name, option.release());
		}

		virtual void execute(const ICommand& command, const User& sender)
		{
			return m_options.at(command.get("name"))->execute(command, sender);
		}

		virtual ~BaseController() = default;

	protected:
		std::unordered_map<std::string, std::shared_ptr<AbstractControllerOption>> m_options;
	};
}