#include "stdafx.h"

#include "toH265.h"
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

		String^ FormMain::getWinnicewArg(FFMpegPriority ffmpegPriority)
		{
			switch (ffmpegPriority)
			{
			case FFMpegPriority::Normal:		return L"--all-normal";
			case FFMpegPriority::BelowNormal:	return L"--all-belownormal";
			case FFMpegPriority::Background:	return L"--all-idle";
			}
			DASSERT(false);
			return String::Empty;
		}
		void FormMain::OnMenuPriorityCommon(FFMpegPriority ffmpegPriority)
		{
			switch (CurrentFFMpegState)
			{
			case FFMpegState::None:
			case FFMpegState::Intermidiate:
				break;
			case FFMpegState::ProcessLaunching:
				break;
			case FFMpegState::Pausing:
			case FFMpegState::Running:
			{
				DASSERT(pidFFMpeg_ != 0);
				if (pidFFMpeg_ == 0)
				{
					CppUtils::Alert(I18N(L"FFMpeg process not found."));
					break;
				}

				String^ arg = String::Format(L"{0} --pid {1}",
					getWinnicewArg(ffmpegPriority),
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
			case FFMpegState::Unknown:
				break;
			}

			checkFFMpegPriority(ffmpegPriority);

			if (!Profile::WriteString(
				SECTION_OPTION, KEY_FFMPEG_PRIORITY, 
				getFFMpegPriorityAsString(ffmpegPriority),
				Program::IniFile))
			{
				CppUtils::Alert(I18N(STR_FAILED_TO_SAVE_SETTING));
			}
		}
		System::Void FormMain::tsmiPriorityNormal_Click(System::Object^ sender, System::EventArgs^ e)
		{
			OnMenuPriorityCommon(FFMpegPriority::Normal);
		}
		System::Void FormMain::tsmiBelowNormal_Click(System::Object^ sender, System::EventArgs^ e)
		{
			OnMenuPriorityCommon(FFMpegPriority::BelowNormal);
		}
		System::Void FormMain::tsmiPriorityBackground_Click(System::Object^ sender, System::EventArgs^ e)
		{
			OnMenuPriorityCommon(FFMpegPriority::Background);
		}

		void FormMain::checkFFMpegPriority(FFMpegPriority ffmpegPriority)
		{
			tsmiPriorityNormal->Checked = false;
			tsmiPriorityBelowNormal->Checked = false;
			tsmiPriorityBackground->Checked = false;
			switch (ffmpegPriority)
			{
			case FFMpegPriority::Normal:
				tsmiPriorityNormal->Checked = true;
				break;
			case FFMpegPriority::BelowNormal:
				tsmiPriorityBelowNormal->Checked = true;
				break;
			case FFMpegPriority::Background:
				tsmiPriorityBackground->Checked = true;
				break;
			}
		}
		String^ FormMain::getFFMpegPriorityAsString(FFMpegPriority ffmpegPriority)
		{
			switch (ffmpegPriority)
			{
			case FFMpegPriority::Normal: return L"normal";
			case FFMpegPriority::BelowNormal: return L"belownormal";
			case FFMpegPriority::Background: return L"background";
			}
			DASSERT(false);
			return String::Empty;
		}
		FormMain::FFMpegPriority FormMain::getFFMpegPriorityFromString(String^ priority)
		{
			if (String::Compare(priority, L"normal", true) == 0)
			{
				return FFMpegPriority::Normal;
			}
			if (String::Compare(priority, L"belownormal", true) == 0)
			{
				return FFMpegPriority::BelowNormal;
			}
			if (String::Compare(priority, L"background", true) == 0)
			{
				return FFMpegPriority::Background;
			}
			return FFMpegPriority::Normal;
		}
	}
}
