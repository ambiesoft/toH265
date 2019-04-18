#include "stdafx.h"

//#include "toH265.h"
//#include "helper.h"

#include "FormMain.h"

namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Drawing;
		using namespace System::Collections::Generic;
		using namespace System::Diagnostics;
		using namespace System::IO;
		using namespace System::Text;
		using namespace System::Threading;

		using namespace Newtonsoft::Json::Linq;

		void FormMain::OnMenuPriorityCommon(bool bBackground)
		{
			switch (FFMpegState)
			{
			case TaskState::None:
				break;
			case TaskState::ProcessLaunching:
				break;
			case TaskState::Pausing:
			case TaskState::Running:
			{
				DASSERT(pidFFMpeg_ != 0);
				if (pidFFMpeg_ == 0)
				{
					CppUtils::Alert(I18N(L"FFMpeg process not found"));
					break;
				}

				String^ arg = String::Format(L"{0} --pid {1}",
					(bBackground ? L" --all-idle" : L" --all-normal"),
					pidFFMpeg_);

				try
				{
					String^ fileName = Path::Combine(Path::GetDirectoryName(Application::ExecutablePath),
						L"winnicew.exe");
					Process::Start(fileName, arg);
				}
				catch (Exception ^ ex)
				{
					CppUtils::Alert(this, ex);
				}
			}
			break;
			case TaskState::Unknown:
				break;
			}
			if (bBackground)
			{
				tsmiPriorityNormal->Checked = false;
				tsmiPriorityBackground->Checked = true;
			}
			else
			{
				tsmiPriorityNormal->Checked = true;
				tsmiPriorityBackground->Checked = false;
			}

			if (!Profile::WriteBool(SECTION_OPTION, KEY_PROCESS_BACKGROUND, bBackground, IniFile))
			{
				CppUtils::Alert(I18N(STR_FAILED_TO_SAVE_SETTING));
			}
		}
		System::Void FormMain::tsmiPriorityNormal_Click(System::Object^ sender, System::EventArgs^ e)
		{
			OnMenuPriorityCommon(false);
		}
		System::Void FormMain::tsmiPriorityBackground_Click(System::Object^ sender, System::EventArgs^ e)
		{
			OnMenuPriorityCommon(true);
		}

	}
}
