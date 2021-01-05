#include "stdafx.h"

#include "toH265.h"

#include "helper.h"

namespace Ambiesoft {
	namespace toH265 {
		using namespace System;
		using namespace System::Windows::Forms;
		using namespace System::Collections::Generic;

		typedef LONG(NTAPI *NtSuspendProcess)(HANDLE ProcessHandle);
		typedef LONG(NTAPI *NtResumeProcess)(HANDLE ProcessHandle);

		static NtSuspendProcess pfnNtSuspendProcess;
		static NtResumeProcess pfnNtResumeProcess;


		static bool prepare()
		{
			if (pfnNtSuspendProcess != NULL && pfnNtResumeProcess != NULL)
				return true;

			pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
				GetModuleHandle(L"ntdll"), "NtSuspendProcess");
			pfnNtResumeProcess = (NtResumeProcess)GetProcAddress(
				GetModuleHandle(L"ntdll"), "NtResumeProcess");

			return pfnNtSuspendProcess != NULL && pfnNtResumeProcess != NULL;
		}

		static bool ProcessCommon(System::Diagnostics::Process^ process, bool bSuspend)
		{
			if (!process)
				return false;

			if (!prepare())
			{
				Ambiesoft::CppUtils::Fatal(I18N(L"Failed to get function pointer for suspend or resume processes."));
				return false;
			}

			HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process->Id);
			if (!processHandle)
				return false;

			if (bSuspend)
				pfnNtSuspendProcess(processHandle);
			else
				pfnNtResumeProcess(processHandle);

			CloseHandle(processHandle);
			return true;

		}
		bool SuspendProcess(System::Diagnostics::Process^ process)
		{
			return ProcessCommon(process, true);
		}

		bool ResumeProcess(System::Diagnostics::Process^ process)
		{
			return ProcessCommon(process, false);
		}

		bool KillProcess(System::Diagnostics::Process^ process)
		{
			if (!process)
				return false;

			try
			{
				process->Kill();
				return true;
			}
			catch (...) {}

			return false;
		}
		bool KillThread(System::Threading::Thread^ thread)
		{
			if (!thread)
				return false;

			thread->Abort();
			return true;
		}

		void SafeJoin(System::Threading::Thread^ thread)
		{
			if (!thread)
				return;
			if (!thread->IsAlive)
				return;

			try
			{
				thread->Join();
				return;
			}
			catch (...)
			{
			}
		}

		String^ sToString(System::Drawing::Size size)
		{
			return String::Format("{0}x{1}", size.Width, size.Height);
		}

		List<String^>^ MakeUnique(List<String^>^ inList)
		{
			List<String^>^ result = gcnew List<String^>();
			for each (String ^ s in inList)
			{
				if (result->Contains(s))
					continue;
				result->Add(s);
			}
			return result;
		}
	}
}