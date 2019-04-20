#include "stdafx.h"

#include "toH265.h"
#include "helper.h"
#include "TargetCodecDialog.h"

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





		void GetStreamInfo(String^ ffprobe, String^ moviefile, String^% audiocodec, String^% videocodec, TimeSpan% ts)
		{
			audiocodec = videocodec = nullptr;

			if (String::IsNullOrEmpty(ffprobe) || !File::Exists(ffprobe))
			{
				CppUtils::Alert(String::Format(I18N(FormMain::STR_0_NOT_FOUND), L"ffprobe"));
				return;
			}

			String^ arg = String::Format(
				L"-v quiet -print_format json -show_format -show_streams \"{0}\"",
				moviefile);

			int retval;
			String^ output;
			String^ err;
			try
			{
				AmbLib::OpenCommandGetResult(
					ffprobe,
					arg,
					System::Text::Encoding::UTF8,
					retval,
					output,
					err);
			}
			catch (Exception^ ex)
			{
				CppUtils::Alert(ex);
				return;
			}


			// Parse json
			JObject^ joutput = JObject::Parse(output);

			String^ duration = (String^)joutput->SelectToken("format.duration");

			for (int i = 0; i < 32; ++i)
			{
				String^ codec_type = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_type", i));
				if (!codec_type)
					break;

				if (codec_type == "audio")
				{
					audiocodec = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_name", i));
				}
				else if (codec_type == "video")
				{
					videocodec = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_name", i));

					if(String::IsNullOrEmpty(duration))
						duration = (String^)joutput->SelectToken(String::Format("streams[{0}].duration", i));
				}

				if (!String::IsNullOrEmpty(audiocodec) && !String::IsNullOrEmpty(videocodec))
				{
					if (String::IsNullOrEmpty(duration))
						return;

					double d = System::Double::Parse(duration);
					ts = TimeSpan::FromMilliseconds(d * 1000);
					return;
				}
			}
		}

		bool FormMain::CheckMovieAndSet(String^ moviefile)
		{
			WaitCursor wc;
			if (String::IsNullOrEmpty(moviefile))
			{
				txtMovie->Text = String::Empty;
			}
			else
			{
				if (!File::Exists(moviefile))
				{
					CppUtils::Alert(this, String::Format(I18N(STR_0_NOT_FOUND), moviefile));
					ReturnValue = RETURN_FILENOTFOUND;
					return false;
				}

				String^ audiocodec;
				String^ videocodec;
				TimeSpan duration;
				GetStreamInfo(FFProbe, moviefile, audiocodec, videocodec, duration);
				if(String::IsNullOrEmpty(videocodec))
				{
					CppUtils::Alert(this, String::Format(I18N(L"'{0}' does not have a video stream."), moviefile));
					ReturnValue = RETURN_STREAMNOTFOUND;
					return false;
				}
				if (duration.TotalMilliseconds==0)
				{
					CppUtils::Alert(this, String::Format(I18N(L"'{0}' does not have duration."), moviefile));
					ReturnValue = RETURN_DURATIONNOTFOUND;
					return false;
				}
				
				// Check if the encoding is already h265 or vp9
				String^ sformat = I18N(L"'{0}' already has a stream of {1}.");
				if (String::Compare(videocodec, "hevc", true) == 0)
				{
					CppUtils::Alert(this, String::Format(sformat, moviefile, L"h265"));
					ReturnValue = RETURN_STREAMISH265;
					return false;
				}
				if (String::Compare(videocodec, "vp9", true) == 0)
				{
					CppUtils::Alert(this, String::Format(sformat, moviefile, L"vp9"));
					ReturnValue = RETURN_STREAMISVP9;
					return false;
				}
				txtMovie->Text = moviefile;
				DASSERT(FFMpegState == TaskState::None);
				SetStatusText(STATUSTEXT::READY);
				slDuration->Text = duration.ToString("hh\\:mm\\:ss");

				// succeeded, set values
				InputAudioCodec = audiocodec;
				InputVideoCodec = videocodec;

				tsOrigMovie_ = duration;
			}
			return true;
		}
		
		void FormMain::SetCodecStatusText()
		{
			{
				StringBuilder sbAC;
				if (!String::IsNullOrEmpty(InputAudioCodec))
				{
					sbAC.Append(InputAudioCodec);
					if (!String::IsNullOrEmpty(OutputAudioCodec))
					{
						sbAC.Append(" -> ");
						sbAC.Append(OutputAudioCodec);
					}
				}
				if (slAudioCodec->Text != sbAC.ToString())
					slAudioCodec->Text = sbAC.ToString();
			}

			{
				StringBuilder sbVC;
				if (!String::IsNullOrEmpty(InputVideoCodec))
				{
					sbVC.Append(InputVideoCodec);
					if (!String::IsNullOrEmpty(OutputVideoCodec))
					{
						sbVC.Append(" -> ");
						sbVC.Append(OutputVideoCodec);
					}
				}
				if (slVideoCodec->Text != sbVC.ToString())
					slVideoCodec->Text = sbVC.ToString();
			}
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
			notifyIconMain->Text = Application::ProductName;
			Text = Application::ProductName;
		}
		String^ FormMain::buildTitleText(int percent, bool bFilenameOnly)
		{
			StringBuilder sbTitle;
			sbTitle.Append(percent);
			sbTitle.Append("% - ");

			if (!String::IsNullOrEmpty(outputMovie_))
			{
				if (bFilenameOnly)
					sbTitle.Append(Path::GetFileName(outputMovie_));
				else
					sbTitle.Append(outputMovie_);
				sbTitle.Append(" - ");
			}

			sbTitle.Append(Application::ProductName);

			return sbTitle.ToString();
		}
		void FormMain::UpdateTitle(int percent)
		{
			String^ titleToSet = buildTitleText(percent, false);
			if (Text != titleToSet)
				Text = titleToSet;

			if (notifyIconMain->Visible)
			{
				// notifyIconMain->Text must be less than 64
				if (titleToSet->Length >= 64)
					titleToSet = buildTitleText(percent, true);
				if (titleToSet->Length >= 64)
					titleToSet = titleToSet->Substring(0, 63);

				if (notifyIconMain->Text != titleToSet)
					notifyIconMain->Text = titleToSet;
			}
		}
		void FormMain::UpdateTitleComplete()
		{
			UpdateTitle(100);
		}
		void FormMain::UpdateTitleTS(TimeSpan tsProgress, double speed)
		{
			double percent = (tsProgress.TotalMilliseconds / tsOrigMovie_.TotalMilliseconds) * 100;
			UpdateTitle((int)percent);

			DTRACE("All:" + tsOrigMovie_.ToString() + " " + tsOrigMovie_.TotalMilliseconds);
			DTRACE("Cur:" + tsProgress.ToString() + " " + tsProgress.TotalMilliseconds);
			double mRemaining = tsOrigMovie_.TotalMilliseconds - tsProgress.TotalMilliseconds;
			DTRACE("Remain:" + mRemaining);

			if (speed == 0)
				speed = 0.00001;
			
			TimeSpan ts = TimeSpan::FromMilliseconds(mRemaining / speed);

			String^ stText;
			if (ts.Days != 0)
				stText = ts.ToString(I18N(L"d'd 'h'h 'm'm'"));
			else
				stText = ts.ToString(I18N(L"h'h 'm'm'"));
			
			SetStatusText(STATUSTEXT::REMAINING, stText);
		}
		
		void FormMain::SetStatusText(STATUSTEXT ss)
		{
			DASSERT(ss == STATUSTEXT::READY);
			SetStatusText(ss, String::Empty);
		}
		void FormMain::SetStatusText(STATUSTEXT ss, String^ supplement)
		{
			String^ text;
			switch (ss)
			{
			case STATUSTEXT::READY:
				text = I18N(L"Ready");
				break;
			case STATUSTEXT::REMAINING:
				text = String::Format(I18N(L"{0} left"), supplement);
				break;
			}
			if (slMain->Text != text)
				slMain->Text = text;
		}

		void FormMain::AddToErr(String^ text)
		{
/*
frame=   61 fps= 18 q=-0.0 size=       0kB time=00:00:02.08 bitrate=   0.2kbits/s dup=1 drop=0 speed=0.619x
frame=   69 fps= 17 q=-0.0 size=       0kB time=00:00:02.34 bitrate=   0.2kbits/s dup=1 drop=0 speed=0.591x
frame=   76 fps= 17 q=-0.0 size=       0kB time=00:00:02.57 bitrate=   0.1kbits/s dup=1 drop=0 speed=0.566x
frame=   85 fps= 17 q=-0.0 size=       0kB time=00:00:02.87 bitrate=   0.1kbits/s dup=1 drop=0 speed=0.566x
*/
			DASSERT(regFFMpeg_);
			if (regFFMpeg_->IsMatch(text))
			{
				RegularExpressions::Match^ match = regFFMpeg_->Match(text);
				String^ timeValue = match->Groups[1]->Value;

				DateTime dtTime = DateTime::ParseExact(timeValue, L"hh:mm:ss",
					System::Globalization::CultureInfo::InvariantCulture);
				TimeSpan tsTime = dtTime - dtTime.Date;

				String^ speedValue = match->Groups[2]->Value;
				double dblSpeed;
				double::TryParse(speedValue, dblSpeed);

				UpdateTitleTS(tsTime, dblSpeed);
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

		void FormMain::ChangeStartButtonText(String^ text)
		{
			btnStart->Text = text;
			tsmiNotifyStart->Text = text;
		}
		void FormMain::ThreadStarted()
		{
			processSuspeded_ = false;
			processTerminated_ = false;

			ChangeStartButtonText(I18N(STR_BUTTONTEXT_PAUSE));

			txtMovie->Enabled = false;
			txtMovie->AllowDrop = false;
			btnBrowseMovie->Enabled = false;

			this->Icon = iconRed_;
			notifyIconMain->Icon = iconRed_;

			dwBackPriority_ = GetPriorityClass(GetCurrentProcess());
			if (tsmiPriorityBackground->Checked)
				SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);

		}
		void FormMain::ThreadEnded(int retval)
		{
			pidFFMpeg_ = 0;
			processSuspeded_ = false;
			SafeJoin(thFFMpeg_);
			delete thFFMpeg_;
			thFFMpeg_ = nullptr;

			processFFMpeg_ = nullptr;

			ChangeStartButtonText(I18N(STR_BUTTONTEXT_START));

			txtMovie->Enabled = true;
			txtMovie->AllowDrop = true;
			btnBrowseMovie->Enabled = true;

			this->Icon = iconBlue_;
			notifyIconMain->Icon = iconBlue_;

			SetStatusText(STATUSTEXT::READY);

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

					String^ ac = String::Empty;
					String^ vc = String::Empty;
					TimeSpan ts;
					GetStreamInfo(FFProbe, outputMovie_, ac, vc, ts);
					DASSERT(!String::IsNullOrEmpty(ac));
					DASSERT(!String::IsNullOrEmpty(vc));
					DASSERT(ts.TotalMilliseconds != 0);

					StringBuilder sbMessage;
					sbMessage.AppendLine(I18N(L"Encoding Succeeded."));
					sbMessage.AppendLine();
					sbMessage.AppendLine(String::Format(I18N(L"Audio codec = {0}"), ac));
					sbMessage.AppendLine(String::Format(I18N(L"Video codec = {0}"), vc));
					sbMessage.AppendLine(String::Format(I18N(L"Duration = {0}"), ts.ToString()));
					CppUtils::Info(this, sbMessage.ToString());

					// Show outputmovie in Explorer
					CppUtils::OpenFolder(this, outputMovie_);
				}
			}
			OutputAudioCodec = String::Empty;
			OutputVideoCodec = String::Empty;
			outputMovie_ = String::Empty;
		}

		void FormMain::OnProcessStarted(Object^ sender, EventArgs^ e)
		{
			if (InvokeRequired)
			{
				this->BeginInvoke(gcnew EventHandler(this, &FormMain::OnProcessStarted), sender, e);
				return;
			}

			DASSERT_IS_CLASS_OR_NULL(sender, Process);
			if (!sender)
				pidFFMpeg_ = 0;
			else
				pidFFMpeg_ = ((Process^)sender)->Id;

			if (tsmiPriorityBackground->Checked)
			{
				SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_END);
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
					Encoding::UTF8,
					retval,
					gcnew DataReceivedEventHandler(this, &FormMain::outputHandler),
					gcnew DataReceivedEventHandler(this, &FormMain::errHandler),
					gcnew EventHandler(this, &FormMain::OnProcessStarted),
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
				ChangeStartButtonText(I18N(STR_BUTTONTEXT_PAUSE));
				processSuspeded_ = false;
				this->Icon = iconRed_;
				notifyIconMain->Icon = iconRed_;
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
				ChangeStartButtonText(I18N(STR_BUTTONTEXT_RESUME));
				processSuspeded_ = true;
				this->Icon = iconYellow_;
				notifyIconMain->Icon = iconYellow_;
				return;
			case TaskState::Unknown:
				CppUtils::Alert(this, I18N(L"Unknow Error."));
				return;
			}
			String^ ffmpeg = FFMpeg;
			if (String::IsNullOrEmpty(ffmpeg) || !File::Exists(ffmpeg))
			{
				CppUtils::Alert(this, String::Format(I18N(STR_0_NOT_FOUND), L"ffmpeg"));
				return;
			}

			String^ inputmovie = txtMovie->Text;
			if (!CheckMovieAndSet(inputmovie))
				return;

			if (String::IsNullOrEmpty(inputmovie))
			{
				CppUtils::Alert(this, I18N(L"input movie is empty."));
				return;
			}


			String^ outFileNameSuffix = String::Empty;
			String^ outExt = String::Empty;
			{
				TargetCodecDialog codecDlg;
				if (InputVideoCodec == "h264")
				{
					codecDlg.rbH265->Checked = true;

					if (InputAudioCodec == "aac" ||
						InputAudioCodec == "vorbis")
					{
						codecDlg.rbCopyAudio->Checked = true;
					}
				}
				else if (InputVideoCodec == "vp8")
				{
					codecDlg.rbVp9->Checked = true;

					if (InputAudioCodec == "opus")
					{
						codecDlg.rbCopyAudio->Checked = true;
					}
				}
				
				if (System::Windows::Forms::DialogResult::OK != codecDlg.ShowDialog())
					return;

				if (codecDlg.rbH265->Checked)
				{
					OutputVideoCodec = "libx265";
					outFileNameSuffix = "hevc";
					outExt = ".mp4";
				}
				else if (codecDlg.rbVp9->Checked)
				{
					OutputVideoCodec = "vp9";
					outFileNameSuffix = "vp9";
					outExt = ".webm";
				}
				else
				{
					CppUtils::Alert(this, I18N(L"No video codec selected."));
					return;
				}

				if (codecDlg.rbCopyAudio->Checked)
					OutputAudioCodec = "copy";
				else if (codecDlg.rbAac->Checked)
					OutputAudioCodec = "aac";
				else if (codecDlg.rbOpus->Checked)
					OutputAudioCodec = "libopus";
				else
				{
					CppUtils::Alert(this, I18N(L"No audio codec selected."));
					return;
				}
			}

			SaveFileDialog dlg;
			
			List<String^> availableext;
			{
				// libx265 must has mp4 foramt
				availableext.Add(Path::GetExtension(outExt));

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
			}

			dlg.InitialDirectory = Path::GetDirectoryName(inputmovie);
			{
				String^ name = Path::GetFileNameWithoutExtension(inputmovie);
				dlg.FileName = String::Format(L"{0} [{1}]{2}",
					name, 
					outFileNameSuffix, 
					outExt);
			}
			
			if (System::Windows::Forms::DialogResult::OK != dlg.ShowDialog())
				return;

			outputMovie_ = dlg.FileName;

			String^ arg = String::Format(L"-y -i \"{0}\" -c:v {1} -c:a {2} \"{3}\"",
				inputmovie, 
				OutputVideoCodec,
				OutputAudioCodec,
				outputMovie_);

			txtLogOut->Clear();
			txtLogErr->Clear();

			txtFFMpegArg->Text = arg;

			Dictionary<String^, String^> param;
			param["ffmpeg"] = ffmpeg;
			param["arg"] = arg;
			thFFMpeg_ = gcnew System::Threading::Thread(gcnew ParameterizedThreadStart(this, &FormMain::StartOfThread));
			thFFMpeg_->Start(%param);
		}

		System::Void FormMain::tsmiNotifyShow_Click(System::Object^  sender, System::EventArgs^  e)
		{
			Show();
			this->WindowState = FormWindowState::Normal;
			notifyIconMain->Visible = false;
		}
		System::Void FormMain::tsmiNotifyStart_Click(System::Object^  sender, System::EventArgs^  e)
		{
			btnStart_Click(sender, e);
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

				ChangeStartButtonText(I18N(STR_BUTTONTEXT_RESUME));
				processSuspeded_ = true;
				return;
			case TaskState::Unknown:
				return;
			}

			return;
		}

		
		System::Void FormMain::tsmiOption_DropDownOpening(System::Object^ sender, System::EventArgs^ e)
		{
			if (!String::IsNullOrEmpty(PeekFFProbe()))
				tsmiSetFFProbe->Text = baseSetFFProbeMenuString_ + L" (" + PeekFFProbe() + L")";
			else
				tsmiSetFFProbe->Text = baseSetFFProbeMenuString_;


			if (!String::IsNullOrEmpty(PeekFFMpeg()))
				tsmiSetFFMpeg->Text = baseSetFFMpegMenuString_ + L" (" + PeekFFMpeg() + L")";
			else
				tsmiSetFFMpeg->Text = baseSetFFMpegMenuString_;
		}

		System::Void FormMain::tsmiSetFFProbe_Click(System::Object^  sender, System::EventArgs^  e)
		{
			ffprobe_ = nullptr;
			getCommon(this, false, SECTION_OPTION, KEY_FFPROBE, Program::IniFile, ffprobe_, true);
		}
		System::Void FormMain::tsmiSetFFMpeg_Click(System::Object^  sender, System::EventArgs^  e)
		{
			ffmpeg_ = nullptr;
			getCommon(this, true, SECTION_OPTION, KEY_FFMPEG, Program::IniFile, ffmpeg_, true);
		}



		
		

		System::Void FormMain::tsmiStop_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if (!ConfirmEncodeStop())
				return;

			StopEncoding();
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