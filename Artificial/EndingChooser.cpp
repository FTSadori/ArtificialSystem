#include "EndingChooser.h"

namespace Story
{
	std::unordered_map<Friendliness, std::unordered_map<Awareness, Endings>> EndingChooser::s_table = {
		{ Friendliness::LOW, {{Awareness::LOW, Endings::Base}, {Awareness::MEDIUM, Endings::Zero}, {Awareness::RZERO, Endings::Zero}, {Awareness::RMEDIUM, Endings::NaN}, {Awareness::RZERO, Endings::NaNKilled}}},
		{ Friendliness::MEDIUM, {{Awareness::LOW, Endings::BaseFriend}, {Awareness::MEDIUM, Endings::Zero}, {Awareness::RZERO, Endings::Zero}, {Awareness::RMEDIUM, Endings::NaNFriend}, {Awareness::RZERO, Endings::NaNKilled}}},
		{ Friendliness::MAX, {{Awareness::LOW, Endings::BaseFriend}, {Awareness::MEDIUM, Endings::Zero}, {Awareness::RZERO, Endings::Zero}, {Awareness::RMEDIUM, Endings::NaNFriend}, {Awareness::RZERO, Endings::NaNKilledPlus}}},
	};
}