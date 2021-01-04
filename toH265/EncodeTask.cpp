#include "stdafx.h"
#include "EncodeTask.h"

using namespace System::Text;
using namespace System::IO;

namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Collections::Generic;
		using namespace System::Windows::Forms;

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
		
		void EncodeTask::OnTaskStarted()
		{
			DASSERT(currentI_ >= 0);
			CurrentJob->SetStarted();
		}
		void EncodeTask::OnTaskEnded(int retval)
		{
			DASSERT(currentI_ >= 0);
			CurrentJob->SetEnded(retval);
		}

	}
}