#include "DynamicEventsHandler.h"

namespace Story
{
	BaseEventHandler DynamicEventsHandler::s_dialogHandler = BaseEventHandler();
	BaseEventHandler DynamicEventsHandler::s_pathHandler = BaseEventHandler();
}