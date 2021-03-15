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
			// Hide();
			//::ShowWindow((HWND)this->Handle.ToPointer(), SW_MINIMIZE);
			WindowState = FormWindowState::Minimized;
			System::Threading::Thread::Sleep(200);
			Hide();
			//AnimateWindow((HWND)this->Handle.ToPointer(),
			//	200,
			//	AW_HIDE| AW_SLIDE);
			notifyIconMain->Visible = true;
		}
		System::Void FormMain::FormMain_Resize(System::Object^ sender, System::EventArgs^)
		{
			if (tsmiMinimizeToTray->Checked && this->WindowState == FormWindowState::Minimized)
			{
				IconizeToTray();
			}

			if (this->WindowState == FormWindowState::Normal)
			{
				System::Drawing::Point topLeft = lblTopLeft->Location;
				System::Drawing::Size size(
					// lblBottomRight->Location.X + lblBottomRight->Size.Width - lblTopLeft->Location.X,
					txtFFMpegArg->Size.Width,
					lblBottomRight->Location.Y + lblBottomRight->Size.Height - lblTopLeft->Location.Y
				);
				DASSERT(size.Width > 0);
				DASSERT(size.Height > 0);

				txtLogErr->Location = topLeft;
				txtLogErr->Size = size;
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