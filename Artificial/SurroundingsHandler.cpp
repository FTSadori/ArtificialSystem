#include "SurroundingsHandler.h"

namespace Story
{
	const std::unordered_map<std::string, std::string> SurroundingsHandler::s_map = {
		{ "CORRIDOR:", "CORE:\\.scripts\\chapter1\\la_corridor.txt" },
		{ "OVERWORLD:", "CORE:\\.scripts\\chapter1\\la_overworld.txt" },
		{ "OVERWORLD:\\largetree\\darkgorge2", "CORE:\\.scripts\\chapter1\\la_gorge_2.txt" },
		{ "OVERWORLD:\\largetree\\darkgorge3\\door\\blockeddoor\\anotherdoor", "CORE:\\.scripts\\chapter1\\la_packmi.txt" },
	};
}