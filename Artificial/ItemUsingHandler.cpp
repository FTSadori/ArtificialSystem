#include "ItemUsingHandler.h"

namespace Story
{
	std::unordered_map<std::string, std::string> ItemUsingHandler::s_event_to_script = {
		{ "[Water flower] OVERWORLD:\\largetree\\darkgorge3\\door\\blockeddoor\\anotherdoor", "CORE:\\.scripts\\chapter1\\u_fl.txt" },
		{ "[Glass key] OVERWORLD:\\largetree\\darkgorge3", "CORE:\\.scripts\\chapter1\\u_key_1.txt" },
		{ "[Glass key] OVERWORLD:\\largetree\\darkgorge3\\door\\blockeddoor", "CORE:\\.scripts\\chapter1\\u_key_2.txt" },
		{ "[Weeping flower] OVERWORLD:\\largetree\\darkgorge3\\door", "CORE:\\.scripts\\chapter1\\u_wipfl_1.txt" },
		{ "[Weeping flower] OVERWORLD:\\largetree\\darkgorge3\\door\\blockeddoor\\anotherdoor", "CORE:\\.scripts\\chapter1\\u_wipfl_2.txt" },
		{ "[Glass fruit] GLASSTOWN:\\town\\lake", "CORE:\\.scripts\\chapter2\\d10_u_glassfruit.txt" },
		{ "[Filled fruit] GLASSTOWN:\\town\\largebuilding", "CORE:\\.scripts\\chapter2\\d11_u_filledfruit.txt" },
		{ "[Test tube] GLASSTOWN:\\roots\\deeper\\darkroots", "CORE:\\.scripts\\chapter2\\d20_u_testtube.txt" },
		{ "[Filled test tube] GLASSTOWN:\\town\\largebuilding", "CORE:\\.scripts\\chapter2\\d21_u_filledtesttube.txt" },
		{ "[Ilo sitelen] BLUE:\\tomosuli", "CORE:\\.scripts\\chapter2\\d30_u_ilositelen.txt" },
		{ "[Glass knife] GLASSTOWN:\\town\\museum", "CORE:\\.scripts\\chapter2\\d55_u_glassknife.txt" },
		{ "[Shiny perl] GLASSTOWN:\\roots\\deeper\\darkroots", "CORE:\\.scripts\\chapter2\\d56_u_perl.txt" },
		{ "[Rare glass fruit] GLASSTOWN:\\town\\smallshop", "CORE:\\.scripts\\chapter2\\d58_u_rarefruit.txt" },
		{ "[Amazing glass fruit] GLASSTOWN:\\town\\house", "CORE:\\.scripts\\chapter2\\d61_u_amazingfruit.txt" },
		{ "[Packmi figurine] GLASSTOWN:\\town\\smallshop", "CORE:\\.scripts\\chapter2\\d62_u_packmifigurine.txt" },
		{ "[Melodic bell] GLASSTOWN:\\town\\house", "CORE:\\.scripts\\chapter2\\d66_u_bell1.txt" },
		{ "[Equations] GLASSTOWN:\\town\\largebuilding", "CORE:\\.scripts\\chapter2\\d69_u_mathtask.txt" },
		{ "[Energetic bell] GLASSTOWN:\\town\\largebuilding", "CORE:\\.scripts\\chapter2\\d72_u_bell3.txt" },
		{ "[Note] GLASSTOWN:\\town\\largebuilding", "CORE:\\.scripts\\chapter2\\d73_u_note.txt" },
		{ "[Button flower] BLUE:\\tomosuli\\tomolili2", "CORE:\\.scripts\\chapter2\\d74_u_buttonflower.txt" },
		{ "[Violet pencil] VIOLET:\\largeweb", "CORE:\\.scripts\\chapter2\\d77_u_purplepensil.txt" },
		{ "[Strong bell] VIOLET:\\largeweb", "CORE:\\.scripts\\chapter2\\d79_u_bell2.txt" },
		{ "[Voice reconding] MIKITOWN:\\bloomingarch", "CORE:\\.scripts\\chapter3\\d4_u_recording.txt" },
	};

	std::string ItemUsingHandler::get_script_for_event(const Memory::FullPath& position, const std::string& item_name)
	{
		std::string full = "[" + item_name + "] " + position.full_path_str();
		if (s_event_to_script.contains(full))
			return s_event_to_script[full];
		else
			return "";
	}
}