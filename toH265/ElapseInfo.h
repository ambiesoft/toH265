#pragma once

namespace Ambiesoft {
	namespace toH265 {

		using namespace System;
		public ref class ElapseInfo
		{
			double progress_;
			LONGLONG timestamp_;
		public:
			ElapseInfo(double progress) : progress_(progress) {
				// timestamp_ = System::Diagnostics::Stopwatch::GetTimestamp();
				timestamp_ = DateTime::Now.Ticks / TimeSpan::TicksPerMillisecond;
			}
			ElapseInfo(double progress, LONGLONG ts) : progress_(progress), timestamp_(ts) {}
			property double Progress
			{
				double get()
				{
					return progress_;
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