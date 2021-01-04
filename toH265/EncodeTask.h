#pragma once

#include "AVCodec.h"
#include "AVDuration.h"
#include "toH265.h"
#include "EncodeJob.h"

namespace Ambiesoft {
	namespace toH265 {
		using namespace System;

		public ref class EncodeTask
		{
			int currentI_ = -1;
			System::Collections::Generic::List<EncodeJob^> jobs_;

			property EncodeJob^ CurrentJob
			{
				EncodeJob^ get() { 
					return jobs_[currentI_]; 
				}
			}
			property bool IsCurrentValid
			{
				bool get() {
					return 0 <= currentI_ && currentI_ < jobs_.Count;
				}
			}
		public:

			void AddJob(
				bool bConcat,
				bool bReEncode,
				array<System::Windows::Forms::ListViewItem^>^ items,
				array<String^>^ inputFiles,
				array<String^>^ outputFiles,
				AVCodec^ outputV, AVCodec^ outputA,
				bool bIsSameSize,
				System::Drawing::Size maxsize,
				array<AVDuration^>^ durations,
				array<double>^ fpses)
			{
				DASSERT(currentI_ <= 0);
				currentI_ = 0;
				if (bConcat)
				{
					DASSERT(outputFiles->Length == 1);
					DASSERT(durations->Length == 1);
					DASSERT(fpses->Length == 1);
					EncodeJob^ job = gcnew EncodeJob(
						bReEncode,
						items,
						inputFiles,
						outputFiles[0],
						outputV,
						outputA,
						bIsSameSize,
						maxsize,
						durations[0],
						fpses[0],
						1.0);
					jobs_.Add(job);
				}
				else
				{
					// each file
					DASSERT(inputFiles->Length > 0);
					DASSERT(inputFiles->Length == outputFiles->Length);
					DASSERT(inputFiles->Length == durations->Length);
					DASSERT(inputFiles->Length == fpses->Length);
					double totalDuration = 0;
					for (int i = 0; i < inputFiles->Length; ++i)
					{
						totalDuration += durations[i]->TotalMilliseconds;
					}
					for (int i = 0; i < inputFiles->Length; ++i)
					{
						EncodeJob^ job = gcnew EncodeJob(
							items[i],
							inputFiles[i],
							outputFiles[i],
							outputV, outputA,
							maxsize,
							durations[i],
							fpses[i],
							durations[i]->TotalMilliseconds / totalDuration
						);
						jobs_.Add(job);
					}
				}
			}
			property String^ CurrentOutputtingMovieFile
			{
				String^ get() { return CurrentJob->OutputtingMovie; }
			}

			void OnTaskStarted();
			void OnTaskEnded(int retval);
			
			void GoNext()
			{
				++currentI_;
			}
			String^ GetArg(String^% report)
			{
				return CurrentJob->GetArg(report);
			}
			property AVCodec^ CurrentOutputVideoCodec
			{
				AVCodec^ get() { return CurrentJob->OutputVideoCodec; }
			}
			property AVCodec^ CurrentOutputAudioCodec
			{
				AVCodec^ get() { return CurrentJob->OutputAudioCodec; }
			}

			bool IsAllEnded() {
				DASSERT(currentI_ >= 0);
				bool ret = currentI_ >= jobs_.Count;
#ifdef _DEBUG
				if (ret)
				{
					for each (EncodeJob ^ job in jobs_)
						DASSERT(job->IsEnded);
				}
#endif
				return ret;
			}

			array<EncodeJob^>^ GetResults();

			property double CurrentTotalMilliseconds
			{
				double get() { return CurrentJob->TotalInputDuration->TotalMilliseconds; }
			}
			property double CurrentPartPercent
			{
				double get() { return CurrentJob->PartPercent; }
			}
			property double EndedPartPercent
			{
				double get() {
					double ret = 0;
					for each (EncodeJob ^ job in jobs_)
					{
						if (!job->IsEnded)
							return ret;
						ret += job->PartPercent;
					}
					DASSERT(0 <= ret && ret <= 1.0);
					return ret;
				}
			}
			property double EndedDurations
			{
				double get() {
					double ret = 0;
					for each (EncodeJob ^ job in jobs_)
					{
						if (!job->IsEnded)
							return ret;
						ret += job->TotalInputDuration->TotalMilliseconds;
					}
					return ret;
				}
			}

			void Cancel() {
				if (IsCurrentValid)
					CurrentJob->Cancel();
			}
		};


		//public ref class EncodeResult
		//{
		//public:
		//	property String^ OutputtedMovie;
		//	array<String^>^ InputMovies;
		//	AVDuration^ TotalInputDuration;
		//	double TotalInputFPS;
		//	EncodeResult(String^ outputtedMovie,
		//		array<String^>^ inputMovies,
		//		AVDuration^ totalInputDuration,
		//		double totalInputFPS)
		//	{
		//		OutputtedMovie = outputtedMovie;
		//		InputMovies = inputMovies;
		//		TotalInputDuration = totalInputDuration;
		//		TotalInputFPS = totalInputFPS;
		//	}
		//};
	}
}