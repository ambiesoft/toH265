#include "stdafx.h"

#include "toH265.h"
#include "helper.h"

#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System::Drawing;
		using namespace System::Collections::Generic;
		using namespace System::Diagnostics;
		using namespace System::IO;
		using namespace System::Text;
		using namespace System::Threading;

		using namespace Newtonsoft::Json::Linq;

		FormMain::FormMain() : dlgAfterFinish_(true,!true,true,true)
		{
			InitializeComponent();

			DefaultStatusColor = statusMain->BackColor;

			lvInputs->Name = L"lvInputs";
			lvInputs->UseCompatibleStateImageBehavior = false;
			lvInputs->View = System::Windows::Forms::View::Details;
			lvInputs->FullRowSelect = !false;
			lvInputs->HideSelection = false;
			lvInputs->MultiSelect = true;
			lvInputs->SmallImageList = ilList;
			// lvInputs->ShowItemToolTips = true;
			lvInputs->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::ListInputs_DragDrop);
			lvInputs->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::ListInputs_DragEnter);
			lvInputs->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::ListInputs_DragOver);
			lvInputs->DragLeave += gcnew System::EventHandler(this, &FormMain::ListInputs_DragLeave);
			lvInputs->AllowDrop = true;

			lvInputs->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Ambiesoft::toH265::FormMain::OnListViewMouseMove);
			lvInputs->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Ambiesoft::toH265::FormMain::OnListViewMouseDown);
			lvInputs->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Ambiesoft::toH265::FormMain::OnListViewMouseUp);
			btnBrowseMovie->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Ambiesoft::toH265::FormMain::OnBtnBrowseMouseMove);
			lvInputs->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &FormMain::OnColumnClick);
			lvInputs->ItemCountChanged += gcnew Ambiesoft::toH265::InputListView::ItemCountChangedHandler(this, &Ambiesoft::toH265::FormMain::OnItemCountChanged);
			lvInputs->ItemSelectionChanged += gcnew System::Windows::Forms::ListViewItemSelectionChangedEventHandler(this, &Ambiesoft::toH265::FormMain::OnItemSelectionChanged);
			for each (ColumnItem^ ci in ColumnItems)
			{
				lvInputs->Columns->Add(ci->Key, ci->Text, ci->Width);
			}


			lvInputs->Dock = DockStyle::Fill;
			panelList->Controls->Add(lvInputs);

			txtLogErr->Font = gcnew System::Drawing::Font(FontFamily::GenericMonospace, txtLogErr->Font->Size + 1);
			txtLogOut->Font = gcnew System::Drawing::Font(FontFamily::GenericMonospace, txtLogOut->Font->Size + 1);

#ifdef _DEBUG
			//StringBuilder sbMessage;
			//sbMessage.AppendLine(String::Format(I18N(L"Compressed = {0}%"), AmbLib::GetRatioString(33333333331LL, 222222221222LL)));
			//MessageBox::Show(sbMessage.ToString());
			// MessageBox::Show(DateTime::Now.ToString());
			tsmiHelp->DropDownItems->Add(
				gcnew ToolStripMenuItem(L"Test", nullptr, gcnew EventHandler(this, &FormMain::FormMain_OnTest)));
#endif

			HashIni ^ ini;
			try
			{
				DTRACE(L"INI=" + Program::IniFile);
				ini = Profile::ReadAll(Program::IniFile, true);
			}
			catch (FileNotFoundException^)
			{
				DASSERT(!ini);
				ini = HashIni::CreateEmptyInstanceForSpecialUse();
			}
			catch (Exception ^ ex)
			{
				CppUtils::Alert(this, ex);
				Environment::Exit(-1);
				return;
			}

			DASSERT(ini);
			bool boolval;
			int intval;
			String^ stringval;
			Profile::GetString(SECTION_OPTION, KEY_FFMPEG_PRIORITY, String::Empty, stringval, ini);
			checkFFMpegPriority(getFFMpegPriorityFromString(stringval));

			Profile::GetBool(SECTION_OPTION, KEY_MINIMIZETOTRAY, false, boolval, ini);
			tsmiMinimizeToTray->Checked = boolval;


			AmbLib::LoadFormXYWH(this, SECTION_LOCATION, ini);
			
			AmbLib::LoadListViewColumnWidth(lvInputs, SECTION_COLUMNS, KEY_LISTVIEW_COLUMNS, ini);
			
			if (Profile::GetInt(SECTION_OPTION, KEY_SPLITROOT_DISTANCE, 50, intval, ini))
			{
				splitRoot->SplitterDistance = intval;
			}

			Profile::GetBool(SECTION_OPTION, KEY_AFTERPROCESS_ENABLED, false, boolval, ini);
			tsmiEnabledtsmiProcessAfterFinish->Checked = boolval;

			Profile::GetBool(SECTION_OPTION, KEY_CPUAFFINITY_ENABLED, false, boolval, ini);
			tsmiCPUAffinityEnable->Checked = boolval;

			cpuAffinity_.Load(SECTION_CPUAFFINITY, ini);

			Profile::GetInt(SECTION_OPTION, KEY_ELAPSEDTIME_QUEUE_LENGTH, -1, intval, ini);
			if (intval > 0)
				elapses_.Size = intval;

			try
			{
#undef ExtractAssociatedIcon
				String^ imagePath = Path::Combine(Path::GetDirectoryName(Application::ExecutablePath), L"images");
				// iconBlue_ = System::Drawing::Icon::FromHandle((gcnew Bitmap(Path::Combine(imagePath, L"icon.png")))->GetHicon());
				iconBlue_ = System::Drawing::Icon::ExtractAssociatedIcon(Path::Combine(imagePath, L"icon.ico"));

				// iconYellow_ = System::Drawing::Icon::FromHandle((gcnew Bitmap(Path::Combine(imagePath, L"pause.png")))->GetHicon());
				iconYellow_ = System::Drawing::Icon::ExtractAssociatedIcon(Path::Combine(imagePath, L"pause.ico"));

				// iconRed_ = System::Drawing::Icon::FromHandle((gcnew Bitmap(Path::Combine(imagePath, L"busy.png")))->GetHicon());
				iconRed_ = System::Drawing::Icon::ExtractAssociatedIcon(Path::Combine(imagePath, L"busy.ico"));
			}
			catch (Exception ^ ex)
			{
				StringBuilder sbMessage;
				sbMessage.AppendLine(I18N(L"Failed to load images."));
				sbMessage.AppendLine(ex->Message);
				CppUtils::Alert(this, sbMessage.ToString());
			}


			DVERIFY(dlgAfterFinish_.LoadValues("AfterFinish", ini));
		}

		void FormMain::OnColumnClick(System::Object^ sender, System::Windows::Forms::ColumnClickEventArgs^ e)
		{
			bSortRev_ = !bSortRev_;
			sortColumn_ = e->Column;
			lvInputs->ListViewItemSorter = this;
			lvInputs->Sort();
		}

		void FormMain::SetItemCountStatus()
		{
			if (lvInputs->SelectedItems->Count != 0)
			{
				slItemCount->Text = String::Format(I18N(L"{0} {1} {2} {3} selected"),
					lvInputs->Items->Count,
					lvInputs->Items->Count == 1 ? I18N("item") : I18N("items"),
					lvInputs->SelectedItems->Count,
					lvInputs->SelectedItems->Count == 1 ? I18N("item") : I18N("items"));
			}
			else
			{
				slItemCount->Text = String::Format(I18N(L"{0} {1}"),
					lvInputs->Items->Count,
					lvInputs->Items->Count == 1 ? I18N("item") : I18N("items"));
			}
		}
		void FormMain::OnItemCountChanged()
		{
			SetItemCountStatus();
		}
		void FormMain::OnItemSelectionChanged(System::Object^ sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs^ e)
		{
			SetItemCountStatus();
		}

	}
}










