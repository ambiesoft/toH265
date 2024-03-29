#pragma once

namespace Ambiesoft {
	namespace toH265 {
		bool SuspendProcess(System::Diagnostics::Process^ process);
		bool ResumeProcess(System::Diagnostics::Process^ process);
		bool KillProcess(System::Diagnostics::Process^ process);
		bool KillThread(System::Threading::Thread^ thread);
		void SafeJoin(System::Threading::Thread^ thread);

		String^ sToString(System::Drawing::Size size);
		System::Collections::Generic::List<System::String^>^ MakeUnique(System::Collections::Generic::List<System::String^>^ inList);
		String^ ToCommonDurationString(System::TimeSpan ts);
	}
}