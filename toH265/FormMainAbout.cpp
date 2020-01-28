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

		System::Void FormMain::tsmiAbout_Click(System::Object^ sender, System::EventArgs^ e)
		{
			StringBuilder sbMessage;
			sbMessage.Append(Application::ProductName);
			sbMessage.Append(" ver");
			sbMessage.Append(AmbLib::getAssemblyVersion(System::Reflection::Assembly::GetExecutingAssembly(), 3));

			sbMessage.AppendLine();
			sbMessage.AppendFormat(I18N(L"ElapseTimeQueueLength:{0}"), elapses_.Size);
			sbMessage.AppendLine();
			sbMessage.AppendFormat("Is64BitProcess:{0}", Environment::Is64BitProcess);
			CppUtils::Info(this, sbMessage.ToString());
		}

		String^ FormMain::GetFFMpegOutput(String^ option)
		{
			int retval;
			String^ output;
			String^ err;

			try
			{
			
				AmbLib::OpenCommandGetResult(FFMpeg,
					option,
					System::Text::Encoding::UTF8,
					retval,
					output,
					err);
				if (retval != 0)
				{

				}
			}
			catch (Exception ^ ex)
			{
				CppUtils::Alert(this, ex);
			}

			// DASSERT(String::IsNullOrEmpty(err));
			DASSERT(!String::IsNullOrEmpty(output));
			return output;
		}
		String^ FormMain::GetFFMpegHelp(String^ subHelpOption)
		{
			String^ arg = L"--help";
			if (!String::IsNullOrEmpty(subHelpOption))
			{
				arg += L" ";
				arg += subHelpOption;
			}
			return GetFFMpegOutput(arg);
		}
		System::Void FormMain::tsmiFFMpegHelp_Click(System::Object^ sender, System::EventArgs^ e)
		{
			List<KeyValuePair<String^, String^>> labelsAndText;

			labelsAndText.Add(KeyValuePair<String^, String^>(I18N(L"Help"), GetFFMpegHelp(String::Empty)));
			labelsAndText.Add(KeyValuePair<String^, String^>(I18N(L"Long Help"), GetFFMpegHelp(L"long")));
			labelsAndText.Add(KeyValuePair<String^, String^>(I18N(L"Full Help"), GetFFMpegHelp(L"full")));
			labelsAndText.Add(KeyValuePair<String^, String^>(I18N(L"formats"), GetFFMpegOutput(L"-formats")));
			labelsAndText.Add(KeyValuePair<String^, String^>(I18N(L"codecs"), GetFFMpegOutput(L"-codecs")));
			labelsAndText.Add(KeyValuePair<String^, String^>(I18N(L"version"), GetFFMpegOutput(L"-version")));

			AmbLib::ShowTextDialog(this,
				I18N(L"FFMpeg Help") + L" - " + Application::ProductName,
				%labelsAndText,
				true);
		}
	}
}