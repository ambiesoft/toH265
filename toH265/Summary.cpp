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
			sb.AppendLine("==============================");
			for each (Paper ^ paper in papers_)
			{
				isWarning |= paper->IsWarning;
				sb.Append(paper->Message);
				sb.AppendLine("==============================");
			}

			//CppUtils::CenteredMessageBox(
			//	win,
			//	sb.ToString(),
			//	Application::ProductName,
			//	MessageBoxButtons::OK,
			//	isWarning ? MessageBoxIcon::Warning : MessageBoxIcon::Information);

			JR::Utils::GUI::Forms::FlexibleMessageBox::Show(win,
				sb.ToString(),
				Application::ProductName,
				MessageBoxButtons::OK,
				isWarning ? MessageBoxIcon::Warning : MessageBoxIcon::Information);
		}
	}
}