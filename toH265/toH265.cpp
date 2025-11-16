#include "stdafx.h"

#include "../../lsMisc/HighDPI.h"
#include "../../lsMisc/CommandLineParser.h"

#include "FormMain.h"
#include "toH265.h"

using namespace Ambiesoft;

namespace Ambiesoft {
	namespace toH265 {

		using namespace System;
		using namespace System::IO;

		String^ Program::IniFile::get()
		{
			return Path::Combine(Path::GetDirectoryName(Application::ExecutablePath),
				Path::GetFileNameWithoutExtension(Application::ExecutablePath) + L".ini");
		}

		String^ Program::Culture::get()
		{
			if (!cultureInfo_)
				return String::Empty;

			return cultureInfo_->Name;
		}
		void Program::Culture::set(String^ value)
		{
			if (String::IsNullOrEmpty(value))
			{
				cultureInfo_ = nullptr;
				return;
			}

			cultureInfo_ = gcnew System::Globalization::CultureInfo(value);
			System::Threading::Thread::CurrentThread->CurrentCulture = cultureInfo_;
			System::Threading::Thread::CurrentThread->CurrentUICulture = cultureInfo_;
		}

		String^ Program::getString(String^ origText)
		{
			String^ ret = theResource_->GetString(origText, cultureInfo_);

#ifdef _DEBUG
			if (String::IsNullOrEmpty(ret) && !listNotI18Ned_.Contains(origText))
			{
				DTRACE(L"\"" + origText + "\" is not I18Ned.");
				listNotI18Ned_.Add(origText);
			}
#endif
			return String::IsNullOrEmpty(ret) ? origText : ret;
		}
		bool Program::preRun()
		{
			String^ specculture;
			if (Profile::GetString(
				FormMain::SECTION_OPTION,
				FormMain::KEY_CULTURE,
				String::Empty,
				specculture,
				IniFile)
				&& !String::IsNullOrEmpty(specculture))
			{
				try
				{
					Culture = specculture;
				}
				catch (Exception ^ ex)
				{
					CppUtils::Fatal(ex);
				}
			}

			CCommandLineParser cmdParser;

			std::wstring shutdownMessageFile;
			cmdParser.AddOption(
				{ L"--after-shutdown" },
				ArgCount::ArgCount_One,
				&shutdownMessageFile,
				ArgEncodingFlags_Default,
				L"Message After Shutdown");

			cmdParser.Parse();

			if (!shutdownMessageFile.empty())
			{
				SummaryText^ st = SummaryText::LoadFromFile(
					gcnew String(shutdownMessageFile.c_str()));
				st->Show(nullptr);

				String^ startupFolder = Environment::GetFolderPath(Environment::SpecialFolder::Startup);
				String^ exePath = System::Windows::Forms::Application::ExecutablePath;
				String^ exeNameNoExt = Path::GetFileNameWithoutExtension(exePath);
				String^ shortcutPath = Path::Combine(startupFolder, exeNameNoExt + "_aftershutdown.lnk");


				// If a startup shortcut exists, inspect it and delete if it points to this exe
				// with arguments "--after-shutdown <shutdownMessageFile>".
				try
				{
					if (File::Exists(shortcutPath))
					{
						// Use WScript.Shell CreateShortcut to read .lnk properties
						Type^ wshType = Type::GetTypeFromProgID("WScript.Shell");
						if (wshType != nullptr)
						{
							Object^ wsh = Activator::CreateInstance(wshType);
							array<Object^>^ createArgs = gcnew array<Object^>(1){ shortcutPath };
							Object^ sc = wshType->InvokeMember(
								"CreateShortcut",
								System::Reflection::BindingFlags::InvokeMethod,
								nullptr, wsh, createArgs);

							if (sc != nullptr)
							{
								Type^ scType = sc->GetType();
								String^ target = safe_cast<String^>(scType->InvokeMember(
									"TargetPath",
									System::Reflection::BindingFlags::GetProperty,
									nullptr, sc, nullptr));
								String^ arguments = safe_cast<String^>(scType->InvokeMember(
									"Arguments",
									System::Reflection::BindingFlags::GetProperty,
									nullptr, sc, nullptr));

								// Normalize paths for comparison
								String^ exeFull = Path::GetFullPath(exePath);
								String^ targetFull = target;
								try { targetFull = Path::GetFullPath(target); }
								catch (...) { /* ignore malformed target */ }

								// Convert shutdownMessageFile (native) to managed
								String^ shutdownFileManaged = gcnew String(shutdownMessageFile.c_str());

								bool targetIsThisExe = !String::IsNullOrEmpty(targetFull) &&
									String::Compare(targetFull, exeFull, true) == 0;
								bool hasFlag = !String::IsNullOrEmpty(arguments) &&
									(arguments->IndexOf("--after-shutdown", StringComparison::InvariantCultureIgnoreCase) >= 0);
								bool hasFile = !String::IsNullOrEmpty(arguments) &&
									(arguments->IndexOf(shutdownFileManaged, StringComparison::InvariantCultureIgnoreCase) >= 0);

								if (targetIsThisExe && hasFlag && hasFile)
								{
									// Delete the shortcut because it matches
									File::Delete(shortcutPath);
									File::Delete(shutdownFileManaged);
								}
							}
						}
					}
				}
				catch (Exception^)
				{
					// fail silently - don't block startup on shortcut inspection errors
				}

				return false;
			}
			return true;
		}

		void Program::SaveSettings()
		{
			bool success = true;

			success &= Profile::WriteString(
				FormMain::SECTION_OPTION,
				FormMain::KEY_CULTURE,
				Culture,
				IniFile);

			if (!success)
			{
				CppUtils::Alert(I18N(FormMain::STR_FAILED_TO_SAVE_SETTING));
			}
		}
		int Program::main(array<System::String ^> ^args)
		{
			InitHighDPISupport();
			Application::EnableVisualStyles();
			Application::SetCompatibleTextRenderingDefault(false);

			if (!preRun())
				return 1;

			for each (String ^ arg in args)
			{
				clMovieFiles_.Add(arg);
			}

			// Create the main window and run it
			FormMain formMain;
			Application::Run(%formMain);

			SaveSettings();
#ifdef _DEBUG
			showUnI18Ned();
#endif
			return formMain.ReturnValue;
		}


	}
}