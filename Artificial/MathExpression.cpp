#include "MathExpression.h"

namespace Mova
{
	const std::unordered_map<std::string, std::string> MathExpression::math_expr = {
	{"=", "SUM"},
	{"+=", "SUM"},
	{"-=", "SUB"},
	{"*=", "MUL"},
	{"/=", "DIVD"},
	{"d=", "DIV"},
	{"%=", "MOD"},
	{"<=", "SHL"},
	{">=", "SHR"},
	};
}