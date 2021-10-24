#include "stdafx.h"

#include "toH265.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		void FormMain::OnBtnBrowseMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		{
			listTip_->Hide(lvInputs);
			lastTPPos_ = System::Drawing::Point(0, 0);
		}
		void FormMain::OnListViewMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		{
			do {
				if (bDragging_)
					break;

				ListViewItem^ item = lvInputs->GetItemAt(0, e->Y);
				if (!item)
					break;

				ListViewItem::ListViewSubItem^ subItem = item->GetSubItemAt(e->X, e->Y);
				if (!subItem)
					break;
				
				int subItemIndex = -1;
				for (int i = 0; i < item->SubItems->Count; ++i)
				{
					if (item->SubItems[i] == subItem)
					{
						subItemIndex = i;
						break;
					}
				}
				System::Collections::Generic::List<int> order;
				for (int i = 0; i < lvInputs->Columns->Count; ++i)
					order.Add(i);
				System::Drawing::Rectangle subItemRect = toH265Helper::GetSubItemBounds(lvInputs,
					item,
					subItemIndex,
					order.ToArray());
				System::Drawing::Point pt(subItemRect.Left, subItemRect.Top);
				if (lastTPPos_.X == pt.X && lastTPPos_.Y == pt.Y)
					return;
				lastTPPos_.X = pt.X;
				lastTPPos_.Y = pt.Y;
				//if (lastTPPos_.X == e->X && lastTPPos_.Y == e->Y)
				//	return;
				//lastTPPos_.X = e->X;
				//lastTPPos_.Y = e->Y;
				// listTip_->ToolTipTitle = subItem->Text;
				listTip_->Show(subItem->Text, lvInputs,
					lastTPPos_.X,
					lastTPPos_.Y,
					5000);
#ifndef NDEBUG
				static int count;
				DTRACE(String::Format(L"{3}, X={0},Y={1},Index={2}", e->X, e->Y, item->Index, ++count));
#endif
				return;
			} while (false);

			listTip_->Hide(lvInputs);
			lastTPPos_ = System::Drawing::Point(0, 0);
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