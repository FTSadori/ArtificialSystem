#include "SymbolsModule.h"

namespace Mova
{
	const std::unordered_map<std::string, std::string> SymbolsModule::map = {
		{"Z", "0000"}, {"S", "0001"}, {"T", "0010"}, {"J", "0011"},
		{"D", "0100"}, {"SUM", "0101"}, {"SUB", "0110"}, {"MUL", "0111"},
		{"DIV", "1000"}, {"MOD", "1001"}, {"DIVD", "1010"}, {"SHL", "1011"},
		{"SHR", "1100"}, {"IN", "1101"}, {"FUNC", "1110"}, {"END", "1111"},
	};
}