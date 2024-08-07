#pragma once
#include "EndingData.h"
#include <string>
#include <unordered_map>

namespace Story
{
	enum class Endings
	{
		Base,
		BaseFriend,
		Zero,
		OneOrTrue,
		NaN,
		NaNFriend,
		NaNKilled,
		NaNKilledPlus,
	};

	class EndingChooser
	{
	public:
		static Endings calculate_ending()
		{
			return s_table[EndingData::friendliness][EndingData::awareness];
		}

	private:
		static std::unordered_map<Friendliness, std::unordered_map<Awareness, Endings>> s_table;
	};
}