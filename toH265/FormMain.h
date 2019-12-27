#pragma once

#include "AVCodec.h"
#include "AVDuration.h"

#include "toH265.h"

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
			ref class ColumnItem
			{
				String^ key_;
				String^ text_;
				int width_;
			public:
				ColumnItem(String^ key, String^ text, int width):
					key_(key),
					text_(text),
					width_(width){}
				ColumnItem(String^ key, String^ text) :
					ColumnItem(key, text, 50) {}
				property String^ Key
				{
					String^ get() {
						return key_;
					}
				}
				property String^ Text
				{
					String^ get() {
						return text_;
					}
				}
				property int Width
				{
					int get() {
						return width_;
					}
				}
			};
			static initonly array<ColumnItem^>^ ColumnItems = gcnew array<ColumnItem^> { 
				gcnew ColumnItem{ "main", "", 0},
				gcnew ColumnItem{ "directory", I18N("Directory"), 150 },
				gcnew ColumnItem{ "filename", I18N("Filename"), 150 },
				gcnew ColumnItem{ "size", I18N("Size") },
				gcnew ColumnItem{ "aspect", I18N("Aspect Ratio") },
				gcnew ColumnItem{ "format", I18N("Format") },
				gcnew ColumnItem{ "vcodec", I18N("Video") },
				gcnew ColumnItem{ "acodec", I18N("Audio") },
				gcnew ColumnItem{ "duration", I18N("Duration") },
			};
		public:
			static initonly cli::array<wchar_t>^ char1x = gcnew cli::array<wchar_t>{L'x'};

			literal String^ SECTION_OPTION = L"Option";
			literal String^ SECTION_LOCATION = L"Location";
			literal String^ SECTION_COLUMNS = L"Columns";

			literal String^ KEY_FFPROBE = L"ffprobe";
			literal String^ KEY_FFMPEG = L"ffmpeg";
			literal String^ KEY_PROCESS_BACKGROUND = L"processbackground";
			literal String^ KEY_MINIMIZETOTRAY = L"minimizetotray";
			literal String^ KEY_CULTURE = L"culture";
			literal String^ KEY_LISTVIEW_COLUMNS = L"columns";
			literal String^ KEY_SPLITROOT_DISTANCE = L"sprootdistance";

			literal String^ STR_BUTTONTEXT_PAUSE = L"&Pause";
			literal String^ STR_BUTTONTEXT_START = L"&Start";
			literal String^ STR_BUTTONTEXT_RESUME = L"Res&ume";

			literal String^ STR_FAILED_TO_SAVE_SETTING = L"Failed to save settings.";
			literal String^ STR_0_NOT_FOUND = L"'{0}' not found.";
			literal String^ STR_0_ALREADY_OPENED = L"'{0}' may be already opened by another application.";
			literal String^ STR_ARE_YOU_SURE_TO_CONTINUE = L"Are you sure to continue?";
			literal String^ STR_NO_OUTPUT_MOVIE = L"No output movie";

		private: System::Windows::Forms::TextBox^ txtLogErr;
		private: System::Windows::Forms::TextBox^ txtLogOut;
		private: System::Windows::Forms::Button^ btnStart;

		private: System::Windows::Forms::MenuStrip^ menuMain;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiFile;
		private: System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiOption;

		private: System::Windows::Forms::ToolStripMenuItem^ tsmiSetFFProbe;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiSetFFMpeg;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiHelp;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiAbout;
		private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem1;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiPriority;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiPriorityNormal;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiPriorityBackground;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiFFMpegHelp;
		private: System::Windows::Forms::NotifyIcon^ notifyIconMain;
		private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem2;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiMinimizeToTray;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiStop;
		private: System::Windows::Forms::ContextMenuStrip^ cmNotify;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiNotifyShow;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiNotifyStart;
		private: System::Windows::Forms::StatusStrip^ statusMain;
		private: System::Windows::Forms::ToolStripStatusLabel^ slMain;
		private: System::Windows::Forms::ToolStripStatusLabel^ slVideoCodec;
		private: System::Windows::Forms::ToolStripStatusLabel^ slDuration;
		private: System::Windows::Forms::ToolStripStatusLabel^ slAudioCodec;

	

		private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem3;
		public:
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiLanguage;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiLanguageOSDefault;
		private: System::Windows::Forms::ToolStripMenuItem^  tsmiLanguageEnglish;






		private: System::Windows::Forms::TextBox^ txtFFMpegArg;
		private: System::Windows::Forms::Panel^  panelList;
		private: System::Windows::Forms::SplitContainer^  splitRoot;
		private: System::Windows::Forms::ToolStripStatusLabel^ slFormat;
		private: System::Windows::Forms::ContextMenuStrip^ cmList;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiRemoveFromList;
		private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem4;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiOpenInputLocations;

		private: System::Windows::Forms::ToolStripMenuItem^ tsmiOpenOutput;
		private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem5;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiNotifyOpenInputLocations;
private: System::Windows::Forms::ToolStripMenuItem^ tsmiNotifyOpenOutputLocation;

		private: System::Windows::Forms::ToolStripMenuItem^ tsmiLanguageJapanese;

			
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

		private: System::Windows::Forms::Button^ btnBrowseMovie;
		private: System::ComponentModel::IContainer^ components;

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
			ListViewCustomReorder::ListViewEx^ lvInputs = gcnew ListViewCustomReorder::ListViewEx();

			bool CheckMovieAndSet(String^ file, bool bSet);


			String^ ffprobe_;
			property String^ FFProbe
			{
				String^ get();
			}
			String^ PeekFFProbe();

			String^ ffmpeg_;
			property String^ FFMpeg
			{
				String^ get();
			}
			String^ PeekFFMpeg();


			String^ getCommon(
				System::Windows::Forms::IWin32Window^ parent,
				bool bFFMpeg,
				String^ regApp,
				String^ regKey,
				String^ inifile,
				String^% target,
				bool bReset);
			String^ getCommon(
				System::Windows::Forms::IWin32Window^ parent,
				bool bFFMpeg,
				String^ regApp,
				String^ regKey,
				String^ inifile,
				String^% target,
				bool bReset,
				bool bPeek);

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
			// TimeSpan tsOrigMovies_;
			String^ tempFile_;

			System::Drawing::Icon^ iconBlue_;
			System::Drawing::Icon^ iconYellow_;
			System::Drawing::Icon^ iconRed_;

			array<String^>^ GetInputMovies();
			String^ outputMovie_;

			void SetFormatStatusText();
			String^ inputFormat_;
			property String^ InputFormat
			{
				String^ get() { return inputFormat_; }
				void set(String^ v) {
					inputFormat_ = v;
					SetFormatStatusText();
				}
			}

			void SetCodecStatusText();
			AVCodec^ inputAudioCodec_ = gcnew AVCodec();
			property AVCodec^ InputAudioCodec
			{
				AVCodec^ get() { return inputAudioCodec_; }
				void set(AVCodec^ v) {
					inputAudioCodec_ = v;
					SetCodecStatusText();
				}
			}
	


			AVCodec^ outputAudioCodec_ = gcnew AVCodec();
			property AVCodec^ OutputAudioCodec
			{
				AVCodec^ get() { return outputAudioCodec_; }
				void set(AVCodec^ v) {
					outputAudioCodec_ = v;
					SetCodecStatusText();
				}
			}

			AVCodec^ inputVideoCodec_ = gcnew AVCodec();
			property AVCodec^ InputVideoCodec
			{
				AVCodec^ get() { return inputVideoCodec_; }
				void set(AVCodec^ v) {
					inputVideoCodec_ = v;
					SetCodecStatusText();
				}
			}
			AVCodec^ outputVideoCodec_ = gcnew AVCodec();
			property AVCodec^ OutputVideoCodec
			{
				AVCodec^ get() { return outputVideoCodec_; }
				void set(AVCodec^ v) {
					outputVideoCodec_ = v;
					SetCodecStatusText();
				}
			}


			void SetTimeStatusText();
			AVDuration^ inputDuration_ = gcnew AVDuration();
			property AVDuration^ InputDuration
			{
				AVDuration^ get() { return inputDuration_; }
				void set(AVDuration^ v) {
					inputDuration_ = v;
					SetTimeStatusText();
				}
			}
			AVDuration^ outputDuration_ = gcnew AVDuration();
			property AVDuration^ OutputDuration
			{
				AVDuration^ get() { return outputDuration_; }
				void set(AVDuration^ v) {
					DASSERT(v);
					outputDuration_ = v;
					SetTimeStatusText();
				}
			}


			property TaskState FFMpegState
			{
				TaskState get();
			}
			DWORD dwBackPriority_;
			int pidFFMpeg_;

			enum class STATUSTEXT {
				READY,
				REMAINING,
			};

			void InsertMovieItem(
				String^ movieFile,
				LONGLONG size,
				System::Drawing::Size aspect,
				String^ format,
				AVCodec^ acodec,
				AVCodec^ vcodec,
				AVDuration^ duration);

			String^ GetMovieFileFromLvi(ListViewItem^ lvi);
			System::Drawing::Size GetVideoSize(ListViewItem^ lvi);
			double GetVideoArea(ListViewItem^ lvi);
			System::Drawing::Size GetMaxVideoSize();
			bool IsSameSizeVideos();
			String^ tsToString(TimeSpan ts);

			void SetStatusText(STATUSTEXT ss);
			void SetStatusText(STATUSTEXT ss, String^ supplement);

			bool GetInfoFromFFMpegoutput(String^ text, TimeSpan% tsTime, double% dblSpeed);
			void UpdateTitleTS(TimeSpan ts, double speed);
			void UpdateTitleComplete();
			String^ buildTitleText(int percent, bool bFilenameOnly);
			void UpdateTitle();
			void UpdateTitle(int percent);

			String^ GetFFMpegOutput(String^ option);
			String^ GetFFMpegHelp(String^ subHelpOption);
			void StartOfThread(Object^ obj);
			delegate void VVDelegate();
			delegate void VIDelegate(int value);
			void ThreadStarted();
			void ThreadEnded(int retval);
			bool ConfirmAndStopEncode();
			void ChangeStartButtonText(String^ text);
			void OnProcessStarted(Object^ sender, EventArgs^ e);

			System::Void FormMain_Load(System::Object^ sender, System::EventArgs^ e);
			System::Void exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
				Close();
			}

			System::Void btnBrowseMovie_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnStart_Click(System::Object^ sender, System::EventArgs^ e);
			void outputHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e);
			void errHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e);

			System::Void FormMain_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
			System::Void FormMain_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);

			System::Void ListInputs_DragOver(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e);
			System::Void ListInputs_DragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e);
			System::Void ListInputs_DragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e);

			System::Void tsmiSetFFProbe_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiSetFFMpeg_Click(System::Object^ sender, System::EventArgs^ e);

			String^ baseSetFFProbeMenuString_;
			String^ baseSetFFMpegMenuString_;

			System::Void tsmiOption_DropDownOpening(System::Object^ sender, System::EventArgs^ e);



			void OnMenuPriorityCommon(bool bBackground);
			System::Void tsmiPriorityNormal_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiPriorityBackground_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiAbout_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiFFMpegHelp_Click(System::Object^ sender, System::EventArgs^ e);



			System::Void FormMain_Resize(System::Object^ sender, System::EventArgs^ e);
			System::Void notifyIconMain_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
			System::Void tsmiMinimizeToTray_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiStop_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiNotifyShow_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiNotifyStart_Click(System::Object^ sender, System::EventArgs^ e);

#ifdef _DEBUG
			System::Void FormMain_OnTest(System::Object^ sender, System::EventArgs^ e);
#endif

			System::Void tsmiLanguageCommon_Click(System::Object^  sender, System::EventArgs^  e);
			System::Void tsmiLanguage_DropDownOpening(System::Object^  sender, System::EventArgs^  e);

			System::Void TsmiRemoveFromList_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiOpenInputLocations_ClickCommon(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiOpenOutput_ClickCommon(System::Object^ sender, System::EventArgs^ e);

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