#pragma once
#include <string>
#include "NameExceptions.h"

namespace Memory
{
	struct NameValidator final
	{
		static bool is_name_valid(const std::string& name);
	};
}