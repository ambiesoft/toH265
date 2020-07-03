#pragma once

namespace Ambiesoft {
	namespace toH265 {
		using namespace System;
		using namespace System::Diagnostics;

		ref class CpuAffinity
		{
			UINT64 cpuAffinity_;
			static initonly int maxCpu = IntPtr::Size * 8;
		public:
			CpuAffinity(){
				cpuAffinity_ = 0;
				int cpucount = Math::Min(maxCpu, Environment::ProcessorCount);
				for (int i = 0; i < cpucount; ++i)
				{
					cpuAffinity_ |= (1LL << i);
				}
			}
			bool Load(String^ section, HashIni^ ini)
			{
				String^ ret;
				if (!Profile::GetString(section, L"CpuAffinity", String::Empty, ret, ini))
					return true; // ok for not have value
				if (!UINT64::TryParse(ret, cpuAffinity_))
					return false;
				return true;
			}
			bool Save(String^ section, HashIni^ ini)
			{
				return Profile::WriteString(section, L"CpuAffinity", cpuAffinity_.ToString(), ini);
			}
			bool IsCPUON(int cpunum)
			{
				DASSERT(cpunum >= 0);
				DASSERT(cpunum <= maxCpu);
				return (cpuAffinity_ & (1LL << cpunum)) != 0;
			}
			void SetCPU(int cpunum, bool bON)
			{
				DASSERT(cpunum >= 0);
				DASSERT(cpunum <= maxCpu);

				if (bON)
					cpuAffinity_ |= (1LL << cpunum);
				else
					cpuAffinity_ &= ~(1LL << cpunum);
			}
			void ClearAllButZero()
			{
				cpuAffinity_ = 1LL;
			}
			System::UInt64 Value() 
			{
				return cpuAffinity_;
			}
			DWORD_PTR ValueAsPtr()
			{
				return (DWORD_PTR)cpuAffinity_;
			}
			void Update(Process^ pro)
			{
				if (!pro)
					return;

				DWORD_PTR dwAffinity = 0;
				DWORD_PTR dwDummy = 0;
				DVERIFY(GetProcessAffinityMask((HANDLE)pro->Handle.ToPointer(),
					&dwAffinity,
					&dwDummy));
				cpuAffinity_ = dwAffinity;
			}
		};

	}
}