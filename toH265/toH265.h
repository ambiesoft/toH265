#pragma once


namespace Ambiesoft {
	namespace toH265 {

		using namespace System;

		ref class Program
		{
			static String^ movieFile_;
			static System::Globalization::CultureInfo^ cultureInfo_;
			static System::Resources::ResourceManager^ theResource_ = 
				gcnew System::Resources::ResourceManager(L"Ambiesoft.toH265.toH265", 
					System::Reflection::Assembly::GetExecutingAssembly());
			static bool preRun();

#ifdef _DEBUG
			static System::Collections::Generic::List<String^> listNotI18Ned_;

			static void showUnI18Ned()
			{
				DTRACE(L"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII18NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
				for each(String^ s in listNotI18Ned_)
				{
					DTRACE(s);
				}
				DTRACE(L"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII18NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
			}
#endif

			[STAThreadAttribute]
			static int main(array<System::String ^> ^args);

			static void SaveSettings();

		public:
			static property String^ IniFile
			{
				String^ get();
			}

			static property String^ MovieFile
			{
				String^ get()
				{
					return movieFile_;
				}
			}

			
			static property String^ Culture
			{
				String^ get();
				void set(String^ value);
			}
			static String^ getString(String^ origText);
		};

	}
}