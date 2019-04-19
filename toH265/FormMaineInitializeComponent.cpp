#include "stdafx.h"

#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		void FormMain::InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMain::typeid));
			this->txtMovie = (gcnew System::Windows::Forms::TextBox());
			this->btnBrowseMovie = (gcnew System::Windows::Forms::Button());
			this->txtLogErr = (gcnew System::Windows::Forms::TextBox());
			this->txtLogOut = (gcnew System::Windows::Forms::TextBox());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->txtFFMpegArg = (gcnew System::Windows::Forms::TextBox());
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
			this->tsmiHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiAbout = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiFFMpegHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panelMain = (gcnew System::Windows::Forms::Panel());
			this->notifyIconMain = (gcnew System::Windows::Forms::NotifyIcon(this->components));
			this->cmNotify = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->tsmiNotifyShow = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiNotifyStart = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusMain = (gcnew System::Windows::Forms::StatusStrip());
			this->slMain = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->slVideoCodec = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->slDuration = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->slAudioCodec = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->menuMain->SuspendLayout();
			this->panelMain->SuspendLayout();
			this->cmNotify->SuspendLayout();
			this->statusMain->SuspendLayout();
			this->SuspendLayout();
			// 
			// txtMovie
			// 
			this->txtMovie->AllowDrop = true;
			resources->ApplyResources(this->txtMovie, L"txtMovie");
			this->txtMovie->Name = L"txtMovie";
			this->txtMovie->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::txtMovie_DragDrop);
			this->txtMovie->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::txtMovie_DragEnter);
			this->txtMovie->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::txtMovie_DragOver);
			this->txtMovie->DragLeave += gcnew System::EventHandler(this, &FormMain::txtMovie_DragLeave);
			// 
			// btnBrowseMovie
			// 
			resources->ApplyResources(this->btnBrowseMovie, L"btnBrowseMovie");
			this->btnBrowseMovie->Name = L"btnBrowseMovie";
			this->btnBrowseMovie->UseVisualStyleBackColor = true;
			this->btnBrowseMovie->Click += gcnew System::EventHandler(this, &FormMain::btnBrowseMovie_Click);
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
			// btnStart
			// 
			resources->ApplyResources(this->btnStart, L"btnStart");
			this->btnStart->Name = L"btnStart";
			this->btnStart->UseVisualStyleBackColor = true;
			this->btnStart->Click += gcnew System::EventHandler(this, &FormMain::btnStart_Click);
			// 
			// txtFFMpegArg
			// 
			resources->ApplyResources(this->txtFFMpegArg, L"txtFFMpegArg");
			this->txtFFMpegArg->Name = L"txtFFMpegArg";
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
			this->tsmiFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->tsmiStop, this->exitToolStripMenuItem });
			this->tsmiFile->Name = L"tsmiFile";
			resources->ApplyResources(this->tsmiFile, L"tsmiFile");
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
			this->tsmiOption->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->tsmiSetFFProbe,
					this->tsmiSetFFMpeg, this->toolStripMenuItem1, this->tsmiPriority, this->toolStripMenuItem2, this->tsmiMinimizeToTray
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
			// panelMain
			// 
			resources->ApplyResources(this->panelMain, L"panelMain");
			this->panelMain->Controls->Add(this->txtMovie);
			this->panelMain->Controls->Add(this->btnBrowseMovie);
			this->panelMain->Controls->Add(this->btnStart);
			this->panelMain->Controls->Add(this->txtFFMpegArg);
			this->panelMain->Controls->Add(this->txtLogOut);
			this->panelMain->Controls->Add(this->txtLogErr);
			this->panelMain->Name = L"panelMain";
			// 
			// notifyIconMain
			// 
			this->notifyIconMain->ContextMenuStrip = this->cmNotify;
			resources->ApplyResources(this->notifyIconMain, L"notifyIconMain");
			this->notifyIconMain->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &FormMain::notifyIconMain_MouseClick);
			// 
			// cmNotify
			// 
			this->cmNotify->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->tsmiNotifyShow, this->tsmiNotifyStart });
			this->cmNotify->Name = L"cmNotify";
			resources->ApplyResources(this->cmNotify, L"cmNotify");
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
			this->statusMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->slMain, this->slVideoCodec,
					this->slAudioCodec, this->slDuration
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
			// slVideoCodec
			// 
			this->slVideoCodec->Name = L"slVideoCodec";
			resources->ApplyResources(this->slVideoCodec, L"slVideoCodec");
			// 
			// slDuration
			// 
			this->slDuration->Name = L"slDuration";
			resources->ApplyResources(this->slDuration, L"slDuration");
			// 
			// slAudioCodec
			// 
			this->slAudioCodec->Name = L"slAudioCodec";
			resources->ApplyResources(this->slAudioCodec, L"slAudioCodec");
			// 
			// FormMain
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->menuMain);
			this->Controls->Add(this->statusMain);
			this->Controls->Add(this->panelMain);
			this->MainMenuStrip = this->menuMain;
			this->Name = L"FormMain";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormMain::FormMain_FormClosed);
			this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
			this->Resize += gcnew System::EventHandler(this, &FormMain::FormMain_Resize);
			this->menuMain->ResumeLayout(false);
			this->menuMain->PerformLayout();
			this->panelMain->ResumeLayout(false);
			this->panelMain->PerformLayout();
			this->cmNotify->ResumeLayout(false);
			this->statusMain->ResumeLayout(false);
			this->statusMain->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	}

}