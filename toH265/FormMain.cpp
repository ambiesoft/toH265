#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Collections::Generic;
		using namespace System::Diagnostics;
		using namespace System::IO;
		using namespace System::Text;
		using namespace Newtonsoft::Json::Linq;
		String^ FormMain::IniFile::get()
		{
			return Path::Combine(Path::GetDirectoryName(Application::ExecutablePath),
				Path::GetFileNameWithoutExtension(Application::ExecutablePath) + L".ini");
		}

	
		bool FormMain::hasVideoStream(String^ file, String^% codecname)
		{
			String^ ffprobe = FFProbe;
			if (String::IsNullOrEmpty(ffprobe) || !File::Exists(ffprobe))
			{
				CppUtils::Alert(I18N(L"ffprobe not found"));
				return false;
			}

			String^ arg = L"-v quiet -print_format json -show_format -show_streams ";
			arg += AmbLib::doubleQuoteIfSpace(file);

			int retval;
			String^ output;
			String^ err;
			try
			{
				AmbLib::OpenCommandGetResult(
					ffprobe,
					arg,
					System::Text::Encoding::Default,
					retval,
					output,
					err);
			}
			catch (Exception^ ex)
			{
				CppUtils::Alert(ex);
				return false;
			}


			// Parse json
			JObject^ joutput = JObject::Parse(output);

			bool videofound = false;
			for (int i=0 ; i < 32; ++i)
			{
				String^ codec_type = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_type", i));
				if (!codec_type)
					break;
				if (codec_type == "video")
				{
					videofound = true;
					codecname = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_name", i));
					return true;
				}
			} 

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

				String^ codecname;
				if (!hasVideoStream(file, codecname))
				{
					CppUtils::Alert(String::Format(I18N(L"'{0}' does not have video stream."), file));
					ReturnValue = RETURN_STREAMNOTFOUND;
					return false;
				}
				// Check if the encoding is already h265
				if (String::Compare(codecname, "hevc", true)==0)
				{
					CppUtils::Alert(String::Format(I18N(L"'{0}' already has a stream of h265."), file));
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

		 Object^ safeEndInvoke(System::Windows::Forms::Control^ c, IAsyncResult^ r, Object^ defRet)
		 {
			 try
			 {
				 return c->EndInvoke(r);
			 }
			 catch (Exception^) {}
			 return defRet;
		 }

		 void FormMain::AddToOutput(String^ text)
		 {
			 txtLogOut->AppendText(text);
		 }
		 void FormMain::outputHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e)
		 {
			 if (!e->Data)
				 return;

			 IAsyncResult^ r = this->BeginInvoke(
				 gcnew AddToLog(this, &FormMain::AddToOutput), e->Data);
		 }
		 
		 void FormMain::AddToErr(String^ text)
		 {
			 txtLogErr->AppendText(text);
			 txtLogErr->AppendText(L"\r\n");
		 }

		 void FormMain::errHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e)
		 {
			 if (!e->Data)
				 return;

			 IAsyncResult^ r = this->BeginInvoke(
				 gcnew AddToLog(this, &FormMain::AddToErr), e->Data);
		 }

		 System::Void FormMain::btnStart_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^ ffmpeg = FFMpeg;
			 if (String::IsNullOrEmpty(ffmpeg) || !File::Exists(ffmpeg))
			 {
				 CppUtils::Alert(I18N(L"ffmpeg not found"));
				 return;
			 }

			 String^ inputmovie = txtMovie->Text;
			 if (String::IsNullOrEmpty(inputmovie) || !File::Exists(inputmovie))
			 {
				 CppUtils::Alert(I18N(L"inputmovie not found"));
				 return;
			 }

			 SaveFileDialog dlg;
			 List<String^> availableext;
			 availableext.Add(Path::GetExtension(inputmovie));
			 
			 StringBuilder sbFilter;
			 for each(String^ ae in availableext)
			 {
				 sbFilter.Append(ae);
				 sbFilter.Append("File ");
				 sbFilter.Append("(*");
				 sbFilter.Append(ae);

				 sbFilter.Append(")|*");
				 sbFilter.Append(ae);
				 sbFilter.Append("|");
			 }
			 sbFilter.Append("All File(*.*)|*.*");
			 dlg.Filter = sbFilter.ToString();

			 if (System::Windows::Forms::DialogResult::OK != dlg.ShowDialog())
				 return;

			 String^ outputmovie = dlg.FileName;

			 String^ arg = String::Format(L"-i \"{0}\" -c:v libx265 -c:a copy \"{1}\"",
				 inputmovie, outputmovie);

			 int retval;
			 sbFFMpegOut_.Clear();
			 sbFFMpegErr_.Clear();

			 txtLogOut->Clear();
			 txtLogErr->Clear();

			 txtFFMpegArg->Text = arg;

			 // TODO: must run in a thread
			 try
			 {
				 AmbLib::OpenCommandGetResultCallback(ffmpeg,
					 arg,
					 Encoding::Default,
					 retval,
					 gcnew DataReceivedEventHandler(this, &FormMain::outputHandler),
					 gcnew DataReceivedEventHandler(this, &FormMain::errHandler)
					 );
			 }
			 catch (Exception^ ex)
			 {
				 CppUtils::Alert(ex);
				 return;
			 }

		 }
		 System::Void FormMain::btnStop_Click(System::Object^  sender, System::EventArgs^  e)
		 {}

	}

}