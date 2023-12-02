#pragma once
#include "ICommand.h"
#include "ISender.h"

namespace Commands
{
	class IControllerOption
	{
	public:
		virtual std::string execute(const ICommand& command, const ISender& sender) = 0;
		virtual std::string send(const ICommand& command) = 0;
		virtual ~IControllerOption() = default;
	};
}
