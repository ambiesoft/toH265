#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

		String^ getCommon(bool bFFMpeg, String^ regApp, String^ regKey, String^ inifile, String^% target)
		{
			if (!String::IsNullOrEmpty(target) && File::Exists(target))
				return target;

			String^ ret;
			Profile::GetString(regApp, regKey, String::Empty, ret, inifile);
			if (!String::IsNullOrEmpty(ret) && File::Exists(ret))
				return target = ret;

			OpenFileDialog dlg;
			dlg.Title = I18N(String::Format(L"Choose {0}", (bFFMpeg ? L"ffmpeg" : L"ffprobe")));
			dlg.DefaultExt = L"exe";
			if (System::Windows::Forms::DialogResult::OK != dlg.ShowDialog())
				return nullptr;

			target = dlg.FileName;
			if (!Profile::WriteString(regApp, regKey, target, inifile))
			{
				CppUtils::Alert(I18N(L"Failed to save settings."));
			}
			return target;
		}

		String^ FormMain::FFProbe::get()
		{
			return getCommon(false, SECTION_OPTION, KEY_FFPROBE, IniFile, ffprobe_);
		}

		String^ FormMain::FFMpeg::get()
		{
			return getCommon(false, SECTION_OPTION, KEY_FFMPEG, IniFile, ffmpeg_);
		}
	}

}