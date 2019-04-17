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
			private: System::Windows::Forms::TextBox^  txtLogErr;
			private: System::Windows::Forms::TextBox^  txtLogOut;
			private: System::Windows::Forms::Button^  btnStart;


			private: System::Windows::Forms::TextBox^  txtFFMpegArg;
			private: System::Windows::Forms::MenuStrip^  menuMain;
			private: System::Windows::Forms::ToolStripMenuItem^  tsmiFile;
			private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
			private: System::Windows::Forms::ToolStripMenuItem^  tsmiOption;

			private: System::Windows::Forms::Panel^  panelMain;
			private: System::Windows::Forms::ToolStripMenuItem^  tsmiSetFFProbe;
			private: System::Windows::Forms::ToolStripMenuItem^  tsmiSetFFMpeg;
			private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiAbout;

			private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
			private: System::Windows::Forms::ToolStripMenuItem^  tsmiPriority;
			private: System::Windows::Forms::ToolStripMenuItem^  tsmiPriorityNormal;
			private: System::Windows::Forms::ToolStripMenuItem^  tsmiPriorityBackground;

					 literal String^ SECTION_OPTION = L"Option";
					 literal String^ SECTION_LOCATION = L"Location";
					 literal String^ KEY_FFPROBE = L"ffprobe";
			literal String^ KEY_FFMPEG = L"ffmpeg";
			literal String^ KEY_PROCESS_BACKGROUND = L"processbackground";
			literal String^ KEY_MINIMIZETOTRAY = L"minimizetotray";


			literal String^ BUTTONTEXT_PAUSE = L"&Pause";
			literal String^ BUTTONTEXT_START = L"&Start";

			literal String^ BUTTONTEXT_RESUME = L"Res&ume";
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiFFMpegHelp;
		private: System::Windows::Forms::NotifyIcon^  notifyIconMain;
		private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem2;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiMinimizeToTray;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiStop;
		private: System::Windows::Forms::ContextMenuStrip^  cmNotify;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiNotifyShow;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiNotifyStart;

				 literal String^ STR_FAILED_TO_SAVE_SETTING = L"Failed to save settings.";
		public:
			FormMain(void);

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
		private: System::ComponentModel::IContainer^  components;

		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>


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
			System::Drawing::Icon^ iconBlue_;
			System::Drawing::Icon^ iconYellow_;
			System::Drawing::Icon^ iconRed_;

			String^ outputMovie_;

			property TaskState FFMpegState
			{
				TaskState get();
			}
			DWORD dwBackPriority_;
			void UpdateTitleTS(TimeSpan ts);
			void UpdateTitleComplete();
			void UpdateTitle();
			void UpdateTitle(int percent);
			
			String^ GetFFMpegHelp(String^ subHelpOption);
			void StartOfThread(Object^ obj);
			delegate void VVDelegate();
			delegate void VIDelegate(int value);
			void ThreadStarted();
			void ThreadEnded(int retval);
			void StopEncoding();
			bool ConfirmEncodeStop();
			void ChangeStartButtonText(String^ text);
			void OnProcessStarted(Object^ sender, EventArgs^ e);

			System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e);
			System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				Close();
			}

			System::Void btnBrowseMovie_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void btnStart_Click(System::Object^  sender, System::EventArgs^  e);
			void outputHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e);
			void errHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e);

			System::Void FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
			System::Void FormMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e);

			System::Void txtMovie_DragOver(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
			System::Void txtMovie_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);
			System::Void txtMovie_DragLeave(System::Object^  sender, System::EventArgs^  e);
			System::Void txtMovie_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e);

			System::Void tsmiSetFFProbe_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void tsmiSetFFMpeg_Click(System::Object^  sender, System::EventArgs^  e);

			String^ baseSetFFProbeMenuString_;
			String^ baseSetFFMpegMenuString_;

			System::Void tsmiOption_DropDownOpening(System::Object^  sender, System::EventArgs^  e);

			

			void OnMenuPriorityCommon(bool bBackground);
			System::Void tsmiPriorityNormal_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void tsmiPriorityBackground_Click(System::Object^  sender, System::EventArgs^  e);

			System::Void tsmiAbout_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void tsmiFFMpegHelp_Click(System::Object^  sender, System::EventArgs^  e);

			

			System::Void FormMain_Resize(System::Object^  sender, System::EventArgs^  e);
			System::Void notifyIconMain_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
			System::Void tsmiMinimizeToTray_Click(System::Object^  sender, System::EventArgs^  e);

			System::Void tsmiStop_Click(System::Object^  sender, System::EventArgs^  e);

			System::Void tsmiNotifyShow_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void tsmiNotifyStart_Click(System::Object^  sender, System::EventArgs^  e);

			

			

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