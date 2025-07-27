#include "stdafx.h"

#include "toH265.h"
#include "TimerMouseMoveData.h"
#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		void FormMain::OnListTipHided(System::Object^ sender, System::EventArgs^ e)
		{
			DASSERT(timerMouseMove->Tag);
			TimerMouseMoveData^ timerData = (TimerMouseMoveData^)timerMouseMove->Tag;
			timerData->SetWillShowText(L"", System::Windows::Forms::Cursor::Position, System::Drawing::Point(0, 0));
		}

		void FormMain::OnBtnBrowseMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		{
			listTip_->Hide(lvInputs);
			lastTPPos_ = System::Drawing::Point(0, 0);
		}

		System::Void FormMain::timerMouseMove_Tick(System::Object^ sender, System::EventArgs^ e)
		{
			DTRACE(timerMouseMove->Enabled);
			TimerMouseMoveData^ timerData = (TimerMouseMoveData^)timerMouseMove->Tag;
			DASSERT_IS_CLASS(timerData, TimerMouseMoveData);
			
			if (toH265Helper::FindControlAtCursor(this)==lvInputs)
			{
				listTip_->Show(timerData->WillShowText, lvInputs,
					timerData->X,
					timerData->Y,
					TimerMouseMoveData::SHOW_DURATION);
			}
			else 
				DTRACE("Not in list view");

			timerMouseMove->Enabled = false;
			timerData->SetLastShowTick(System::Environment::TickCount);
			timerData->SetMousePos(System::Windows::Forms::Cursor::Position);
		}
		void FormMain::OnListViewMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
		{
			DASSERT(timerMouseMove->Tag);
			TimerMouseMoveData^ timerData = (TimerMouseMoveData^)timerMouseMove->Tag;

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

				if (timerData->IsWillShowData(subItem->Text))
				{
					DTRACE("IsWillShowData");
					if (!timerMouseMove->Enabled)
					{
						DTRACE("timer not enabled");
						if (timerData->HasShowPeriodPassed)
						{
							DTRACE("HasShowPeriodPassed");
							if (!timerData->IsDataSame(subItem->Text, System::Windows::Forms::Cursor::Position))
							{
								DTRACE("!timerData->IsDataSame");
								timerData->SetWillShowText(subItem->Text, System::Windows::Forms::Cursor::Position, pt);
								timerMouseMove->Enabled = false;
								timerMouseMove->Interval = 500;
								timerMouseMove->Enabled = true;
							}
						}
					}
				}
				else
				{
					DTRACE("NOT timerData->IsWillShowData");
					timerData->SetWillShowText(subItem->Text, System::Windows::Forms::Cursor::Position, pt);
					timerMouseMove->Enabled = false;
					timerMouseMove->Interval = 500;
					timerMouseMove->Enabled = true;
				}

				if (lastTPPos_.X == pt.X && lastTPPos_.Y == pt.Y)
					return;

				lastTPPos_.X = pt.X;
				lastTPPos_.Y = pt.Y;
			
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