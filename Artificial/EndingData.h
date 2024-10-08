#pragma once

namespace Story
{
	enum class Friendliness
	{
		LOW,
		MEDIUM,
		MAX
	};

	enum class Awareness
	{
		LOW,
		MEDIUM,
		RZERO,
		RMEDIUM,
		RALL,
	};

	struct EndingData
	{
		static int extralvls;
		static Awareness awareness;
		static Friendliness friendliness;
	};
}