#include "stdafx.h"

#include "toH265.h"
#include "TargetCodecDialog.h"

#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

#ifdef _DEBUG
		System::Void FormMain::FormMain_OnTest(System::Object^ sender, System::EventArgs^ e)
		{
			if (true)
			{
				HashIni^ ini = Profile::ReadAll(Program::IniFile);
				DVERIFY(dlgAfterFinish_.LoadValues("AfterFinish", ini));
				this->dlgAfterFinish_.DoNotify();
			}
			if(true)
			{
				String^ text = File::ReadAllText("randomText.txt");
				MessageBoxIcon icon = MessageBoxIcon::Question;
				MessageBoxButtons buttons = MessageBoxButtons::AbortRetryIgnore;
				MessageBox::Show(this,
					text,
					Application::ProductName,
					buttons,
					icon);
					
				JR::Utils::GUI::Forms::FlexibleMessageBox::Show(this,
					text,
					Application::ProductName,
					buttons,
					icon);
			}
			if (false)
			{
				TargetCodecDialog dlg(true,
					Program::IniFile,
					SECTION_TARGETCODECDIALOG,
					gcnew array<String^>{"aaa.mp4"},
					gcnew AVCodec("h264"),
					gcnew AVCodec("acc"));

				dlg.ShowDialog();
			}
		}
#endif
	}
}