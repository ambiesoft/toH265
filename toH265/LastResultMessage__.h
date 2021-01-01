#pragma once


namespace Ambiesoft {
	namespace toH265 {
		using namespace System;

		public ref class LastResultMessage
		{
			String^ message_;
			bool isWarning_;

		public:
			bool IsEmpty() {
				return message_ == nullptr;
			}
			property String^ Message
			{
				String^ get() {
					return message_;
				}
			}
			property bool IsWarning
			{
				bool get() {
					return isWarning_;
				}
			}

			void Set(String^ message, bool isWarning)
			{
				message_ = message;
				isWarning_ = isWarning;
			}
			void ShowMessage(System::Windows::Forms::IWin32Window^ win)
			{
				CppUtils::CenteredMessageBox(win,
					message_,
					System::Windows::Forms::Application::ProductName,
					System::Windows::Forms::MessageBoxButtons::OK,
					isWarning_ ? System::Windows::Forms::MessageBoxIcon::Warning : System::Windows::Forms::MessageBoxIcon::Information);
			}
		};

	}
}