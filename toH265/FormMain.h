#pragma once


namespace Ambiesoft {
	namespace toH265 {

		using namespace System;
		using namespace System::ComponentModel;
		using namespace System::Collections;
		using namespace System::Windows::Forms;
		using namespace System::Data;
		using namespace System::Drawing;

		/// <summary>
		/// Summary for FormMain
		///
		/// WARNING: If you change the name of this class, you will need to change the
		///          'Resource File Name' property for the managed resource compiler tool
		///          associated with all .resx files this class depends on.  Otherwise,
		///          the designers will not be able to interact properly with localized
		///          resources associated with this form.
		/// </summary>
		public ref class FormMain : public System::Windows::Forms::Form
		{
			literal String^ SECTION_OPTION = L"Option";
		private: System::Windows::Forms::TextBox^  txtLogErr;
		private: System::Windows::Forms::TextBox^  txtLogOut;
		private: System::Windows::Forms::Button^  btnStart;
		private: System::Windows::Forms::Button^  btnStop;
				 literal String^ KEY_FFPROBE = L"ffprobe";
		private: System::Windows::Forms::TextBox^  txtFFMpegArg;
				 literal String^ KEY_FFMPEG = L"ffmpeg";

		public:
			FormMain(void)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
			}
			property int ReturnValue;
		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~FormMain()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::Label^  lblMovie;
		protected:
		private: System::Windows::Forms::TextBox^  txtMovie;
		private: System::Windows::Forms::Button^  btnBrowseMovie;

		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>
			System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
			/// <summary>
			/// Required method for Designer support - do not modify
			/// the contents of this method with the code editor.
			/// </summary>
			void InitializeComponent(void);
#pragma endregion

		private:
			bool hasVideoStream(String^ file, String^% codecname);
			bool CheckMovieAndSet(String^ file);

			property String^ IniFile
			{
				String^ get();
			}

			String^ ffprobe_;
			property String^ FFProbe
			{
				String^ get();
			}
			String^ ffmpeg_;
			property String^ FFMpeg
			{
				String^ get();
			}

			System::Text::StringBuilder sbFFMpegOut_;
			System::Text::StringBuilder sbFFMpegErr_;

			delegate void AddToLog(String^ text);
			void AddToOutput(String^ text);
			void AddToErr(String^ text);

			System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e);
			System::Void btnBrowseMovie_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void btnStart_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void btnStop_Click(System::Object^  sender, System::EventArgs^  e);
			void outputHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e);
			void errHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e);
		};
	}

}