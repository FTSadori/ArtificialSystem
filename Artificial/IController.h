#pragma once
#include "AbstractControllerOption.h"
#include <string>
#include <memory>

namespace Commands
{
	class IController
	{
	public:
		virtual bool has_option(const std::string& name) = 0;
		virtual void add_option(const std::string& name, std::unique_ptr<AbstractControllerOption>&& option) = 0;
		virtual std::string execute(const ICommand& command, const User& sender) = 0;
		virtual ~IController() = default;
	};
}