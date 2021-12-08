#include "stdafx.h"


#include "toH265.h"
#include "helper.h"
#include "TargetCodecDialog.h"
#include "ContinueException.h"
#include "ListViewItemData.h"

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

			try
			{
				DateTime dt = File::GetLastWriteTime(movieFile);
				lvi->SubItems["lastmodified"]->Text = dt.ToString();
				lvi->SubItems["lastmodified"]->Tag = dt;
			}
			catch(Exception^){}

			lvi->ImageKey = IMAGEKEY_NORMAL;
			// lvi->ToolTipText = movieFile;

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

			DASSERT(CurrentFFMpegState == FFMpegState::None);
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
			StatusTotalInputDuration = gcnew AVDuration(all);
			StatusTotalInputFormat = tryFormat;
			StatusTotalInputAudioCodec = totalInputAudioCodec;
			StatusTotalInputVideoCodec = totalInputVideoCodec;
			TotalInputFPS = tryfps;
			// tsOrigMovies_ = all;
		}

		void FormMain::SetFormatStatusText()
		{
			slFormat->Text = Ambiesoft::toH265Helper::human_format(StatusTotalInputFormat);
		}

		void FormMain::SetCodecStatusText()
		{
			{
				StringBuilder sbAC;
				if(IsTaskActive)
				{ 
					sbAC.Append(encodeTask_->CurrentInputAudioCodec);
					sbAC.Append(" -> ");
					sbAC.Append(encodeTask_->CurrentOutputAudioCodec);
				}
				else if (!StatusTotalInputAudioCodec->IsEmpty)
				{
					sbAC.Append(StatusTotalInputAudioCodec);
					//if (totalout
					//{
					//	sbAC.Append(" -> ");
					//	sbAC.Append(encodeTask_->CurrentOutputAudioCodec);
					//}
				}


				if (slAudioCodec->Text != sbAC.ToString())
					slAudioCodec->Text = sbAC.ToString();
			}

			{
				StringBuilder sbVC;
				if (IsTaskActive)
				{
					sbVC.Append(encodeTask_->CurrentInputVideoCodec);
					sbVC.Append(" -> ");
					sbVC.Append(encodeTask_->CurrentOutputVideoCodec);
				}
				else if (!StatusTotalInputVideoCodec->IsEmpty)
				{
					sbVC.Append(StatusTotalInputVideoCodec->ToString());
					//if (IsTaskActive && !encodeTask_->CurrentOutputVideoCodec->IsEmpty)
					//{
					//	sbVC.Append(" -> ");
					//	sbVC.Append(encodeTask_->CurrentOutputVideoCodec);
					//}
				}


				if (slVideoCodec->Text != sbVC.ToString())
					slVideoCodec->Text = sbVC.ToString();
			}
		}

		void FormMain::SetTimeStatusText()
		{
			StringBuilder sb;
			if (IsTaskActive)
			{
				sb.Append(encodeTask_->TotalInputDuration);
				sb.Append(" -> ");
				sb.Append(StatusOutputDuration);
			}
			else if (!StatusTotalInputDuration->IsEmpty())
			{
				sb.Append(StatusTotalInputDuration);
				if (!StatusOutputDuration->IsEmpty())
				{
					sb.Append(" -> ");
					sb.Append(StatusOutputDuration);
				}
			}
			else
			{
				sb.Append(StatusTotalInputDuration);
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
			if(!IsTaskActive)
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
			if (!IsTaskActive)
				return;

			// for culculating eta
			double totalProgress = encodeTask_->EndedDurations + tsProgress.TotalMilliseconds;
			ElapseInfo^ lastElapse = gcnew ElapseInfo(totalProgress);
			ElapseInfo^ firstElapse = elapses_.Enqueue(lastElapse);

			double percent = (totalProgress) / encodeTask_->TotalInputDuration->TotalMilliseconds;
			UpdateTitle((int)(percent * 100));

			if (this->WindowState == FormWindowState::Minimized)
				return;

			SetStatusText(STATUSTEXT::REMAINING,
				GetRemainingTimeText(firstElapse, lastElapse, encodeTask_->TotalInputDuration->TotalMilliseconds));

			StatusOutputDuration = gcnew AVDuration(totalProgress);
		}

		void FormMain::SetStatusText(STATUSTEXT ss)
		{
			DASSERT(ss == STATUSTEXT::READY||ss==STATUSTEXT::INTERMEDIATE);
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
			case STATUSTEXT::INTERMEDIATE:
				text = I18N(L"...");
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

		FormMain::FFMpegState FormMain::CurrentFFMpegState::get()
		{
			if (!thFFMpeg_ && !processFFMpeg_)
			{
				if (IsTaskActive)
					return FFMpegState::Intermidiate;
				else
					return FFMpegState::None;
			}
			else if (thFFMpeg_ && !processFFMpeg_)
			{
				DASSERT(IsTaskActive);
				return FFMpegState::ProcessLaunching;
			}
			else if (!thFFMpeg_ && processFFMpeg_)
			{
				DASSERT(false);
				return FFMpegState::Unknown;
			}
			else if (thFFMpeg_ && processFFMpeg_)
			{
				DASSERT(IsTaskActive);
				if (processSuspeded_)
					return FFMpegState::Pausing;
				else
					return FFMpegState::Running;
			}

			return FFMpegState::Unknown;
		}

		void FormMain::ChangeStartButtonText(StartButtonText sbt)
		{
			String^ text = String::Empty;
			switch (sbt)
			{
			case StartButtonText::Start:
				text = I18N(STR_BUTTONTEXT_START);
				break;
			case StartButtonText::Pause:
				text = I18N(STR_BUTTONTEXT_PAUSE);
				break;
			case StartButtonText::Resume:
				text = I18N(STR_BUTTONTEXT_RESUME);
				break;
			default:
				DASSERT(false);
			}
			btnStart->Text = text;
			tsmiNotifyStart->Text = text;
		}
		void FormMain::ThreadStarted()
		{
			processSuspeded_ = false;
			processTerminatedDuetoAppClose_ = false;

			CurrentTaskState = TaskState::Encoding;

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

			CurrentTaskState = TaskState::Intermediate;

			OnEncodeTaskEnded(retval);
			if (encodeTask_)
			{
				encodeTask_->GoNext();
				DoNextEncodeTask();
			}
			else
			{
				CurrentTaskState = TaskState::Ready;
			}
		}

		
		void FormMain::CurrentTaskState::set(TaskState state)
		{
			taskState_ = state;
			switch (taskState_)
			{
			case TaskState::Ready:
				btnStart->Enabled = true;
				ChangeStartButtonText(StartButtonText::Start);

				// lvInputs->Enabled = true;
				lvInputs->AllowDrop = true;
				btnBrowseMovie->Enabled = true;

				this->Icon = iconBlue_;
				notifyIconMain->Icon = iconBlue_;


				// TODO(does not go normal): statusMain->BackColor = DefaultStatusColor;

				SetStatusText(STATUSTEXT::READY);
				
				break;

			case TaskState::Intermediate:
				btnStart->Enabled = false;
				DASSERT(!lvInputs->AllowDrop);
				DASSERT(!btnBrowseMovie->Enabled);

				SetStatusText(STATUSTEXT::INTERMEDIATE);
				break;

			case TaskState::Encoding:
				btnStart->Enabled = true;
				ChangeStartButtonText(StartButtonText::Pause);

				// lvInputs->Enabled = false;
				lvInputs->AllowDrop = false;
				btnBrowseMovie->Enabled = false;

				this->Icon = iconRed_;
				notifyIconMain->Icon = iconRed_;
				// TODO(does not go normal): statusMain->BackColor = Color::Red;
				break;
			}
		}

		void FormMain::OnEncodeTaskEnded(int retval)
		{
			if (processTerminatedDuetoAppClose_)
				return;

			if (encodeTask_)
			{
				encodeTask_->OnTaskEnded(retval);
			}
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

			SetCodecStatusText();
			encodeTask_->OnTaskStarted();
		}


		void FormMain::OnTaskStarted()
		{
			DoNextEncodeTask();
		}
		void FormMain::OnAllTaskEnded()
		{
			// Succeeded
			UpdateTitleComplete();

			CurrentTaskState = TaskState::Ready;

			if (tsmiEnabledtsmiProcessAfterFinish->Checked)
			{
				dlgAfterFinish_.DoNotify();
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
					}
					else
					{
						DASSERT(!String::IsNullOrEmpty(job->OutputtedMovie));
						GetStreamInfo(FFProbe, job->OutputtedMovie, outputtedFormat, outputtedAC, outputtedVC, outputtedAspect, outputtedTS, outputtedFps);
						for each (String ^ infile in job->EndedInputMovies)
							inputSize += FileInfo(infile).Length;
						outputtedSize = FileInfo(job->OutputtedMovie).Length;

						DASSERT(!String::IsNullOrEmpty(outputtedAC));
						DASSERT(!String::IsNullOrEmpty(outputtedVC));
						DASSERT(outputtedTS.TotalMilliseconds != 0);

						sbMessage.AppendFormat(I18N(L"Encoding successfully finished at {0}."),
							job->FinishedDateString);

						if (!job->InputAudioCodec->IsEmpty && String::IsNullOrEmpty(outputtedAC))
						{
							sbMessage.AppendLine();
							sbMessage.AppendLine(
								String::Format(L"{0} {1}",
									I18N(L"Be carefull."),
									I18N(L"Audio codec not found.")));
							isWarning = true;
						}
						if (!job->InputVideoCodec->IsEmpty && String::IsNullOrEmpty(outputtedVC))
						{
							sbMessage.AppendLine();
							sbMessage.AppendLine(
								String::Format(L"{0} {1}",
									I18N(L"Be carefull."),
									I18N(L"Video codec not found.")));
							isWarning = true;
						}

						if (!AmbLib::IsAlmostSame(job->TotalInputDuration->TotalMilliseconds,
							outputtedTS.TotalMilliseconds))
						{
							sbMessage.AppendLine();
							sbMessage.AppendLine(
								String::Format(L"{0} {1}",
									I18N(L"Be carefull."),
									I18N(L"The durations differs.")));
							isWarning = true;
						}
						if (!AmbLib::IsAlmostSame(job->TotalInputFPS, outputtedFps))
						{
							sbMessage.AppendLine();
							sbMessage.AppendLine(
								String::Format(L"{0} {1}",
									I18N(L"Be carefull."),
									I18N(L"The FPSs differs.")));
							isWarning = true;
						}
						if (job->IsMoveFailed)
						{
							sbMessage.AppendLine();
							sbMessage.AppendLine(
								String::Format(L"{0} {1}",
									I18N(L"Be carefull."),
									I18N("Failed to move some files.")));
							sbMessage.AppendLine(job->ErrorFailedMove);
							isWarning = true;
						}
						sbMessage.AppendLine();
						job->PrintEndedInputFiles(% sbMessage);
						job->PrintOutputFile(% sbMessage);
						sbMessage.AppendLine(String::Format(I18N(L"Format = {0}"), outputtedFormat));
						sbMessage.AppendLine(String::Format(I18N(L"Audio codec = {0}"), outputtedAC));
						sbMessage.AppendLine(String::Format(I18N(L"Video codec = {0}"), outputtedVC));
						sbMessage.AppendLine(String::Format(I18N(L"Duration = {0}"), outputtedTS.ToString()));
						sbMessage.AppendLine(String::Format(I18N(L"FPS = {0}"), Ambiesoft::toH265Helper::FormatFPS(outputtedFps)));
						sbMessage.AppendLine(String::Format(I18N(L"Original Size = {0}"), AmbLib::FormatSize(inputSize)));
						sbMessage.AppendLine(String::Format(I18N(L"Output Size = {0}"), AmbLib::FormatSize(outputtedSize)));
						sbMessage.AppendLine(String::Format(I18N(L"Compressed = {0}%"), AmbLib::GetRatioString(outputtedSize, inputSize)));
					}
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

			StatusOutputDuration = gcnew AVDuration();

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
		AVCodec^ FormMain::GetVCodecFromLvi(ListViewItem^ lvi)
		{
			return gcnew AVCodec(lvi->SubItems["vcodec"]->Text);
		}
		AVCodec^ FormMain::GetACodecFromLvi(ListViewItem^ lvi)
		{
			return gcnew AVCodec(lvi->SubItems["acodec"]->Text);
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
		Size FormMain::GetMaxVideoSize(ItemSelection sel)
		{
			double maxWidth = 0;
			ListViewItem^ lviMax;
			for each (ListViewItem ^ lvi in GetItems(sel))
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
		bool FormMain::IsSameSizeVideos(ItemSelection sel)
		{
			System::Drawing::Size size;
			bool first = false;
			
			for each (ListViewItem ^ lvi in GetItems(sel))
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

		ArrayList^ FormMain::GetItemsCommon(ItemToGet toGet, ItemSelection sel)
		{
			ArrayList^ rets = gcnew ArrayList();
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				switch (sel)
				{
				case ItemSelection::All:
					break;
				case ItemSelection::Completed:
					if(lvi->ImageKey != IMAGEKEY_DONE)
						continue;
					break;
				case ItemSelection::Incompleted:
					if (lvi->ImageKey == IMAGEKEY_DONE)
						continue;
					break;
				case ItemSelection::Selectet:
					if (!lvi->Selected)
						continue;
					break;
				default:
					DASSERT(false);
					continue;
				}

				switch (toGet)
				{
				case ItemToGet::Item:
				{
					rets->Add(lvi);
				}
				break;
				case ItemToGet::Name:
				{
					String^ inputmovie = GetMovieFileFromLvi(lvi);
					rets->Add(inputmovie);
				}
				break;
				case ItemToGet::Duration:
				{
					AVDuration^ duration = GetDurationFromLvi(lvi);
					rets->Add(duration);
				}
				break;
				case ItemToGet::Fps:
				{
					double fps = GetFPSFromLvi(lvi);
					rets->Add(fps);
				}
				break;
				default:
					DASSERT(false);
				}
			}
			return rets;
		}
		array<ListViewItem^>^ FormMain::GetItems(ItemSelection sel)
		{
			return (array<ListViewItem^>^)GetItemsCommon(ItemToGet::Item, sel)->ToArray(ListViewItem::typeid);
		}
		array<String^>^ FormMain::GetInputMovies(ItemSelection sel)
		{
			return (array<String^>^) GetItemsCommon(ItemToGet::Name, sel)->ToArray(String::typeid);
		}
		array<AVDuration^>^ FormMain::GetInputDurations(ItemSelection sel)
		{
			return (array<AVDuration^>^)GetItemsCommon(ItemToGet::Duration, sel)->ToArray(AVDuration::typeid);
			//List<AVDuration^> inputdurations;
			//for each (ListViewItem ^ lvi in lvInputs->Items)
			//{
			//	AVDuration^ duration = GetDurationFromLvi(lvi);
			//	inputdurations.Add(duration);
			//}
			//return inputdurations.ToArray();
		}
		array<double>^ FormMain::GetInputFPSes(ItemSelection sel)
		{
			return (array<double>^)GetItemsCommon(ItemToGet::Fps, sel)->ToArray(double::typeid);
			//List<double> inputFpses;
			//for each (ListViewItem ^ lvi in lvInputs->Items)
			//{
			//	double duration = GetFPSFromLvi(lvi);
			//	inputFpses.Add(duration);
			//}
			//return inputFpses.ToArray();
		}
		bool FormMain::HasCompleteItems::get()
		{
			for each (ListViewItem ^ lvi in lvInputs->Items)
			{
				if (lvi->ImageKey == IMAGEKEY_DONE)
					return true;
			}
			return false;
		}

		System::Void FormMain::btnStart_Click(System::Object^ sender, System::EventArgs^ e)
		{
			switch (CurrentFFMpegState)
			{
			case FFMpegState::None:
			case FFMpegState::Intermidiate:
				break;
			case FFMpegState::Pausing:
				if (!ResumeProcess(processFFMpeg_))
				{
					CppUtils::Alert(this, I18N(L"Failed to resume process."));
					return;
				}
				ChangeStartButtonText(StartButtonText::Pause);
				processSuspeded_ = false;
				this->Icon = iconRed_;
				notifyIconMain->Icon = iconRed_;
				return;
			case FFMpegState::ProcessLaunching:
				CppUtils::Alert(this, I18N(L"Intermidiate state. Wait a while and do it again."));
				return;
			case FFMpegState::Running:
				if (!SuspendProcess(processFFMpeg_))
				{
					CppUtils::Alert(this, I18N(L"Failed to suspend process."));
					return;
				}
				ChangeStartButtonText(StartButtonText::Resume);
				processSuspeded_ = true;
				this->Icon = iconYellow_;
				notifyIconMain->Icon = iconYellow_;
				elapses_.Clear();
				return;
			case FFMpegState::Unknown:
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

			DASSERT(!IsTaskActive);
			encodeTask_ = nullptr;

			// check input movie
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

			ItemSelection itemSelection = ItemSelection::All;
			if (HasCompleteItems)
			{
				System::Windows::Forms::DialogResult result =
					CppUtils::CenteredMessageBox(
						I18N(L"There are items which are already encoded. Do you want to encode them again?"),
						Application::ProductName,
						MessageBoxButtons::YesNoCancel,
						MessageBoxIcon::Question,
						MessageBoxDefaultButton::Button2);
				if (result == System::Windows::Forms::DialogResult::Yes)
				{
					itemSelection = ItemSelection::All;
				}
				else if (result == System::Windows::Forms::DialogResult::No)
				{
					itemSelection = ItemSelection::Incompleted;
				}
				else if (result == System::Windows::Forms::DialogResult::Cancel)
				{
					return;
				}
				else
				{
					DASSERT(false);
					return;
				}
			}

			array<String^>^ inputmovies = GetInputMovies(itemSelection);
			array<AVDuration^>^ inputdurations = GetInputDurations(itemSelection);
			array<double>^ inputFpses = GetInputFPSes(itemSelection);

			if (inputmovies->Length == 0)
			{
				CppUtils::Alert(I18N(L"No items to encode."));
				return;
			}

			bool isLosslessable = inputmovies->Length != 0 &&
				StatusTotalInputFormat != "mixed" &&
				!StatusTotalInputAudioCodec->IsMixed && !StatusTotalInputVideoCodec->IsMixed;
			TargetCodecDialog codecDlg(isLosslessable, 
				Program::IniFile, 
				SECTION_TARGETCODECDIALOG,
				inputmovies,
				StatusTotalInputVideoCodec, StatusTotalInputAudioCodec);

			if (StatusTotalInputVideoCodec->IsH264)
			{
				codecDlg.rbVideoH265->Checked = true;

				//if (InputAudioCodec == "aac" ||
				//	InputAudioCodec == "vorbis")
				{
					codecDlg.rbAudioCopy->Checked = true;
				}
			}
			else if (StatusTotalInputVideoCodec->IsVp8)
			{
				codecDlg.rbVideoVp9->Checked = true;

				// if (InputAudioCodec == "opus")
				{
					codecDlg.rbAudioCopy->Checked = true;
				}
			}


			if (StatusTotalInputAudioCodec->IsMixed)
			{
				codecDlg.rbAudioCopy->Checked = false;
			}

			// check afterfinish
			if (tsmiEnabledtsmiProcessAfterFinish->Checked)
			{
				if (dlgAfterFinish_.chkPlaySound->Checked)
				{
					String^ templ = I18N(L"It is set to play wav after task is finished. {0} Please check '[Option] -> [Process after Finish]' settings.");
					if (String::IsNullOrEmpty(dlgAfterFinish_.txtWav->Text))
					{
						CppUtils::Alert(String::Format(templ,I18N(L"But wav is empty.")));
						return;
					}
					if (!File::Exists(dlgAfterFinish_.txtWav->Text))
					{
						CppUtils::Alert(String::Format(templ, I18N(L"But wav does not exist.")));
						return;
					}
				}

				if (dlgAfterFinish_.chkLaunchApp->Checked)
				{
					String^ templ = I18N(L"It is set to launch app after task is finished. {0} Please check '[Option] -> [Process after Finish]' settings.");
					if (String::IsNullOrEmpty(dlgAfterFinish_.txtApp->Text) && 
						String::IsNullOrEmpty(dlgAfterFinish_.txtArg->Text))
					{
						CppUtils::Alert(String::Format(templ, I18N(L"But both 'Application' and 'Arguments' are empty.")));
						return;
					}

					// maybe in path
					//if (!String::IsNullOrEmpty(dlgAfterFinish_.txtApp->Text) &&
					//	!File::Exists(dlgAfterFinish_.txtApp->Text))
					//{
					//	CppUtils::Alert(String::Format(templ, I18N(L"But 'Application' does not exist.")));
					//	return;
					//}
				}
			}

			// show the dialog
			if (System::Windows::Forms::DialogResult::OK != codecDlg.ShowDialog())
				return;

			// confirms shutdown
			if (tsmiEnabledtsmiProcessAfterFinish->Checked)
			{
				if (dlgAfterFinish_.chkShutdown->Checked)
				{
					switch (CppUtils::CenteredMessageBox(
						I18N(L"System Shutdown is enabled after the task finished. Do you want to keep it enabled?"),
						Application::ProductName,
						MessageBoxButtons::YesNoCancel,
						MessageBoxIcon::Question,
						MessageBoxDefaultButton::Button2))
					{
					case System::Windows::Forms::DialogResult::Yes:
						break;
					case System::Windows::Forms::DialogResult::No:
						dlgAfterFinish_.chkShutdown->Checked = false;
						break;
					case System::Windows::Forms::DialogResult::Cancel:
						return;
					}
				}
			}

			// Create Task
			DASSERT(!IsTaskActive);
			encodeTask_ = gcnew EncodeTask();

			array<ListViewItem^>^ items = GetItems(itemSelection);
			DASSERT(items->Length == inputmovies->Length);
			DASSERT(items->Length == inputdurations->Length);
			DASSERT(items->Length == inputFpses->Length);
			encodeTask_->AddJob(
				codecDlg.IsConcat,
				codecDlg.IsReEncode,
				codecDlg.AdditonalOptionsBeforeInput, codecDlg.AdditonalOptionsAfterInput,
				items,
				inputmovies,
				codecDlg.OutputFiles,
				codecDlg.OutputVideoCodec,
				codecDlg.OutputAudioCodec,
				IsSameSizeVideos(itemSelection),
				GetMaxVideoSize(itemSelection),
				codecDlg.IsConcat ? gcnew array<AVDuration^>{ StatusTotalInputDuration } : inputdurations,
				codecDlg.IsConcat ? gcnew array<double>{TotalInputFPS} : inputFpses,
				codecDlg.IsMoveFinishedInputFiles);
			
			

			txtLogOut->Clear();
			txtLogErr->Clear();

			processTerminatedDuetoAppClose_ = false;
			elapses_.Clear();
			OnTaskStarted();
			
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

		System::Void FormMain::tsmiRemoveFromList_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (IsTaskActive)
				return;
			
			encodeTask_ = nullptr;

			while (lvInputs->SelectedItems->Count != 0)
			{
				lvInputs->SelectedItems[0]->Remove();
			}

			if(lvInputs->Items->Count==0)
			{
				StatusTotalInputFormat = String::Empty;
				StatusTotalInputAudioCodec = gcnew AVCodec();
				StatusTotalInputVideoCodec = gcnew AVCodec();
				StatusTotalInputDuration = gcnew AVDuration();
			}
			else
			{
				for each (ListViewItem ^ lvi in lvInputs->Items)
					CheckMovieAndSet(GetMovieFileFromLvi(lvi), false, false);
			}
		}
		System::Void FormMain::tsmiShowInputFileInExplorer_Click(System::Object^ sender, System::EventArgs^ e)
		{
			for each (ListViewItem ^ selItem in lvInputs->SelectedItems)
			{
				CppUtils::OpenFolder(this, GetMovieFileFromLvi(selItem));
			}
		}

		array<String^>^ FormMain::GetOutputtedMoviesFromList(bool bSelectedOnly)
		{
			List<String^>^ results = gcnew List<String^>();


			for each (ListViewItem ^ item in lvInputs->Items)
			{
				if (bSelectedOnly && !item->Selected)
					continue;
				if (item->ImageKey == IMAGEKEY_DONE ||
					item->ImageKey == IMAGEKEY_ENCODING)
				{
					results->Add(ListViewItemData::Get(item)->OutputtedFile);
				}
			}
			return MakeUnique(results)->ToArray();
		}
		System::Void FormMain::tsmiShowOutputFileInExplorer_Click(System::Object^ sender, System::EventArgs^ e)
		{
			for each(String^ file in GetOutputtedMoviesFromList(true))
				CppUtils::OpenFolder(this, file);
		}

		System::Void FormMain::tsmiOpenInputLocations_ClickCommon(System::Object^ sender, System::EventArgs^ e)
		{
			for each (String ^ inputmovie in GetInputMovies(ItemSelection::All))
			{
				OpenFolder((HWND)this->Handle.ToPointer(),
					getStdWstring(inputmovie).c_str());
			}
		}
		System::Void FormMain::tsmiOpenOutput_ClickCommon(System::Object^ sender, System::EventArgs^ e)
		{
			//String^ outputMovie;
			//do
			//{
			//	if (!encodeTask_)
			//	{
			//		if (!lastSummary_)
			//		{
			//			break;
			//		}
			//		outputMovie = lastSummary_->LastOutputMovie;
			//	}
			//	else if (encodeTask_->IsAllEnded())
			//	{
			//		outputMovie = lastSummary_->LastOutputMovie;
			//	}
			//	else
			//	{
			//		outputMovie = encodeTask_->CurrentOutputtingMovieFile;
			//	}
			//} while (false);


			//if (String::IsNullOrEmpty(outputMovie))
			//{
			//	CppUtils::Alert(this, I18N(STR_NO_OUTPUT_MOVIE));
			//	return;
			//}

			//OpenFolder((HWND)this->Handle.ToPointer(),
			//	getStdWstring(outputMovie).c_str());

			for each (String ^ file in GetOutputtedMoviesFromList(false))
				CppUtils::OpenFolder(this, file);

		}
		System::Void FormMain::tsmiProcesstsmiProcessAfterFinish_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (System::Windows::Forms::DialogResult::OK != dlgAfterFinish_.ShowDialog())
			{
				return;
			}

			HashIni^ ini = Profile::ReadAll(Program::IniFile);
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
		System::Void FormMain::tsmiEnableHalfUpper_Click(System::Object^ sender, System::EventArgs^ e)
		{
			cpuAffinity_.EnableUpperHalf();
			tsmiCPUAffinityEnable_Click(nullptr, nullptr);
		}
		System::Void FormMain::tsmiEnableHalfEven_Click(System::Object^ sender, System::EventArgs^ e)
		{
			cpuAffinity_.EnableEvenHalf();
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

