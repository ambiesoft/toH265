#include "stdafx.h"

#include "toH265.h"
#include "TargetCodecDialog.h"

#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

		System::Void FormMain::tsmiEdit_DropDownOpening(System::Object^ sender, System::EventArgs^ e)
		{
			tsmiClearCompletedItems->Enabled = !IsTaskActive;
			tsmiClearAllItems->Enabled = !IsTaskActive;
		}
		System::Void FormMain::cmList_Opening(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e)
		{
			tsmiRemoveFromList->Enabled = !IsTaskActive;
		}

		System::Void FormMain::tsmiClearCompletedItems_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (IsTaskActive)
				return;
			for each(ListViewItem^ item in lvInputs->Items)
			{
				if (item->ImageKey == IMAGEKEY_DONE)
					item->Remove();
			}
		}

		System::Void FormMain::tsmiClearAllItems_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (IsTaskActive)
				return;
			if (lvInputs->Items->Count == 0)
				return;
			if (System::Windows::Forms::DialogResult::Yes !=
				CppUtils::YesOrNo(I18N(L"Are you sure to clear all items?")))
			{
				return;
			}

			lvInputs->Items->Clear();
		}
		
#ifdef _DEBUG
		System::Void FormMain::FormMain_OnTest(System::Object^ sender, System::EventArgs^ e)
		{
			if (!true)
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