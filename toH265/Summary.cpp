#include "stdafx.h"

#include "helper.h"
#include "Summary.h"

using namespace System::Text;

namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Windows::Forms;

		void SummaryText::Show(System::Windows::Forms::Form^ form)
		{
			JR::Utils::GUI::Forms::FlexibleMessageBox::Show(
				form,
				Text,
				Application::ProductName,
				MessageBoxButtons::OK,
				IsWarning ? MessageBoxIcon::Warning : MessageBoxIcon::Information);
		}

		void Summary::AddOverallResult(EncodeTask^ encodeTask)
		{
			System::Text::StringBuilder sb;
			sb.AppendLine(I18N(L"Task Ended"));
			sb.AppendLine(I18N("Start Time") + L": " + encodeTask->StartTime.ToString());
			sb.AppendLine(I18N(L"Finish Time") + L":" + System::DateTime::Now.ToString());
			sb.AppendLine(I18N(L"Elapsed Time") + L": " +
				ToCommonDurationString(System::DateTime::Now - encodeTask->StartTime));

			overallResult_ = sb.ToString();
		}

		SummaryText^ Summary::GetSummaryText()
		{
			bool isWarning = false;
			StringBuilder sb;
			sb.AppendLine(overallResult_);
			sb.AppendLine("==============================");
			for each (Paper ^ paper in papers_)
			{
				isWarning |= paper->IsWarning;
				sb.Append(paper->Message);
				sb.AppendLine("==============================");
			}
			return gcnew SummaryText(sb.ToString(), isWarning);
		}
		void Summary::Show(Form^ form)
		{
			SummaryText^ st = GetSummaryText();
			st->Show(form);
		}
	}
}