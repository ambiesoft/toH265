#pragma once

namespace Ambiesoft {
	namespace toH265 {
		using namespace System;
		public ref class InputListView :
			public ListViewCustomReorder::ListViewEx
		{
			using ParentClass = ListViewCustomReorder::ListViewEx;

			void OnAfterItemCountChanging()
			{
				ItemCountChanged();
			}
			

		protected:
			void WndProc(System::Windows::Forms::Message% m) override;
		public:
			delegate void ItemCountChangedHandler();
			event ItemCountChangedHandler^ ItemCountChanged;
			
		};

	}
}