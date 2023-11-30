#pragma once
#include "IControllerOption.h"
#include <memory>

namespace Commands
{
	class IController
	{
	public:
		virtual void add_option(const std::string& name, std::unique_ptr<IControllerOption> option) = 0;
		virtual std::string execute(const ICommand& command) = 0;
		virtual ~IController() = default;
	};
}