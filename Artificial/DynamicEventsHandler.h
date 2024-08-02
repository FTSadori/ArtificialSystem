#pragma once
#include "BaseEventHandler.h"

namespace Story 
{
	class DynamicEventsHandler
	{
	public:
		static BaseEventHandler s_dialogHandler;
		static BaseEventHandler s_pathHandler;
	};
}