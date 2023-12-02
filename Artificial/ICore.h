#pragma once
#include "IController.h"

namespace Commands
{
	class ICore
	{
	public:
		virtual void add_controller(std::unique_ptr<IController> controller) = 0;
		virtual std::string execute(const ICommand& command, const ISender& sender) = 0;
		virtual ~ICore() = default;
	};
}