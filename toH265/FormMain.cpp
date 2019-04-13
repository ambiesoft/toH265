#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::IO;

		// TODO: implement
		bool isH256Video(String^ file)
		{
			return false;
		}
		// TODO: implement
		bool hasStream(String^ file)
		{
			return false;
		}

		bool FormMain::CheckMovieAndSet(String^ file)
		{
			if (String::IsNullOrEmpty(file))
			{
				txtMovie->Text = String::Empty;
			}
			else
			{
				if (!File::Exists(file))
				{
					CppUtils::Alert(String::Format(I18N(L"Not found '{0}'."), file));
					ReturnValue = RETURN_FILENOTFOUND;
					return false;
				}

				if (!hasStream(file))
				{
					CppUtils::Alert(String::Format(I18N(L"'{0}' does not have video stream."), file));
					ReturnValue = RETURN_STREAMNOTFOUND;
					return false;
				}
				// Check if the encoding is already h265
				if (isH256Video(file))
				{
					CppUtils::Alert(String::Format(I18N(L"'{0}' is already has h265 encoding."), file));
					ReturnValue = RETURN_STREAMISH265;
					return false;
				}
				txtMovie->Text = file;
			}
			return true;
		}
		System::Void FormMain::FormMain_Load(System::Object^  sender, System::EventArgs^  e)
		{
			Text = Application::ProductName;

			CheckMovieAndSet(Program::MovieFile);
		}

		 System::Void FormMain::btnBrowseMovie_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 OpenFileDialog dlg;
			 if (System::Windows::Forms::DialogResult::OK != dlg.ShowDialog())
				 return;
			 
			 CheckMovieAndSet(dlg.FileName);
		 }
	}

}