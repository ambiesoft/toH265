#include "stdafx.h"

#include "toH265.h"
#include "helper.h"

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

		FormMain::FormMain()
		{
			regFFMpeg_ = gcnew RegularExpressions::Regex("frame=.*fps=.*size=.*time=(\\d\\d:\\d\\d:\\d\\d)\\.\\d\\d.*speed=\\s*(-?\\d+(?:\\.\\d+)?)x");
			/*
			frame=   61 fps= 18 q=-0.0 size=       0kB time=00:00:02.08 bitrate=   0.2kbits/s dup=1 drop=0 speed=0.619x
			frame=   69 fps= 17 q=-0.0 size=       0kB time=00:00:02.34 bitrate=   0.2kbits/s dup=1 drop=0 speed=0.591x
			frame=   76 fps= 17 q=-0.0 size=       0kB time=00:00:02.57 bitrate=   0.1kbits/s dup=1 drop=0 speed=0.566x
			frame=   85 fps= 17 q=-0.0 size=       0kB time=00:00:02.87 bitrate=   0.1kbits/s dup=1 drop=0 speed=0.566x
			*/
			
		/*	String^ text = "frame=  695 fps= 18 q=0.0 size=     363kB time=00:00:23.20 bitrate= 128.0kbits/s speed= 0.6x";
			if (regFFMpeg_->IsMatch(text))
			{
				RegularExpressions::Match^ match = regFFMpeg_->Match(text);
				String^ timeValue = match->Groups[1]->Value;

				DateTime dtTime = DateTime::ParseExact(timeValue, L"hh:mm:ss",
					System::Globalization::CultureInfo::InvariantCulture);
				TimeSpan tsTime = dtTime - dtTime.Date;

				String^ speedValue = match->Groups[2]->Value;
				double dblSpeed;
				double::TryParse(speedValue, dblSpeed);

				UpdateTitleTS(tsTime, dblSpeed);
				txtLogOut->Text = text;
			}*/
		




			InitializeComponent();

			txtLogErr->Font = gcnew System::Drawing::Font(FontFamily::GenericMonospace, txtLogErr->Font->Size + 1);
			txtLogOut->Font = gcnew System::Drawing::Font(FontFamily::GenericMonospace, txtLogOut->Font->Size + 1);

#ifdef _DEBUG
			tsmiHelp->DropDownItems->Add(
				gcnew ToolStripMenuItem(L"Test", nullptr, gcnew EventHandler(this, &FormMain::FormMain_OnTest)));
#endif

			HashIni ^ ini;
			try
			{
				DTRACE(L"INI=" + Program::IniFile);
				ini = Profile::ReadAll(Program::IniFile, true);
			}
			catch (FileNotFoundException^)
			{
				DASSERT(!ini);
				ini = HashIni::CreateEmptyInstanceForSpecialUse();
			}
			catch (Exception ^ ex)
			{
				CppUtils::Alert(this, ex);
				Environment::Exit(-1);
				return;
			}

			DASSERT(ini);
			bool boolval;
			Profile::GetBool(SECTION_OPTION, KEY_PROCESS_BACKGROUND, false, boolval, ini);
			if (boolval)
			{
				tsmiPriorityNormal->Checked = false;
				tsmiPriorityBackground->Checked = true;
			}

			Profile::GetBool(SECTION_OPTION, KEY_MINIMIZETOTRAY, false, boolval, ini);
			tsmiMinimizeToTray->Checked = boolval;


			AmbLib::LoadFormXYWH(this, SECTION_LOCATION, ini);

			try
			{
				String^ imagePath = Path::Combine(Path::GetDirectoryName(Application::ExecutablePath), L"images");
				iconBlue_ = System::Drawing::Icon::FromHandle((gcnew Bitmap(Path::Combine(imagePath, L"icon.png")))->GetHicon());
				iconYellow_ = System::Drawing::Icon::FromHandle((gcnew Bitmap(Path::Combine(imagePath, L"pause.png")))->GetHicon());
				iconRed_ = System::Drawing::Icon::FromHandle((gcnew Bitmap(Path::Combine(imagePath, L"busy.png")))->GetHicon());
			}
			catch (Exception ^ ex)
			{
				StringBuilder sbMessage;
				sbMessage.AppendLine(I18N(L"Failed to load images."));
				sbMessage.AppendLine(ex->Message);
				CppUtils::Alert(this, sbMessage.ToString());
			}
		}
	}
}