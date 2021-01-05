#pragma once

namespace Ambiesoft {
	namespace toH265 {

		using namespace System;

		ref class ListViewItemData
		{
			String^ outputFile_;

			ListViewItemData(){}

		public:
			static ListViewItemData^ Get(System::Windows::Forms::ListViewItem^ item) {
				if (!item)
					return nullptr;
				if (!item->Tag)
					item->Tag = gcnew ListViewItemData();

				return (ListViewItemData^)item->Tag;
			}

			property String^ OutputFile
			{
				String^ get() { return outputFile_; }
				void set(String^ v) { outputFile_ = v; }
			}
		};

	}
}