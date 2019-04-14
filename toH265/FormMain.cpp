#include "stdafx.h"

#include "toH265.h"
#include "helper.h"

#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Collections::Generic;
		using namespace System::Diagnostics;
		using namespace System::IO;
		using namespace System::Text;
		using namespace System::Threading;

		using namespace Newtonsoft::Json::Linq;
		String^ FormMain::IniFile::get()
		{
			return Path::Combine(Path::GetDirectoryName(Application::ExecutablePath),
				Path::GetFileNameWithoutExtension(Application::ExecutablePath) + L".ini");
		}

		System::Void FormMain::aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			StringBuilder sbMessage;
			sbMessage.Append(Application::ProductName);
			sbMessage.Append(" ver");
			sbMessage.Append(AmbLib::getAssemblyVersion(System::Reflection::Assembly::GetExecutingAssembly(), 3));

			CppUtils::Info(this, sbMessage.ToString());
		}

		bool FormMain::hasVideoStream(String^ file, String^% codecname, TimeSpan% ts)
		{
			String^ ffprobe = FFProbe;
			if (String::IsNullOrEmpty(ffprobe) || !File::Exists(ffprobe))
			{
				CppUtils::Alert(this, I18N(L"ffprobe not found"));
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
				CppUtils::Alert(this, ex);
				return false;
			}


			// Parse json
			JObject^ joutput = JObject::Parse(output);

			bool videofound = false;
			for (int i = 0; i < 32; ++i)
			{
				String^ codec_type = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_type", i));
				if (!codec_type)
					break;
				if (codec_type == "video")
				{
					videofound = true;
					codecname = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_name", i));

					String^ duration = (String^)joutput->SelectToken(String::Format("streams[{0}].duration", i));
					double d = System::Double::Parse(duration);
					// to 100nanosec 
					d *= 10 * 1000 * 1000;
					ts = TimeSpan((Int64)d);
					return true;
				}
			}

			return false;
		}

		bool FormMain::CheckMovieAndSet(String^ file)
		{
			WaitCursor wc;
			if (String::IsNullOrEmpty(file))
			{
				txtMovie->Text = String::Empty;
			}
			else
			{
				if (!File::Exists(file))
				{
					CppUtils::Alert(this, String::Format(I18N(L"Not found '{0}'."), file));
					ReturnValue = RETURN_FILENOTFOUND;
					return false;
				}

				String^ codecname;
				if (!hasVideoStream(file, codecname, tsOrigMovie_))
				{
					CppUtils::Alert(this, String::Format(I18N(L"'{0}' does not have video stream."), file));
					ReturnValue = RETURN_STREAMNOTFOUND;
					return false;
				}
				// Check if the encoding is already h265
				if (String::Compare(codecname, "hevc", true) == 0)
				{
					CppUtils::Alert(this, String::Format(I18N(L"'{0}' already has a stream of h265."), file));
					ReturnValue = RETURN_STREAMISH265;
					return false;
				}
				txtMovie->Text = file;
			}
			return true;
		}
		System::Void FormMain::FormMain_Load(System::Object^  sender, System::EventArgs^  e)
		{
			baseSetFFProbeMenuString_ = tsmiSetFFProbe->Text;
			baseSetFFMpegMenuString_ = tsmiSetFFMpeg->Text;

			Text = Application::ProductName;
			btnStart->Text = I18N(BUTTONTEXT_START);
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
			txtLogOut->Text = text;
		}
		void FormMain::outputHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e)
		{
			if (!e->Data)
				return;

			IAsyncResult^ r = this->BeginInvoke(
				gcnew AddToLog(this, &FormMain::AddToOutput), e->Data);
		}

		void FormMain::UpdateTitle()
		{
			Text = Application::ProductName;
		}
		void FormMain::UpdateTitle(int percent)
		{
			StringBuilder sbTitle;
			sbTitle.Append(percent);
			sbTitle.Append("% - ");
			sbTitle.Append(Application::ProductName);

			if (Text != sbTitle.ToString())
				Text = sbTitle.ToString();
		}
		void FormMain::UpdateTitleComplete()
		{
			UpdateTitle(100);
		}
		void FormMain::UpdateTitleTS(TimeSpan ts)
		{
			double percent = (ts.TotalMilliseconds / tsOrigMovie_.TotalMilliseconds) * 100;
			UpdateTitle((int)percent);
		}
		
		void FormMain::AddToErr(String^ text)
		{
			/*
frame=   61 fps= 18 q=-0.0 size=       0kB time=00:00:02.08 bitrate=   0.2kbits/s dup=1 drop=0 speed=0.619x
frame=   69 fps= 17 q=-0.0 size=       0kB time=00:00:02.34 bitrate=   0.2kbits/s dup=1 drop=0 speed=0.591x
frame=   76 fps= 17 q=-0.0 size=       0kB time=00:00:02.57 bitrate=   0.1kbits/s dup=1 drop=0 speed=0.566x
frame=   85 fps= 17 q=-0.0 size=       0kB time=00:00:02.87 bitrate=   0.1kbits/s dup=1 drop=0 speed=0.566x
*/
			if (!regFFMpeg_)
				regFFMpeg_ = gcnew RegularExpressions::Regex("frame=\\s+\\d+.*time=(\\d\\d:\\d\\d:\\d\\d)\\.\\d\\d");
			if (regFFMpeg_->IsMatch(text))
			{
				//StringBuilder sb;
				//RegularExpressions::Match^ match = reg.Match(text);
				//do
				//{
				//	match->Value
				//}
				RegularExpressions::Match^ match = regFFMpeg_->Match(text);
				String^ timeValue = match->Groups[1]->Value;
				
				DateTime dt = DateTime::ParseExact(timeValue, L"hh:mm:ss", 
					System::Globalization::CultureInfo::InvariantCulture);
				TimeSpan ts = dt - dt.Date;
				UpdateTitleTS(ts);
				txtLogOut->Text = text;
			}
			else
			{
				txtLogErr->AppendText(text);
				txtLogErr->AppendText(L"\r\n");
			}
		}

		void FormMain::errHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e)
		{
			if (!e->Data)
				return;

			IAsyncResult^ r = this->BeginInvoke(
				gcnew AddToLog(this, &FormMain::AddToErr), e->Data);
		}

		FormMain::TaskState FormMain::FFMpegState::get()
		{
			if (!thFFMpeg_ && !processFFMpeg_)
			{
				return TaskState::None;
			}
			else if (thFFMpeg_ && !processFFMpeg_)
			{
				return TaskState::ProcessLaunching;
			}
			else if (!thFFMpeg_ && processFFMpeg_)
			{
				DASSERT(false);
				return TaskState::Unknown;
			}
			else if (thFFMpeg_ && processFFMpeg_)
			{
				if (processSuspeded_)
					return TaskState::Pausing;
				else
					return TaskState::Running;
			}

			return TaskState::Unknown;
		}
		void FormMain::ThreadStarted()
		{
			processSuspeded_ = false;
			processTerminated_ = false;

			btnStart->Text = I18N(BUTTONTEXT_PAUSE);

			txtMovie->Enabled = false;
			txtMovie->AllowDrop = false;
			btnBrowseMovie->Enabled = false;
		}
		void FormMain::ThreadEnded(int retval)
		{
			processSuspeded_ = false;
			SafeJoin(thFFMpeg_);
			delete thFFMpeg_;
			thFFMpeg_ = nullptr;

			processFFMpeg_ = nullptr;

			btnStart->Text = I18N(BUTTONTEXT_START);

			txtMovie->Enabled = true;
			txtMovie->AllowDrop = true;
			btnBrowseMovie->Enabled = true;

			if (!processTerminated_)
			{
				// Process ended normally
				if (retval != 0)
				{
					CppUtils::Alert(this, String::Format(L"Process exited with {0}.", retval));
				}
				else
				{
					// Succeeded
					UpdateTitleComplete();
					CppUtils::Info(this, I18N(L"Encoding Succeeded."));
				}
			}
		}

		void FormMain::StartOfThread(Object^ obj)
		{
			EndInvoke(BeginInvoke(gcnew VVDelegate(this, &FormMain::ThreadStarted)));
			Dictionary<String^, String^>^ hash = (Dictionary<String^, String^>^)obj;
			String^ ffmpeg = hash["ffmpeg"];
			String^ arg = hash["arg"];
			int retval;
			try
			{
				AmbLib::OpenCommandGetResultCallback(ffmpeg,
					arg,
					Encoding::Default,
					retval,
					gcnew DataReceivedEventHandler(this, &FormMain::outputHandler),
					gcnew DataReceivedEventHandler(this, &FormMain::errHandler),
					processFFMpeg_
					);
			}
			catch (Exception^ ex)
			{
				CppUtils::Alert(ex);
				return;
			}

			BeginInvoke(gcnew VIDelegate(this, &FormMain::ThreadEnded), retval);
		}
		System::Void FormMain::btnStart_Click(System::Object^  sender, System::EventArgs^  e)
		{
			switch (FFMpegState)
			{
			case TaskState::None:
				break;
			case TaskState::Pausing:
				if (!ResumeProcess(processFFMpeg_))
				{
					CppUtils::Alert(this, I18N(L"Failed to resume process."));
					return;
				}
				btnStart->Text = I18N(BUTTONTEXT_PAUSE);
				processSuspeded_ = false;
				return;
			case TaskState::ProcessLaunching:
				CppUtils::Alert(this, I18N(L"Intermidiate state. Wait a while and do it again."));
				return;
			case TaskState::Running:
				if (!SuspendProcess(processFFMpeg_))
				{
					CppUtils::Alert(this, I18N(L"Failed to suspend process."));
					return;
				}
				btnStart->Text = I18N(BUTTONTEXT_RESUME);
				processSuspeded_ = true;
				return;
			case TaskState::Unknown:
				CppUtils::Alert(this, I18N(L"Unknow Error."));
				return;
			}
			String^ ffmpeg = FFMpeg;
			if (String::IsNullOrEmpty(ffmpeg) || !File::Exists(ffmpeg))
			{
				CppUtils::Alert(this, I18N(L"ffmpeg not found"));
				return;
			}

			String^ inputmovie = txtMovie->Text;
			if (!CheckMovieAndSet(inputmovie))
				return;

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
			dlg.InitialDirectory = Path::GetDirectoryName(inputmovie);
			if (System::Windows::Forms::DialogResult::OK != dlg.ShowDialog())
				return;

			String^ outputmovie = dlg.FileName;

			String^ arg = String::Format(L"-y -i \"{0}\" -c:v libx265 -c:a copy \"{1}\"",
				inputmovie, outputmovie);

			txtLogOut->Clear();
			txtLogErr->Clear();

			txtFFMpegArg->Text = arg;

			Dictionary<String^, String^> param;
			param["ffmpeg"] = ffmpeg;
			param["arg"] = arg;
			thFFMpeg_ = gcnew System::Threading::Thread(gcnew ParameterizedThreadStart(this, &FormMain::StartOfThread));
			thFFMpeg_->Start(%param);
		}
		void FormMain::StopEncoding()
		{
			switch (FFMpegState)
			{
			case TaskState::None:
				break;
			case TaskState::Pausing:
			case TaskState::ProcessLaunching:
			case TaskState::Running:
				processTerminated_ = true;
				if (!KillProcess(processFFMpeg_))
				{
					processTerminated_ = false;
					CppUtils::Alert(this, I18N(L"Failed to kill process."));
				}
				processFFMpeg_ = nullptr;
				//if (!KillThread(thFFMpeg_))
				//{
				//	CppUtils::Alert(I18N(L"Failed to kill thread."));
				//}
				SafeJoin(thFFMpeg_);
				thFFMpeg_ = nullptr;

				btnStart->Text = I18N(BUTTONTEXT_RESUME);
				processSuspeded_ = true;
				return;
			case TaskState::Unknown:
				return;
			}

			return;
		}

		

		System::Void FormMain::txtMovie_DragOver(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		{
			if (e->Data->GetDataPresent(DataFormats::FileDrop, true))
				e->Effect = DragDropEffects::Copy;
		}
		System::Void FormMain::txtMovie_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		{
			if (e->Data->GetDataPresent(DataFormats::FileDrop, true))
				e->Effect = DragDropEffects::Copy;
		}
		System::Void FormMain::txtMovie_DragLeave(System::Object^  sender, System::EventArgs^  e)
		{}
		System::Void FormMain::txtMovie_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		{
			if (e->Data->GetDataPresent(DataFormats::FileDrop, true))
			{
				cli::array<String^>^ ss = (cli::array<String^>^)e->Data->GetData(DataFormats::FileDrop, true);
				for each(String^ s in ss)
				{
					txtMovie->Text = s;
					break;
				}
			}
		}

		System::Void FormMain::tsmiOption_DropDownOpening(System::Object^  sender, System::EventArgs^  e)
		{
			if (!String::IsNullOrEmpty(ffprobe_))
				tsmiSetFFProbe->Text = baseSetFFProbeMenuString_ + L" (" + ffprobe_ + L")";
			else
				tsmiSetFFProbe->Text = baseSetFFProbeMenuString_;


			if (!String::IsNullOrEmpty(ffmpeg_))
				tsmiSetFFMpeg->Text = baseSetFFMpegMenuString_ + L" (" + ffmpeg_ + L")";
			else
				tsmiSetFFMpeg->Text = baseSetFFMpegMenuString_;
		}

		System::Void FormMain::tsmiSetFFProbe_Click(System::Object^  sender, System::EventArgs^  e)
		{
			ffprobe_ = nullptr;
			getCommon(this, false, SECTION_OPTION, KEY_FFPROBE, IniFile, ffprobe_, true);
		}
		System::Void FormMain::tsmiSetFFMpeg_Click(System::Object^  sender, System::EventArgs^  e)
		{
			ffmpeg_ = nullptr;
			getCommon(this, true, SECTION_OPTION, KEY_FFMPEG, IniFile, ffmpeg_, true);
		}

		WaitCursor::WaitCursor()
		{
			if (1 == System::Threading::Interlocked::Increment(counter_))
			{
				cur_ = System::Windows::Forms::Cursor::Current;
				System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;
			}
		}

		WaitCursor::~WaitCursor()
		{
			if (0 == System::Threading::Interlocked::Decrement(counter_))
			{
				System::Windows::Forms::Cursor::Current = cur_;
			}
		}
	}

}