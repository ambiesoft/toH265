#include "stdafx.h"

//#include "toH265.h"
//#include "helper.h"

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


		System::Void FormMain::ListInputs_DragOver(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
		{
			if (IsTaskActive)
				return;
			if (e->Data->GetDataPresent(DataFormats::FileDrop, true))
				e->Effect = DragDropEffects::Copy;
		}
		System::Void FormMain::ListInputs_DragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
		{
			if (IsTaskActive)
				return;
			if (e->Data->GetDataPresent(DataFormats::FileDrop, true))
				e->Effect = DragDropEffects::Copy;
		}


		void FormMain::AfterDrop(array<String^>^ files)
		{
			if (closed_)
				return;
			if (IsTaskActive)
				return;

			for each (String ^ file in files)
			{
				CheckMovieAndSet(file, true, true);
			}
		}
		System::Void FormMain::ListInputs_DragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
		{
			if (IsTaskActive)
				return;
			if (e->Data->GetDataPresent(DataFormats::FileDrop, true))
			{
				array<String^>^ ss = (cli::array<String^>^)e->Data->GetData(DataFormats::FileDrop, true);
				this->BeginInvoke(gcnew VSADelegate(this, &FormMain::AfterDrop),
					gcnew array<Object^>{ss});
			}
		}
		System::Void FormMain::ListInputs_DragLeave(System::Object^ sender, System::EventArgs^ e)
		{

		}
	
	}
}
