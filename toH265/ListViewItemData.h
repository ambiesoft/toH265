#pragma once

namespace Ambiesoft {
	namespace toH265 {

		using namespace System;

		ref class ListViewItemData
		{
			String^ outputtingFile_;
			String^ outputtedFile_;

			ListViewItemData(){}

		public:
			static ListViewItemData^ Get(System::Windows::Forms::ListViewItem^ item) {
				if (!item)
					return nullptr;
				if (!item->Tag)
					item->Tag = gcnew ListViewItemData();

				return (ListViewItemData^)item->Tag;
			}

			property String^ OutputtingFile
			{
				String^ get() { return outputtingFile_; }
				void set(String^ v) { outputtingFile_ = v; }
			}
			property String^ OutputtedFile
			{
				String^ get() { return outputtedFile_; }
				void set(String^ v) { outputtedFile_ = v; }
			}
			//property bool IsCompleted
			//{
			//	bool get() { return !String::IsNullOrEmpty(outputtedFile_); }
			//}
		};

	}
}