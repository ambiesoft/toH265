#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;
	
		System::Void FormMain::FormMain_Load(System::Object^ sender, System::EventArgs^ e)
		{
			baseSetFFProbeMenuString_ = tsmiSetFFProbe->Text;
			baseSetFFMpegMenuString_ = tsmiSetFFMpeg->Text;

			AmbLib::MakeTripleClickTextBox(txtFFMpegArg, GetDoubleClickTime());

			notifyIconMain->Text = Application::ProductName;

			Text = Application::ProductName;
			ChangeStartButtonText(I18N(STR_BUTTONTEXT_START));

			for each (String ^ file in Program::CmdMovieFiles)
				CheckMovieAndSet(file, true, true);
		}
	}
}
