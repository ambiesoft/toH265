#pragma once

#include "AVCodec.h"
#include "AVDuration.h"
#include "toH265.h"


#include "EncodeTask.h"

namespace Ambiesoft {
	namespace toH265 {
		using namespace System;

		public ref class EncodeJob
		{
			bool ended_ = false;
			int retval_ = -1;
			String^ tempFile_;
			String^ report_;
			DateTime^ finishData_;

			bool ReEncode = false;
			String^ AdditionalOptionsBeforeInput;
			String^ AdditionalOptionsAfterInput;

			array<String^>^ inputMovies_;
			array<System::Windows::Forms::ListViewItem^>^ items_;
			bool IsSameSize;
			System::Drawing::Size MaxSize;
			AVDuration^ totalInputDuration_;
			double totalInputFPS_;
			double partPercent_;
			AVCodec^ inputVideoCodec_ = gcnew AVCodec();
			AVCodec^ inputAudioCodec_ = gcnew AVCodec();
			AVCodec^ outputVideoCodec_ = gcnew AVCodec();
			AVCodec^ outputAudioCodec_ = gcnew AVCodec();

			void CreateTempFile();
			void DeleteTempFile();
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
			property AVCodec^ InputVideoCodec
			{
				AVCodec^ get() { return inputVideoCodec_; }
			}
			property AVCodec^ InputAudioCodec
			{
				AVCodec^ get() { return inputAudioCodec_; }
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
			property double PartPercent
			{
				double get() { return partPercent_; }
			}
			void init(bool bReEncode,
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
				double partPercent);

			// each
			EncodeJob(
				String^ addiopbi, String^ addiopai,
				System::Windows::Forms::ListViewItem^ item,
				String^ inputMovie,
				String^ outputtingMovie,
				AVCodec^ outputVideoCodec,
				AVCodec^ outputAudioCodec,
				System::Drawing::Size size,
				AVDuration^ duration,
				double fps,
				double partPercent)
			{
				init(true,
					addiopbi, addiopai,
					gcnew array<System::Windows::Forms::ListViewItem^>{item},
					gcnew array<String^>{inputMovie},
					outputtingMovie,
					outputVideoCodec,
					outputAudioCodec,
					true,
					size,
					duration,
					fps,
					partPercent);
			}
			// concat
			EncodeJob(bool bReEncode,
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
				init(bReEncode,
					addiopbi, addiopai,
					items,
					inputMovies,
					outputtingMovie,
					outputVideoCodec,
					outputAudioCodec,
					isSameSize,
					maxSize,
					totalInputDuration,
					totalInputFPS,
					partPercent);
			}
			property bool IsEnded
			{
				bool get() { return ended_; }
			}

			void SetStarted();
			void SetEnded(int retval);
			void Cancel();

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
	}
}