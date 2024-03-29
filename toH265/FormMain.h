#pragma once

#include "AVCodec.h"
#include "AVDuration.h"
#include "ElapseInfo.h"
#include "CpuAffinity.h"
#include "EncodeTask.h"
#include "Summary.h"
#include "InputListView.h"

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
		public ref class FormMain : public 
			System::Windows::Forms::Form,
			public System::Collections::IComparer
		{
			ref class ColumnItem
			{
			public:
				enum class SORTTYPE {
					SORT_INT,
					SORT_STRING,
					SORT_ISTRING,
					SORT_LONGLONG,
					SORT_SIZE,
					SORT_DOUBLE,
					SORT_DATE,
				};
			private:
				String^ key_;
				String^ text_;
				int width_;
				SORTTYPE sorttype_;
			public:
				ColumnItem(String^ key, String^ text, int width, SORTTYPE sorttype) :
					key_(key),
					text_(text),
					width_(width),
					sorttype_(sorttype){}
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
				int Compare(ListViewItem::ListViewSubItem^ x, ListViewItem::ListViewSubItem^ y)
				{
					switch (sorttype_)
					{
					case SORTTYPE::SORT_INT:
						int ix, iy;
						int::TryParse(x->Text, ix);
						int::TryParse(y->Text, iy);
						return ix - iy;
					case SORTTYPE::SORT_STRING:
						return String::Compare(x->Text, y->Text);
					case SORTTYPE::SORT_ISTRING:
						return String::Compare(x->Text, y->Text, true);
					case SORTTYPE::SORT_LONGLONG:
					{
						LONGLONG diff = ((LONGLONG)x->Tag - (LONGLONG)y->Tag);
						if (int::MinValue <= diff && diff <= int::MaxValue)
							return (int)diff;
						return diff == 0 ? 0 :
							(diff < 0 ? -1 : 1);
					}
					case SORTTYPE::SORT_DOUBLE:
					{
						double diff = ((double)x->Tag - (double)y->Tag);
						if (int::MinValue <= diff && diff <= int::MaxValue)
							return (int)diff;
						return diff == 0 ? 0 :
							(diff < 0 ? -1 : 1);
					}
					case SORTTYPE::SORT_SIZE:
					{
						System::Drawing::Size^ s1 = (System::Drawing::Size^)x->Tag;
						System::Drawing::Size^ s2 = (System::Drawing::Size^)y->Tag;
						return (s1->Width * s1->Height) - (s2->Width * s2->Height);
					}
					case SORTTYPE::SORT_DATE:
					{
						DateTime dt1 = (DateTime)x->Tag;
						DateTime dt2 = (DateTime)y->Tag;
						TimeSpan span = dt1 - dt2;
						double diff = span.TotalMilliseconds;
						return diff == 0 ? 0 :
							(diff < 0 ? -1 : 1);
					}
					default:
						DASSERT(false);
					}
					
					return 0;
				}
			};
			static initonly array<ColumnItem^>^ ColumnItems = gcnew array<ColumnItem^> {
					gcnew ColumnItem{ "main", "", 16, ColumnItem::SORTTYPE::SORT_STRING},
					gcnew ColumnItem{ "directory", I18N("Directory"), 150, ColumnItem::SORTTYPE::SORT_ISTRING },
					gcnew ColumnItem{ "filename", I18N("Filename"), 150, ColumnItem::SORTTYPE::SORT_ISTRING },
					gcnew ColumnItem{ "size", I18N("Size"), 50, ColumnItem::SORTTYPE::SORT_LONGLONG },
					gcnew ColumnItem{ "aspect", I18N("Aspect Ratio"), 50, ColumnItem::SORTTYPE::SORT_SIZE },
					gcnew ColumnItem{ "format", I18N("Format"),50,ColumnItem::SORTTYPE::SORT_ISTRING },
					gcnew ColumnItem{ "vcodec", I18N("Video"),50,ColumnItem::SORTTYPE::SORT_ISTRING },
					gcnew ColumnItem{ "acodec", I18N("Audio"),50,ColumnItem::SORTTYPE::SORT_ISTRING },
					gcnew ColumnItem{ "duration", I18N("Duration") ,50,ColumnItem::SORTTYPE::SORT_DOUBLE },
					gcnew ColumnItem{ "fps", I18N("FPS") ,50,ColumnItem::SORTTYPE::SORT_DOUBLE },
					gcnew ColumnItem{ "lastmodified", I18N("Last Modified") ,50,ColumnItem::SORTTYPE::SORT_DATE },
			};

			CpuAffinity cpuAffinity_;
			bool closed_ = false;

			// System::Windows::Forms::Timer^ timerSetAffinity_;
			bool bCloseFromMenu_;

			CTaskBarProgress* m_pTaskbarProgress = nullptr;

		private: System::Windows::Forms::ToolStripMenuItem^ tsmiClearAllItems;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiShowInputFileInExplorer;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiShowOutputFileInExplorer;
		private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem8;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiEdit;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiClearCompletedItems;
		private: System::Windows::Forms::ImageList^ ilList;
private: System::Windows::Forms::Label^ lblBottomRight;
private: System::Windows::Forms::Label^ lblTopLeft;
private: System::Windows::Forms::ToolStripMenuItem^ tsmiEnableHalfUpper;
private: System::Windows::Forms::ToolStripMenuItem^ tsmiEnableHalfEven;
private: System::Windows::Forms::Timer^ timerLogError;
private: System::Windows::Forms::Timer^ timerMouseMove;

			EncodeTask^ encodeTask_;
			property bool IsTaskActive
			{
				bool get() { return encodeTask_ && !encodeTask_->IsAllEnded(); }
			}
			void SetItemCountStatus();


			System::Text::StringBuilder sbLogErrBuffer_;

		public:
			// static initonly cli::array<wchar_t>^ char1x = gcnew cli::array<wchar_t>{L'x'};

			literal String^ SECTION_OPTION = L"Option";
			literal String^ SECTION_LOCATION = L"Location";
			literal String^ SECTION_COLUMNS = L"Columns";
			literal String^ SECTION_CPUAFFINITY = L"CpuAffinity";
			literal String^ SECTION_TARGETCODECDIALOG = L"TargetCodecDialog";

			literal String^ KEY_FFPROBE = L"ffprobe";
			literal String^ KEY_FFMPEG = L"ffmpeg";
			literal String^ KEY_FFMPEG_PRIORITY = L"ffmpegpriority";
			literal String^ KEY_MINIMIZETOTRAY = L"minimizetotray";
			literal String^ KEY_CULTURE = L"culture";
			literal String^ KEY_LISTVIEW_COLUMNS = L"columns";
			literal String^ KEY_SPLITROOT_DISTANCE = L"sprootdistance";
			literal String^ KEY_AFTERPROCESS_ENABLED = L"afterprocessenabled";
			literal String^ KEY_CPUAFFINITY_ENABLED = L"cpuaffinityenabled";
			literal String^ KEY_ELAPSEDTIME_QUEUE_LENGTH = L"ElapsedTimeQueueLength";

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



		private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem3;
		public:
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiLanguage;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiLanguageOSDefault;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiLanguageEnglish;






		private: System::Windows::Forms::TextBox^ txtFFMpegArg;
		private: System::Windows::Forms::Panel^ panelList;
		private: System::Windows::Forms::SplitContainer^ splitRoot;
		private: System::Windows::Forms::ToolStripStatusLabel^ slFormat;
		private: System::Windows::Forms::ContextMenuStrip^ cmList;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiRemoveFromList;
		private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem4;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiOpenInputLocations;

		private: System::Windows::Forms::ToolStripMenuItem^ tsmiOpenOutput;
		private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem5;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiNotifyOpenInputLocations;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiNotifyOpenOutputLocation;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiProcessAfterFinish;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiEnabledtsmiProcessAfterFinish;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiProcesstsmiProcessAfterFinish;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiCPUAffinity;

		private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem6;
		private: System::Windows::Forms::ToolStripMenuItem^ tsmiCPUAffinityEnable;
private: System::Windows::Forms::ToolStripSeparator^ tsmsCpuAffinityBegin;

		private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem7;
private: System::Windows::Forms::ToolStripMenuItem^ tsmiClearAllButZero;
private: System::Windows::Forms::ToolStripSeparator^ tsmsCpuAffinityEnd;
private: System::Windows::Forms::ToolStripMenuItem^ tsmiEnableAll;
private: System::Windows::Forms::ToolStripMenuItem^ tsmiPriorityBelowNormal;
private: System::Windows::Forms::ToolStripMenuItem^ tsmiToggleAll;
private: System::Windows::Forms::ToolStripMenuItem^ tsmiView;
private: System::Windows::Forms::ToolStripMenuItem^ tsmiShowLastResult;
private: System::Windows::Forms::ToolStripStatusLabel^ slItemCount;




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
				delete m_pTaskbarProgress;
				m_pTaskbarProgress = nullptr;

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

		public:
			bool bSortRev_;
			int sortColumn_;
			virtual int Compare(Object^ x, Object^ y)
			{
				DASSERT_IS_CLASS(x, ListViewItem);
				DASSERT_IS_CLASS(y, ListViewItem);
				ColumnItem^ ci = ColumnItems[sortColumn_];

				int ret = ci->Compare(((ListViewItem^)x)->SubItems[sortColumn_]
					, ((ListViewItem^)y)->SubItems[sortColumn_]);
				return bSortRev_ ? -ret : ret;
			}
			static AVCodec^ GetVCodecFromLvi(ListViewItem^ lvi);
			static AVCodec^ GetACodecFromLvi(ListViewItem^ lvi);


		protected:
			Summary^ lastSummary_;
			initonly System::Drawing::Color DefaultStatusColor;

			// ListViewCustomReorder::ListViewEx^ lvInputs = gcnew ListViewCustomReorder::ListViewEx();
			InputListView^ lvInputs = gcnew InputListView();
			ToolTip^ listTip_ = gcnew ToolTip();
			System::Drawing::Point lastTPPos_;
			bool bDragging_;

			bool CheckMovieAndSet(String^ file, bool bSet, bool bShowCodecAlert);


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
			void AddToErrBuffered(String^ text);
			void AfterDrop(array<String^>^ files);

			System::Threading::Thread^ thFFMpeg_;
			System::Diagnostics::Process^ processFFMpeg_;
			bool processSuspeded_;
			bool processTerminatedDuetoAppClose_;
			enum class FFMpegState {
				None,
				ProcessLaunching,
				Running,
				Pausing,
				Intermidiate,
				Unknown,
			};
			enum class FFMpegPriority {
				Normal,
				BelowNormal,
				Background,
			};
			// System::Text::RegularExpressions::Regex^ regFFMpeg_;

			System::Drawing::Icon^ iconBlue_;
			System::Drawing::Icon^ iconYellow_;
			System::Drawing::Icon^ iconRed_;

			enum class ItemSelection {
				All,
				Selectet,
				Incompleted,
				Completed,
			};
			enum class ItemToGet {
				Item,
				Name,
				Duration,
				Fps,
				Aspect,
			};
			System::Collections::ArrayList^ GetItemsCommon(ItemToGet toGet, ItemSelection sel);
			array<ListViewItem^>^ GetItems(ItemSelection sel);
			array<String^>^ GetInputMovies(ItemSelection sel);
			array<AVDuration^>^ GetInputDurations(ItemSelection sel);
			array<double>^ GetInputFPSes(ItemSelection sel);
			array<System::Drawing::Size>^ GetInputAspects(ItemSelection sel);
			property bool HasCompleteItems
			{
				bool get();
			}
			void SetTotalInputInfo();
			void SetFormatStatusText();

			void OnTaskStarted();
			void OnEncodeTaskEnded(int retval);

			void DoNextEncodeTask();
			void OnAllTaskEnded();

			String^ statusTotalInputFormat_;
			property String^ StatusTotalInputFormat
			{
				String^ get() { return statusTotalInputFormat_; }
				void set(String^ v) {
					statusTotalInputFormat_ = v;
					SetFormatStatusText();
				}
			}

			void SetCodecStatusText();
			AVCodec^ statusTotalInputVideoCodec_ = gcnew AVCodec();
			property AVCodec^ StatusTotalInputVideoCodec
			{
				AVCodec^ get() { return statusTotalInputVideoCodec_; }
				void set(AVCodec^ v) {
					statusTotalInputVideoCodec_ = v;
					SetCodecStatusText();
				}
			}
			AVCodec^ statusTotalInputAudioCodec_ = gcnew AVCodec();
			property AVCodec^ StatusTotalInputAudioCodec
			{
				AVCodec^ get() { return statusTotalInputAudioCodec_; }
				void set(AVCodec^ v) {
					statusTotalInputAudioCodec_ = v;
					SetCodecStatusText();
				}
			}






			void SetTimeStatusText();
			AVDuration^ statusTotalInputDuration_ = gcnew AVDuration();
			property AVDuration^ StatusTotalInputDuration
			{
				AVDuration^ get() { return statusTotalInputDuration_; }
				void set(AVDuration^ v) {
					statusTotalInputDuration_ = v;
					SetTimeStatusText();
				}
			}
			
			AVDuration^ statusOutputDuration_ = gcnew AVDuration();
			property AVDuration^ StatusOutputDuration
			{
				AVDuration^ get() { return statusOutputDuration_; }
				void set(AVDuration^ v) {
					DASSERT(v);
					statusOutputDuration_ = v;
					SetTimeStatusText();
				}
			}
			property double TotalInputFPS;

			property FFMpegState CurrentFFMpegState
			{
				FFMpegState get();
			}
			DWORD dwBackPriority_;
			int pidFFMpeg_;

			enum class STATUSTEXT {
				READY,
				REMAINING,
				INTERMEDIATE,
			};

			static initonly int ELAPSESIZE = 50;
			Ambiesoft::FixSizedQueue<ElapseInfo^> elapses_{ ELAPSESIZE };
			String^ GetRemainingTimeText(ElapseInfo^ firstElapse, ElapseInfo^ lastElapse, double total);

			void InsertMovieItem(
				String^ movieFile,
				LONGLONG size,
				System::Drawing::Size aspect,
				String^ format,
				AVCodec^ acodec,
				AVCodec^ vcodec,
				AVDuration^ duration,
				double fps);

			String^ GetMovieFileFromLvi(ListViewItem^ lvi);
			AVDuration^ GetDurationFromLvi(ListViewItem^ lvi);
			double GetFPSFromLvi(ListViewItem^ lvi);

			System::Drawing::Size GetVideoSize(ListViewItem^ lvi);
			double GetVideoArea(ListViewItem^ lvi);
			System::Drawing::Size GetMaxVideoSize(ItemSelection sel);
			bool IsSameSizeVideos(ItemSelection sel);
			String^ tsToString(TimeSpan ts);

			void SetStatusText(STATUSTEXT ss);
			void SetStatusText(STATUSTEXT ss, String^ supplement);

			// bool GetInfoFromFFMpegoutput(String^ text, TimeSpan% tsTime, double% dblSpeed);
			void UpdateTitleTS(TimeSpan ts);
			void UpdateTitleComplete();
			property String^ CurrentEncodingOutputtingFile
			{
				String^ get();
			}
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

			enum class TaskState {
				Ready,
				Intermediate,
				Encoding,
			} taskState_ = TaskState::Ready;
			property TaskState CurrentTaskState
			{
				TaskState get() { return taskState_; }
				void set(TaskState state);
			}

			bool ConfirmAndStopEncode();

			enum class StartButtonText {
				Start,
				Pause,
				Resume,
			};
			void ChangeStartButtonText(StartButtonText sbt);

			void OnProcessStarted(Object^ sender, EventArgs^ e);
			void IconizeToTray();
			array<String^>^ GetOutputtedMoviesFromList(bool bSelectedOnly);

			void UpdateMacros_notyet(ListViewItem^ item);
			void UpdateMacrosFromString_notyet(String^ input, String^ output);
			AfterFinish::OptionDialog dlgAfterFinish_;

			System::Void FormMain_Load(System::Object^ sender, System::EventArgs^ e);
			System::Void exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void btnBrowseMovie_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void btnStart_Click(System::Object^ sender, System::EventArgs^ e);
			void outputHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e);
			void errHandler(Object^ sender, System::Diagnostics::DataReceivedEventArgs^ e);
			void resetControlSize();

			System::Void FormMain_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
			System::Void FormMain_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);

			System::Void ListInputs_DragOver(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e);
			System::Void ListInputs_DragEnter(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e);
			System::Void ListInputs_DragDrop(System::Object^ sender, System::Windows::Forms::DragEventArgs^ e);
			System::Void ListInputs_DragLeave(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiSetFFProbe_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiSetFFMpeg_Click(System::Object^ sender, System::EventArgs^ e);

			String^ baseSetFFProbeMenuString_;
			String^ baseSetFFMpegMenuString_;

			System::Void tsmiOption_DropDownOpening(System::Object^ sender, System::EventArgs^ e);


			String^ getWinnicewArg(FFMpegPriority ffmpegPriority);
			void OnMenuPriorityCommon(FFMpegPriority ffmpegPriority);
			void checkFFMpegPriority(FFMpegPriority ffmpegPriority);
			String^ getFFMpegPriorityAsString(FFMpegPriority ffmpegPriority);
			FFMpegPriority getFFMpegPriorityFromString(String^ priority);

			System::Void tsmiPriorityNormal_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiBelowNormal_Click(System::Object^ sender, System::EventArgs^ e);
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

			System::Void tsmiLanguageCommon_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiLanguage_DropDownOpening(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiRemoveFromList_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiOpenInputLocations_ClickCommon(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiOpenOutput_ClickCommon(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiProcesstsmiProcessAfterFinish_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiCPUAffinity_DropDownOpening(System::Object^ sender, System::EventArgs^ e);

			void OnToggleCPU(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiCPUAffinityEnable_Click(System::Object^ sender, System::EventArgs^ e);

			void OnTick(System::Object^ sender, System::EventArgs^ e);
			void OnColumnClick(System::Object^ sender, System::Windows::Forms::ColumnClickEventArgs^ e);
			System::Void tsmiClearAllButZero_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiEnableAll_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiToggleAll_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiShowLastResult_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiEnableHalfUpper_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiEnableHalfEven_Click(System::Object^ sender, System::EventArgs^ e);

			void OnItemCountChanged();
			void OnItemSelectionChanged(System::Object^ sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs^ e);
			System::Void tsmiClearCompletedItems_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiClearAllItems_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiShowInputFileInExplorer_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void tsmiShowOutputFileInExplorer_Click(System::Object^ sender, System::EventArgs^ e);

			System::Void tsmiEdit_DropDownOpening(System::Object^ sender, System::EventArgs^ e);
			System::Void cmList_Opening(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e);
			System::Void splitRoot_SplitterMoved(System::Object^ sender, System::Windows::Forms::SplitterEventArgs^ e);



			void OnListViewMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
			void OnListViewMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
			void OnListViewMouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
			void OnBtnBrowseMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);

			System::Void timerLogError_Tick(System::Object^ sender, System::EventArgs^ e);

			System::Void timerMouseMove_Tick(System::Object^ sender, System::EventArgs^ e);

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