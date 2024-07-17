#pragma once
#include "BaseTask.h"

namespace Story
{
	class IncrementTask final : public BaseTask
	{
	public:
		IncrementTask()
			: BaseTask({{1.0}, {2.0}, {532.0}}, {{2.0}, {3.0}, {533.0}}, "Increment", "---", "BAZA:\\dialogs")
		{}
	};
}