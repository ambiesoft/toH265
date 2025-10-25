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

			if (retval == 0 && CurrentJob->IsMoveFinishedInputMovies)
			{
				List<String^> inEnded;
				for each (String ^ inputMovie in CurrentJob->InputMovies)
				{
					int maxTry = 256;
					bool gaveup = false;
					String^ movedFile = String::Empty;
					for (int i = 0; ; ++i)
					{
						String^ movingFolder = L"old";
						String^ folderForEncoded = Path::GetDirectoryName(inputMovie);
						String^ filenameWE = Path::GetFileNameWithoutExtension(inputMovie);
						String^ num = i == 0 ? String::Empty :
							String::Format(L"({0})", i);
						String^ ext = Path::GetExtension(inputMovie);

						movedFile = Path::Combine(
							folderForEncoded,
							movingFolder,
							filenameWE + num + ext);
						if(i >= maxTry)
						{
							gaveup = true;
							break;
						}
						if (File::Exists(movedFile) || Directory::Exists(movedFile))
							continue;
						break;
					}
					if (gaveup)
					{
						inEnded.Add(inputMovie);
						continue;
					}

					try 
					{
						Directory::CreateDirectory(Path::GetDirectoryName(movedFile));
						File::Move(inputMovie, movedFile);
						inEnded.Add(movedFile);
					}
					catch(Exception^ ex)
					{
						CurrentJob->AddMoveFailed(
							String::Format(
								I18N(L"Failed to move '{0}' to '{1}'. ({2})"),
								inputMovie, movedFile, ex->Message));
						inEnded.Add(inputMovie);
					}
				}
				CurrentJob->SetEndedInputMovies(inEnded.ToArray());
			}
		}

	}
}