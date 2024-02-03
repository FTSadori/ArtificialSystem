#include "FinalModule.h"

namespace Mova
{
	const std::vector<std::string> FinalModule::comparison_expr = { "==", ">", "<", "<=", ">=", "!=" };

	std::unordered_map<double, size_t> FinalModule::s_constants;
	std::unordered_map<std::string, size_t> FinalModule::s_marks;
	std::unordered_map<std::string, FuncInfo> FinalModule::s_functions;
	size_t FinalModule::s_current_nesting_lvl = 0;
	size_t FinalModule::s_current_var_ptr = 0;
}
