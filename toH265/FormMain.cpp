#include "stdafx.h"

#include "../../lsMisc/IsFileOpen.h"
#include "../../lsMisc/getStdString.net.h"
#include "../../lsMisc/OpenCommon.h"

#include "toH265.h"
#include "helper.h"
#include "TargetCodecDialog.h"

#include "FormMain.h"

#pragma comment(lib, "shell32.lib")

namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Drawing;
		using namespace System::Collections::Generic;
		using namespace System::Diagnostics;
		using namespace System::IO;
		using namespace System::Text;
		using namespace System::Threading;

		using namespace Newtonsoft::Json::Linq;





		void GetStreamInfo(
			String^ ffprobe,
			String^ moviefile,
			String^% format,
			String^% audiocodec,
			String^% videocodec,
			System::Drawing::Size% aspect,
			TimeSpan% ts)
		{
			WaitCursor wc;

			audiocodec = videocodec = nullptr;

			if (String::IsNullOrEmpty(ffprobe) || !File::Exists(ffprobe))
			{
				throw gcnew Exception(String::Format(I18N(FormMain::STR_0_NOT_FOUND), L"ffprobe"));
			}

			String^ arg = String::Format(
				L"-v quiet -print_format json -show_format -show_streams \"{0}\"",
				moviefile);

			int retval;
			String^ output;
			String^ err;
			AmbLib::OpenCommandGetResult(
				ffprobe,
				arg,
				System::Text::Encoding::UTF8,
				retval,
				output,
				err);


			// Parse json
			JObject^ joutput = JObject::Parse(output);
			String^ myDuration = (String^)joutput->SelectToken("format.duration");
			format = (String^)joutput->SelectToken("format.format_name");
			int nVideos = 0;
			int nAudios = 0;
			try
			{
				for (int i = 0; i < 32; ++i)
				{
					String^ codec_type = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_type", i));
					if (!codec_type)
						break;

					if (codec_type == "audio")
					{
						++nAudios;
						audiocodec = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_name", i));
					}
					else if (codec_type == "video")
					{
						++nVideos;
						videocodec = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_name", i));

						if (String::IsNullOrEmpty(myDuration))
							myDuration = (String^)joutput->SelectToken(String::Format("streams[{0}].duration", i));

						int width = (int)joutput->SelectToken(String::Format("streams[{0}].width", i));
						int height = (int)joutput->SelectToken(String::Format("streams[{0}].height", i));
						aspect = System::Drawing::Size(width, height);
					}

					if (!String::IsNullOrEmpty(audiocodec) && !String::IsNullOrEmpty(videocodec))
					{
						if (nAudios > 1 || nVideos > 1)
						{
							audiocodec = videocodec = String::Empty;
							throw gcnew Exception(I18N("Could not handle more than 1 audio or video streams."));
							return;
						}
					}
				}
			}
			catch (Exception ^ ex)
			{
				throw ex;
			}
			finally
			{
				if (!String::IsNullOrEmpty(myDuration))
				{
					double d = System::Double::Parse(myDuration);
					ts = TimeSpan::FromMilliseconds(d * 1000);
				}
			}
		}
		void FormMain::InsertMovieItem(
			String^ movieFile,
			LONGLONG size,
			System::Drawing::Size aspect,
			String^ format,
			AVCodec^ acodec,
			AVCodec^ vcodec,
			AVDuration^ duration)
		{
			ListViewItem^ lvi = gcnew ListViewItem();

			for each (ColumnItem ^ ci in ColumnItems)
			{
				if (ci->Key == "main")
					continue;
				ListViewItem::ListViewSubItem^ newSubItem;
				bool toAdd = false;
				if (lvi->SubItems[ci->Key] == nullptr)
				{
					newSubItem = gcnew ListViewItem::ListViewSubItem();
					toAdd = true;
				}
				else
				{
					newSubItem = lvi->SubItems[ci->Key];
				}
				newSubItem->Name = ci->Key;
				newSubItem->Text = ci->Text;
				if (toAdd)
				{
					lvi->SubItems->Add(newSubItem);
					DASSERT(lvi->SubItems[ci->Key]);
				}
			}

			lvi->SubItems["directory"]->Text = Path::GetDirectoryName(movieFile);
			lvi->SubItems["filename"]->Text = Path::GetFileName(movieFile);

			lvi->SubItems["size"]->Text = AmbLib::FormatSize(size);

			lvi->SubItems["aspect"]->Text = sToString(aspect);
			lvi->SubItems["aspect"]->Tag = aspect;

			lvi->SubItems["format"]->Text = Ambiesoft::toH265Helper::human_format(format->ToString());

			lvi->SubItems["vcodec"]->Text = vcodec->ToString();

			lvi->SubItems["acodec"]->Text = acodec->ToString();

			lvi->SubItems["duration"]->Text = duration->ToString();

			lvInputs->Items->Add(lvi);
		}
		bool FormMain::CheckMovieAndSet(String^ moviefile, bool bSet)
		{
			WaitCursor wc;
			if (String::IsNullOrEmpty(moviefile))
			{
				return false;
			}
			else
			{
				if (!File::Exists(moviefile))
				{
					CppUtils::Alert(this, String::Format(I18N(STR_0_NOT_FOUND), moviefile));
					ReturnValue = RETURN_FILENOTFOUND;
					return false;
				}

				LONGLONG size;
				String^ audiocodec;
				String^ videocodec;
				String^ format;
				System::Drawing::Size aspect;
				TimeSpan duration;

				try
				{
					size = FileInfo(moviefile).Length;
				}
				catch (Exception ^ ex)
				{
					CppUtils::Alert(this, ex->Message);
					return false;
				}

				try
				{
					GetStreamInfo(FFProbe, moviefile, format, audiocodec, videocodec, aspect, duration);
				}
				catch (Exception ^ ex)
				{
					CppUtils::Alert(this, ex->Message);
					return false;
				}

				if (String::IsNullOrEmpty(videocodec))
				{
					CppUtils::Alert(this, String::Format(I18N(L"'{0}' does not have a video stream."), moviefile));
					ReturnValue = RETURN_STREAMNOTFOUND;
					return false;
				}
				if (duration.TotalMilliseconds == 0)
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

				if (bSet)
				{
					InsertMovieItem(moviefile,
						size,
						aspect,
						format,
						gcnew AVCodec(audiocodec), gcnew AVCodec(videocodec),
						gcnew AVDuration(duration));
				}

				DASSERT(FFMpegState == TaskState::None);
				SetStatusText(STATUSTEXT::READY);

				InputDuration = gcnew AVDuration();
				AVCodec^ inputAudioCodec = gcnew AVCodec();
				AVCodec^ inputVideoCodec = gcnew AVCodec();
				String^ tryFormat;
				double mDuration = 0;
				for each(ListViewItem^ lvi in lvInputs->Items)
				{
					if (String::IsNullOrEmpty(tryFormat))
					{
						tryFormat = lvi->SubItems["format"]->Text;
					}
					else if(tryFormat=="mixed")
					{
						
					}
					else
					{
						// not mixed
						if (tryFormat != lvi->SubItems["format"]->Text)
							tryFormat = "mixed";
					}
					// succeeded, set values
					inputAudioCodec->Merge(gcnew AVCodec(lvi->SubItems["acodec"]->Text));
					inputVideoCodec->Merge(gcnew AVCodec(lvi->SubItems["vcodec"]->Text));

					TimeSpan ts = TimeSpan::Parse(lvi->SubItems["duration"]->Text);
					mDuration += ts.TotalMilliseconds;
				}
				TimeSpan all(10 * 1000 * (long long)mDuration);
				// InputDuration = all.ToString("hh\\:mm\\:ss");
				InputDuration = gcnew AVDuration(all);
				InputFormat = tryFormat;
				InputAudioCodec = inputAudioCodec;
				InputVideoCodec = inputVideoCodec;
				// tsOrigMovies_ = all;
			}
			return true;
		}

		void FormMain::SetFormatStatusText()
		{
			slFormat->Text = Ambiesoft::toH265Helper::human_format(inputFormat_);
		}

		void FormMain::SetCodecStatusText()
		{
			{
				StringBuilder sbAC;
				if (!InputAudioCodec->IsEmpty)
				{
					sbAC.Append(InputAudioCodec);
					if (!OutputAudioCodec->IsEmpty)
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
				if (!InputVideoCodec->IsEmpty)
				{
					sbVC.Append(InputVideoCodec->ToString());
					if (!OutputVideoCodec->IsEmpty)
					{
						sbVC.Append(" -> ");
						sbVC.Append(OutputVideoCodec);
					}
				}
				if (slVideoCodec->Text != sbVC.ToString())
					slVideoCodec->Text = sbVC.ToString();
			}
		}

		void FormMain::SetTimeStatusText()
		{
			StringBuilder sb;
			if (!InputDuration->IsEmpty())
			{
				sb.Append(InputDuration);
				if (!OutputDuration->IsEmpty())
				{
					sb.Append(" -> ");
					sb.Append(OutputDuration);
				}
			}
			else
			{
				sb.Append(InputDuration);
			}
			if (slDuration->Text != sb.ToString())
				slDuration->Text = sb.ToString();
		}

		System::Void FormMain::btnBrowseMovie_Click(System::Object^  sender, System::EventArgs^  e)
		{
			OpenFileDialog dlg;
			dlg.Multiselect = true;
			if (System::Windows::Forms::DialogResult::OK != dlg.ShowDialog())
				return;

			for each(String^ file in dlg.FileNames)
				CheckMovieAndSet(file, true);
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

			if (!String::IsNullOrEmpty(outputtingMovie_))
			{
				if (bFilenameOnly)
					sbTitle.Append(Path::GetFileName(outputtingMovie_));
				else
					sbTitle.Append(outputtingMovie_);
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

		String^ FormMain::tsToString(TimeSpan ts)
		{
			if (ts.Days != 0)
				return ts.ToString(I18N(L"d'd 'h'h 'm'm'"));

			return ts.ToString(I18N(L"h'h 'm'm'"));
		}
		String^ FormMain::GetRemainingTimeText(ElapseInfo^ firstElapse, ElapseInfo^ lastElapse, double total)
		{
			do
			{
				DASSERT(lastElapse);
				if (!elapses_.Filled)
					break;
				if (!firstElapse)
				{
					firstElapse = elapses_.Peek();
				}

				DASSERT(firstElapse->Progress <= lastElapse->Progress);
				DASSERT(firstElapse->TimeStamp <= lastElapse->TimeStamp);

				double progress = (lastElapse->Progress - firstElapse->Progress) / 1000.0;
				if (!progress)
					break;
				double interval = (lastElapse->TimeStamp - firstElapse->TimeStamp) / (1000.0);

				try
				{

					double progressPerSec = progress / interval;

					double remainSec = total / 1000.0 - lastElapse->Progress / 1000.0;

					double result = remainSec / progressPerSec;

					return tsToString(TimeSpan::FromSeconds(result));
				}
				catch (Exception^)
				{
				}
			} while (false);
			return I18N("Unknown");
		}
		void FormMain::UpdateTitleTS(TimeSpan tsProgress, double speed)
		{
			// for culculating eta
			ElapseInfo^ lastElapse = gcnew ElapseInfo(tsProgress.TotalMilliseconds);
			ElapseInfo^ firstElapse = elapses_.Enqueue(lastElapse);

			double percent = (tsProgress.TotalMilliseconds / InputDuration->TotalMilliseconds) * 100;
			UpdateTitle((int)percent);
			if (this->WindowState == FormWindowState::Minimized)
				return;

			//DTRACE("All:" + InputDuration->ToString() + " " + InputDuration->TotalMilliseconds);
			//DTRACE("Cur:" + tsProgress.ToString() + " " + tsProgress.TotalMilliseconds);
			//double mRemaining = InputDuration->TotalMilliseconds - tsProgress.TotalMilliseconds;
			//DTRACE("Remain:" + mRemaining);

			//if (speed == 0)
			//	speed = 0.00001;

			//TimeSpan ts = TimeSpan::FromMilliseconds(mRemaining / speed);

			//String^ stRemainingText = tsToString(ts);
			//SetStatusText(STATUSTEXT::REMAINING, stRemainingText);
			SetStatusText(STATUSTEXT::REMAINING, 
				GetRemainingTimeText(firstElapse, lastElapse, InputDuration->TotalMilliseconds));

			// String^ stElapsedText = tsProgress.ToString("hh\\:mm\\:ss");
			OutputDuration = gcnew AVDuration(tsProgress);
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

		//bool FormMain::GetInfoFromFFMpegoutput(String^ text, TimeSpan% tsTime, double% dblSpeed)
		//{
		//	if (regFFMpeg_->IsMatch(text))
		//	{
		//		RegularExpressions::Match^ match = regFFMpeg_->Match(text);
		//		String^ timeValue = match->Groups["time"]->Value;
		//		if (!String::IsNullOrEmpty(timeValue) && timeValue[0] == '-')
		//			timeValue = "00:00:00.00";
		//		
		//		//DateTime dtTime = DateTime::ParseExact(timeValue, L"hh:mm:ss.ff",
		//		//	System::Globalization::CultureInfo::InvariantCulture);
		//		//tsTime = dtTime - dtTime.Date;
		//		
		//		tsTime = TimeSpan::Parse(timeValue);

		//		String ^ speedValue = match->Groups["speed"]->Value;
		//		if (speedValue == "N/A")
		//		{
		//			dblSpeed = 0;
		//		}
		//		else
		//		{
		//			speedValue = speedValue->Trim()->TrimEnd(char1x);
		//			if (!double::TryParse(speedValue, dblSpeed))
		//				return false;
		//		}
		//		//UpdateTitleTS(tsTime, dblSpeed);
		//		//txtLogOut->Text = text;
		//		return true;
		//	}
		//	return false;
		//}

		void FormMain::AddToErr(String^ text)
		{
			try
			{
				// DASSERT(regFFMpeg_);
				TimeSpan tsTime;
				double dblSpeed;
				if (FFMpegHelper::GetInfoFromFFMpegoutput(text, tsTime, dblSpeed))
				{
					UpdateTitleTS(tsTime, dblSpeed);
					if (this->WindowState != FormWindowState::Minimized)
					{
						txtLogOut->Text = text;
					}
				}
				else
				{
					txtLogErr->AppendText(text);
					txtLogErr->AppendText(L"\r\n");
				}
			}
			catch (Exception ^ ex)
			{
				txtLogErr->AppendText(ex->Message);
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

			lvInputs->Enabled = false;
			lvInputs->AllowDrop = false;
			btnBrowseMovie->Enabled = false;

			this->Icon = iconRed_;
			notifyIconMain->Icon = iconRed_;

			dwBackPriority_ = GetPriorityClass(GetCurrentProcess());
			if (tsmiPriorityBackground->Checked)
				SetPriorityClass(GetCurrentProcess(), PROCESS_MODE_BACKGROUND_BEGIN);

			elapses_.Clear();
		}
		void FormMain::ThreadEnded(int retval)
		{
			if (!String::IsNullOrEmpty(tempFile_))
			{
				try
				{
					File::Delete(tempFile_);
				}
				catch (Exception^)
				{
				}
			}
			pidFFMpeg_ = 0;
			processSuspeded_ = false;
			SafeJoin(thFFMpeg_);
			delete thFFMpeg_;
			thFFMpeg_ = nullptr;

			processFFMpeg_ = nullptr;

			ChangeStartButtonText(I18N(STR_BUTTONTEXT_START));

			lvInputs->Enabled = true;
			lvInputs->AllowDrop = true;
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

					if (tsmiEnabledtsmiProcessAfterFinish->Checked)
					{
						HashIni^ ini = Profile::ReadAll(Program::IniFile);
						DVERIFY(dlgAfterFinish_.LoadValues("AfterFinish", ini));

						if (dlgAfterFinish_.chkOpenFolder->Checked)
						{
							// Show outputmovie in Explorer
							CppUtils::OpenFolder(this, outputtingMovie_);
						}

						if (dlgAfterFinish_.chkLaunchApp->Checked)
						{
							Process::Start(dlgAfterFinish_.txtApp->Text, dlgAfterFinish_.txtArg->Text);
						}

						if (dlgAfterFinish_.chkShutdown->Checked)
						{
							Process::Start(Application::ExecutablePath, "-shutdown");
						}
					}
					else  // no process after finish
					{
						String^ outputtedFormat;
						String^ outputtedAC = String::Empty;
						String^ outputtedVC = String::Empty;
						System::Drawing::Size outputtedAspect;
						TimeSpan outputtedTS;
						LONGLONG inputSize;
						LONGLONG outputtedSize;

						try
						{
							GetStreamInfo(FFProbe, outputtingMovie_, outputtedFormat, outputtedAC, outputtedVC, outputtedAspect, outputtedTS);
							for each (String ^ infile in GetInputMovies())
								inputSize += FileInfo(infile).Length;
							outputtedSize = FileInfo(outputtingMovie_).Length;
						}
						catch (Exception^ ex)
						{
							CppUtils::Alert(this, ex);
						}
						DASSERT(!String::IsNullOrEmpty(outputtedAC));
						DASSERT(!String::IsNullOrEmpty(outputtedVC));
						DASSERT(outputtedTS.TotalMilliseconds != 0);

						StringBuilder sbMessage;
						sbMessage.AppendLine(I18N(L"Encoding Succeeded."));
						sbMessage.AppendLine();
						sbMessage.AppendLine(String::Format(I18N(L"Format = {0}"), outputtedFormat));
						sbMessage.AppendLine(String::Format(I18N(L"Audio codec = {0}"), outputtedAC));
						sbMessage.AppendLine(String::Format(I18N(L"Video codec = {0}"), outputtedVC));
						sbMessage.AppendLine(String::Format(I18N(L"Duration = {0}"), outputtedTS.ToString()));
						sbMessage.AppendLine(String::Format(I18N(L"Size = {0}"), AmbLib::FormatSize(outputtedSize)));
						sbMessage.AppendLine(String::Format(I18N(L"Compressed = {0}%"), AmbLib::GetRatioString(outputtedSize, inputSize)));
						CppUtils::Info(this, sbMessage.ToString());
					}
				}
			}
			OutputAudioCodec = gcnew AVCodec();
			OutputVideoCodec = gcnew AVCodec();
			OutputDuration = gcnew AVDuration();
			outputtedMovie_ = outputtingMovie_;
			outputtingMovie_ = String::Empty;
			
			elapses_.Clear();
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
		String^ FormMain::GetMovieFileFromLvi(ListViewItem^ lvi)
		{
			return Path::Combine(lvi->SubItems["directory"]->Text, lvi->SubItems["filename"]->Text);
		}
		Size FormMain::GetVideoSize(ListViewItem^ lvi)
		{
			DASSERT(lvi->SubItems["aspect"]->Tag);
			return (System::Drawing::Size)(lvi->SubItems["aspect"]->Tag);
		}
		double FormMain::GetVideoArea(ListViewItem^ lvi)
		{
			System::Drawing::Size size = GetVideoSize(lvi);
			return size.Width * size.Height;
		}
		Size FormMain::GetMaxVideoSize()
		{
			double maxWidth = 0;
			ListViewItem^ lviMax;
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				const double area = GetVideoArea(lvi);
				if (maxWidth < area)
				{
					maxWidth = area;
					lviMax = lvi;
				}
			}
			DASSERT(lviMax);
			return GetVideoSize(lviMax);
		}
		bool FormMain::IsSameSizeVideos()
		{
			System::Drawing::Size size;
			bool first = false;
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				if (!first)
				{
					size = GetVideoSize(lvi);
					first = true;
					continue;
				}
				if (size != GetVideoSize(lvi))
					return false;
			}
			return true;
		}

		List<String^>^ MakeUnique(List<String^>^ inList)
		{
			List<String^>^ result = gcnew List<String^>();
			for each (String ^ s in inList)
			{
				if (result->Contains(s))
					continue;
				result->Add(s);
			}
			return result;
		}
		array<String^>^ FormMain::GetInputMovies()
		{
			List<String^> inputmovies;
			// add files 
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				String^ inputmovie = GetMovieFileFromLvi(lvi);
				inputmovies.Add(inputmovie);
			}
			return inputmovies.ToArray();
		}
		System::Void FormMain::btnStart_Click(System::Object^ sender, System::EventArgs^ e)
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
				elapses_.Clear();
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

			if (lvInputs->Items->Count == 0)
			{
				CppUtils::Alert(this, I18N("No items"));
				return;
			}



			List<String^>^ outExtsNormalPriority = gcnew List<String^>();
			List<String^>^ outExtsHighPriority = gcnew List<String^>();
			String^ initialDir;
			String^ baseFileName = "Encoded";
			

			// check input
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				String^ inputmovie = GetMovieFileFromLvi(lvi);

				if (String::IsNullOrEmpty(inputmovie))
				{
					CppUtils::Alert(this, I18N(L"input movie is empty."));
					return;
				}
				if (!CheckMovieAndSet(inputmovie, false))
					return;
			}

			List<String^> inputmovies = GetInputMovies();

			// set basename
			baseFileName = Ambiesoft::toH265Helper::GetCommonFilename(%inputmovies);
			if (String::IsNullOrEmpty(baseFileName))
				baseFileName = "output";

			if (String::IsNullOrEmpty(initialDir))
				initialDir = Path::GetDirectoryName(inputmovies[0]);



			bool isLosslessable = inputmovies.Count != 0 &&
				InputFormat != "mixed" &&
				!InputAudioCodec->IsMixed && !InputVideoCodec->IsMixed;
			TargetCodecDialog codecDlg(isLosslessable);

			if (InputVideoCodec->IsH264)
			{
				codecDlg.rbVideoH265->Checked = true;

				//if (InputAudioCodec == "aac" ||
				//	InputAudioCodec == "vorbis")
				{
					codecDlg.rbAudioCopy->Checked = true;
				}
			}
			else if (InputVideoCodec->IsVp8)
			{
				codecDlg.rbVideoVp9->Checked = true;

				// if (InputAudioCodec == "opus")
				{
					codecDlg.rbAudioCopy->Checked = true;
				}
			}


			if (InputAudioCodec->IsMixed)
			{
				codecDlg.rbAudioCopy->Checked = false;
			}

			// show the dialog
			if (System::Windows::Forms::DialogResult::OK != codecDlg.ShowDialog())
				return;

			bool bReEncode = codecDlg.IsReEncode;
			if(!bReEncode)
			{
				outExtsNormalPriority->Add(Path::GetExtension(inputmovies[0]));
				OutputAudioCodec = gcnew AVCodec(AVCodec::VC::VC_COPY);
				OutputVideoCodec = gcnew AVCodec(AVCodec::VC::VC_COPY);
			}
			else
			{
				// check if both codecs are 'copy'
				if (codecDlg.rbVideoCopy->Checked && codecDlg.rbAudioCopy->Checked)
				{
					if (System::Windows::Forms::DialogResult::Yes != CppUtils::CenteredMessageBox(
						this,
						I18N("Both audio and video codes are 'copy'. Are you sure to continue?"),
						Application::ProductName,
						MessageBoxButtons::YesNo,
						MessageBoxIcon::Question,
						MessageBoxDefaultButton::Button2))
					{
						return;
					}
				}
				if (codecDlg.rbVideoH265->Checked)
				{
					OutputVideoCodec = gcnew AVCodec(AVCodec::VC::VC_H265);
					outExtsNormalPriority->Add(".mp4");
				}
				else if (codecDlg.rbVideoVp9->Checked)
				{
					OutputVideoCodec = gcnew AVCodec("vp9");
					outExtsNormalPriority->Add(".webm");
				}
				else if (codecDlg.rbVideoCopy->Checked)
				{
					if (InputVideoCodec->IsH264)
					{
						if (codecDlg.rbAudioOpus->Checked)
							outExtsNormalPriority->Add(".mkv");
						else
							outExtsNormalPriority->Add(".mp4");
					}
					OutputVideoCodec = gcnew AVCodec(AVCodec::VC::VC_COPY);
					outExtsNormalPriority->Add(Path::GetExtension(inputmovies[0]));
				}
				else if (codecDlg.rbAV1->Checked)
				{
					OutputVideoCodec = gcnew AVCodec("av1");
					outExtsNormalPriority->Add(".mkv");
				}
				else
				{
					CppUtils::Alert(this, I18N(L"No video codec selected."));
					return;
				}

				if (codecDlg.rbAudioCopy->Checked)
					OutputAudioCodec = gcnew AVCodec(AVCodec::VC::VC_COPY);
				else if (codecDlg.rbAudioAac->Checked)
					OutputAudioCodec = gcnew AVCodec(AVCodec::VC::VC_AAC);
				else if (codecDlg.rbAudioOpus->Checked)
					OutputAudioCodec = gcnew AVCodec(AVCodec::VC::VC_OPUS);
				else
				{
					CppUtils::Alert(this, I18N(L"No audio codec selected."));
					return;
				}


				AVCodec^ targetVideoCodec = OutputVideoCodec->IsCopy ? InputVideoCodec : OutputVideoCodec;
				AVCodec^ targetAudioCodec = OutputAudioCodec->IsCopy ? InputAudioCodec : OutputAudioCodec;

				// vp9 can only hold audio of "vorbis" or "opus"
				if (OutputVideoCodec->IsVp9)
				{
					if (!targetAudioCodec->IsVorbis && !targetAudioCodec->IsOpus)
					{
						outExtsNormalPriority->Add(".mkv");
					}
				}
				if (outExtsNormalPriority->Contains(".mp4"))
				{
					if (targetVideoCodec->IsH265 && targetAudioCodec->IsOpus)
					{
						outExtsHighPriority->Add(".mkv");
					}
				}

				outExtsNormalPriority->Add(".mkv");
			}
			
			outExtsHighPriority = MakeUnique(outExtsHighPriority);
			outExtsNormalPriority->InsertRange(0, outExtsHighPriority);
			outExtsNormalPriority = MakeUnique(outExtsNormalPriority);

			SaveFileDialog dlg;


			{
		
				StringBuilder sbFilter;
				for each (String ^ ae in outExtsNormalPriority)
				{
					sbFilter.AppendFormat("{0} (*{1})|*{2}|",
						ae, ae, ae);
				}
				sbFilter.Append(I18N("All File") + "(*.*)|*.*");
				dlg.Filter = sbFilter.ToString();
			}

			dlg.InitialDirectory = initialDir;

			String^ firstExt;
			for each (String ^ s in outExtsNormalPriority)
			{
				firstExt = s;
				break;
			}

			DASSERT(!String::IsNullOrEmpty(OutputVideoCodec->ToString()));
			dlg.FileName = String::Format(L"{0} [{1}]{2}",
				baseFileName,
				OutputVideoCodec->ToString(),
				firstExt);
					
			if (System::Windows::Forms::DialogResult::OK != dlg.ShowDialog())
				return;
			if (IsFileOpen(getStdWstring(dlg.FileName).c_str()))
			{
				StringBuilder sb;
				sb.AppendLine(String::Format(I18N(STR_0_ALREADY_OPENED), dlg.FileName));
				sb.AppendLine();
				sb.AppendLine(I18N(STR_ARE_YOU_SURE_TO_CONTINUE));
				if (System::Windows::Forms::DialogResult::Yes != CppUtils::CenteredMessageBox(
					this,
					sb.ToString(),
					Application::ProductName,
					MessageBoxButtons::YesNo,
					MessageBoxIcon::Warning,
					MessageBoxDefaultButton::Button2))
				{
					return;
				}
			}
			outputtingMovie_ = dlg.FileName;

			String^ arg;
			
			DASSERT(inputmovies.Count != 0);
			if (inputmovies.Count == 1)
			{
				arg = String::Format(L"-y -i \"{0}\" -c copy -c:v {1} -c:a {2} \"{3}\"",
					inputmovies[0],
					OutputVideoCodec->ToFFMpegString(),
					OutputAudioCodec->ToFFMpegString(),
					outputtingMovie_);
			}
			else
			{
				if (!bReEncode)
				{
					tempFile_ = Path::GetTempFileName();
					StreamWriter sw(tempFile_, false, gcnew UTF8Encoding(false));

					for each (String ^ file in inputmovies)
					{
						sw.WriteLine(String::Format("file '{0}'", file));
					}

					arg = String::Format("-y -safe 0 -f concat -i \"{0}\" -c copy \"{1}\"",
						tempFile_,
						outputtingMovie_);
				}
				else
				{
					/*
	-filter_complex "[0:v:0] [0:a:0] [1:v:0] [1:a:0] [2:v:0] [2:a:0] [3:v:0] [3:a:0] concat=n=4:v=1:a=1 [v] [a]"
	-map "[v]" -map "[a]" "Y:\Share\3333.mkv"
					*/

					/*
	-i 480.mp4 -i 640.mp4 -filter_complex \
	"[0:v]scale=640:480:force_original_aspect_ratio=decrease,pad=640:480:(ow-iw)/2:(oh-ih)/2[v0]; \
	 [v0][0:a][1:v][1:a]concat=n=2:v=1:a=1[v][a]" \
	-map "[v]" -map "[a]" -c:v libx264 -c:a aac -movflags +faststart output.mp4
	Your ffmpeg is old: you should really consider updating to a build from the current git master branch. The ea
					*/
					StringBuilder sb;
					sb.Append("-y ");

					for each (String ^ f in inputmovies)
						sb.AppendFormat("-i \"{0}\" ", f);

					sb.Append("-filter_complex \"");
					if(IsSameSizeVideos())
					{
						for (int i = 0; i < inputmovies.Count; ++i)
						{
							sb.AppendFormat("[{0}:v:0]", i);
							sb.AppendFormat("[{0}:a:0]", i);
						}
						sb.AppendFormat("concat=n={0}:v=1:a=1[v][a]", inputmovies.Count);
					}
					else
					{
						System::Drawing::Size size = GetMaxVideoSize();
						for (int i = 0; i < inputmovies.Count; ++i)
						{
							sb.AppendFormat("[{0}:v:0]", i);
							sb.AppendFormat("scale={1}:{2}:force_original_aspect_ratio=decrease,pad={1}:{2}:(ow-iw)/2:(oh-ih)/2[v{0}];",
								i, size.Width, size.Height);
						}
						for (int i = 0; i < inputmovies.Count; ++i)
						{
							sb.AppendFormat("[v{0}][{0}:a:0]", i);
						}
						sb.AppendFormat("concat=n={0}:v=1:a=1[v][a]", inputmovies.Count);
					}
					sb.Append("\" ");

					sb.Append("-map \"[v]\" -map \"[a]\" ");

					sb.AppendFormat("-c:v {0} -c:a {1} \"{2}\"",
						OutputVideoCodec->ToFFMpegString(),
						OutputAudioCodec->ToFFMpegString(),
						outputtingMovie_);

					arg = sb.ToString();
				}
			}

			txtLogOut->Clear();
			txtLogErr->Clear();

			txtFFMpegArg->Text = String::Format(L"{0} {1}",
				AmbLib::doubleQuoteIfSpace(ffmpeg), arg);

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
			if (!ConfirmAndStopEncode())
				return;

		}

		System::Void FormMain::TsmiRemoveFromList_Click(System::Object^ sender, System::EventArgs^ e)
		{
			while (lvInputs->SelectedItems->Count != 0)
			{
				lvInputs->SelectedItems[0]->Remove();
			}

			if(lvInputs->Items->Count==0)
			{
				InputFormat = String::Empty;
				InputAudioCodec = gcnew AVCodec();
				InputVideoCodec = gcnew AVCodec();
				InputDuration = gcnew AVDuration();
			}
			else
			{
				for each (ListViewItem ^ lvi in lvInputs->Items)
					CheckMovieAndSet(GetMovieFileFromLvi(lvi), false);
			}
		}

		System::Void FormMain::tsmiOpenInputLocations_ClickCommon(System::Object^ sender, System::EventArgs^ e)
		{
			for each (String ^ inputmovie in GetInputMovies())
			{
				OpenFolder((HWND)this->Handle.ToPointer(),
					getStdWstring(inputmovie).c_str());
			}
		}
		System::Void FormMain::tsmiOpenOutput_ClickCommon(System::Object^ sender, System::EventArgs^ e)
		{
			String^ file = outputtingMovie_;
			if (String::IsNullOrEmpty(file))
				file = outputtedMovie_;
			if (String::IsNullOrEmpty(file))
			{
				CppUtils::Alert(this, I18N(STR_NO_OUTPUT_MOVIE));
				return;
			}

			OpenFolder((HWND)this->Handle.ToPointer(),
				getStdWstring(file).c_str());
		}
		System::Void FormMain::tsmiProcesstsmiProcessAfterFinish_Click(System::Object^ sender, System::EventArgs^ e)
		{
			HashIni^ ini = Profile::ReadAll(Program::IniFile);
			DVERIFY(dlgAfterFinish_.LoadValues("AfterFinish", ini));
			if (System::Windows::Forms::DialogResult::OK != dlgAfterFinish_.ShowDialog())
			{
				return;
			}

			if (dlgAfterFinish_.chkPlaySound->Checked)
			{
				CppUtils::Alert("playsound not yet implemented");
			}

			if (!dlgAfterFinish_.SaveValues("AfterFinish", ini) || !Profile::WriteAll(ini, Program::IniFile))
			{
				CppUtils::Alert(I18N(L"Failed to save ini"));
				return;
			}
		}

	}

}