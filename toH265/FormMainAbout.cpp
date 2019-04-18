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

			CppUtils::Info(this, sbMessage.ToString());
		}


		String^ FormMain::GetFFMpegHelp(String^ subHelpOption)
		{
			int retval;
			String^ output;
			String^ err;

			try
			{
				String^ arg = L"--help";
				if (!String::IsNullOrEmpty(subHelpOption))
				{
					arg += L" ";
					arg += subHelpOption;
				}
				AmbLib::OpenCommandGetResult(FFMpeg,
					arg,
					System::Text::Encoding::Default,
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
		System::Void FormMain::tsmiFFMpegHelp_Click(System::Object^ sender, System::EventArgs^ e)
		{
			String^ normalHelp = GetFFMpegHelp(String::Empty);
			String^ longHelp = GetFFMpegHelp(L"long");
			String^ fullHelp = GetFFMpegHelp(L"full");

			List<KeyValuePair<String^, String^>> labelsAndText;

			labelsAndText.Add(KeyValuePair<String^, String^>(I18N(L"Help"), normalHelp));
			labelsAndText.Add(KeyValuePair<String^, String^>(I18N(L"Long Help"), longHelp));
			labelsAndText.Add(KeyValuePair<String^, String^>(I18N(L"Full Help"), fullHelp));

			AmbLib::ShowTextDialog(this,
				I18N(L"FFMpeg Help") + L" - " + Application::ProductName,
				% labelsAndText,
				true);
		}
	}
}