#include "stdafx.h"

// #include "../../lsMisc/CloseConsoleWin.h"

#include "toH265.h"
#include "helper.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

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

		void FormMain::CreateShutdownedSummary()
		{
			// Save summary to a temp file and create a startup shortcut that launches
			// this app with: --after-shutdown "<tempFile>"
			String^ tempFile = Path::GetTempFileName();
			DASSERT(lastSummary_);
			if (lastSummary_)
			{
				lastSummary_->GetSummaryText()->SaveToFile(tempFile);

				try
				{
					// Prepare paths and arguments
					String^ startupFolder = Environment::GetFolderPath(Environment::SpecialFolder::Startup);
					String^ exePath = System::Windows::Forms::Application::ExecutablePath;
					String^ exeNameNoExt = Path::GetFileNameWithoutExtension(exePath);
					String^ shortcutPath = Path::Combine(startupFolder, exeNameNoExt + "_aftershutdown.lnk");
					String^ args = "--after-shutdown \"" + tempFile + "\"";
					String^ workDir = Path::GetDirectoryName(exePath);

					// Marshal managed strings to native wide strings
					System::IntPtr pExe = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(exePath);
					System::IntPtr pArgs = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(args);
					System::IntPtr pShortcut = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(shortcutPath);
					System::IntPtr pWorkDir = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(workDir);

					// Convert to native pointers
					LPCOLESTR wszExe = static_cast<LPCOLESTR>(pExe.ToPointer());
					LPCOLESTR wszArgs = static_cast<LPCOLESTR>(pArgs.ToPointer());
					LPCOLESTR wszShortcut = static_cast<LPCOLESTR>(pShortcut.ToPointer());
					LPCOLESTR wszWorkDir = static_cast<LPCOLESTR>(pWorkDir.ToPointer());

					HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
					IShellLinkW* psl = nullptr;
					IPersistFile* ppf = nullptr;

					if (SUCCEEDED(hr))
					{
						hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void**)&psl);
						if (SUCCEEDED(hr) && psl)
						{
							// Set executable, arguments and working directory
							psl->SetPath(wszExe);
							psl->SetArguments(wszArgs);
							psl->SetWorkingDirectory(wszWorkDir);

							hr = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
							if (SUCCEEDED(hr) && ppf)
							{
								hr = ppf->Save(wszShortcut, TRUE);
								ppf->Release();
								ppf = nullptr;
							}
							psl->Release();
							psl = nullptr;
						}
						CoUninitialize();
					}

					// Free unmanaged memory
					System::Runtime::InteropServices::Marshal::FreeHGlobal(pExe);
					System::Runtime::InteropServices::Marshal::FreeHGlobal(pArgs);
					System::Runtime::InteropServices::Marshal::FreeHGlobal(pShortcut);
					System::Runtime::InteropServices::Marshal::FreeHGlobal(pWorkDir);

					if (FAILED(hr))
					{
						CppUtils::Alert(this, I18N(L"Failed to create startup shortcut."));
					}
				}
				catch (Exception^)
				{
					CppUtils::Alert(this, I18N(L"Failed to create startup shortcut."));
				}
			}
		}

		void FormMain::OnLaunch(System::Object^ sender, Ambiesoft::AfterFinish::LaunchEventArgs^ e)
		{
			shutdownStarted_ = true;
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

			if (shutdownStarted_ && e->CloseReason == CloseReason::WindowsShutDown)
			{
				CreateShutdownedSummary();
			}
			closed_ = true;
		}

		System::Void FormMain::FormMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
		{
			SetThreadExecutionState(ES_CONTINUOUS);

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