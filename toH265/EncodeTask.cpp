#include "stdafx.h"
#include "EncodeTask.h"

using namespace System::Text;
using namespace System::IO;

namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Collections::Generic;

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
				arg = String::Format(L"-y -i \"{0}\" -max_muxing_queue_size 9999 -c copy -c:v {1} -c:a {2} \"{3}\"",
					InputMovies[0],
					OutputVideoCodec->ToFFMpegString(),
					OutputAudioCodec->ToFFMpegString(),
					OutputtingMovie);
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

					arg = String::Format("-y -safe 0 -f concat -i \"{0}\" -max_muxing_queue_size 9999 -c copy \"{1}\"",
						tempFile_,
						OutputtingMovie);
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

					for each (String ^ f in InputMovies)
						sb.AppendFormat("-i \"{0}\" ", f);

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
		
		array<EncodeJob^>^ EncodeTask::GetResults()
		{
			DASSERT(IsAllEnded());
			List<EncodeJob^> ret;
			for each (EncodeJob ^ job in jobs_)
			{
				//EncodeResult^ result = gcnew EncodeResult(
				//	job->OutputtingMove,
				//	job->InputMovies,
				//	job->TotalInputDuration,
				//	job->TotalInputFPS);

				ret.Add(job);
			}
			return ret.ToArray();
		}
		
	}
}