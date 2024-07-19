#pragma once
#include <string>
#include <vector>
#include <array>
#include "Parser.h"
#include "Separator.h"

namespace Mova
{
	class MovaVersionHandler 
	{
	public:
		static void add_package(const std::string& name);
		static std::string get_line();
		static bool is_good(const std::string& entered_version);

	private:
		static std::array<int, 6> s_version;
	};
}