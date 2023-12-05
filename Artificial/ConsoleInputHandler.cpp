#include "ConsoleInputHandler.h"

namespace GUI
{
	void ConsoleInputHandler::start()
	{
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
						std::lock_guard lock(m_events_mutex);
						m_last_events.push_back(input_buffer[i].Event.KeyEvent);
						m_cv.notify_one();
					}
				}
			}
		});

		th.detach();
	}

	KEY_EVENT_RECORD ConsoleInputHandler::get_last_key_event()
	{
		std::lock_guard lock(m_events_mutex);
		KEY_EVENT_RECORD last_event = m_last_events.back();
		m_last_events.pop_back();
		return last_event;
	}

	void ConsoleInputHandler::wait_for_events()
	{
		std::unique_lock lock(m_events_mutex);
		m_cv.wait(lock, [&] { return !m_last_events.empty(); });
	}
}
