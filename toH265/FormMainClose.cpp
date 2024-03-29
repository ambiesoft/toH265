#include "stdafx.h"

// #include "../../lsMisc/CloseConsoleWin.h"

#include "toH265.h"
#include "helper.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

		//bool FormMain::IsEncoding()
		//{
		//	switch (FFMpegState)
		//	{
		//	case TaskState::Pausing:
		//	case TaskState::ProcessLaunching:
		//	case TaskState::Running:
		//		return true;
		//	}
		//	
		//	return false;
		//}

		bool FormMain::ConfirmAndStopEncode()
		{
			switch (CurrentFFMpegState)
			{
			case FFMpegState::None:
				return true;
			case FFMpegState::Intermidiate:
			case FFMpegState::Pausing:
			case FFMpegState::ProcessLaunching:
			case FFMpegState::Running:
			{
				if (System::Windows::Forms::DialogResult::Yes !=
					CppUtils::YesOrNo(this, I18N(L"Encoding is in progress. Are you sure to quit?"),
						MessageBoxDefaultButton::Button2))
				{
					return false;
				}
				processTerminatedDuetoAppClose_ = true;
		
				if (!KillProcess(processFFMpeg_))
				{
					processTerminatedDuetoAppClose_ = false;
					CppUtils::Alert(this, I18N(L"Failed to kill process."));
				}
				
				processFFMpeg_ = nullptr;
				SafeJoin(thFFMpeg_);
				thFFMpeg_ = nullptr;

				ChangeStartButtonText(StartButtonText::Resume);
				processSuspeded_ = true;
				if (encodeTask_)
					encodeTask_->Cancel();
				encodeTask_ = nullptr;
				return true;
			}
			break;

			case FFMpegState::Unknown:
				CppUtils::Alert(this, I18N(L"Unknow Error."));
				return false;
			}

			DASSERT(false);
			return false;
		}

		System::Void FormMain::exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
		{
			bCloseFromMenu_ = true;
			Close();
		}

		System::Void FormMain::FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
		{
			if (e->CloseReason != CloseReason::WindowsShutDown &&
				tsmiMinimizeToTray->Checked &&
				!bCloseFromMenu_
				&& IsTaskActive)
			{
				IconizeToTray();
				e->Cancel = true;
				return;
			}

			if (!ConfirmAndStopEncode())
			{
				e->Cancel = true;
				return;
			}
			closed_ = true;
		}

		System::Void FormMain::FormMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
		{
			DTRACE(L"INI=" + Program::IniFile);
			HashIni^ ini = Profile::ReadAll(Program::IniFile);

			bool success = true;
			if (this->WindowState == FormWindowState::Normal)
			{
				success &= AmbLib::SaveFormXYWH(this, SECTION_LOCATION, ini);
				success &= Profile::WriteInt(SECTION_OPTION, KEY_SPLITROOT_DISTANCE, splitRoot->SplitterDistance, ini);
			}
			success &= AmbLib::SaveListViewColumnWidth(lvInputs, SECTION_COLUMNS, KEY_LISTVIEW_COLUMNS, ini);
			success &= Profile::WriteBool(SECTION_OPTION, KEY_AFTERPROCESS_ENABLED, tsmiEnabledtsmiProcessAfterFinish->Checked, ini);
			success &= Profile::WriteBool(SECTION_OPTION, KEY_CPUAFFINITY_ENABLED, tsmiCPUAffinityEnable->Checked, ini);
			success &= cpuAffinity_.Save(SECTION_CPUAFFINITY, ini);
			if (success)
			{
				success = Profile::WriteAll(ini, Program::IniFile);
			}
			if (!success)
			{
				CppUtils::Alert(this, I18N(STR_FAILED_TO_SAVE_SETTING));
			}
		}
	}
}