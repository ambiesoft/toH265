#include "stdafx.h"

#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		void FormMain::InitializeComponent(void)
		{
			this->txtMovie = (gcnew System::Windows::Forms::TextBox());
			this->btnBrowseMovie = (gcnew System::Windows::Forms::Button());
			this->txtLogErr = (gcnew System::Windows::Forms::TextBox());
			this->txtLogOut = (gcnew System::Windows::Forms::TextBox());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->txtFFMpegArg = (gcnew System::Windows::Forms::TextBox());
			this->menuMain = (gcnew System::Windows::Forms::MenuStrip());
			this->tsmiFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiOption = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiSetFFProbe = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tsmiSetFFMpeg = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panelMain = (gcnew System::Windows::Forms::Panel());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuMain->SuspendLayout();
			this->panelMain->SuspendLayout();
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
			this->tsmiFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->exitToolStripMenuItem });
			this->tsmiFile->Name = L"tsmiFile";
			this->tsmiFile->Size = System::Drawing::Size(37, 20);
			this->tsmiFile->Text = L"&File";
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(92, 22);
			this->exitToolStripMenuItem->Text = L"&Exit";
			// 
			// tsmiOption
			// 
			this->tsmiOption->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->tsmiSetFFProbe,
					this->tsmiSetFFMpeg
			});
			this->tsmiOption->Name = L"tsmiOption";
			this->tsmiOption->Size = System::Drawing::Size(56, 20);
			this->tsmiOption->Text = L"&Option";
			this->tsmiOption->DropDownOpening += gcnew System::EventHandler(this, &FormMain::tsmiOption_DropDownOpening);
			// 
			// tsmiSetFFProbe
			// 
			this->tsmiSetFFProbe->Name = L"tsmiSetFFProbe";
			this->tsmiSetFFProbe->Size = System::Drawing::Size(145, 22);
			this->tsmiSetFFProbe->Text = L"&Set FFProbe...";
			this->tsmiSetFFProbe->Click += gcnew System::EventHandler(this, &FormMain::tsmiSetFFProbe_Click);
			// 
			// tsmiSetFFMpeg
			// 
			this->tsmiSetFFMpeg->Name = L"tsmiSetFFMpeg";
			this->tsmiSetFFMpeg->Size = System::Drawing::Size(145, 22);
			this->tsmiSetFFMpeg->Text = L"Set FF&Mpeg...";
			this->tsmiSetFFMpeg->Click += gcnew System::EventHandler(this, &FormMain::tsmiSetFFMpeg_Click);
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
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->aboutToolStripMenuItem });
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"&Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->aboutToolStripMenuItem->Text = L"&About...";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::aboutToolStripMenuItem_Click);
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(585, 297);
			this->Controls->Add(this->menuMain);
			this->Controls->Add(this->panelMain);
			this->MainMenuStrip = this->menuMain;
			this->Name = L"FormMain";
			this->Text = L"FormMain";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
			this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
			this->menuMain->ResumeLayout(false);
			this->menuMain->PerformLayout();
			this->panelMain->ResumeLayout(false);
			this->panelMain->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	}

}