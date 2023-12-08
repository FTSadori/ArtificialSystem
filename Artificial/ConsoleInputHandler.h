#pragma once
#include <Windows.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace GUI
{
	class ConsoleInputHandler final
	{
	public:
		static void start();
		static KEY_EVENT_RECORD get_last_key_event();
		static void wait_for_events();
	
	private:
		static std::vector<KEY_EVENT_RECORD> s_last_events;
		static std::mutex s_events_mutex;
		static std::condition_variable s_cv;
	};
}