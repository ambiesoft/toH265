#include "stdafx.h"

// not IDE support (not copy these files to output folder)
//#using "C:/Linkout/DNAssembly.NET4/Ambiesoft.AmbLib.dll"
//#using "C:/Linkout/DNAssembly.NET4/Ambiesoft.AmbLibcpp.dll"

#include "../../lsMisc/HighDPI.h"
#include "FormMain.h"
#include "toH265.h"

using namespace Ambiesoft;

namespace Ambiesoft {
	namespace toH265 {

		using namespace System;
		using namespace System::IO;

	
	
		int Program::main(array<System::String ^> ^args)
		{
			InitHighDPISupport();

			//if (args->Length == 0)
			//{
			//	CppUtils::Alert(I18N(L"No input files"));
			//	return RETURN_NOINPUTFILE;
			//}

			if (args->Length > 1)
			{
				CppUtils::Alert(I18N(L"Too many input files"));
				return RETURN_TOOMANYINPUTFILES;
			}

			if (args->Length > 0)
			{
				movieFile_ = args[0];
			}




			// Enabling Windows XP visual effects before any controls are created
			Application::EnableVisualStyles();
			Application::SetCompatibleTextRenderingDefault(false);

			// Create the main window and run it
			FormMain formMain;
			Application::Run(%formMain);
			return formMain.ReturnValue;
		}


	}
}