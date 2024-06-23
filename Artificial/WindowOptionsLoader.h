#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "OpenTextEditorOption.h"
#include "OpenImageViewerOption.h"
#include "ChangeColours.h"

namespace Commands
{
	class WindowOptionsLoader final
	{
	public:
		static BaseController Load(ICore& core)
		{
			BaseController controller;
			controller.add_option("nano", std::make_unique<OpenTextEditorOption>(core));
			controller.add_option("lookat", std::make_unique<OpenImageViewerOption>(core));
			controller.add_option("changecolours", std::make_unique<ChangeColours>(core));
			return controller;
		}
	};
}