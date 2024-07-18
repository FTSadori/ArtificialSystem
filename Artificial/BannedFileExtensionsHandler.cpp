#include "BannedFileExtensionsHandler.h"

namespace Story {
	std::unordered_map<std::string, uint8_t> BannedFileExtensionsHandler::s_banned = { {".girl", 255}, {".item", 200}, {".pack", 100}};
}