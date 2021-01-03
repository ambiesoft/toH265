#include "stdafx.h"


#include "toH265.h"
#include "helper.h"
#include "TargetCodecDialog.h"
#include "ContinueException.h"
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
			TimeSpan% ts,
			double% fps)
		{
			WaitCursor wc;

			audiocodec = videocodec = nullptr;
			fps = 0;
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
						if (nAudios == 1)
						{
							audiocodec = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_name", i));
						}
					}
					else if (codec_type == "video")
					{
						++nVideos;
						if (nVideos == 1)
						{
							videocodec = (String^)joutput->SelectToken(String::Format("streams[{0}].codec_name", i));

							if (String::IsNullOrEmpty(myDuration))
								myDuration = (String^)joutput->SelectToken(String::Format("streams[{0}].duration", i));

							int width = (int)joutput->SelectToken(String::Format("streams[{0}].width", i));
							int height = (int)joutput->SelectToken(String::Format("streams[{0}].height", i));
							aspect = System::Drawing::Size(width, height);

							String^ sfps = (String^)joutput->SelectToken(String::Format("streams[{0}].r_frame_rate", i));
							if (!String::IsNullOrEmpty(sfps))
							{
								array<String^>^ two = sfps->Split('/');
								if (two && two->Length == 2)
								{
									double first, second;
									if (double::TryParse(two[0], first) && double::TryParse(two[1], second) &&
										first != 0 && second != 0)
									{
										fps = first / second;
									}
								}
							}
						}
					}

					if (!String::IsNullOrEmpty(audiocodec) && !String::IsNullOrEmpty(videocodec))
					{
						if (nAudios > 1 || nVideos > 1)
						{
							throw gcnew ContinueException(I18N("This video contains more than 1 audio or video streams."));
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
			AVDuration^ duration,
			double fps)
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
			lvi->SubItems["directory"]->Tag = Path::GetDirectoryName(movieFile);

			lvi->SubItems["filename"]->Text = Path::GetFileName(movieFile);
			lvi->SubItems["filename"]->Tag = Path::GetFileName(movieFile);

			lvi->SubItems["size"]->Text = AmbLib::FormatSize(size);
			lvi->SubItems["size"]->Tag = size;

			lvi->SubItems["aspect"]->Text = sToString(aspect);
			lvi->SubItems["aspect"]->Tag = aspect;

			lvi->SubItems["format"]->Text = Ambiesoft::toH265Helper::human_format(format->ToString());
			lvi->SubItems["format"]->Tag = Ambiesoft::toH265Helper::human_format(format->ToString());

			lvi->SubItems["vcodec"]->Text = vcodec->ToString();
			lvi->SubItems["vcodec"]->Tag = vcodec->ToString();

			lvi->SubItems["acodec"]->Text = acodec->ToString();
			lvi->SubItems["acodec"]->Tag = acodec->ToString();

			lvi->SubItems["duration"]->Text = duration->ToString();
			lvi->SubItems["duration"]->Tag = duration->TotalMilliseconds;

			lvi->SubItems["fps"]->Text = Ambiesoft::toH265Helper::FormatFPS(fps);
			lvi->SubItems["fps"]->Tag = fps;

			lvInputs->Items->Add(lvi);
		}
		bool FormMain::CheckMovieAndSet(String^ moviefile, bool bSet, bool bShowCodecAlert)
		{
			WaitCursor wc;
			if (String::IsNullOrEmpty(moviefile))
			{
				return false;
			}

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
			double fps;

			try
			{
				size = FileInfo(moviefile).Length;
			}
			catch (Exception^ ex)
			{
				CppUtils::Alert(this, ex->Message);
				return false;
			}

			try
			{
				GetStreamInfo(FFProbe, moviefile, format, audiocodec, videocodec, aspect, duration, fps);
			}
			catch (ContinueException^ ex)
			{
				CppUtils::Alert(this, ex->Message);
			}
			catch (Exception^ ex)
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
				if (bShowCodecAlert)
					CppUtils::Alert(this, String::Format(sformat, moviefile, L"h265"));
				ReturnValue = RETURN_STREAMISH265;
			}
			if (String::Compare(videocodec, "vp9", true) == 0)
			{
				if (bShowCodecAlert)
					CppUtils::Alert(this, String::Format(sformat, moviefile, L"vp9"));
				ReturnValue = RETURN_STREAMISVP9;
			}

			if (bSet)
			{
				InsertMovieItem(moviefile,
					size,
					aspect,
					format,
					gcnew AVCodec(audiocodec), gcnew AVCodec(videocodec),
					gcnew AVDuration(duration),
					fps);
			}

			DASSERT(FFMpegState == TaskState::None);
			SetStatusText(STATUSTEXT::READY);

			SetTotalInputInfo();

			return true;
		}

		void FormMain::SetTotalInputInfo()
		{
			AVCodec^ totalInputAudioCodec = gcnew AVCodec();
			AVCodec^ totalInputVideoCodec = gcnew AVCodec();
			String^ tryFormat;
			double tryfps = 0;
			double mDuration = 0;
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				if (String::IsNullOrEmpty(tryFormat))
				{
					tryFormat = lvi->SubItems["format"]->Text;
				}
				else if (tryFormat == "mixed")
				{

				}
				else
				{
					// not mixed
					if (tryFormat != lvi->SubItems["format"]->Text)
						tryFormat = "mixed";
				}
				// succeeded, set values
				totalInputAudioCodec->Merge(gcnew AVCodec(lvi->SubItems["acodec"]->Text));
				totalInputVideoCodec->Merge(gcnew AVCodec(lvi->SubItems["vcodec"]->Text));

				TimeSpan ts = TimeSpan::Parse(lvi->SubItems["duration"]->Text);
				mDuration += ts.TotalMilliseconds;

				double d;
				double::TryParse(lvi->SubItems["fps"]->Text, d);
				tryfps = Math::Max(tryfps, d);
			}
			TimeSpan all(10 * 1000 * (long long)mDuration);
			// InputDuration = all.ToString("hh\\:mm\\:ss");
			TotalInputDuration = gcnew AVDuration(all);
			TotalInputFormat = tryFormat;
			TotalInputAudioCodec = totalInputAudioCodec;
			TotalInputVideoCodec = totalInputVideoCodec;
			TotalInputFPS = tryfps;
			// tsOrigMovies_ = all;
		}

		void FormMain::SetFormatStatusText()
		{
			slFormat->Text = Ambiesoft::toH265Helper::human_format(totalInputFormat_);
		}

		void FormMain::SetCodecStatusText()
		{
			{
				StringBuilder sbAC;
				if (!TotalInputAudioCodec->IsEmpty)
				{
					sbAC.Append(TotalInputAudioCodec);
					if (encodeTask_ && !encodeTask_->CurrentOutputAudioCodec->IsEmpty)
					{
						sbAC.Append(" -> ");
						sbAC.Append(encodeTask_->CurrentOutputAudioCodec);
					}
				}
				if (slAudioCodec->Text != sbAC.ToString())
					slAudioCodec->Text = sbAC.ToString();
			}

			{
				StringBuilder sbVC;
				if (!TotalInputVideoCodec->IsEmpty)
				{
					sbVC.Append(TotalInputVideoCodec->ToString());
					if (encodeTask_ && !encodeTask_->CurrentOutputVideoCodec->IsEmpty)
					{
						sbVC.Append(" -> ");
						sbVC.Append(encodeTask_->CurrentOutputVideoCodec);
					}
				}
				if (slVideoCodec->Text != sbVC.ToString())
					slVideoCodec->Text = sbVC.ToString();
			}
		}

		void FormMain::SetTimeStatusText()
		{
			StringBuilder sb;
			if (!TotalInputDuration->IsEmpty())
			{
				sb.Append(TotalInputDuration);
				if (!OutputDuration->IsEmpty())
				{
					sb.Append(" -> ");
					sb.Append(OutputDuration);
				}
			}
			else
			{
				sb.Append(TotalInputDuration);
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
				CheckMovieAndSet(file, true, false);
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

		String^ FormMain::CurrentEncodingOutputtingFile::get()
		{
			if (!encodeTask_ || encodeTask_->IsAllEnded())
				return nullptr;
			return encodeTask_->CurrentOutputtingMovieFile;
		}
		String^ FormMain::buildTitleText(int percent, bool bFilenameOnly)
		{
			StringBuilder sbTitle;
			sbTitle.Append(percent);
			sbTitle.Append("% - ");

			if (!String::IsNullOrEmpty(CurrentEncodingOutputtingFile))
			{
				if (bFilenameOnly)
					sbTitle.Append(Path::GetFileName(CurrentEncodingOutputtingFile));
				else
					sbTitle.Append(CurrentEncodingOutputtingFile);
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

			// return ts.ToString(I18N(L"h'h 'm'm'"));
			return ts.ToString(I18N(L"h'h 'm'm 's's'"));
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
		void FormMain::UpdateTitleTS(TimeSpan tsProgress)
		{
			// for culculating eta
			double totalProgress = encodeTask_->EndedDurations + tsProgress.TotalMilliseconds;
			ElapseInfo^ lastElapse = gcnew ElapseInfo(totalProgress);
			ElapseInfo^ firstElapse = elapses_.Enqueue(lastElapse);

			DASSERT(encodeTask_);
			double percent = (totalProgress) / TotalInputDuration->TotalMilliseconds;
			UpdateTitle((int)(percent * 100));

			if (this->WindowState == FormWindowState::Minimized)
				return;

			SetStatusText(STATUSTEXT::REMAINING,
				GetRemainingTimeText(firstElapse, lastElapse, TotalInputDuration->TotalMilliseconds));

			OutputDuration = gcnew AVDuration(totalProgress);
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
			try
			{
				// DASSERT(regFFMpeg_);
				TimeSpan tsTime;
				double dblSpeed;
				if (FFMpegHelper::GetInfoFromFFMpegoutput(text, tsTime, dblSpeed))
				{
					UpdateTitleTS(tsTime);
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
			processTerminatedDuetoAppClose_ = false;

			ChangeStartButtonText(I18N(STR_BUTTONTEXT_PAUSE));

			lvInputs->Enabled = false;
			lvInputs->AllowDrop = false;
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

			lvInputs->Enabled = true;
			lvInputs->AllowDrop = true;
			btnBrowseMovie->Enabled = true;

			this->Icon = iconBlue_;
			notifyIconMain->Icon = iconBlue_;

			SetStatusText(STATUSTEXT::READY);

			OnEncodeTaskEnded(retval);
			if (encodeTask_)
			{
				encodeTask_->GoNext();
				DoNextEncodeTask();
			}
		}
		void FormMain::OnEncodeTaskEnded(int retval)
		{
			if (processTerminatedDuetoAppClose_)
				return;

			if(encodeTask_)
				encodeTask_->OnTaskEnded(retval);
		}
		
			
		void FormMain::DoNextEncodeTask()
		{
			if (processTerminatedDuetoAppClose_)
				return;

			if (encodeTask_->IsAllEnded())
			{
				OnAllTaskEnded();
				return;
			}

			DASSERT(!thFFMpeg_);

			// Start FFMpeg
			String^ report;
			String^ arg = encodeTask_->GetArg(report);

			txtLogErr->AppendText(report);

			txtFFMpegArg->Text = String::Format(L"{0} {1}",
				AmbLib::doubleQuoteIfSpace(FFMpeg), arg);

			Dictionary<String^, String^> param;
			param["ffmpeg"] = FFMpeg;
			param["arg"] = arg;
			thFFMpeg_ = gcnew System::Threading::Thread(
				gcnew ParameterizedThreadStart(this, &FormMain::StartOfThread));
			thFFMpeg_->Start(% param);
		}

		void FormMain::OnAllTaskEnded()
		{
			// Succeeded
			UpdateTitleComplete();

			if (tsmiEnabledtsmiProcessAfterFinish->Checked)
			{
				HashIni^ ini = Profile::ReadAll(Program::IniFile);
				DVERIFY(dlgAfterFinish_.LoadValues("AfterFinish", ini));

				if (dlgAfterFinish_.chkPlaySound->Checked)
				{
					dlgAfterFinish_.PlayWav(true);
				}

				if (dlgAfterFinish_.chkOpenFolder->Checked)
				{
					// Show outputmovie in Explorer
					// CppUtils::OpenFolder(this, outputtingMovie_);
				}

				if (dlgAfterFinish_.chkLaunchApp->Checked)
				{
					try
					{
						Process::Start(dlgAfterFinish_.txtApp->Text, dlgAfterFinish_.txtArg->Text);
					}
					catch (Exception^ ex)
					{
						CppUtils::Alert(ex);
					}
				}

				if (dlgAfterFinish_.chkShutdown->Checked)
				{
					Process::Start(Application::ExecutablePath, "-shutdown");
				}
			}

			Summary^ summary = gcnew Summary();

			// default process after finish
			for each (EncodeJob ^ job in encodeTask_->GetResults())
			{
				DASSERT(job->IsEnded);
				String^ outputtedFormat;
				String^ outputtedAC = String::Empty;
				String^ outputtedVC = String::Empty;
				System::Drawing::Size outputtedAspect;
				TimeSpan outputtedTS;
				LONGLONG inputSize = 0;
				LONGLONG outputtedSize = 0;
				double outputtedFps;

				StringBuilder sbMessage;
				bool isWarning = false;

				try
				{
					if (job->RetVal != 0)
					{
						isWarning = true;
						sbMessage.AppendLine(String::Format(L"Process exited with {0}.", job->RetVal));
						return;
					}
					DASSERT(!String::IsNullOrEmpty(job->OutputtedMovie));
					GetStreamInfo(FFProbe, job->OutputtedMovie, outputtedFormat, outputtedAC, outputtedVC, outputtedAspect, outputtedTS, outputtedFps);
					for each (String ^ infile in job->InputMovies)
						inputSize += FileInfo(infile).Length;
					outputtedSize = FileInfo(job->OutputtedMovie).Length;

					DASSERT(!String::IsNullOrEmpty(outputtedAC));
					DASSERT(!String::IsNullOrEmpty(outputtedVC));
					DASSERT(outputtedTS.TotalMilliseconds != 0);

					sbMessage.AppendFormat(I18N(L"Encoding successfully finished at {0}."),
						job->FinishedDateString);
					// sbMessage.AppendLine();

					if (!AmbLib::IsAlmostSame(job->TotalInputDuration->TotalMilliseconds,
						outputtedTS.TotalMilliseconds))
					{
						sbMessage.AppendLine();
						sbMessage.AppendLine(I18N("Be carefull. The durations differs."));
						isWarning = true;
					}
					if (!AmbLib::IsAlmostSame(job->TotalInputFPS, outputtedFps))
					{
						sbMessage.AppendLine();
						sbMessage.AppendLine(I18N("Be carefull. The FPSs differs."));
						isWarning = true;
					}
					sbMessage.AppendLine();
					job->PrintInputFiles(%sbMessage);
					job->PrintOutputFile(%sbMessage);
					sbMessage.AppendLine(String::Format(I18N(L"Format = {0}"), outputtedFormat));
					sbMessage.AppendLine(String::Format(I18N(L"Audio codec = {0}"), outputtedAC));
					sbMessage.AppendLine(String::Format(I18N(L"Video codec = {0}"), outputtedVC));
					sbMessage.AppendLine(String::Format(I18N(L"Duration = {0}"), outputtedTS.ToString()));
					sbMessage.AppendLine(String::Format(I18N(L"FPS = {0}"), Ambiesoft::toH265Helper::FormatFPS(outputtedFps)));
					sbMessage.AppendLine(String::Format(I18N(L"Original Size = {0}"), AmbLib::FormatSize(inputSize)));
					sbMessage.AppendLine(String::Format(I18N(L"Output Size = {0}"), AmbLib::FormatSize(outputtedSize)));
					sbMessage.AppendLine(String::Format(I18N(L"Compressed = {0}%"), AmbLib::GetRatioString(outputtedSize, inputSize)));
				}
				catch (Exception^ ex)
				{
					// CppUtils::Alert(this, ex);
					isWarning = true;
					sbMessage.AppendLine(ex->Message);
				}

				summary->AddPaper(isWarning, sbMessage.ToString(), job);
			}
			lastSummary_ = summary;

			lastSummary_->Show(this);

			OutputDuration = gcnew AVDuration();

			elapses_.Clear();
		}
		System::Void FormMain::tsmiShowLastResult_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (!lastSummary_)
			{
				CppUtils::Alert(I18N("No last message"));
				return;
			}
			lastSummary_->Show(this);
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
			else if (tsmiPriorityBelowNormal->Checked)
			{
				OnMenuPriorityCommon(FFMpegPriority::BelowNormal);
			}
			System::Windows::Forms::Timer^ timerSetAffinity = gcnew System::Windows::Forms::Timer();
			timerSetAffinity->Interval = 5 * 1000;
			timerSetAffinity->Enabled = true;
			timerSetAffinity->Tick += gcnew System::EventHandler(this, &Ambiesoft::toH265::FormMain::OnTick);
			timerSetAffinity->Tag = timerSetAffinity;
		}
		int affinitySetProcess_;
		void FormMain::OnTick(System::Object^ sender, System::EventArgs^ e)
		{
			System::Windows::Forms::Timer^ timerSetAffinity = (System::Windows::Forms::Timer^)sender;
			timerSetAffinity->Enabled = false;
			delete timerSetAffinity;
			timerSetAffinity = nullptr;
			tsmiCPUAffinityEnable_Click(nullptr, nullptr);
			if(processFFMpeg_)
				affinitySetProcess_ = processFFMpeg_->Id;
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
		AVDuration^ FormMain::GetDurationFromLvi(ListViewItem^ lvi)
		{
			return gcnew AVDuration(TimeSpan::Parse(lvi->SubItems["duration"]->Text));
		}
		double FormMain::GetFPSFromLvi(ListViewItem^ lvi)
		{
			double d;
			double::TryParse(lvi->SubItems["fps"]->Text, d);
			return d;
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


		array<String^>^ FormMain::GetInputMovies()
		{
			List<String^> inputmovies;
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				String^ inputmovie = GetMovieFileFromLvi(lvi);
				inputmovies.Add(inputmovie);
			}
			return inputmovies.ToArray();
		}
		array<AVDuration^>^ FormMain::GetInputDurations()
		{
			List<AVDuration^> inputdurations;
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				AVDuration^ duration = GetDurationFromLvi(lvi);
				inputdurations.Add(duration);
			}
			return inputdurations.ToArray();
		}
		array<double>^ FormMain::GetInputFPSes()
		{
			List<double> inputFpses;
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				double duration = GetFPSFromLvi(lvi);
				inputFpses.Add(duration);
			}
			return inputFpses.ToArray();
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
			
			// check input
			if (lvInputs->Items->Count == 0)
			{
				CppUtils::Alert(this, I18N("No items"));
				return;
			}

			DASSERT(!encodeTask_ || encodeTask_->IsAllEnded());
			encodeTask_ = nullptr;
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				String^ inputmovie = GetMovieFileFromLvi(lvi);

				if (String::IsNullOrEmpty(inputmovie))
				{
					CppUtils::Alert(this, I18N(L"input movie is empty."));
					return;
				}
				if (!CheckMovieAndSet(inputmovie, false, false))
					return;
			}



			array<String^>^ inputmovies = GetInputMovies();
			array<AVDuration^>^ inputdurations = GetInputDurations();
			array<double>^ inputFpses = GetInputFPSes();

			bool isLosslessable = inputmovies->Length != 0 &&
				TotalInputFormat != "mixed" &&
				!TotalInputAudioCodec->IsMixed && !TotalInputVideoCodec->IsMixed;
			TargetCodecDialog codecDlg(isLosslessable, 
				Program::IniFile, 
				SECTION_TARGETCODECDIALOG,
				inputmovies,
				TotalInputVideoCodec, TotalInputAudioCodec);

			if (TotalInputVideoCodec->IsH264)
			{
				codecDlg.rbVideoH265->Checked = true;

				//if (InputAudioCodec == "aac" ||
				//	InputAudioCodec == "vorbis")
				{
					codecDlg.rbAudioCopy->Checked = true;
				}
			}
			else if (TotalInputVideoCodec->IsVp8)
			{
				codecDlg.rbVideoVp9->Checked = true;

				// if (InputAudioCodec == "opus")
				{
					codecDlg.rbAudioCopy->Checked = true;
				}
			}


			if (TotalInputAudioCodec->IsMixed)
			{
				codecDlg.rbAudioCopy->Checked = false;
			}

			// show the dialog
			if (System::Windows::Forms::DialogResult::OK != codecDlg.ShowDialog())
				return;


			// Create Task
			DASSERT(!encodeTask_ || encodeTask_->IsAllEnded());
			encodeTask_ = gcnew EncodeTask();

			encodeTask_->AddJob(
				codecDlg.IsConcat,
				codecDlg.IsReEncode,
				inputmovies,
				codecDlg.OutputFiles,
				codecDlg.OutputVideoCodec,
				codecDlg.OutputAudioCodec,
				IsSameSizeVideos(),
				GetMaxVideoSize(),
				codecDlg.IsConcat ? gcnew array<AVDuration^>{ TotalInputDuration } : inputdurations,
				codecDlg.IsConcat ? gcnew array<double>{TotalInputFPS} : inputFpses);
			
			

			txtLogOut->Clear();
			txtLogErr->Clear();

			processTerminatedDuetoAppClose_ = false;
			DoNextEncodeTask();
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
			DASSERT(!encodeTask_ || encodeTask_->IsAllEnded());
			encodeTask_ = nullptr;

			while (lvInputs->SelectedItems->Count != 0)
			{
				lvInputs->SelectedItems[0]->Remove();
			}

			if(lvInputs->Items->Count==0)
			{
				TotalInputFormat = String::Empty;
				TotalInputAudioCodec = gcnew AVCodec();
				TotalInputVideoCodec = gcnew AVCodec();
				TotalInputDuration = gcnew AVDuration();
			}
			else
			{
				for each (ListViewItem ^ lvi in lvInputs->Items)
					CheckMovieAndSet(GetMovieFileFromLvi(lvi), false, false);
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
			String^ outputMovie;
			do
			{
				if (!encodeTask_)
				{
					if (!lastSummary_)
					{
						break;
					}
					outputMovie = lastSummary_->LastOutputMovie;
				}
				else if (encodeTask_->IsAllEnded())
				{
					outputMovie = lastSummary_->LastOutputMovie;
				}
				else
				{
					outputMovie = encodeTask_->CurrentOutputtingMovieFile;
				}
			} while (false);

			if (String::IsNullOrEmpty(outputMovie))
			{
				CppUtils::Alert(this, I18N(STR_NO_OUTPUT_MOVIE));
				return;
			}

			OpenFolder((HWND)this->Handle.ToPointer(),
				getStdWstring(outputMovie).c_str());
		}
		System::Void FormMain::tsmiProcesstsmiProcessAfterFinish_Click(System::Object^ sender, System::EventArgs^ e)
		{
			HashIni^ ini = Profile::ReadAll(Program::IniFile);
			DVERIFY(dlgAfterFinish_.LoadValues("AfterFinish", ini));
			if (System::Windows::Forms::DialogResult::OK != dlgAfterFinish_.ShowDialog())
			{
				return;
			}

			//if (dlgAfterFinish_.chkPlaySound->Checked)
			//{
			//	CppUtils::Alert("playsound not yet implemented");
			//}

			if (!dlgAfterFinish_.SaveValues("AfterFinish", ini) || !Profile::WriteAll(ini, Program::IniFile))
			{
				CppUtils::Alert(I18N(L"Failed to save ini"));
				return;
			}
		}

		System::Void FormMain::tsmiCPUAffinity_DropDownOpening(System::Object^ sender, System::EventArgs^ e)
		{
			// Create CPU affinity menu

			// Find insert pos
			int insertIndexBegin = tsmiCPUAffinity->DropDownItems->IndexOf(tsmsCpuAffinityBegin)+1;
			DASSERT(insertIndexBegin >= 0);
			int insertIndexEnd = tsmiCPUAffinity->DropDownItems->IndexOf(tsmsCpuAffinityEnd);
			DASSERT(insertIndexBegin <= insertIndexEnd);


			// Remove items below insertIndex
			const int countToRemove = insertIndexEnd - insertIndexBegin;
			for (int i = 0; i < countToRemove; ++i)
			{
				tsmiCPUAffinity->DropDownItems->RemoveAt(insertIndexBegin);
			}
			
			if (tsmiCPUAffinityEnable->Checked &&
				processFFMpeg_ && processFFMpeg_->Id == affinitySetProcess_)
			{
				cpuAffinity_.Update(processFFMpeg_);
			}

			for (int i = 0; i < Environment::ProcessorCount; ++i)
			{
				ToolStripMenuItem^ item = gcnew ToolStripMenuItem();
				item->CheckOnClick = true;
				item->Enabled = tsmiCPUAffinityEnable->Checked;
				item->Text = (i).ToString();
				item->Name = String::Format("CpuAffinity{0}", i);
				item->Checked = cpuAffinity_.IsCPUON(i);
				item->Tag = i;
				item->Click += gcnew System::EventHandler(this, &Ambiesoft::toH265::FormMain::OnToggleCPU);
				tsmiCPUAffinity->DropDownItems->Insert(insertIndexBegin++, item);
			}
			tsmiClearAllButZero->Enabled = tsmiCPUAffinityEnable->Checked;
			tsmiToggleAll->Enabled = tsmiCPUAffinityEnable->Checked;
			tsmiEnableAll->Enabled = tsmiCPUAffinityEnable->Checked;
		}
		System::Void FormMain::tsmiCPUAffinityEnable_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (!processFFMpeg_)
				return;

			HANDLE hProcess = (HANDLE)processFFMpeg_->Handle.ToPointer();
			WaitForInputIdle(hProcess, INFINITE);

			try
			{
				if (tsmiCPUAffinityEnable->Checked)
				{
					DTRACE(String::Format("Set Process {0} Affinity to {1}", pidFFMpeg_, cpuAffinity_.Value()));
					if (!SetProcessAffinityMask(hProcess, cpuAffinity_.ValueAsPtr()))
					{
						throw gcnew Win32Exception();
					}
				}
				else
				{
					// set default;
					DWORD_PTR a;
					DWORD_PTR s;
					DVERIFY(GetProcessAffinityMask(hProcess, &a, &s));
					DVERIFY(SetProcessAffinityMask(hProcess, s));
				}
			}
			catch (Win32Exception^ ex)
			{
				CppUtils::Alert(ex);
			}
		}
		System::Void FormMain::tsmiClearAllButZero_Click(System::Object^ sender, System::EventArgs^ e)
		{
			cpuAffinity_.ClearAllButZero();
			tsmiCPUAffinityEnable_Click(nullptr, nullptr);
		}
		System::Void FormMain::tsmiEnableAll_Click(System::Object^ sender, System::EventArgs^ e)
		{
			cpuAffinity_.EnableAll();
			tsmiCPUAffinityEnable_Click(nullptr, nullptr);
		}
		System::Void FormMain::tsmiToggleAll_Click(System::Object^ sender, System::EventArgs^ e)
		{
			cpuAffinity_.ToggleAll();
			tsmiCPUAffinityEnable_Click(nullptr, nullptr);
		}
		void FormMain::OnToggleCPU(System::Object^ sender, System::EventArgs^ e)
		{
			ToolStripMenuItem^ item = (ToolStripMenuItem^)sender;
			int cpunum = (int)item->Tag;
			cpuAffinity_.SetCPU(cpunum, item->Checked);
			tsmiCPUAffinityEnable_Click(nullptr, nullptr);
		}

	}

}

