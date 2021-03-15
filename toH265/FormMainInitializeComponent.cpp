#include "stdafx.h"

#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		void FormMain::InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMain::typeid));
            this->splitRoot = (gcnew System::Windows::Forms::SplitContainer());
            this->btnBrowseMovie = (gcnew System::Windows::Forms::Button());
            this->panelList = (gcnew System::Windows::Forms::Panel());
            this->cmList = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->tsmiShowInputFileInExplorer = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiShowOutputFileInExplorer = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem8 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->tsmiRemoveFromList = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->lblBottomRight = (gcnew System::Windows::Forms::Label());
            this->lblTopLeft = (gcnew System::Windows::Forms::Label());
            this->txtFFMpegArg = (gcnew System::Windows::Forms::TextBox());
            this->btnStart = (gcnew System::Windows::Forms::Button());
            this->txtLogErr = (gcnew System::Windows::Forms::TextBox());
            this->txtLogOut = (gcnew System::Windows::Forms::TextBox());
            this->menuMain = (gcnew System::Windows::Forms::MenuStrip());
            this->tsmiFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiOpenInputLocations = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiOpenOutput = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->tsmiStop = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem7 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiEdit = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiClearCompletedItems = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiClearAllItems = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiView = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiShowLastResult = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiOption = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiSetFFProbe = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiSetFFMpeg = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->tsmiPriority = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiPriorityNormal = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiPriorityBelowNormal = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiPriorityBackground = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiCPUAffinity = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiCPUAffinityEnable = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmsCpuAffinityBegin = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->tsmsCpuAffinityEnd = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->tsmiEnableAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiToggleAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiClearAllButZero = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem6 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->tsmiProcessAfterFinish = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiEnabledtsmiProcessAfterFinish = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiProcesstsmiProcessAfterFinish = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->tsmiMinimizeToTray = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->tsmiLanguage = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiLanguageOSDefault = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiLanguageEnglish = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiLanguageJapanese = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiAbout = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiFFMpegHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->notifyIconMain = (gcnew System::Windows::Forms::NotifyIcon(this->components));
            this->cmNotify = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->tsmiNotifyOpenInputLocations = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiNotifyOpenOutputLocation = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripMenuItem5 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->tsmiNotifyShow = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->tsmiNotifyStart = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->statusMain = (gcnew System::Windows::Forms::StatusStrip());
            this->slMain = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->slItemCount = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->slFormat = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->slVideoCodec = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->slAudioCodec = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->slDuration = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->ilList = (gcnew System::Windows::Forms::ImageList(this->components));
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitRoot))->BeginInit();
            this->splitRoot->Panel1->SuspendLayout();
            this->splitRoot->Panel2->SuspendLayout();
            this->splitRoot->SuspendLayout();
            this->cmList->SuspendLayout();
            this->menuMain->SuspendLayout();
            this->cmNotify->SuspendLayout();
            this->statusMain->SuspendLayout();
            this->SuspendLayout();
            // 
            // splitRoot
            // 
            resources->ApplyResources(this->splitRoot, L"splitRoot");
            this->splitRoot->Name = L"splitRoot";
            // 
            // splitRoot.Panel1
            // 
            this->splitRoot->Panel1->Controls->Add(this->btnBrowseMovie);
            this->splitRoot->Panel1->Controls->Add(this->panelList);
            // 
            // splitRoot.Panel2
            // 
            this->splitRoot->Panel2->Controls->Add(this->lblBottomRight);
            this->splitRoot->Panel2->Controls->Add(this->lblTopLeft);
            this->splitRoot->Panel2->Controls->Add(this->txtFFMpegArg);
            this->splitRoot->Panel2->Controls->Add(this->btnStart);
            this->splitRoot->Panel2->Controls->Add(this->txtLogErr);
            this->splitRoot->Panel2->Controls->Add(this->txtLogOut);
            // 
            // btnBrowseMovie
            // 
            resources->ApplyResources(this->btnBrowseMovie, L"btnBrowseMovie");
            this->btnBrowseMovie->Name = L"btnBrowseMovie";
            this->btnBrowseMovie->UseVisualStyleBackColor = true;
            this->btnBrowseMovie->Click += gcnew System::EventHandler(this, &FormMain::btnBrowseMovie_Click);
            // 
            // panelList
            // 
            resources->ApplyResources(this->panelList, L"panelList");
            this->panelList->ContextMenuStrip = this->cmList;
            this->panelList->Name = L"panelList";
            // 
            // cmList
            // 
            this->cmList->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
                this->tsmiShowInputFileInExplorer,
                    this->tsmiShowOutputFileInExplorer, this->toolStripMenuItem8, this->tsmiRemoveFromList
            });
            this->cmList->Name = L"cmList";
            resources->ApplyResources(this->cmList, L"cmList");
            this->cmList->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &FormMain::cmList_Opening);
            // 
            // tsmiShowInputFileInExplorer
            // 
            this->tsmiShowInputFileInExplorer->Name = L"tsmiShowInputFileInExplorer";
            resources->ApplyResources(this->tsmiShowInputFileInExplorer, L"tsmiShowInputFileInExplorer");
            this->tsmiShowInputFileInExplorer->Click += gcnew System::EventHandler(this, &FormMain::tsmiShowInputFileInExplorer_Click);
            // 
            // tsmiShowOutputFileInExplorer
            // 
            this->tsmiShowOutputFileInExplorer->Name = L"tsmiShowOutputFileInExplorer";
            resources->ApplyResources(this->tsmiShowOutputFileInExplorer, L"tsmiShowOutputFileInExplorer");
            this->tsmiShowOutputFileInExplorer->Click += gcnew System::EventHandler(this, &FormMain::tsmiShowOutputFileInExplorer_Click);
            // 
            // toolStripMenuItem8
            // 
            this->toolStripMenuItem8->Name = L"toolStripMenuItem8";
            resources->ApplyResources(this->toolStripMenuItem8, L"toolStripMenuItem8");
            // 
            // tsmiRemoveFromList
            // 
            this->tsmiRemoveFromList->Name = L"tsmiRemoveFromList";
            resources->ApplyResources(this->tsmiRemoveFromList, L"tsmiRemoveFromList");
            this->tsmiRemoveFromList->Click += gcnew System::EventHandler(this, &FormMain::tsmiRemoveFromList_Click);
            // 
            // lblBottomRight
            // 
            resources->ApplyResources(this->lblBottomRight, L"lblBottomRight");
            this->lblBottomRight->Name = L"lblBottomRight";
            // 
            // lblTopLeft
            // 
            resources->ApplyResources(this->lblTopLeft, L"lblTopLeft");
            this->lblTopLeft->Name = L"lblTopLeft";
            // 
            // txtFFMpegArg
            // 
            resources->ApplyResources(this->txtFFMpegArg, L"txtFFMpegArg");
            this->txtFFMpegArg->Name = L"txtFFMpegArg";
            this->txtFFMpegArg->ReadOnly = true;
            // 
            // btnStart
            // 
            resources->ApplyResources(this->btnStart, L"btnStart");
            this->btnStart->Name = L"btnStart";
            this->btnStart->UseVisualStyleBackColor = true;
            this->btnStart->Click += gcnew System::EventHandler(this, &FormMain::btnStart_Click);
            // 
            // txtLogErr
            // 
            resources->ApplyResources(this->txtLogErr, L"txtLogErr");
            this->txtLogErr->Name = L"txtLogErr";
            this->txtLogErr->ReadOnly = true;
            // 
            // txtLogOut
            // 
            resources->ApplyResources(this->txtLogOut, L"txtLogOut");
            this->txtLogOut->Name = L"txtLogOut";
            // 
            // menuMain
            // 
            this->menuMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
                this->tsmiFile, this->tsmiEdit,
                    this->tsmiView, this->tsmiOption, this->tsmiHelp
            });
            resources->ApplyResources(this->menuMain, L"menuMain");
            this->menuMain->Name = L"menuMain";
            // 
            // tsmiFile
            // 
            this->tsmiFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
                this->tsmiOpenInputLocations,
                    this->tsmiOpenOutput, this->toolStripMenuItem4, this->tsmiStop, this->toolStripMenuItem7, this->exitToolStripMenuItem
            });
            this->tsmiFile->Name = L"tsmiFile";
            resources->ApplyResources(this->tsmiFile, L"tsmiFile");
            // 
            // tsmiOpenInputLocations
            // 
            this->tsmiOpenInputLocations->Name = L"tsmiOpenInputLocations";
            resources->ApplyResources(this->tsmiOpenInputLocations, L"tsmiOpenInputLocations");
            this->tsmiOpenInputLocations->Click += gcnew System::EventHandler(this, &FormMain::tsmiOpenInputLocations_ClickCommon);
            // 
            // tsmiOpenOutput
            // 
            this->tsmiOpenOutput->Name = L"tsmiOpenOutput";
            resources->ApplyResources(this->tsmiOpenOutput, L"tsmiOpenOutput");
            this->tsmiOpenOutput->Click += gcnew System::EventHandler(this, &FormMain::tsmiOpenOutput_ClickCommon);
            // 
            // toolStripMenuItem4
            // 
            this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
            resources->ApplyResources(this->toolStripMenuItem4, L"toolStripMenuItem4");
            // 
            // tsmiStop
            // 
            this->tsmiStop->Name = L"tsmiStop";
            resources->ApplyResources(this->tsmiStop, L"tsmiStop");
            this->tsmiStop->Click += gcnew System::EventHandler(this, &FormMain::tsmiStop_Click);
            // 
            // toolStripMenuItem7
            // 
            this->toolStripMenuItem7->Name = L"toolStripMenuItem7";
            resources->ApplyResources(this->toolStripMenuItem7, L"toolStripMenuItem7");
            // 
            // exitToolStripMenuItem
            // 
            this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
            resources->ApplyResources(this->exitToolStripMenuItem, L"exitToolStripMenuItem");
            this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::exitToolStripMenuItem_Click);
            // 
            // tsmiEdit
            // 
            this->tsmiEdit->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->tsmiClearCompletedItems,
                    this->tsmiClearAllItems
            });
            this->tsmiEdit->Name = L"tsmiEdit";
            resources->ApplyResources(this->tsmiEdit, L"tsmiEdit");
            this->tsmiEdit->DropDownOpening += gcnew System::EventHandler(this, &FormMain::tsmiEdit_DropDownOpening);
            // 
            // tsmiClearCompletedItems
            // 
            this->tsmiClearCompletedItems->Name = L"tsmiClearCompletedItems";
            resources->ApplyResources(this->tsmiClearCompletedItems, L"tsmiClearCompletedItems");
            this->tsmiClearCompletedItems->Click += gcnew System::EventHandler(this, &FormMain::tsmiClearCompletedItems_Click);
            // 
            // tsmiClearAllItems
            // 
            this->tsmiClearAllItems->Name = L"tsmiClearAllItems";
            resources->ApplyResources(this->tsmiClearAllItems, L"tsmiClearAllItems");
            this->tsmiClearAllItems->Click += gcnew System::EventHandler(this, &FormMain::tsmiClearAllItems_Click);
            // 
            // tsmiView
            // 
            this->tsmiView->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->tsmiShowLastResult });
            this->tsmiView->Name = L"tsmiView";
            resources->ApplyResources(this->tsmiView, L"tsmiView");
            // 
            // tsmiShowLastResult
            // 
            this->tsmiShowLastResult->Name = L"tsmiShowLastResult";
            resources->ApplyResources(this->tsmiShowLastResult, L"tsmiShowLastResult");
            this->tsmiShowLastResult->Click += gcnew System::EventHandler(this, &FormMain::tsmiShowLastResult_Click);
            // 
            // tsmiOption
            // 
            this->tsmiOption->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(11) {
                this->tsmiSetFFProbe,
                    this->tsmiSetFFMpeg, this->toolStripMenuItem1, this->tsmiPriority, this->tsmiCPUAffinity, this->toolStripMenuItem6, this->tsmiProcessAfterFinish,
                    this->toolStripMenuItem2, this->tsmiMinimizeToTray, this->toolStripMenuItem3, this->tsmiLanguage
            });
            this->tsmiOption->Name = L"tsmiOption";
            resources->ApplyResources(this->tsmiOption, L"tsmiOption");
            this->tsmiOption->DropDownOpening += gcnew System::EventHandler(this, &FormMain::tsmiOption_DropDownOpening);
            // 
            // tsmiSetFFProbe
            // 
            this->tsmiSetFFProbe->Name = L"tsmiSetFFProbe";
            resources->ApplyResources(this->tsmiSetFFProbe, L"tsmiSetFFProbe");
            this->tsmiSetFFProbe->Click += gcnew System::EventHandler(this, &FormMain::tsmiSetFFProbe_Click);
            // 
            // tsmiSetFFMpeg
            // 
            this->tsmiSetFFMpeg->Name = L"tsmiSetFFMpeg";
            resources->ApplyResources(this->tsmiSetFFMpeg, L"tsmiSetFFMpeg");
            this->tsmiSetFFMpeg->Click += gcnew System::EventHandler(this, &FormMain::tsmiSetFFMpeg_Click);
            // 
            // toolStripMenuItem1
            // 
            this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
            resources->ApplyResources(this->toolStripMenuItem1, L"toolStripMenuItem1");
            // 
            // tsmiPriority
            // 
            this->tsmiPriority->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
                this->tsmiPriorityNormal,
                    this->tsmiPriorityBelowNormal, this->tsmiPriorityBackground
            });
            this->tsmiPriority->Name = L"tsmiPriority";
            resources->ApplyResources(this->tsmiPriority, L"tsmiPriority");
            // 
            // tsmiPriorityNormal
            // 
            this->tsmiPriorityNormal->Checked = true;
            this->tsmiPriorityNormal->CheckState = System::Windows::Forms::CheckState::Checked;
            this->tsmiPriorityNormal->Name = L"tsmiPriorityNormal";
            resources->ApplyResources(this->tsmiPriorityNormal, L"tsmiPriorityNormal");
            this->tsmiPriorityNormal->Click += gcnew System::EventHandler(this, &FormMain::tsmiPriorityNormal_Click);
            // 
            // tsmiPriorityBelowNormal
            // 
            this->tsmiPriorityBelowNormal->Name = L"tsmiPriorityBelowNormal";
            resources->ApplyResources(this->tsmiPriorityBelowNormal, L"tsmiPriorityBelowNormal");
            this->tsmiPriorityBelowNormal->Click += gcnew System::EventHandler(this, &FormMain::tsmiBelowNormal_Click);
            // 
            // tsmiPriorityBackground
            // 
            this->tsmiPriorityBackground->Name = L"tsmiPriorityBackground";
            resources->ApplyResources(this->tsmiPriorityBackground, L"tsmiPriorityBackground");
            this->tsmiPriorityBackground->Click += gcnew System::EventHandler(this, &FormMain::tsmiPriorityBackground_Click);
            // 
            // tsmiCPUAffinity
            // 
            this->tsmiCPUAffinity->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
                this->tsmiCPUAffinityEnable,
                    this->tsmsCpuAffinityBegin, this->tsmsCpuAffinityEnd, this->tsmiEnableAll, this->tsmiToggleAll, this->tsmiClearAllButZero
            });
            this->tsmiCPUAffinity->Name = L"tsmiCPUAffinity";
            resources->ApplyResources(this->tsmiCPUAffinity, L"tsmiCPUAffinity");
            this->tsmiCPUAffinity->DropDownOpening += gcnew System::EventHandler(this, &FormMain::tsmiCPUAffinity_DropDownOpening);
            // 
            // tsmiCPUAffinityEnable
            // 
            this->tsmiCPUAffinityEnable->CheckOnClick = true;
            this->tsmiCPUAffinityEnable->Name = L"tsmiCPUAffinityEnable";
            resources->ApplyResources(this->tsmiCPUAffinityEnable, L"tsmiCPUAffinityEnable");
            this->tsmiCPUAffinityEnable->Click += gcnew System::EventHandler(this, &FormMain::tsmiCPUAffinityEnable_Click);
            // 
            // tsmsCpuAffinityBegin
            // 
            this->tsmsCpuAffinityBegin->Name = L"tsmsCpuAffinityBegin";
            resources->ApplyResources(this->tsmsCpuAffinityBegin, L"tsmsCpuAffinityBegin");
            // 
            // tsmsCpuAffinityEnd
            // 
            this->tsmsCpuAffinityEnd->Name = L"tsmsCpuAffinityEnd";
            resources->ApplyResources(this->tsmsCpuAffinityEnd, L"tsmsCpuAffinityEnd");
            // 
            // tsmiEnableAll
            // 
            this->tsmiEnableAll->Name = L"tsmiEnableAll";
            resources->ApplyResources(this->tsmiEnableAll, L"tsmiEnableAll");
            this->tsmiEnableAll->Click += gcnew System::EventHandler(this, &FormMain::tsmiEnableAll_Click);
            // 
            // tsmiToggleAll
            // 
            this->tsmiToggleAll->Name = L"tsmiToggleAll";
            resources->ApplyResources(this->tsmiToggleAll, L"tsmiToggleAll");
            this->tsmiToggleAll->Click += gcnew System::EventHandler(this, &FormMain::tsmiToggleAll_Click);
            // 
            // tsmiClearAllButZero
            // 
            this->tsmiClearAllButZero->Name = L"tsmiClearAllButZero";
            resources->ApplyResources(this->tsmiClearAllButZero, L"tsmiClearAllButZero");
            this->tsmiClearAllButZero->Click += gcnew System::EventHandler(this, &FormMain::tsmiClearAllButZero_Click);
            // 
            // toolStripMenuItem6
            // 
            this->toolStripMenuItem6->Name = L"toolStripMenuItem6";
            resources->ApplyResources(this->toolStripMenuItem6, L"toolStripMenuItem6");
            // 
            // tsmiProcessAfterFinish
            // 
            this->tsmiProcessAfterFinish->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->tsmiEnabledtsmiProcessAfterFinish,
                    this->tsmiProcesstsmiProcessAfterFinish
            });
            this->tsmiProcessAfterFinish->Name = L"tsmiProcessAfterFinish";
            resources->ApplyResources(this->tsmiProcessAfterFinish, L"tsmiProcessAfterFinish");
            // 
            // tsmiEnabledtsmiProcessAfterFinish
            // 
            this->tsmiEnabledtsmiProcessAfterFinish->CheckOnClick = true;
            this->tsmiEnabledtsmiProcessAfterFinish->Name = L"tsmiEnabledtsmiProcessAfterFinish";
            resources->ApplyResources(this->tsmiEnabledtsmiProcessAfterFinish, L"tsmiEnabledtsmiProcessAfterFinish");
            // 
            // tsmiProcesstsmiProcessAfterFinish
            // 
            this->tsmiProcesstsmiProcessAfterFinish->Name = L"tsmiProcesstsmiProcessAfterFinish";
            resources->ApplyResources(this->tsmiProcesstsmiProcessAfterFinish, L"tsmiProcesstsmiProcessAfterFinish");
            this->tsmiProcesstsmiProcessAfterFinish->Click += gcnew System::EventHandler(this, &FormMain::tsmiProcesstsmiProcessAfterFinish_Click);
            // 
            // toolStripMenuItem2
            // 
            this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
            resources->ApplyResources(this->toolStripMenuItem2, L"toolStripMenuItem2");
            // 
            // tsmiMinimizeToTray
            // 
            this->tsmiMinimizeToTray->Name = L"tsmiMinimizeToTray";
            resources->ApplyResources(this->tsmiMinimizeToTray, L"tsmiMinimizeToTray");
            this->tsmiMinimizeToTray->Click += gcnew System::EventHandler(this, &FormMain::tsmiMinimizeToTray_Click);
            // 
            // toolStripMenuItem3
            // 
            this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
            resources->ApplyResources(this->toolStripMenuItem3, L"toolStripMenuItem3");
            // 
            // tsmiLanguage
            // 
            this->tsmiLanguage->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
                this->tsmiLanguageOSDefault,
                    this->tsmiLanguageEnglish, this->tsmiLanguageJapanese
            });
            this->tsmiLanguage->Name = L"tsmiLanguage";
            resources->ApplyResources(this->tsmiLanguage, L"tsmiLanguage");
            this->tsmiLanguage->DropDownOpening += gcnew System::EventHandler(this, &FormMain::tsmiLanguage_DropDownOpening);
            // 
            // tsmiLanguageOSDefault
            // 
            this->tsmiLanguageOSDefault->Name = L"tsmiLanguageOSDefault";
            resources->ApplyResources(this->tsmiLanguageOSDefault, L"tsmiLanguageOSDefault");
            this->tsmiLanguageOSDefault->Click += gcnew System::EventHandler(this, &FormMain::tsmiLanguageCommon_Click);
            // 
            // tsmiLanguageEnglish
            // 
            this->tsmiLanguageEnglish->Name = L"tsmiLanguageEnglish";
            resources->ApplyResources(this->tsmiLanguageEnglish, L"tsmiLanguageEnglish");
            this->tsmiLanguageEnglish->Tag = L"en-US";
            this->tsmiLanguageEnglish->Click += gcnew System::EventHandler(this, &FormMain::tsmiLanguageCommon_Click);
            // 
            // tsmiLanguageJapanese
            // 
            this->tsmiLanguageJapanese->Name = L"tsmiLanguageJapanese";
            resources->ApplyResources(this->tsmiLanguageJapanese, L"tsmiLanguageJapanese");
            this->tsmiLanguageJapanese->Tag = L"ja-JP";
            this->tsmiLanguageJapanese->Click += gcnew System::EventHandler(this, &FormMain::tsmiLanguageCommon_Click);
            // 
            // tsmiHelp
            // 
            this->tsmiHelp->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->tsmiAbout,
                    this->tsmiFFMpegHelp
            });
            this->tsmiHelp->Name = L"tsmiHelp";
            resources->ApplyResources(this->tsmiHelp, L"tsmiHelp");
            // 
            // tsmiAbout
            // 
            this->tsmiAbout->Name = L"tsmiAbout";
            resources->ApplyResources(this->tsmiAbout, L"tsmiAbout");
            this->tsmiAbout->Click += gcnew System::EventHandler(this, &FormMain::tsmiAbout_Click);
            // 
            // tsmiFFMpegHelp
            // 
            this->tsmiFFMpegHelp->Name = L"tsmiFFMpegHelp";
            resources->ApplyResources(this->tsmiFFMpegHelp, L"tsmiFFMpegHelp");
            this->tsmiFFMpegHelp->Click += gcnew System::EventHandler(this, &FormMain::tsmiFFMpegHelp_Click);
            // 
            // notifyIconMain
            // 
            this->notifyIconMain->ContextMenuStrip = this->cmNotify;
            resources->ApplyResources(this->notifyIconMain, L"notifyIconMain");
            this->notifyIconMain->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &FormMain::notifyIconMain_MouseClick);
            // 
            // cmNotify
            // 
            this->cmNotify->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
                this->tsmiNotifyOpenInputLocations,
                    this->tsmiNotifyOpenOutputLocation, this->toolStripMenuItem5, this->tsmiNotifyShow, this->tsmiNotifyStart
            });
            this->cmNotify->Name = L"cmNotify";
            resources->ApplyResources(this->cmNotify, L"cmNotify");
            // 
            // tsmiNotifyOpenInputLocations
            // 
            this->tsmiNotifyOpenInputLocations->Name = L"tsmiNotifyOpenInputLocations";
            resources->ApplyResources(this->tsmiNotifyOpenInputLocations, L"tsmiNotifyOpenInputLocations");
            this->tsmiNotifyOpenInputLocations->Click += gcnew System::EventHandler(this, &FormMain::tsmiOpenInputLocations_ClickCommon);
            // 
            // tsmiNotifyOpenOutputLocation
            // 
            this->tsmiNotifyOpenOutputLocation->Name = L"tsmiNotifyOpenOutputLocation";
            resources->ApplyResources(this->tsmiNotifyOpenOutputLocation, L"tsmiNotifyOpenOutputLocation");
            this->tsmiNotifyOpenOutputLocation->Click += gcnew System::EventHandler(this, &FormMain::tsmiOpenOutput_ClickCommon);
            // 
            // toolStripMenuItem5
            // 
            this->toolStripMenuItem5->Name = L"toolStripMenuItem5";
            resources->ApplyResources(this->toolStripMenuItem5, L"toolStripMenuItem5");
            // 
            // tsmiNotifyShow
            // 
            this->tsmiNotifyShow->Name = L"tsmiNotifyShow";
            resources->ApplyResources(this->tsmiNotifyShow, L"tsmiNotifyShow");
            this->tsmiNotifyShow->Click += gcnew System::EventHandler(this, &FormMain::tsmiNotifyShow_Click);
            // 
            // tsmiNotifyStart
            // 
            this->tsmiNotifyStart->Name = L"tsmiNotifyStart";
            resources->ApplyResources(this->tsmiNotifyStart, L"tsmiNotifyStart");
            this->tsmiNotifyStart->Click += gcnew System::EventHandler(this, &FormMain::tsmiNotifyStart_Click);
            // 
            // statusMain
            // 
            this->statusMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
                this->slMain, this->slItemCount,
                    this->slFormat, this->slVideoCodec, this->slAudioCodec, this->slDuration
            });
            resources->ApplyResources(this->statusMain, L"statusMain");
            this->statusMain->Name = L"statusMain";
            // 
            // slMain
            // 
            this->slMain->Name = L"slMain";
            resources->ApplyResources(this->slMain, L"slMain");
            this->slMain->Spring = true;
            // 
            // slItemCount
            // 
            this->slItemCount->Name = L"slItemCount";
            resources->ApplyResources(this->slItemCount, L"slItemCount");
            // 
            // slFormat
            // 
            this->slFormat->Name = L"slFormat";
            resources->ApplyResources(this->slFormat, L"slFormat");
            // 
            // slVideoCodec
            // 
            this->slVideoCodec->Name = L"slVideoCodec";
            resources->ApplyResources(this->slVideoCodec, L"slVideoCodec");
            // 
            // slAudioCodec
            // 
            this->slAudioCodec->Name = L"slAudioCodec";
            resources->ApplyResources(this->slAudioCodec, L"slAudioCodec");
            // 
            // slDuration
            // 
            this->slDuration->Name = L"slDuration";
            resources->ApplyResources(this->slDuration, L"slDuration");
            // 
            // ilList
            // 
            this->ilList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^>(resources->GetObject(L"ilList.ImageStream")));
            this->ilList->TransparentColor = System::Drawing::Color::Transparent;
            this->ilList->Images->SetKeyName(0, L"listimage_normal.png");
            this->ilList->Images->SetKeyName(1, L"listimage_encoding.png");
            this->ilList->Images->SetKeyName(2, L"listimage_done.png");
            // 
            // FormMain
            // 
            resources->ApplyResources(this, L"$this");
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
            this->Controls->Add(this->splitRoot);
            this->Controls->Add(this->menuMain);
            this->Controls->Add(this->statusMain);
            this->MainMenuStrip = this->menuMain;
            this->Name = L"FormMain";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormMain::FormMain_FormClosed);
            this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
            this->Resize += gcnew System::EventHandler(this, &FormMain::FormMain_Resize);
            this->splitRoot->Panel1->ResumeLayout(false);
            this->splitRoot->Panel2->ResumeLayout(false);
            this->splitRoot->Panel2->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitRoot))->EndInit();
            this->splitRoot->ResumeLayout(false);
            this->cmList->ResumeLayout(false);
            this->menuMain->ResumeLayout(false);
            this->menuMain->PerformLayout();
            this->cmNotify->ResumeLayout(false);
            this->statusMain->ResumeLayout(false);
            this->statusMain->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }

	}

}