#pragma once

namespace Ambiesoft {
	namespace toH265 {

		using namespace System;
		using namespace System::ComponentModel;
		using namespace System::Collections;
		using namespace System::Windows::Forms;
		using namespace System::Data;
		using namespace System::Drawing;

		/// <summary>
		/// Summary for TargetCodecDialog
		/// </summary>
		public ref class TargetCodecDialog : public System::Windows::Forms::Form
		{
		public:
			TargetCodecDialog(bool bLosslessable);

			property bool IsReEncode
			{
				bool get() {
					if (!cmbEncodeType->Enabled)
						return false;
					return cmbEncodeType->SelectedIndex == 1;
				}
			}
		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~TargetCodecDialog()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::GroupBox^ groupVideoCodec;
		protected:
		private: System::Windows::Forms::GroupBox^ groupAudioCodec;
		internal: System::Windows::Forms::RadioButton^  rbVp9;
		private:
		internal: System::Windows::Forms::RadioButton^  rbH265;
		internal: System::Windows::Forms::RadioButton^  rbOpus;
		internal: System::Windows::Forms::RadioButton^  rbAac;
		internal: System::Windows::Forms::RadioButton^  rbCopyAudio;
		private: System::Windows::Forms::Button^  btnOK;
		internal:
		private: System::Windows::Forms::Button^  btnCancel;
		internal: System::Windows::Forms::RadioButton^ rbCopyVideo;
		private: System::Windows::Forms::ComboBox^ cmbEncodeType;
		internal:
		private:









		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>
			System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
			/// <summary>
			/// Required method for Designer support - do not modify
			/// the contents of this method with the code editor.
			/// </summary>
			void InitializeComponent(void)
			{
				this->groupVideoCodec = (gcnew System::Windows::Forms::GroupBox());
				this->rbVp9 = (gcnew System::Windows::Forms::RadioButton());
				this->rbCopyVideo = (gcnew System::Windows::Forms::RadioButton());
				this->rbH265 = (gcnew System::Windows::Forms::RadioButton());
				this->groupAudioCodec = (gcnew System::Windows::Forms::GroupBox());
				this->rbOpus = (gcnew System::Windows::Forms::RadioButton());
				this->rbAac = (gcnew System::Windows::Forms::RadioButton());
				this->rbCopyAudio = (gcnew System::Windows::Forms::RadioButton());
				this->btnOK = (gcnew System::Windows::Forms::Button());
				this->btnCancel = (gcnew System::Windows::Forms::Button());
				this->cmbEncodeType = (gcnew System::Windows::Forms::ComboBox());
				this->groupVideoCodec->SuspendLayout();
				this->groupAudioCodec->SuspendLayout();
				this->SuspendLayout();
				// 
				// groupVideoCodec
				// 
				this->groupVideoCodec->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
					| System::Windows::Forms::AnchorStyles::Right));
				this->groupVideoCodec->Controls->Add(this->rbVp9);
				this->groupVideoCodec->Controls->Add(this->rbCopyVideo);
				this->groupVideoCodec->Controls->Add(this->rbH265);
				this->groupVideoCodec->Location = System::Drawing::Point(12, 59);
				this->groupVideoCodec->Name = L"groupVideoCodec";
				this->groupVideoCodec->Size = System::Drawing::Size(289, 100);
				this->groupVideoCodec->TabIndex = 1;
				this->groupVideoCodec->TabStop = false;
				this->groupVideoCodec->Text = L"Video Codec";
				// 
				// rbVp9
				// 
				this->rbVp9->AutoSize = true;
				this->rbVp9->Location = System::Drawing::Point(6, 65);
				this->rbVp9->Name = L"rbVp9";
				this->rbVp9->Size = System::Drawing::Size(43, 17);
				this->rbVp9->TabIndex = 2;
				this->rbVp9->TabStop = true;
				this->rbVp9->Text = L"&vp9";
				this->rbVp9->UseVisualStyleBackColor = true;
				// 
				// rbCopyVideo
				// 
				this->rbCopyVideo->AutoSize = true;
				this->rbCopyVideo->Checked = true;
				this->rbCopyVideo->Location = System::Drawing::Point(6, 19);
				this->rbCopyVideo->Name = L"rbCopyVideo";
				this->rbCopyVideo->Size = System::Drawing::Size(48, 17);
				this->rbCopyVideo->TabIndex = 0;
				this->rbCopyVideo->TabStop = true;
				this->rbCopyVideo->Text = L"&copy";
				this->rbCopyVideo->UseVisualStyleBackColor = true;
				// 
				// rbH265
				// 
				this->rbH265->AutoSize = true;
				this->rbH265->Location = System::Drawing::Point(6, 42);
				this->rbH265->Name = L"rbH265";
				this->rbH265->Size = System::Drawing::Size(82, 17);
				this->rbH265->TabIndex = 1;
				this->rbH265->TabStop = true;
				this->rbH265->Text = L"&hevc (h265)";
				this->rbH265->UseVisualStyleBackColor = true;
				// 
				// groupAudioCodec
				// 
				this->groupAudioCodec->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
					| System::Windows::Forms::AnchorStyles::Right));
				this->groupAudioCodec->Controls->Add(this->rbOpus);
				this->groupAudioCodec->Controls->Add(this->rbAac);
				this->groupAudioCodec->Controls->Add(this->rbCopyAudio);
				this->groupAudioCodec->Location = System::Drawing::Point(12, 165);
				this->groupAudioCodec->Name = L"groupAudioCodec";
				this->groupAudioCodec->Size = System::Drawing::Size(289, 100);
				this->groupAudioCodec->TabIndex = 1;
				this->groupAudioCodec->TabStop = false;
				this->groupAudioCodec->Text = L"Audio Codec";
				// 
				// rbOpus
				// 
				this->rbOpus->AutoSize = true;
				this->rbOpus->Location = System::Drawing::Point(6, 65);
				this->rbOpus->Name = L"rbOpus";
				this->rbOpus->Size = System::Drawing::Size(48, 17);
				this->rbOpus->TabIndex = 2;
				this->rbOpus->TabStop = true;
				this->rbOpus->Text = L"&opus";
				this->rbOpus->UseVisualStyleBackColor = true;
				// 
				// rbAac
				// 
				this->rbAac->AutoSize = true;
				this->rbAac->Location = System::Drawing::Point(6, 42);
				this->rbAac->Name = L"rbAac";
				this->rbAac->Size = System::Drawing::Size(43, 17);
				this->rbAac->TabIndex = 1;
				this->rbAac->TabStop = true;
				this->rbAac->Text = L"&aac";
				this->rbAac->UseVisualStyleBackColor = true;
				// 
				// rbCopyAudio
				// 
				this->rbCopyAudio->AutoSize = true;
				this->rbCopyAudio->Checked = true;
				this->rbCopyAudio->Location = System::Drawing::Point(6, 19);
				this->rbCopyAudio->Name = L"rbCopyAudio";
				this->rbCopyAudio->Size = System::Drawing::Size(48, 17);
				this->rbCopyAudio->TabIndex = 0;
				this->rbCopyAudio->TabStop = true;
				this->rbCopyAudio->Text = L"c&opy";
				this->rbCopyAudio->UseVisualStyleBackColor = true;
				// 
				// btnOK
				// 
				this->btnOK->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
				this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->btnOK->Location = System::Drawing::Point(145, 291);
				this->btnOK->Name = L"btnOK";
				this->btnOK->Size = System::Drawing::Size(75, 23);
				this->btnOK->TabIndex = 2;
				this->btnOK->Text = L"&OK";
				this->btnOK->UseVisualStyleBackColor = true;
				this->btnOK->Click += gcnew System::EventHandler(this, &TargetCodecDialog::BtnOK_Click);
				// 
				// btnCancel
				// 
				this->btnCancel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
				this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->btnCancel->Location = System::Drawing::Point(226, 291);
				this->btnCancel->Name = L"btnCancel";
				this->btnCancel->Size = System::Drawing::Size(75, 23);
				this->btnCancel->TabIndex = 3;
				this->btnCancel->Text = L"&Cancel";
				this->btnCancel->UseVisualStyleBackColor = true;
				// 
				// cmbEncodeType
				// 
				this->cmbEncodeType->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
					| System::Windows::Forms::AnchorStyles::Right));
				this->cmbEncodeType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
				this->cmbEncodeType->FormattingEnabled = true;
				this->cmbEncodeType->Location = System::Drawing::Point(18, 12);
				this->cmbEncodeType->Name = L"cmbEncodeType";
				this->cmbEncodeType->Size = System::Drawing::Size(283, 21);
				this->cmbEncodeType->TabIndex = 4;
				this->cmbEncodeType->SelectedIndexChanged += gcnew System::EventHandler(this, &TargetCodecDialog::CmbEncodeType_SelectedIndexChanged);
				// 
				// TargetCodecDialog
				// 
				this->AcceptButton = this->btnOK;
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->CancelButton = this->btnCancel;
				this->ClientSize = System::Drawing::Size(313, 321);
				this->Controls->Add(this->cmbEncodeType);
				this->Controls->Add(this->btnCancel);
				this->Controls->Add(this->btnOK);
				this->Controls->Add(this->groupAudioCodec);
				this->Controls->Add(this->groupVideoCodec);
				this->MaximizeBox = false;
				this->MinimizeBox = false;
				this->MinimumSize = System::Drawing::Size(194, 292);
				this->Name = L"TargetCodecDialog";
				this->ShowIcon = false;
				this->ShowInTaskbar = false;
				this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
				this->Text = L"TargetCodecDialog";
				this->groupVideoCodec->ResumeLayout(false);
				this->groupVideoCodec->PerformLayout();
				this->groupAudioCodec->ResumeLayout(false);
				this->groupAudioCodec->PerformLayout();
				this->ResumeLayout(false);

			}
#pragma endregion
		private: 
			System::Void CmbEncodeType_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
			System::Void BtnOK_Click(System::Object^ sender, System::EventArgs^ e);
		
};
	}
}