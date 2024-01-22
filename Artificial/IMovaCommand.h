#pragma once
#include "IProcessor.h"
#include "string"

namespace Mova
{
	class IMovaCommand
	{
	public:
		virtual void execute(const std::string& command, IProcessor& processor) = 0;
		virtual ~IMovaCommand() = default;
	};
}