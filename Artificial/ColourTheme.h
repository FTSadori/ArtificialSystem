#pragma once
#include "Colours.h"

namespace GUI
{
	struct SystemColourTheme final
	{
		Colours background;
		Colours window;
		Colours border;

		SystemColourTheme(Colours _background, Colours _window, Colours _border)
			: background(_background), window(_window), border(_border) {}
	};

	struct TextColourTheme final
	{
		Colours main;
		Colours secondary;
		Colours third;

		TextColourTheme(Colours _main, Colours _secondary, Colours _third)
			: main(_main), secondary(_secondary), third(_third) {}
	};
}