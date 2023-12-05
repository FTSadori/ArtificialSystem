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
		void start();
		KEY_EVENT_RECORD get_last_key_event();
		void wait_for_events();
	
	private:
		std::vector<KEY_EVENT_RECORD> m_last_events;
		std::mutex m_events_mutex;
		std::condition_variable m_cv;
	};
}