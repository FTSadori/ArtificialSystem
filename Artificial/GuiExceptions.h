#pragma once
#include "BaseException.h"

namespace GUI
{
	struct GuiException final : public Exception
	{
		GuiException(const std::string& _message) : Exception(_message) {}
		GuiException(const char* _message) : Exception(_message) {}
	};
}