#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;
		using namespace System::Text;
		String^ FormMain::getCommon(
			System::Windows::Forms::IWin32Window^ parent,
			bool bFFMpeg,
			String^ regApp,
			String^ regKey,
			String^ inifile,
			String^% target,
			bool bReset)
		{
			return getCommon(
				parent,
				bFFMpeg,
				regApp,
				regKey,
				inifile,
				target,
				bReset,
				false);
		}

		String^ FormMain::getCommon(
			System::Windows::Forms::IWin32Window^ parent,
			bool bFFMpeg,
			String^ regApp,
			String^ regKey,
			String^ inifile,
			String^% target,
			bool bReset,
			bool bPeek)
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

			if (bPeek)
				return target;

			String^ title = String::Format(I18N(L"Please Choose {0}."), (bFFMpeg ? L"ffmpeg" : L"ffprobe"));
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

			// check if file is ffprobe or ffmpeg
			int retval;
			String^ output;
			String^ err;
			try
			{
				String^ arg = L"-version";
				AmbLib::OpenCommandGetResult(dlg.FileName,
					arg,
					System::Text::Encoding::UTF8,
					retval,
					output,
					err);
				if (retval != 0)
				{
					StringBuilder sbMessage;
					sbMessage.AppendFormat(I18N(L"Command '{0} {1}' exited with {2}."),
						dlg.FileName,
						arg,
						retval);
					CppUtils::Alert(this, sbMessage.ToString());
					return nullptr;
				}
			}
			catch (Exception ^ ex)
			{
				CppUtils::Alert(this, ex);
				return nullptr;
			}

			if (String::IsNullOrEmpty(output))
			{
				CppUtils::Alert(I18N(L"No outputs found."));
				return nullptr;
			}

			String^ findInOutput = bFFMpeg ? "ffmpeg" : "ffprobe";
			bool bCorrectOutput = (output->ToLower()->IndexOf(findInOutput) == 0);
			if (!bCorrectOutput)
			{
				StringBuilder sbMessage;
				sbMessage.AppendFormat(I18N(L"This file does not look like '{0}'."), findInOutput);
				CppUtils::Alert(this, sbMessage.ToString());
				return nullptr;
			}

			target = dlg.FileName;
			if (!Profile::WriteString(regApp, regKey, target, inifile))
			{
				CppUtils::Alert(I18N(STR_FAILED_TO_SAVE_SETTING));
			}
			return target;
		}

		String^ FormMain::FFProbe::get()
		{
			return getCommon(this, false, SECTION_OPTION, KEY_FFPROBE, Program::IniFile, ffprobe_, false);
		}
		String^ FormMain::FFMpeg::get()
		{
			return getCommon(this, true, SECTION_OPTION, KEY_FFMPEG, Program::IniFile, ffmpeg_, false);
		}

		String^ FormMain::PeekFFProbe()
		{
			return getCommon(this, false, SECTION_OPTION, KEY_FFPROBE, Program::IniFile, ffprobe_, false, true);
		}
		String^ FormMain::PeekFFMpeg()
		{
			return getCommon(this, true, SECTION_OPTION, KEY_FFMPEG, Program::IniFile, ffmpeg_, false, true);
		}
	}

}