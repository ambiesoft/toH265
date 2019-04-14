#include "stdafx.h"

#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		void FormMain::InitializeComponent(void)
		{
			this->lblMovie = (gcnew System::Windows::Forms::Label());
			this->txtMovie = (gcnew System::Windows::Forms::TextBox());
			this->btnBrowseMovie = (gcnew System::Windows::Forms::Button());
			this->txtLogErr = (gcnew System::Windows::Forms::TextBox());
			this->txtLogOut = (gcnew System::Windows::Forms::TextBox());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->btnStop = (gcnew System::Windows::Forms::Button());
			this->txtFFMpegArg = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// lblMovie
			// 
			this->lblMovie->AutoSize = true;
			this->lblMovie->Location = System::Drawing::Point(12, 9);
			this->lblMovie->Name = L"lblMovie";
			this->lblMovie->Size = System::Drawing::Size(39, 13);
			this->lblMovie->TabIndex = 0;
			this->lblMovie->Text = L"&Movie:";
			// 
			// txtMovie
			// 
			this->txtMovie->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtMovie->Location = System::Drawing::Point(15, 25);
			this->txtMovie->Name = L"txtMovie";
			this->txtMovie->Size = System::Drawing::Size(528, 20);
			this->txtMovie->TabIndex = 1;
			// 
			// btnBrowseMovie
			// 
			this->btnBrowseMovie->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnBrowseMovie->Location = System::Drawing::Point(549, 23);
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
			this->txtLogErr->Location = System::Drawing::Point(15, 77);
			this->txtLogErr->Multiline = true;
			this->txtLogErr->Name = L"txtLogErr";
			this->txtLogErr->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->txtLogErr->Size = System::Drawing::Size(528, 178);
			this->txtLogErr->TabIndex = 3;
			// 
			// txtLogOut
			// 
			this->txtLogOut->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtLogOut->Location = System::Drawing::Point(15, 261);
			this->txtLogOut->Name = L"txtLogOut";
			this->txtLogOut->Size = System::Drawing::Size(528, 20);
			this->txtLogOut->TabIndex = 4;
			// 
			// btnStart
			// 
			this->btnStart->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnStart->Location = System::Drawing::Point(15, 288);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(451, 23);
			this->btnStart->TabIndex = 5;
			this->btnStart->Text = L"&Start";
			this->btnStart->UseVisualStyleBackColor = true;
			this->btnStart->Click += gcnew System::EventHandler(this, &FormMain::btnStart_Click);
			// 
			// btnStop
			// 
			this->btnStop->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->btnStop->Location = System::Drawing::Point(468, 288);
			this->btnStop->Name = L"btnStop";
			this->btnStop->Size = System::Drawing::Size(75, 23);
			this->btnStop->TabIndex = 6;
			this->btnStop->Text = L"S&top";
			this->btnStop->UseVisualStyleBackColor = true;
			this->btnStop->Click += gcnew System::EventHandler(this, &FormMain::btnStop_Click);
			// 
			// txtFFMpegArg
			// 
			this->txtFFMpegArg->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtFFMpegArg->Location = System::Drawing::Point(15, 51);
			this->txtFFMpegArg->Name = L"txtFFMpegArg";
			this->txtFFMpegArg->Size = System::Drawing::Size(528, 20);
			this->txtFFMpegArg->TabIndex = 7;
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(585, 323);
			this->Controls->Add(this->txtFFMpegArg);
			this->Controls->Add(this->btnStop);
			this->Controls->Add(this->btnStart);
			this->Controls->Add(this->txtLogOut);
			this->Controls->Add(this->txtLogErr);
			this->Controls->Add(this->btnBrowseMovie);
			this->Controls->Add(this->txtMovie);
			this->Controls->Add(this->lblMovie);
			this->Name = L"FormMain";
			this->Text = L"FormMain";
			this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	}

}