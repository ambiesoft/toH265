#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

		System::Void FormMain::tsmiMinimizeToTray_Click(System::Object^ sender, System::EventArgs^ e)
		{
			bool newval = !tsmiMinimizeToTray->Checked;
			tsmiMinimizeToTray->Checked = newval;

			if (!Profile::WriteBool(SECTION_OPTION, KEY_MINIMIZETOTRAY, newval, Program::IniFile))
			{
				CppUtils::Alert(this, I18N(STR_FAILED_TO_SAVE_SETTING));
			}
		}
		
		void FormMain::IconizeToTray()
		{
			Hide();
			notifyIconMain->Visible = true;
		}
		System::Void FormMain::FormMain_Resize(System::Object^ sender, System::EventArgs^ e)
		{
			if (tsmiMinimizeToTray->Checked && this->WindowState == FormWindowState::Minimized)
			{
				IconizeToTray();
			}
		}
		System::Void FormMain::notifyIconMain_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		{
			if (e->Button != System::Windows::Forms::MouseButtons::Left)
				return;

			tsmiNotifyShow_Click(sender, nullptr);
		}
	}
}