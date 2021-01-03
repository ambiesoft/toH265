#pragma once

#include "AVCodec.h"
#include "AVDuration.h"
#include "toH265.h"

namespace Ambiesoft {
	namespace toH265 {
		using namespace System;

		// ref class EncodeResult;

		public ref class EncodeJob
		{
			bool ended_ = false;
			int retval_ = -1;
			String^ TempFile;
			DateTime^ finishData_;

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
			property String^ OutputtingMovie;
			property String^ OutputtedMovie
			{
				String^ get() {
					if (ended_)
						return OutputtingMovie;
					return String::Empty;
				}
			}
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
				this->OutputtingMovie = outputtingMovie;
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
				finishData_ = DateTime::Now;
			}
			String^ GetArg(String^% report);
			property int RetVal 
			{
				int get() { return retval_; }
			}
			property String^ FinishedDateString
			{
				String^ get() {
					if (finishData_)
						return finishData_->ToString();
					return String::Empty;
				}
			}
			void PrintInputFiles(System::Text::StringBuilder^ sb) {
				sb->AppendLine(I18N(L"Input Files:"));
				for each (String ^ file in inputMovies_)
				{
					sb->Append(L"  ");
					sb->AppendLine(file);
				}
			}
			void PrintOutputFile(System::Text::StringBuilder^ sb) {
				sb->AppendLine(I18N(L"Output File:"));
				sb->Append(L"  ");
				sb->AppendLine(OutputtedMovie);
			}
		};

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
		public:

			void AddJob(
				bool bConcat,
				bool bReEncode,
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
					EncodeJob^ job = gcnew EncodeJob(bReEncode,
						inputFiles,
						outputFiles[0],
						outputV,
						outputA,
						bIsSameSize,
						maxsize,
						durations[0],
						fpses[0]);
					jobs_.Add(job);
				}
				else
				{
					// each file
					DASSERT(inputFiles->Length > 0);
					DASSERT(inputFiles->Length == outputFiles->Length);
					DASSERT(inputFiles->Length == durations->Length);
					DASSERT(inputFiles->Length == fpses->Length);
					for(int i=0;i<inputFiles->Length;++i)
					{
						EncodeJob^ job = gcnew EncodeJob(inputFiles[i],
							outputFiles[i],
							outputV, outputA,
							maxsize,
							durations[i],
							fpses[i]);
						jobs_.Add(job);
					}
				}
			}
			property String^ CurrentOutputtingMovieFile
			{
				String^ get() { return CurrentJob->OutputtingMovie; }
			}

			void OnTaskEnded(int retval)
			{
				DASSERT(currentI_ >= 0);
				CurrentJob->SetEnded(retval);
			}
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