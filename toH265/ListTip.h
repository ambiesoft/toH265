#pragma once

using namespace System;
using namespace System::Windows::Forms;

namespace Ambiesoft {
    namespace toH265 {
        public ref class ListTip : public ToolTip
        {
        public:
            event EventHandler^ Hided;
            ListTip() : ToolTip() {}
            virtual void Hide(Control^ ctl) 
            {
                ToolTip::Hide(ctl);
                Hided(this, EventArgs::Empty);
            }
        };
    }
}
