#include "stdafx.h"
#include "InputListView.h"


namespace Ambiesoft {
	namespace toH265 {
		void InputListView::WndProc(System::Windows::Forms::Message% m)
		{
			ParentClass::WndProc(m);

            switch (m.Msg) {
            case 0x1007:    //ListViewItemAdd-A
                ItemCountChanged();
                break;
            case 0x104D:    //ListViewItemAdd-W
                ItemCountChanged();
                break;
                //edit for itemremove
            case 0x1008:    // Item removed
                this->BeginInvoke(gcnew VVDelegate(this, &InputListView::OnAfterItemCountChanging));
                break;
            case 0x1009:    // Item removed all
                this->BeginInvoke(gcnew VVDelegate(this, &InputListView::OnAfterItemCountChanging));
                break;
            default:
                break;
            }
		}
	}
}