#pragma once

#include "EncodeJob.h"
#include "EncodeTask.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System;

		ref class EncodeJob;

		ref class Paper
		{
			bool hasWarning_;
			String^ text_;
			EncodeJob^ job_;

		public:
			Paper(bool hasWaring, String^ text, EncodeJob^ job) :
				hasWarning_(hasWaring),
				text_(text),
				job_(job) {}
			property String^ OutputtedMovie
			{
				String^ get() { return job_->OutputtedMovie; }
			}
			property bool IsWarning
			{
				bool get() { return hasWarning_; }
			}
			property String^ Message
			{
				String^ get() { return text_; }
			}
		};
		public ref class Summary
		{
			String^ overallResult_ = String::Empty;
			System::Collections::Generic::List<Paper^> papers_;

		public:
			void AddOverallResult(EncodeTask^ encodeTask);
			void AddPaper(bool hasWarning, String^ text, EncodeJob^ job)
			{
				papers_.Add(gcnew Paper(hasWarning, text, job));
			}

			void Show(System::Windows::Forms::Form^ form);

			property String^ LastOutputMovie
			{
				String^ get() {
					if (papers_.Count == 0)
						return nullptr;
					for (int i = papers_.Count - 1; i >= 0; --i)
						if (!String::IsNullOrEmpty(papers_[i]->OutputtedMovie))
							return papers_[i]->OutputtedMovie;
					return nullptr;
				}
			}
		};

	}
}