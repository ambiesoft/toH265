#pragma once

namespace Ambiesoft {
	namespace toH265 {

		using namespace System;
		public ref class ElapseInfo
		{
			UInt64 progressPixels_;
			LONGLONG timestamp_;
		public:
			ElapseInfo(UInt64 progressPixels) : progressPixels_(progressPixels) {
				// timestamp_ = System::Diagnostics::Stopwatch::GetTimestamp();
				timestamp_ = DateTime::Now.Ticks / TimeSpan::TicksPerMillisecond;
			}
			// ElapseInfo(UInt64 progressPixels, LONGLONG ts) : progressPixels_(progressPixels), timestamp_(ts) {}

			property UInt64 ProgressPixels
			{
				UInt64 get()
				{
					return progressPixels_;
				}
			}
			property LONGLONG TimeStamp
			{
				LONGLONG get()
				{
					return timestamp_;
				}
			}
		};

	}
}