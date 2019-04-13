#pragma once


namespace Ambiesoft {
	namespace toH265 {

		using namespace System;

		ref class Program
		{
			static String^ movieFile_;

			[STAThreadAttribute]
			static int main(array<System::String ^> ^args);

		public:
			static property String^ MovieFile
			{
				String^ get()
				{
					return movieFile_;
				}
			}
		};

	}
}