#pragma once
#include "ICore.h"
#include "BaseController.h"
#include "GUIHandler.h"
#include "OpenTextEditorOption.h"
#include "OpenImageViewerOption.h"
#include "ChangeColours.h"
#include "ChangeTextColours.h"
#include "EchoOption.h"
#include "ClearConsoleOption.h"
#include "SudoOption.h"

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
			controller.add_option("changetextcolours", std::make_unique<ChangeTextColours>(core));
			controller.add_option("e", std::make_unique<EchoOption>(core));
			controller.add_option("clear", std::make_unique<ClearConsoleOption>(core));
			controller.add_option("sudo", std::make_unique<SudoOption>(core));
			return controller;
		}
	};
}