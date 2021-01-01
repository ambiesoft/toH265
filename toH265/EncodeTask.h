#pragma once

#include "AVCodec.h"
#include "AVDuration.h"

namespace Ambiesoft {
	namespace toH265 {
		using namespace System;

		// ref class EncodeResult;

		public ref class EncodeJob
		{
			bool ended_ = false;
			int retval_ = -1;
			String^ TempFile;

			bool ReEncode = false;
			array<String^>^ inputMovies_;
			bool IsSameSize;
			System::Drawing::Size MaxSize;
			AVDuration^ totalInputDuration_;
			double totalInputFPS_;

			AVCodec^ outputVideoCodec_ = gcnew AVCodec();
			AVCodec^ outputAudioCodec_ = gcnew AVCodec();

		public:
			property array<String^>^ InputMovies
			{
				array<String^>^ get() { return inputMovies_; }
			}
			property AVDuration^ TotalInputDuration
			{
				AVDuration^ get() { return totalInputDuration_; }
			}
			property double TotalInputFPS
			{
				double get() { return totalInputFPS_; }
			}
			property String^ OutputtingMove;
			property AVCodec^ OutputVideoCodec
			{
				AVCodec^ get() { return outputVideoCodec_; }
				void set(AVCodec^ v) {
					outputVideoCodec_ = v;
					// SetCodecStatusText();
				}
			}
			property AVCodec^ OutputAudioCodec
			{
				AVCodec^ get() { return outputAudioCodec_; }
				void set(AVCodec^ v) {
					outputAudioCodec_ = v;
					// SetCodecStatusText();
				}
			}

			void init(bool bReEncode,
				array<String^>^ inputMovies,
				String^ outputtingMovie,
				AVCodec^ outputVideoCodec,
				AVCodec^ outputAudioCodec,
				bool isSameSize,
				System::Drawing::Size maxSize,
				AVDuration^ totalInputDuration,
				double totalInputFPS)
			{
				this->ReEncode = bReEncode;
				this->inputMovies_ = inputMovies;
				this->OutputtingMove = outputtingMovie;
				this->OutputVideoCodec = outputVideoCodec;
				this->OutputAudioCodec = outputAudioCodec;
				this->IsSameSize = isSameSize;
				this->MaxSize = maxSize;
				this->totalInputDuration_ = totalInputDuration;
				this->totalInputFPS_ = totalInputFPS;
			}

			// each
			EncodeJob(String^ inputMovie,
				String^ outputtingMovie,
				AVCodec^ outputVideoCodec,
				AVCodec^ outputAudioCodec,
				System::Drawing::Size size,
				AVDuration^ duration,
				double fps)
			{
				init(true,
					gcnew array<String^>{inputMovie},
					outputtingMovie,
					outputVideoCodec,
					outputAudioCodec,
					true,
					size,
					duration,
					fps);
			}
			// concat
			EncodeJob(bool bReEncode,
				array<String^>^ inputMovies,
				String^ outputtingMovie,
				AVCodec^ outputVideoCodec,
				AVCodec^ outputAudioCodec,
				bool isSameSize,
				System::Drawing::Size maxSize,
				AVDuration^ totalInputDuration,
				double totalInputFPS)
			{
				init(bReEncode,
					inputMovies,
					outputtingMovie,
					outputVideoCodec,
					outputAudioCodec,
					isSameSize,
					maxSize,
					totalInputDuration,
					totalInputFPS);
			}
			property bool IsEnded
			{
				bool get() { return ended_; }
			}
			void SetEnded(int retval)
			{
				ended_ = true;
				retval_ = retval;
			}
			String^ GetArg(String^% report);
		};

		public ref class EncodeTask
		{
			int currentI_ = -1;
			System::Collections::Generic::List<EncodeJob^> jobs_;

			property EncodeJob^ CurrentJob
			{
				EncodeJob^ get() { return jobs_[currentI_]; }
			}
		public:

			void AddJob(
				bool bConcat,
				bool bReEncode,
				array<String^>^ inputFiles,
				String^ outputFile,
				AVCodec^ outputV, AVCodec^ outputA,
				bool bIsSameSize,
				System::Drawing::Size maxsize,
				AVDuration^ totalDuration,
				double totalFPS)
			{
				EncodeJob^ job;
				if (bConcat)
				{
					job = gcnew EncodeJob(bReEncode,
						inputFiles,
						outputFile,
						outputV,
						outputA,
						bIsSameSize,
						maxsize,
						totalDuration,
						totalFPS);
				}
				else
				{
					// each file
					DASSERT(inputFiles->Length == 1);
					job = gcnew EncodeJob(inputFiles[0],
						outputFile,
						outputV, outputA,
						maxsize,
						totalDuration,
						totalFPS);
				}
				jobs_.Add(job);
			}
			property String^ CurrentOutputtingMovieFile
			{
				String^ get() { return CurrentJob->OutputtingMove; }
			}

			void OnTaskEnded(int retval)
			{
				DASSERT(currentI_ >= 0);
				if (retval != 0)
				{
					//CppUtils::Alert(this, String::Format(L"Process exited with {0}.", retval));
					CurrentJob->SetEnded(retval);
				}
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
				DASSERT(currentI_ > 0);
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