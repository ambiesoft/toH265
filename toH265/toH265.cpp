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

		String^ Program::IniFile::get()
		{
			return Path::Combine(Path::GetDirectoryName(Application::ExecutablePath),
				Path::GetFileNameWithoutExtension(Application::ExecutablePath) + L".ini");
		}

		String^ Program::Culture::get()
		{
			if (!cultureInfo_)
				return String::Empty;

			return cultureInfo_->Name;
		}
		void Program::Culture::set(String^ value)
		{
			if (String::IsNullOrEmpty(value))
			{
				cultureInfo_ = nullptr;
				return;
			}

			cultureInfo_ = gcnew System::Globalization::CultureInfo(value);
			System::Threading::Thread::CurrentThread->CurrentCulture = cultureInfo_;
			System::Threading::Thread::CurrentThread->CurrentUICulture = cultureInfo_;
		}

		String^ Program::getString(String^ origText)
		{
			String^ ret = theResource_->GetString(origText, cultureInfo_);

#ifdef _DEBUG
			if (String::IsNullOrEmpty(ret) && !listNotI18Ned_.Contains(origText))
			{
				DTRACE(L"\"" + origText + "\" is not I18Ned.");
				listNotI18Ned_.Add(origText);
			}
#endif
			return String::IsNullOrEmpty(ret) ? origText : ret;
		}
		bool Program::preRun()
		{
			String^ specculture;
			if (Profile::GetString(
				FormMain::SECTION_OPTION,
				FormMain::KEY_CULTURE,
				String::Empty,
				specculture,
				IniFile)
				&& !String::IsNullOrEmpty(specculture))
			{
				try
				{
					Culture = specculture;
				}
				catch (Exception ^ ex)
				{
					CppUtils::Fatal(ex);
				}
			}
			return true;
		}

		void Program::SaveSettings()
		{
			bool success = true;

			success &= Profile::WriteString(
				FormMain::SECTION_OPTION,
				FormMain::KEY_CULTURE,
				Culture,
				IniFile);

			if (!success)
			{
				CppUtils::Alert(I18N(FormMain::STR_FAILED_TO_SAVE_SETTING));
			}
		}
		int Program::main(array<System::String ^> ^args)
		{
			InitHighDPISupport();
			Application::EnableVisualStyles();
			Application::SetCompatibleTextRenderingDefault(false);

			if (!preRun())
				return 1;

			bool shutdown = false;
			for each (String ^ arg in args)
			{
				if (arg == "-shutdown")
					shutdown = true;
				else
					clMovieFiles_.Add(arg);
			}
			if (shutdown && clMovieFiles_.Count != 0)
			{
				CppUtils::Alert(I18N(L"-shutdown must not be with other args."));
				return RETURN_SHUTDOWN_MUST_NOT_WITH_ARGS;
			}
			if (shutdown)
			{
				Ambiesoft::AfterRunLib::FormMain form;
				form.IsShutdown = true;
				form.Interval = 30;
				form.TopMost = true;
				Application::Run(%form);
				return 0;
			}


			// Create the main window and run it
			FormMain formMain;
			Application::Run(%formMain);

			SaveSettings();
#ifdef _DEBUG
			showUnI18Ned();
#endif
			return formMain.ReturnValue;
		}


	}
}