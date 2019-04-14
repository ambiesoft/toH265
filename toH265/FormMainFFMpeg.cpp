#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

		String^ FormMain::getCommon(System::Windows::Forms::IWin32Window^ parent,
			bool bFFMpeg, String^ regApp, String^ regKey, String^ inifile, String^% target, bool bReset)
		{
			if (!String::IsNullOrEmpty(target) && File::Exists(target))
				return target;

			if (!bReset)
			{
				String^ ret;
				Profile::GetString(regApp, regKey, String::Empty, ret, inifile);
				if (!String::IsNullOrEmpty(ret) && File::Exists(ret))
					return target = ret;
			}

			String^ title = I18N(String::Format(L"Choose {0}", (bFFMpeg ? L"ffmpeg" : L"ffprobe")));
			CppUtils::Info(parent, title);
			OpenFileDialog dlg;
			dlg.Title = title;
			dlg.DefaultExt = L"exe";
			System::Text::StringBuilder sbFilter;
			String^ ae = ".exe";
			sbFilter.Append(ae);
			sbFilter.Append("File ");
			sbFilter.Append("(*");
			sbFilter.Append(ae);

			sbFilter.Append(")|*");
			sbFilter.Append(ae);
			sbFilter.Append("|");
			sbFilter.Append("All File(*.*)|*.*");
			dlg.Filter = sbFilter.ToString();
			if (System::Windows::Forms::DialogResult::OK != dlg.ShowDialog(parent))
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
			return getCommon(this, false, SECTION_OPTION, KEY_FFPROBE, IniFile, ffprobe_, false);
		}

		String^ FormMain::FFMpeg::get()
		{
			return getCommon(this, true, SECTION_OPTION, KEY_FFMPEG, IniFile, ffmpeg_, false);
		}
	}

}