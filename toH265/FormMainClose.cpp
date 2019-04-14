#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

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
				if (System::Windows::Forms::DialogResult::Yes !=
					CppUtils::YesOrNo(this, I18N(L"Encoding is in progress. Are you sure to quit?"),
					MessageBoxDefaultButton::Button2))
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
	}
}