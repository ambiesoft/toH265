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
			this->tsmiRemoveFromList = (gcnew System::Windows::Forms::ToolStripMenuItem());
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
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiOption = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiSetFFProbe = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiSetFFMpeg = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tsmiPriority = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiPriorityNormal = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiPriorityBackground = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiCPUAffinity = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiCPUAffinityEnable = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmsCpuAffinity = (gcnew System::Windows::Forms::ToolStripSeparator());
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
			this->slFormat = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->slVideoCodec = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->slAudioCodec = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->slDuration = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripMenuItem7 = (gcnew System::Windows::Forms::ToolStripSeparator());
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
			this->cmList->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->tsmiRemoveFromList });
			this->cmList->Name = L"cmList";
			resources->ApplyResources(this->cmList, L"cmList");
			// 
			// tsmiRemoveFromList
			// 
			this->tsmiRemoveFromList->Name = L"tsmiRemoveFromList";
			resources->ApplyResources(this->tsmiRemoveFromList, L"tsmiRemoveFromList");
			this->tsmiRemoveFromList->Click += gcnew System::EventHandler(this, &FormMain::TsmiRemoveFromList_Click);
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
			this->menuMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->tsmiFile, this->tsmiOption,
					this->tsmiHelp
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
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			resources->ApplyResources(this->exitToolStripMenuItem, L"exitToolStripMenuItem");
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::exitToolStripMenuItem_Click);
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
			this->tsmiPriority->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->tsmiPriorityNormal,
					this->tsmiPriorityBackground
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
			// tsmiPriorityBackground
			// 
			this->tsmiPriorityBackground->Name = L"tsmiPriorityBackground";
			resources->ApplyResources(this->tsmiPriorityBackground, L"tsmiPriorityBackground");
			this->tsmiPriorityBackground->Click += gcnew System::EventHandler(this, &FormMain::tsmiPriorityBackground_Click);
			// 
			// tsmiCPUAffinity
			// 
			this->tsmiCPUAffinity->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->tsmiCPUAffinityEnable,
					this->tsmsCpuAffinity
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
			// tsmsCpuAffinity
			// 
			this->tsmsCpuAffinity->Name = L"tsmsCpuAffinity";
			resources->ApplyResources(this->tsmsCpuAffinity, L"tsmsCpuAffinity");
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
			this->statusMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->slMain, this->slFormat,
					this->slVideoCodec, this->slAudioCodec, this->slDuration
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
			// toolStripMenuItem7
			// 
			this->toolStripMenuItem7->Name = L"toolStripMenuItem7";
			resources->ApplyResources(this->toolStripMenuItem7, L"toolStripMenuItem7");
			// 
			// FormMain
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
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