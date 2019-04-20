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

		static String^ o2s(Object^ o)
		{
			if (!o)
				return nullptr;
			return o->ToString();
		}
		System::Void FormMain::tsmiLanguage_DropDownOpening(System::Object^  sender, System::EventArgs^  e)
		{
			for each(ToolStripMenuItem^ ti in tsmiLanguage->DropDownItems)
			{
				ti->Checked = false;
			}

			if ( o2s(tsmiLanguageEnglish->Tag->ToString())==Program::Culture)
				tsmiLanguageEnglish->Checked = true;
			else if (o2s(tsmiLanguageJapanese->Tag) == Program::Culture)
				tsmiLanguageJapanese->Checked = true;
			else
				tsmiLanguageOSDefault->Checked = true;
		}
		System::Void FormMain::tsmiLanguageCommon_Click(System::Object^  sender, System::EventArgs^  e)
		{
			DASSERT_IS_CLASS(sender, ToolStripMenuItem);
			ToolStripMenuItem^ item = (ToolStripMenuItem^)sender;

			String^ lang = item->Tag ? item->Tag->ToString() : String::Empty;

			try
			{
				Program::Culture = lang;
				CppUtils::Info(I18N("The application needs to restart for the change to take effect."));
				return;
			}
			catch (Exception^ ex)
			{
				CppUtils::Fatal(ex);
			}
		}
	}
}