#include "ComparingExpression.h"

namespace Mova
{
	const std::unordered_map<std::string, std::string> ComparingExpression::expressions = {
		{ "==", "0" },
		{ "<",  "1" },
		{ ">",  "2" },
		{ "!=", "3" },
		{ ">=", "4" },
		{ "<=", "5" },
	};
}