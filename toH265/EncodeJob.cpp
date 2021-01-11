#include "stdafx.h"
#include "EncodeTask.h"
#include "ListViewItemData.h"

using namespace System::Text;
using namespace System::IO;

namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Collections::Generic;
		using namespace System::Windows::Forms;


		void EncodeJob::init(bool bReEncode,
			String^ addiopbi, String^ addiopai,
			array<System::Windows::Forms::ListViewItem^>^ items,
			array<String^>^ inputMovies,
			String^ outputtingMovie,
			AVCodec^ outputVideoCodec,
			AVCodec^ outputAudioCodec,
			bool isSameSize,
			System::Drawing::Size maxSize,
			AVDuration^ totalInputDuration,
			double totalInputFPS,
			double partPercent)
		{
			this->ReEncode = bReEncode;
			this->AdditionalOptionsBeforeInput = addiopbi;
			this->AdditionalOptionsAfterInput = addiopai;
			this->items_ = items;
			this->inputMovies_ = inputMovies;
			this->OutputtingMovie = outputtingMovie;
			this->OutputVideoCodec = outputVideoCodec;
			this->OutputAudioCodec = outputAudioCodec;
			this->IsSameSize = isSameSize;
			this->MaxSize = maxSize;
			this->totalInputDuration_ = totalInputDuration;
			this->totalInputFPS_ = totalInputFPS;
			this->partPercent_ = partPercent;

			for each (System::Windows::Forms::ListViewItem ^ item in items_)
			{
				item->ImageKey = IMAGEKEY_NORMAL;
				this->inputVideoCodec_ = FormMain::GetVCodecFromLvi(item);
				this->inputAudioCodec_ = FormMain::GetACodecFromLvi(item);
			}
		}
		void EncodeJob::CreateTempFile()
		{
			DASSERT(String::IsNullOrEmpty(tempFile_));
			tempFile_ = Path::GetTempFileName();
		}
		void EncodeJob::DeleteTempFile()
		{
			if (String::IsNullOrEmpty(tempFile_))
				return;

			try
			{
				String^ all = File::ReadAllText(tempFile_);
				if (all == report_)
				{
					File::Delete(tempFile_);
					tempFile_ = nullptr;
				}
			}
			catch(Exception^)
			{ }
		}

		String^ EncodeJob::GetArg(String^% report)
		{
			String^ arg;
			StringBuilder sbReportFile;
			StringBuilder sbFileText;
			DASSERT(InputMovies->Length != 0);
			if (InputMovies->Length == 1)
			{
				DASSERT(!String::IsNullOrEmpty(OutputtingMovie));
				arg = String::Format(L"-y {4} -i \"{0}\" {5} -max_muxing_queue_size 9999 -c copy -c:v {1} -c:a {2} \"{3}\"",
					InputMovies[0],
					OutputVideoCodec->ToFFMpegString(),
					OutputAudioCodec->ToFFMpegString(),
					OutputtingMovie,
					AdditionalOptionsBeforeInput,
					AdditionalOptionsAfterInput);
			}
			else
			{
				if (!ReEncode)
				{
					CreateTempFile();
					sbReportFile.AppendLine(tempFile_);
					
					
					StreamWriter sw(tempFile_, false, gcnew UTF8Encoding(false));

					for each (String ^ file in InputMovies)
					{
						String^ line = String::Format("file '{0}'", file);
						sbReportFile.AppendLine(line);
						sw.WriteLine(line);
						sbFileText.AppendLine(line);
					}
					sbReportFile.AppendLine();

					arg = String::Format("-y -safe 0 -f concat {2} -i \"{0}\" {3} -max_muxing_queue_size 9999 -c copy \"{1}\"",
						tempFile_,
						OutputtingMovie,
						AdditionalOptionsBeforeInput,
						AdditionalOptionsAfterInput);
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
					sb.Append(AdditionalOptionsBeforeInput + " ");
					for each (String ^ f in InputMovies)
					{
						sb.AppendFormat("-i \"{0}\" ", f);
					}
					sb.Append(AdditionalOptionsAfterInput + " ");
					sb.Append("-max_muxing_queue_size 9999 ");

					sb.Append("-filter_complex \"");
					if (IsSameSize) // IsSameSizeVideos())
					{
						for (int i = 0; i < InputMovies->Length; ++i)
						{
							sb.AppendFormat("[{0}:v:0]", i);
							sb.AppendFormat("[{0}:a:0]", i);
						}
						sb.AppendFormat("concat=n={0}:v=1:a=1[v][a]", InputMovies->Length);
					}
					else
					{
						System::Drawing::Size size = MaxSize; // GetMaxVideoSize();
						for (int i = 0; i < InputMovies->Length; ++i)
						{
							sb.AppendFormat("[{0}:v:0]", i);
							sb.AppendFormat("scale={1}:{2}:force_original_aspect_ratio=decrease,pad={1}:{2}:(ow-iw)/2:(oh-ih)/2[v{0}];",
								i, size.Width, size.Height);
						}
						for (int i = 0; i < InputMovies->Length; ++i)
						{
							sb.AppendFormat("[v{0}][{0}:a:0]", i);
						}
						sb.AppendFormat("concat=n={0}:v=1:a=1[v][a]", InputMovies->Length);
					}
					sb.Append("\" ");

					sb.Append("-map \"[v]\" -map \"[a]\" ");

					sb.AppendFormat("-c:v {0} -c:a {1} \"{2}\"",
						OutputVideoCodec->ToFFMpegString(),
						OutputAudioCodec->ToFFMpegString(),
						OutputtingMovie);

					arg = sb.ToString();
				}
			}
			report = sbReportFile.ToString();
			report_ = sbFileText.ToString();
			return arg;
		}
		
		void EncodeJob::SetStarted()
		{
			for each (ListViewItem ^ item in items_)
			{
				item->ImageKey = IMAGEKEY_ENCODING;
				ListViewItemData::Get(item)->OutputtingFile = this->OutputtingMovie;
				ListViewItemData::Get(item)->OutputtedFile = nullptr;
				item->EnsureVisible();
			}
		}
		void EncodeJob::SetEnded(int retval)
		{
			ended_ = true;
			retval_ = retval;
			finishData_ = DateTime::Now;
			DeleteTempFile();
			for each (ListViewItem ^ item in items_)
			{
				item->ImageKey = retval==0 ? IMAGEKEY_DONE : IMAGEKEY_NORMAL;
				ListViewItemData::Get(item)->OutputtedFile = this->OutputtedMovie;
			}
		}
		void EncodeJob::Cancel()
		{
			DASSERT(!IsEnded);
			for each (ListViewItem ^ item in items_)
				item->ImageKey = IMAGEKEY_NORMAL;
		}
	}
}