#include "SurroundingsHandler.h"

namespace Story
{
	const std::unordered_map<std::string, std::string> SurroundingsHandler::s_map = {
		{ "CORRIDOR:", "CORE:\\.scripts\\chapter1\\la_corridor.txt" },
		{ "OVERWORLD:", "CORE:\\.scripts\\chapter1\\la_overworld.txt" },
		{ "OVERWORLD:\\largetree\\darkgorge2", "CORE:\\.scripts\\chapter1\\la_gorge_2.txt" },
		{ "OVERWORLD:\\largetree\\darkgorge3\\door\\blockeddoor\\anotherdoor", "CORE:\\.scripts\\chapter1\\la_packmi.txt" },
		{ "GLASSTOWN:\\bubblemountain", "CORE:\\.scripts\\chapter2\\la_bubblem.txt" },
		{ "GLASSTOWN:\\darkpassage", "CORE:\\.scripts\\chapter2\\la_darkpassage.txt" },
		{ "GLASSTOWN:\\roots\\deeper\\darkroots", "CORE:\\.scripts\\chapter2\\la_darkroots.txt" },
		{ "GLASSTOWN:\\roots\\deeper", "CORE:\\.scripts\\chapter2\\la_deeper.txt" },
		{ "GLASSTOWN:", "CORE:\\.scripts\\chapter2\\la_glasstown.txt" },
		{ "GLASSTOWN:\\town\\house", "CORE:\\.scripts\\chapter2\\la_house.txt" },
		{ "GLASSTOWN:\\town\\lake", "CORE:\\.scripts\\chapter2\\la_lake.txt" },
		{ "GLASSTOWN:\\town\\largebuilding", "CORE:\\.scripts\\chapter2\\la_largebuilding.txt" },
		{ "GLASSTOWN:\\town\\museum", "CORE:\\.scripts\\chapter2\\la_museum.txt" },
		{ "GLASSTOWN:\\town\\smallshop", "CORE:\\.scripts\\chapter2\\la_smallshop.txt" },
		{ "GLASSTOWN:\\roots", "CORE:\\.scripts\\chapter2\\la_roots.txt" },
		{ "GLASSTOWN:\\roots\\deeper\\sevenseeds", "CORE:\\.scripts\\chapter2\\la_seeds.txt" },
		{ "GLASSTOWN:\\town", "CORE:\\.scripts\\chapter2\\la_town.txt" },
		{ "BLACK:", "CORE:\\.scripts\\chapter2\\la_sblack1.txt" },
		{ "BLACK:\\pileofdust\\doorinfloor", "CORE:\\.scripts\\chapter2\\la_sblack2.txt" },
		{ "BLACK:\\pileofdust\\doorinfloor\\largehead4", "CORE:\\.scripts\\chapter2\\la_sblack3.txt" },
		{ "BLUE:", "CORE:\\.scripts\\chapter2\\la_sblue.txt" },
		{ "GREEN:", "CORE:\\.scripts\\chapter2\\la_sgreen.txt" },
		{ "RED:", "CORE:\\.scripts\\chapter2\\la_sred.txt" },
		{ "VIOLET:", "CORE:\\.scripts\\chapter2\\la_sviolet.txt" },
		{ "WHITE:", "CORE:\\.scripts\\chapter2\\la_swhite.txt" },
		{ "YELLOW:", "CORE:\\.scripts\\chapter2\\la_syellow.txt" },
	};
}