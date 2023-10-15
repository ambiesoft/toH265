#pragma once

namespace Ambiesoft {
	namespace toH265 {

		using namespace System;
		ref class TimerMouseMoveData
		{
		public:
			literal int SHOW_DURATION = 5000;
		private:
			System::Drawing::Point mousePos_;
			System::Drawing::Point showPos_;
			String^ willShowText_;
			int lastShowTick_;
		public:
			bool IsWillShowData(String^ text) {
				return text == willShowText_;
			}
			void SetWillShowText(String^ text, System::Drawing::Point mousePos, System::Drawing::Point showPos) {
				willShowText_ = text;
				mousePos_ = mousePos;
				showPos_ = showPos;
			}
			property String^ WillShowText
			{
				String^ get() {
					return willShowText_;
				}
			}
			property int X
			{
				int get() {
					return showPos_.X;
				}
			}
			property int Y
			{
				int get() {
					return showPos_.Y;
				}
			}
			void SetLastShowTick(int tick) {
				lastShowTick_ = tick;
			}
			property bool HasShowPeriodPassed
			{
				bool get() {
					return System::Environment::TickCount - lastShowTick_ > SHOW_DURATION;
				}
			}
			bool IsDataSame(String^ text, System::Drawing::Point pt)
			{
				return text == willShowText_ && pt == mousePos_;
			}
			void SetMousePos(System::Drawing::Point pt)
			{
				mousePos_ = pt;
			}
		};

	}
}