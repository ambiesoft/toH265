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
			regFFMpeg_ = gcnew RegularExpressions::Regex(
"frame=.*fps=.*size=.*time=(?<time>.*)\\.\\d\\d.*speed=\\s*(?<speed>.*)"
			);

#ifdef _DEBUG
			List<String^> ls;
			ls.Add("frame=   61 fps= 18 q=-0.0 size=       0kB time=00:00:02.08 bitrate=   0.2kbits/s dup=1 drop=0 speed=0.619x");
			ls.Add("frame=    0 fps=0.0 q=0.0 size=       0kB time=-577014:32:22.77 bitrate=  -0.0kbits/s speed=N/A");
			for each (String ^ text in ls)
			{
				TimeSpan ts;
				double dbSpeed;
				GetInfoFromFFMpegoutput(text, ts, dbSpeed);
			}
#endif // _DEBUG



			InitializeComponent();

			
			lvInputs->Name = L"lvInputs";
			lvInputs->UseCompatibleStateImageBehavior = false;
			lvInputs->View = System::Windows::Forms::View::Details;
			lvInputs->FullRowSelect = true;
			lvInputs->HideSelection = false;
			lvInputs->MultiSelect = false;
			lvInputs->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::ListInputs_DragDrop);
			lvInputs->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::ListInputs_DragEnter);
			lvInputs->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::ListInputs_DragOver);
			lvInputs->AllowDrop = true;
			lvInputs->Columns->Add("directory", I18N("Directory"));
			lvInputs->Columns->Add("filename", I18N("Filename"));
			lvInputs->Columns->Add("aspect", I18N("Aspect"));
			lvInputs->Columns->Add("vcodec", I18N("VideoCodec"));
			lvInputs->Columns->Add("acodec", I18N("AudioCodec"));
			lvInputs->Columns->Add("duration", I18N("Duration"));

			lvInputs->Dock = DockStyle::Fill;
			panelList->Controls->Add(lvInputs);

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
			int intval;
			Profile::GetBool(SECTION_OPTION, KEY_PROCESS_BACKGROUND, false, boolval, ini);
			if (boolval)
			{
				tsmiPriorityNormal->Checked = false;
				tsmiPriorityBackground->Checked = true;
			}

			Profile::GetBool(SECTION_OPTION, KEY_MINIMIZETOTRAY, false, boolval, ini);
			tsmiMinimizeToTray->Checked = boolval;


			AmbLib::LoadFormXYWH(this, SECTION_LOCATION, ini);
			
			AmbLib::LoadListViewColumnWidth(lvInputs, SECTION_COLUMNS, KEY_LISTVIEW_COLUMNS, ini);
			
			if (Profile::GetInt(SECTION_OPTION, KEY_SPLITROOT_DISTANCE, 50, intval, ini))
			{
				splitRoot->SplitterDistance = intval;
			}

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