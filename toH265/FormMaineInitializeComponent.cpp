#include "stdafx.h"

#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		void FormMain::InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMain::typeid));
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
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiAbout = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiFFMpegHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panelMain = (gcnew System::Windows::Forms::Panel());
			this->notifyIconMain = (gcnew System::Windows::Forms::NotifyIcon(this->components));
			this->cmNotify = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->tsmiNotifyShow = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiNotifyStart = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuMain->SuspendLayout();
			this->panelMain->SuspendLayout();
			this->cmNotify->SuspendLayout();
			this->SuspendLayout();
			// 
			// txtMovie
			// 
			this->txtMovie->AllowDrop = true;
			this->txtMovie->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtMovie->Location = System::Drawing::Point(0, 0);
			this->txtMovie->Name = L"txtMovie";
			this->txtMovie->Size = System::Drawing::Size(527, 20);
			this->txtMovie->TabIndex = 1;
			this->txtMovie->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::txtMovie_DragDrop);
			this->txtMovie->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::txtMovie_DragEnter);
			this->txtMovie->DragOver += gcnew System::Windows::Forms::DragEventHandler(this, &FormMain::txtMovie_DragOver);
			this->txtMovie->DragLeave += gcnew System::EventHandler(this, &FormMain::txtMovie_DragLeave);
			// 
			// btnBrowseMovie
			// 
			this->btnBrowseMovie->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnBrowseMovie->Location = System::Drawing::Point(533, 0);
			this->btnBrowseMovie->Name = L"btnBrowseMovie";
			this->btnBrowseMovie->Size = System::Drawing::Size(28, 23);
			this->btnBrowseMovie->TabIndex = 2;
			this->btnBrowseMovie->Text = L"&...";
			this->btnBrowseMovie->UseVisualStyleBackColor = true;
			this->btnBrowseMovie->Click += gcnew System::EventHandler(this, &FormMain::btnBrowseMovie_Click);
			// 
			// txtLogErr
			// 
			this->txtLogErr->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtLogErr->Location = System::Drawing::Point(0, 55);
			this->txtLogErr->Multiline = true;
			this->txtLogErr->Name = L"txtLogErr";
			this->txtLogErr->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->txtLogErr->Size = System::Drawing::Size(561, 148);
			this->txtLogErr->TabIndex = 3;
			// 
			// txtLogOut
			// 
			this->txtLogOut->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtLogOut->Location = System::Drawing::Point(0, 209);
			this->txtLogOut->Name = L"txtLogOut";
			this->txtLogOut->Size = System::Drawing::Size(561, 20);
			this->txtLogOut->TabIndex = 4;
			// 
			// btnStart
			// 
			this->btnStart->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnStart->Location = System::Drawing::Point(0, 235);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(561, 23);
			this->btnStart->TabIndex = 5;
			this->btnStart->Text = L"&Start";
			this->btnStart->UseVisualStyleBackColor = true;
			this->btnStart->Click += gcnew System::EventHandler(this, &FormMain::btnStart_Click);
			// 
			// txtFFMpegArg
			// 
			this->txtFFMpegArg->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtFFMpegArg->Location = System::Drawing::Point(0, 29);
			this->txtFFMpegArg->Name = L"txtFFMpegArg";
			this->txtFFMpegArg->Size = System::Drawing::Size(561, 20);
			this->txtFFMpegArg->TabIndex = 7;
			// 
			// menuMain
			// 
			this->menuMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->tsmiFile, this->tsmiOption,
					this->helpToolStripMenuItem
			});
			this->menuMain->Location = System::Drawing::Point(0, 0);
			this->menuMain->Name = L"menuMain";
			this->menuMain->Size = System::Drawing::Size(585, 24);
			this->menuMain->TabIndex = 0;
			// 
			// tsmiFile
			// 
			this->tsmiFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->tsmiStop, this->exitToolStripMenuItem });
			this->tsmiFile->Name = L"tsmiFile";
			this->tsmiFile->Size = System::Drawing::Size(37, 20);
			this->tsmiFile->Text = L"&File";
			// 
			// tsmiStop
			// 
			this->tsmiStop->Name = L"tsmiStop";
			this->tsmiStop->Size = System::Drawing::Size(98, 22);
			this->tsmiStop->Text = L"&Stop";
			this->tsmiStop->Click += gcnew System::EventHandler(this, &FormMain::tsmiStop_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(98, 22);
			this->exitToolStripMenuItem->Text = L"&Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::exitToolStripMenuItem_Click);
			// 
			// tsmiOption
			// 
			this->tsmiOption->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->tsmiSetFFProbe,
					this->tsmiSetFFMpeg, this->toolStripMenuItem1, this->tsmiPriority, this->toolStripMenuItem2, this->tsmiMinimizeToTray
			});
			this->tsmiOption->Name = L"tsmiOption";
			this->tsmiOption->Size = System::Drawing::Size(56, 20);
			this->tsmiOption->Text = L"&Option";
			this->tsmiOption->DropDownOpening += gcnew System::EventHandler(this, &FormMain::tsmiOption_DropDownOpening);
			// 
			// tsmiSetFFProbe
			// 
			this->tsmiSetFFProbe->Name = L"tsmiSetFFProbe";
			this->tsmiSetFFProbe->Size = System::Drawing::Size(162, 22);
			this->tsmiSetFFProbe->Text = L"&Set FFProbe...";
			this->tsmiSetFFProbe->Click += gcnew System::EventHandler(this, &FormMain::tsmiSetFFProbe_Click);
			// 
			// tsmiSetFFMpeg
			// 
			this->tsmiSetFFMpeg->Name = L"tsmiSetFFMpeg";
			this->tsmiSetFFMpeg->Size = System::Drawing::Size(162, 22);
			this->tsmiSetFFMpeg->Text = L"Set FF&Mpeg...";
			this->tsmiSetFFMpeg->Click += gcnew System::EventHandler(this, &FormMain::tsmiSetFFMpeg_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(159, 6);
			// 
			// tsmiPriority
			// 
			this->tsmiPriority->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->tsmiPriorityNormal,
					this->tsmiPriorityBackground
			});
			this->tsmiPriority->Name = L"tsmiPriority";
			this->tsmiPriority->Size = System::Drawing::Size(162, 22);
			this->tsmiPriority->Text = L"&Priority";
			// 
			// tsmiPriorityNormal
			// 
			this->tsmiPriorityNormal->Checked = true;
			this->tsmiPriorityNormal->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tsmiPriorityNormal->Name = L"tsmiPriorityNormal";
			this->tsmiPriorityNormal->Size = System::Drawing::Size(138, 22);
			this->tsmiPriorityNormal->Text = L"&Normal";
			this->tsmiPriorityNormal->Click += gcnew System::EventHandler(this, &FormMain::tsmiPriorityNormal_Click);
			// 
			// tsmiPriorityBackground
			// 
			this->tsmiPriorityBackground->Name = L"tsmiPriorityBackground";
			this->tsmiPriorityBackground->Size = System::Drawing::Size(138, 22);
			this->tsmiPriorityBackground->Text = L"&Background";
			this->tsmiPriorityBackground->Click += gcnew System::EventHandler(this, &FormMain::tsmiPriorityBackground_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(159, 6);
			// 
			// tsmiMinimizeToTray
			// 
			this->tsmiMinimizeToTray->Name = L"tsmiMinimizeToTray";
			this->tsmiMinimizeToTray->Size = System::Drawing::Size(162, 22);
			this->tsmiMinimizeToTray->Text = L"&Minimize to Tray";
			this->tsmiMinimizeToTray->Click += gcnew System::EventHandler(this, &FormMain::tsmiMinimizeToTray_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->tsmiAbout,
					this->tsmiFFMpegHelp
			});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"&Help";
			// 
			// tsmiAbout
			// 
			this->tsmiAbout->Name = L"tsmiAbout";
			this->tsmiAbout->Size = System::Drawing::Size(162, 22);
			this->tsmiAbout->Text = L"&About...";
			this->tsmiAbout->Click += gcnew System::EventHandler(this, &FormMain::tsmiAbout_Click);
			// 
			// tsmiFFMpegHelp
			// 
			this->tsmiFFMpegHelp->Name = L"tsmiFFMpegHelp";
			this->tsmiFFMpegHelp->Size = System::Drawing::Size(162, 22);
			this->tsmiFFMpegHelp->Text = L"FFMpeg\'s Help...";
			this->tsmiFFMpegHelp->Click += gcnew System::EventHandler(this, &FormMain::tsmiFFMpegHelp_Click);
			// 
			// panelMain
			// 
			this->panelMain->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->panelMain->Controls->Add(this->txtMovie);
			this->panelMain->Controls->Add(this->btnBrowseMovie);
			this->panelMain->Controls->Add(this->btnStart);
			this->panelMain->Controls->Add(this->txtFFMpegArg);
			this->panelMain->Controls->Add(this->txtLogOut);
			this->panelMain->Controls->Add(this->txtLogErr);
			this->panelMain->Location = System::Drawing::Point(12, 27);
			this->panelMain->Name = L"panelMain";
			this->panelMain->Size = System::Drawing::Size(561, 271);
			this->panelMain->TabIndex = 100;
			// 
			// notifyIconMain
			// 
			this->notifyIconMain->ContextMenuStrip = this->cmNotify;
			this->notifyIconMain->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"notifyIconMain.Icon")));
			this->notifyIconMain->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &FormMain::notifyIconMain_MouseClick);
			// 
			// cmNotify
			// 
			this->cmNotify->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->tsmiNotifyShow, this->tsmiNotifyStart });
			this->cmNotify->Name = L"cmNotify";
			this->cmNotify->Size = System::Drawing::Size(142, 48);
			// 
			// tsmiNotifyShow
			// 
			this->tsmiNotifyShow->Name = L"tsmiNotifyShow";
			this->tsmiNotifyShow->Size = System::Drawing::Size(141, 22);
			this->tsmiNotifyShow->Text = L"&Show";
			this->tsmiNotifyShow->Click += gcnew System::EventHandler(this, &FormMain::tsmiNotifyShow_Click);
			// 
			// tsmiNotifyStart
			// 
			this->tsmiNotifyStart->Name = L"tsmiNotifyStart";
			this->tsmiNotifyStart->Size = System::Drawing::Size(141, 22);
			this->tsmiNotifyStart->Text = L"StartDummy";
			this->tsmiNotifyStart->Click += gcnew System::EventHandler(this, &FormMain::tsmiNotifyStart_Click);
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(585, 297);
			this->Controls->Add(this->menuMain);
			this->Controls->Add(this->panelMain);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuMain;
			this->Name = L"FormMain";
			this->Text = L"FormMain";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
			this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
			this->Resize += gcnew System::EventHandler(this, &FormMain::FormMain_Resize);
			this->menuMain->ResumeLayout(false);
			this->menuMain->PerformLayout();
			this->panelMain->ResumeLayout(false);
			this->panelMain->PerformLayout();
			this->cmNotify->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	}

}