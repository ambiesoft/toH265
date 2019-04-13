#include "stdafx.h"

#include "FormMain.h"
#include "toH265.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System;


		int Program::main(array<System::String ^> ^args)
		{
			if (args->Count == 0)
			{

			}
			// Enabling Windows XP visual effects before any controls are created
			Application::EnableVisualStyles();
			Application::SetCompatibleTextRenderingDefault(false);

			// Create the main window and run it
			Application::Run(gcnew FormMain());
			return 0;
		}


	}
}