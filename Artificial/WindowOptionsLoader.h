#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "OpenTextEditorOption.h"

namespace Commands
{
	class WindowOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("nano", std::make_unique<OpenTextEditorOption>(core));
			return controller;
		}
	};
}