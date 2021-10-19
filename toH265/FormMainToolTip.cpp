#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		void FormMain::OnListViewMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		{
			if (bDragging_)
			{
				listTip_->Hide(lvInputs);
				return;
			}

			ListViewItem^ item = lvInputs->GetItemAt(0, e->Y);
			if (!item)
			{
				listTip_->Hide(lvInputs);
				return;
			}
			ListViewItem::ListViewSubItem^ subItem = item->GetSubItemAt(e->X, e->Y);
			if (!subItem)
			{
				listTip_->Hide(lvInputs);
				return;
			}

			if (lastTPPos_.X == e->X && lastTPPos_.Y == e->Y)
				return;
			lastTPPos_.X = e->X;
			lastTPPos_.Y = e->Y;

			listTip_->Show(subItem->Text, lvInputs, e->X + 16, e->Y + 16, 5000);
#ifndef NDEBUG
			static int count;
			DTRACE(String::Format(L"{3}, X={0},Y={1},Index={2}", e->X, e->Y, item->Index, ++count));
#endif
		}


		void FormMain::OnListViewMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		{
			listTip_->Hide(lvInputs);
			bDragging_ = true;
		}


		void FormMain::OnListViewMouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		{
			bDragging_ = false;
		}
	}
}