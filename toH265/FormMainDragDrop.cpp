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


		System::Void FormMain::txtMovie_DragOver(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
		{
			if (FFMpegState != TaskState::None)
				return;
			if (e->Data->GetDataPresent(DataFormats::FileDrop, true))
				e->Effect = DragDropEffects::Copy;
		}
		System::Void FormMain::txtMovie_DragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
		{
			if (FFMpegState != TaskState::None)
				return;
			if (e->Data->GetDataPresent(DataFormats::FileDrop, true))
				e->Effect = DragDropEffects::Copy;
		}
		System::Void FormMain::txtMovie_DragLeave(System::Object^ sender, System::EventArgs^ e)
		{}
		System::Void FormMain::txtMovie_DragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e)
		{
			if (FFMpegState != TaskState::None)
				return;
			if (e->Data->GetDataPresent(DataFormats::FileDrop, true))
			{
				cli::array<String^>^ ss = (cli::array<String^>^)e->Data->GetData(DataFormats::FileDrop, true);
				for each (String ^ s in ss)
				{
					CheckMovieAndSet(s);
					return;
				}
			}
		}

	
	}
}
