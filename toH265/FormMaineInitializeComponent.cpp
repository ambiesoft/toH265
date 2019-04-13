#include "stdafx.h"

#include "FormMain.h"


namespace Ambiesoft {
	namespace toH265 {

		void FormMain::InitializeComponent(void)
		{
			this->lblMovie = (gcnew System::Windows::Forms::Label());
			this->txtMovie = (gcnew System::Windows::Forms::TextBox());
			this->btnBrowseMovie = (gcnew System::Windows::Forms::Button());
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
			this->txtMovie->Location = System::Drawing::Point(15, 25);
			this->txtMovie->Name = L"txtMovie";
			this->txtMovie->Size = System::Drawing::Size(556, 20);
			this->txtMovie->TabIndex = 1;
			// 
			// btnBrowseMovie
			// 
			this->btnBrowseMovie->Location = System::Drawing::Point(577, 23);
			this->btnBrowseMovie->Name = L"btnBrowseMovie";
			this->btnBrowseMovie->Size = System::Drawing::Size(28, 23);
			this->btnBrowseMovie->TabIndex = 2;
			this->btnBrowseMovie->Text = L"&...";
			this->btnBrowseMovie->UseVisualStyleBackColor = true;
			this->btnBrowseMovie->Click += gcnew System::EventHandler(this, &FormMain::btnBrowseMovie_Click);
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(613, 367);
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