#pragma once
#include <string>
#include "NameExceptions.h"

namespace Core::Memory
{
	struct NameValidator final
	{
		static bool is_name_valid(const std::string& name);
	};
}