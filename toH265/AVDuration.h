#pragma once
namespace Ambiesoft {
	namespace toH265 {
		using namespace System;

		ref class AVDuration
		{
			TimeSpan^ ts_ = gcnew TimeSpan();
		public:
			AVDuration()
			{}
			AVDuration(TimeSpan^ ts) :ts_(ts)
			{}
			virtual String^ ToString() override {
				return ts_->ToString("hh\\:mm\\:ss");
			}
			// With this, status not updated becase set not called
			//void Clear() {
			//	ts_ = gcnew TimeSpan();
			//}
			bool IsEmpty() {
				return ts_->TotalMilliseconds == 0;
			}
			property double TotalMilliseconds
			{
				double get() {
					return ts_->TotalMilliseconds;
				}
			}
		};


	}
}