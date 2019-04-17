#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

		bool FormMain::ConfirmEncodeStop()
		{
			if (System::Windows::Forms::DialogResult::Yes !=
				CppUtils::YesOrNo(this, I18N(L"Encoding is in progress. Are you sure to quit?"),
				MessageBoxDefaultButton::Button2))
			{
				return false;
			}
			return true;
		}
		System::Void FormMain::FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
		{
			if (e->CloseReason == CloseReason::WindowsShutDown)
			{
				// TODO: anyway ffmpeg process closes
				return;
			}

			switch (FFMpegState)
			{
			case TaskState::None:
				break;
			case TaskState::Pausing:
			case TaskState::ProcessLaunching:
			case TaskState::Running:
				if (!ConfirmEncodeStop())
				{
					e->Cancel = true;
					return;
				}
				break;
			case TaskState::Unknown:
				break;
			}

			StopEncoding();
		}

		System::Void FormMain::FormMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
		{
			DTRACE(L"INI=" + IniFile);
			HashIni^ ini = Profile::ReadAll(IniFile);

			bool success = true;
			success &= AmbLib::SaveFormXYWH(this, SECTION_LOCATION, ini);

			if (success)
			{
				success = Profile::WriteAll(ini, IniFile);
			}
			if (!success)
			{
				CppUtils::Alert(this, I18N(STR_FAILED_TO_SAVE_SETTING));
			}
		}
	}
}