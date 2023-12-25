#pragma once
#include "ICore.h"

namespace Commands
{
	class AbstractControllerOption
	{
	public:
		AbstractControllerOption(ICore& _core)
			: m_core(_core) {}

		virtual void execute(const ICommand& command, const User& sender) = 0;

		virtual ~AbstractControllerOption() = default;

	protected:
		ICore& m_core;
	};
}