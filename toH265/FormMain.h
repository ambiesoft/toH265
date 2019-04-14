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

				 literal String^ KEY_FFPROBE = L"ffprobe";
		private: System::Windows::Forms::TextBox^  txtFFMpegArg;
				 literal String^ KEY_FFMPEG = L"ffmpeg";
				 literal String^ BUTTONTEXT_PAUSE = L"&Pause";
				 literal String^ BUTTONTEXT_START = L"&Start";
		private: System::Windows::Forms::MenuStrip^  menuMain;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiFile;
		private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiOption;

		private: System::Windows::Forms::Panel^  panelMain;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiSetFFProbe;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiSetFFMpeg;
		private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;

				 literal String^ BUTTONTEXT_RESUME = L"Res&ume";
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
			bool hasVideoStream(String^ file, String^% codecname, TimeSpan% ts);
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
			String^ getCommon(System::Windows::Forms::IWin32Window^ parent,
				bool bFFMpeg, String^ regApp, String^ regKey, String^ inifile, String^% target, bool bReset);

			delegate void AddToLog(String^ text);
			void AddToOutput(String^ text);
			void AddToErr(String^ text);

			System::Threading::Thread^ thFFMpeg_;
			System::Diagnostics::Process^ processFFMpeg_;
			bool processSuspeded_;
			bool processTerminated_;
			enum class TaskState {
				None,
				ProcessLaunching,
				Running,
				Pausing,
				Unknown,
			};
			System::Text::RegularExpressions::Regex^ regFFMpeg_;
			TimeSpan tsOrigMovie_;
			property TaskState FFMpegState
			{
				TaskState get();
			}
			void UpdateTitleTS(TimeSpan ts);
			void UpdateTitleComplete();
			void UpdateTitle();
			void UpdateTitle(int percent);

			void StartOfThread(Object^ obj);
			delegate void VVDelegate();
			delegate void VIDelegate(int value);
			void ThreadStarted();
			void ThreadEnded(int retval);
			void FormMain::StopEncoding();
			System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e);
			System::Void btnBrowseMovie_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void btnStart_Click(System::Object^  sender, System::EventArgs^  e);
			void outputHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e);
			void errHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e);

			System::Void FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);

			System::Void txtMovie_DragOver(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
			System::Void txtMovie_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
			System::Void txtMovie_DragLeave(System::Object^  sender, System::EventArgs^  e);
			System::Void txtMovie_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);

			System::Void tsmiSetFFProbe_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void tsmiSetFFMpeg_Click(System::Object^  sender, System::EventArgs^  e);

			String^ baseSetFFProbeMenuString_;
			String^ baseSetFFMpegMenuString_;

			System::Void tsmiOption_DropDownOpening(System::Object^  sender, System::EventArgs^  e);

			System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);

		};

		public ref class WaitCursor
		{
			static int counter_;
			static System::Windows::Forms::Cursor^ cur_;
		public:
			WaitCursor();
			~WaitCursor();
		};
	}

}