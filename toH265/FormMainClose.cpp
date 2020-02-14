#include "stdafx.h"

// #include "../../lsMisc/CloseConsoleWin.h"

#include "toH265.h"
#include "helper.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

		bool FormMain::IsEncoding()
		{
			switch (FFMpegState)
			{
			case TaskState::Pausing:
			case TaskState::ProcessLaunching:
			case TaskState::Running:
				return true;
			}
			
			return false;
		}

		bool FormMain::ConfirmAndStopEncode()
		{
			switch (FFMpegState)
			{
			case TaskState::None:
				return true;
			case TaskState::Pausing:
			case TaskState::ProcessLaunching:
			case TaskState::Running:
			{
				if (System::Windows::Forms::DialogResult::Yes !=
					CppUtils::YesOrNo(this, I18N(L"Encoding is in progress. Are you sure to quit?"),
						MessageBoxDefaultButton::Button2))
				{
					return false;
				}
				processTerminated_ = true;
		
				if (!KillProcess(processFFMpeg_))
				{
					processTerminated_ = false;
					CppUtils::Alert(this, I18N(L"Failed to kill process."));
				}
				
				processFFMpeg_ = nullptr;
				SafeJoin(thFFMpeg_);
				thFFMpeg_ = nullptr;

				ChangeStartButtonText(I18N(STR_BUTTONTEXT_RESUME));
				processSuspeded_ = true;
				return true;
			}
			break;

			case TaskState::Unknown:
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
				&& IsEncoding())
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
		}

		System::Void FormMain::FormMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
		{
			DTRACE(L"INI=" + Program::IniFile);
			HashIni^ ini = Profile::ReadAll(Program::IniFile);

			bool success = true;
			success &= AmbLib::SaveFormXYWH(this, SECTION_LOCATION, ini);
			success &= AmbLib::SaveListViewColumnWidth(lvInputs, SECTION_COLUMNS, KEY_LISTVIEW_COLUMNS, ini);
			success &= Profile::WriteInt(SECTION_OPTION, KEY_SPLITROOT_DISTANCE, splitRoot->SplitterDistance, ini);
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