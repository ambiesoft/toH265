#include "stdafx.h"
#include "Summary.h"
using namespace System::Text;

namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Windows::Forms;

		void Summary::Show(IWin32Window^ win)
		{
			bool isWarning = false;
			StringBuilder sb;
			for each (Paper ^ paper in papers_)
			{
				isWarning |= paper->IsWarning;
				sb.AppendLine(paper->Message);
				sb.AppendLine("==============================");
			}

			CppUtils::CenteredMessageBox(
				win,
				sb.ToString(),
				Application::ProductName,
				MessageBoxButtons::OK,
				isWarning ? MessageBoxIcon::Warning : MessageBoxIcon::Information);
		}
	}
}