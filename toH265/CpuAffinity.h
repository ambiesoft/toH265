#pragma once

namespace Ambiesoft {
	namespace toH265 {
		using namespace System;
		using namespace System::Diagnostics;

		ref class CpuAffinity
		{
			UINT64 cpuAffinity_;
			static initonly int maxCpu = IntPtr::Size * 8;
			static property int CpuCount
			{
				int get()
				{
					return Math::Min(maxCpu, Environment::ProcessorCount);
				}
			}
			static property UINT64 CpuBitMask
			{
				UINT64 get()
				{
					UINT64 ret = 0;
					for (int i = 0; i < CpuCount; ++i)
					{
						ret |= (1LL << i);
					}
					return ret;
				}
			}
			void init() {
				cpuAffinity_ = 0;
				for (int i = 0; i < CpuCount; ++i)
				{
					cpuAffinity_ |= (1LL << i);
				}
			}
		public:
			CpuAffinity(){
				init();
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
			void ToggleAll()
			{
				cpuAffinity_ = (~cpuAffinity_ & CpuBitMask);
				if (cpuAffinity_ == 0)
					cpuAffinity_ = 1;
			}
			void EnableAll()
			{
				init();
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