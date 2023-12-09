#include "ConsoleInputHandler.h"
#include <signal.h>

namespace GUI
{
	std::vector<KEY_EVENT_RECORD> ConsoleInputHandler::s_last_events;
	std::mutex ConsoleInputHandler::s_events_mutex;
	std::condition_variable ConsoleInputHandler::s_cv;

	void ConsoleInputHandler::start()
	{
		signal(SIGINT, SIG_IGN);

		std::thread th([&]() {
			HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
			DWORD events_number;
			const int buffer_size = 128;
			INPUT_RECORD input_buffer[buffer_size];

			while (true)
			{
				ReadConsoleInput(
					handle,
					input_buffer,
					buffer_size,
					&events_number);

				for (int i = 0; i < events_number; ++i)
				{
					if (input_buffer[i].EventType == KEY_EVENT)
					{
						std::lock_guard lock(s_events_mutex);
						s_last_events.push_back(input_buffer[i].Event.KeyEvent);
						s_cv.notify_one();
					}
				}
			}
		});

		th.detach();
	}

	KEY_EVENT_RECORD ConsoleInputHandler::get_last_key_event()
	{
		std::lock_guard lock(s_events_mutex);
		KEY_EVENT_RECORD last_event = s_last_events.back();
		s_last_events.pop_back();
		return last_event;
	}

	void ConsoleInputHandler::wait_for_events()
	{
		std::unique_lock lock(s_events_mutex);
		s_cv.wait(lock, [&] { return !s_last_events.empty(); });
	}
}
