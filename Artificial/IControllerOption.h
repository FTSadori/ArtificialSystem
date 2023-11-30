#pragma once
#include "ICommand.h"

namespace Commands
{
	class IControllerOption
	{
	public:
		virtual std::string execute(const ICommand& command) = 0;
		virtual ~IControllerOption() = default;
	};
}
