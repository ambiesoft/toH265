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
			this->tsmiStop = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiOption = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiSetFFProbe = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiSetFFMpeg = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->tsmiPriority = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiPriorityNormal = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiPriorityBackground = (gcnew System::Windows::Forms::ToolStripMenuItem());
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
			this->tsmiNotifyShow = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiNotifyStart = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusMain = (gcnew System::Windows::Forms::StatusStrip());
			this->slMain = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->slFormat = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->slVideoCodec = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->slAudioCodec = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->slDuration = (gcnew System::Windows::Forms::ToolStripStatusLabel());
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
			resources->ApplyResources(this->splitRoot->Panel1, L"splitRoot.Panel1");
			this->splitRoot->Panel1->Controls->Add(this->btnBrowseMovie);
			this->splitRoot->Panel1->Controls->Add(this->panelList);
			// 
			// splitRoot.Panel2
			// 
			resources->ApplyResources(this->splitRoot->Panel2, L"splitRoot.Panel2");
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
			resources->ApplyResources(this->cmList, L"cmList");
			this->cmList->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->tsmiRemoveFromList });
			this->cmList->Name = L"cmList";
			// 
			// tsmiRemoveFromList
			// 
			resources->ApplyResources(this->tsmiRemoveFromList, L"tsmiRemoveFromList");
			this->tsmiRemoveFromList->Name = L"tsmiRemoveFromList";
			this->tsmiRemoveFromList->Click += gcnew System::EventHandler(this, &FormMain::TsmiRemoveFromList_Click);
			// 
			// txtFFMpegArg
			// 
			resources->ApplyResources(this->txtFFMpegArg, L"txtFFMpegArg");
			this->txtFFMpegArg->Name = L"txtFFMpegArg";
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
			// 
			// txtLogOut
			// 
			resources->ApplyResources(this->txtLogOut, L"txtLogOut");
			this->txtLogOut->Name = L"txtLogOut";
			// 
			// menuMain
			// 
			resources->ApplyResources(this->menuMain, L"menuMain");
			this->menuMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->tsmiFile, this->tsmiOption,
					this->tsmiHelp
			});
			this->menuMain->Name = L"menuMain";
			// 
			// tsmiFile
			// 
			resources->ApplyResources(this->tsmiFile, L"tsmiFile");
			this->tsmiFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->tsmiStop, this->exitToolStripMenuItem });
			this->tsmiFile->Name = L"tsmiFile";
			// 
			// tsmiStop
			// 
			resources->ApplyResources(this->tsmiStop, L"tsmiStop");
			this->tsmiStop->Name = L"tsmiStop";
			this->tsmiStop->Click += gcnew System::EventHandler(this, &FormMain::tsmiStop_Click);
			// 
			// exitToolStripMenuItem
			// 
			resources->ApplyResources(this->exitToolStripMenuItem, L"exitToolStripMenuItem");
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::exitToolStripMenuItem_Click);
			// 
			// tsmiOption
			// 
			resources->ApplyResources(this->tsmiOption, L"tsmiOption");
			this->tsmiOption->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {
				this->tsmiSetFFProbe,
					this->tsmiSetFFMpeg, this->toolStripMenuItem1, this->tsmiPriority, this->toolStripMenuItem2, this->tsmiMinimizeToTray, this->toolStripMenuItem3,
					this->tsmiLanguage
			});
			this->tsmiOption->Name = L"tsmiOption";
			this->tsmiOption->DropDownOpening += gcnew System::EventHandler(this, &FormMain::tsmiOption_DropDownOpening);
			// 
			// tsmiSetFFProbe
			// 
			resources->ApplyResources(this->tsmiSetFFProbe, L"tsmiSetFFProbe");
			this->tsmiSetFFProbe->Name = L"tsmiSetFFProbe";
			this->tsmiSetFFProbe->Click += gcnew System::EventHandler(this, &FormMain::tsmiSetFFProbe_Click);
			// 
			// tsmiSetFFMpeg
			// 
			resources->ApplyResources(this->tsmiSetFFMpeg, L"tsmiSetFFMpeg");
			this->tsmiSetFFMpeg->Name = L"tsmiSetFFMpeg";
			this->tsmiSetFFMpeg->Click += gcnew System::EventHandler(this, &FormMain::tsmiSetFFMpeg_Click);
			// 
			// toolStripMenuItem1
			// 
			resources->ApplyResources(this->toolStripMenuItem1, L"toolStripMenuItem1");
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			// 
			// tsmiPriority
			// 
			resources->ApplyResources(this->tsmiPriority, L"tsmiPriority");
			this->tsmiPriority->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->tsmiPriorityNormal,
					this->tsmiPriorityBackground
			});
			this->tsmiPriority->Name = L"tsmiPriority";
			// 
			// tsmiPriorityNormal
			// 
			resources->ApplyResources(this->tsmiPriorityNormal, L"tsmiPriorityNormal");
			this->tsmiPriorityNormal->Checked = true;
			this->tsmiPriorityNormal->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tsmiPriorityNormal->Name = L"tsmiPriorityNormal";
			this->tsmiPriorityNormal->Click += gcnew System::EventHandler(this, &FormMain::tsmiPriorityNormal_Click);
			// 
			// tsmiPriorityBackground
			// 
			resources->ApplyResources(this->tsmiPriorityBackground, L"tsmiPriorityBackground");
			this->tsmiPriorityBackground->Name = L"tsmiPriorityBackground";
			this->tsmiPriorityBackground->Click += gcnew System::EventHandler(this, &FormMain::tsmiPriorityBackground_Click);
			// 
			// toolStripMenuItem2
			// 
			resources->ApplyResources(this->toolStripMenuItem2, L"toolStripMenuItem2");
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			// 
			// tsmiMinimizeToTray
			// 
			resources->ApplyResources(this->tsmiMinimizeToTray, L"tsmiMinimizeToTray");
			this->tsmiMinimizeToTray->Name = L"tsmiMinimizeToTray";
			this->tsmiMinimizeToTray->Click += gcnew System::EventHandler(this, &FormMain::tsmiMinimizeToTray_Click);
			// 
			// toolStripMenuItem3
			// 
			resources->ApplyResources(this->toolStripMenuItem3, L"toolStripMenuItem3");
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			// 
			// tsmiLanguage
			// 
			resources->ApplyResources(this->tsmiLanguage, L"tsmiLanguage");
			this->tsmiLanguage->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->tsmiLanguageOSDefault,
					this->tsmiLanguageEnglish, this->tsmiLanguageJapanese
			});
			this->tsmiLanguage->Name = L"tsmiLanguage";
			this->tsmiLanguage->DropDownOpening += gcnew System::EventHandler(this, &FormMain::tsmiLanguage_DropDownOpening);
			// 
			// tsmiLanguageOSDefault
			// 
			resources->ApplyResources(this->tsmiLanguageOSDefault, L"tsmiLanguageOSDefault");
			this->tsmiLanguageOSDefault->Name = L"tsmiLanguageOSDefault";
			this->tsmiLanguageOSDefault->Click += gcnew System::EventHandler(this, &FormMain::tsmiLanguageCommon_Click);
			// 
			// tsmiLanguageEnglish
			// 
			resources->ApplyResources(this->tsmiLanguageEnglish, L"tsmiLanguageEnglish");
			this->tsmiLanguageEnglish->Name = L"tsmiLanguageEnglish";
			this->tsmiLanguageEnglish->Tag = L"en-US";
			this->tsmiLanguageEnglish->Click += gcnew System::EventHandler(this, &FormMain::tsmiLanguageCommon_Click);
			// 
			// tsmiLanguageJapanese
			// 
			resources->ApplyResources(this->tsmiLanguageJapanese, L"tsmiLanguageJapanese");
			this->tsmiLanguageJapanese->Name = L"tsmiLanguageJapanese";
			this->tsmiLanguageJapanese->Tag = L"ja-JP";
			this->tsmiLanguageJapanese->Click += gcnew System::EventHandler(this, &FormMain::tsmiLanguageCommon_Click);
			// 
			// tsmiHelp
			// 
			resources->ApplyResources(this->tsmiHelp, L"tsmiHelp");
			this->tsmiHelp->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->tsmiAbout,
					this->tsmiFFMpegHelp
			});
			this->tsmiHelp->Name = L"tsmiHelp";
			// 
			// tsmiAbout
			// 
			resources->ApplyResources(this->tsmiAbout, L"tsmiAbout");
			this->tsmiAbout->Name = L"tsmiAbout";
			this->tsmiAbout->Click += gcnew System::EventHandler(this, &FormMain::tsmiAbout_Click);
			// 
			// tsmiFFMpegHelp
			// 
			resources->ApplyResources(this->tsmiFFMpegHelp, L"tsmiFFMpegHelp");
			this->tsmiFFMpegHelp->Name = L"tsmiFFMpegHelp";
			this->tsmiFFMpegHelp->Click += gcnew System::EventHandler(this, &FormMain::tsmiFFMpegHelp_Click);
			// 
			// notifyIconMain
			// 
			resources->ApplyResources(this->notifyIconMain, L"notifyIconMain");
			this->notifyIconMain->ContextMenuStrip = this->cmNotify;
			this->notifyIconMain->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &FormMain::notifyIconMain_MouseClick);
			// 
			// cmNotify
			// 
			resources->ApplyResources(this->cmNotify, L"cmNotify");
			this->cmNotify->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->tsmiNotifyShow, this->tsmiNotifyStart });
			this->cmNotify->Name = L"cmNotify";
			// 
			// tsmiNotifyShow
			// 
			resources->ApplyResources(this->tsmiNotifyShow, L"tsmiNotifyShow");
			this->tsmiNotifyShow->Name = L"tsmiNotifyShow";
			this->tsmiNotifyShow->Click += gcnew System::EventHandler(this, &FormMain::tsmiNotifyShow_Click);
			// 
			// tsmiNotifyStart
			// 
			resources->ApplyResources(this->tsmiNotifyStart, L"tsmiNotifyStart");
			this->tsmiNotifyStart->Name = L"tsmiNotifyStart";
			this->tsmiNotifyStart->Click += gcnew System::EventHandler(this, &FormMain::tsmiNotifyStart_Click);
			// 
			// statusMain
			// 
			resources->ApplyResources(this->statusMain, L"statusMain");
			this->statusMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->slMain, this->slFormat,
					this->slVideoCodec, this->slAudioCodec, this->slDuration
			});
			this->statusMain->Name = L"statusMain";
			// 
			// slMain
			// 
			resources->ApplyResources(this->slMain, L"slMain");
			this->slMain->Name = L"slMain";
			this->slMain->Spring = true;
			// 
			// slFormat
			// 
			resources->ApplyResources(this->slFormat, L"slFormat");
			this->slFormat->Name = L"slFormat";
			// 
			// slVideoCodec
			// 
			resources->ApplyResources(this->slVideoCodec, L"slVideoCodec");
			this->slVideoCodec->Name = L"slVideoCodec";
			// 
			// slAudioCodec
			// 
			resources->ApplyResources(this->slAudioCodec, L"slAudioCodec");
			this->slAudioCodec->Name = L"slAudioCodec";
			// 
			// slDuration
			// 
			resources->ApplyResources(this->slDuration, L"slDuration");
			this->slDuration->Name = L"slDuration";
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