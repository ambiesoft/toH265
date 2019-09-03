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
					//if (!cmbEncodeType->Enabled)
					//	return false;
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
		internal: System::Windows::Forms::RadioButton^ rbVideoVp9;
		private:
		internal: System::Windows::Forms::RadioButton^ rbVideoH265;
		internal: System::Windows::Forms::RadioButton^ rbAudioOpus;
		internal: System::Windows::Forms::RadioButton^ rbAudioAac;
		internal: System::Windows::Forms::RadioButton^ rbAudioCopy;
		private: System::Windows::Forms::Button^ btnOK;
		internal:
		private: System::Windows::Forms::Button^ btnCancel;
		internal: System::Windows::Forms::RadioButton^ rbVideoCopy;
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
				this->rbVideoVp9 = (gcnew System::Windows::Forms::RadioButton());
				this->rbVideoCopy = (gcnew System::Windows::Forms::RadioButton());
				this->rbVideoH265 = (gcnew System::Windows::Forms::RadioButton());
				this->groupAudioCodec = (gcnew System::Windows::Forms::GroupBox());
				this->rbAudioOpus = (gcnew System::Windows::Forms::RadioButton());
				this->rbAudioAac = (gcnew System::Windows::Forms::RadioButton());
				this->rbAudioCopy = (gcnew System::Windows::Forms::RadioButton());
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
				this->groupVideoCodec->Controls->Add(this->rbVideoVp9);
				this->groupVideoCodec->Controls->Add(this->rbVideoCopy);
				this->groupVideoCodec->Controls->Add(this->rbVideoH265);
				this->groupVideoCodec->Location = System::Drawing::Point(12, 59);
				this->groupVideoCodec->Name = L"groupVideoCodec";
				this->groupVideoCodec->Size = System::Drawing::Size(289, 100);
				this->groupVideoCodec->TabIndex = 1;
				this->groupVideoCodec->TabStop = false;
				this->groupVideoCodec->Text = L"Video Codec";
				// 
				// rbVideoVp9
				// 
				this->rbVideoVp9->AutoSize = true;
				this->rbVideoVp9->Location = System::Drawing::Point(6, 65);
				this->rbVideoVp9->Name = L"rbVideoVp9";
				this->rbVideoVp9->Size = System::Drawing::Size(43, 17);
				this->rbVideoVp9->TabIndex = 2;
				this->rbVideoVp9->TabStop = true;
				this->rbVideoVp9->Text = L"&vp9";
				this->rbVideoVp9->UseVisualStyleBackColor = true;
				// 
				// rbVideoCopy
				// 
				this->rbVideoCopy->AutoSize = true;
				this->rbVideoCopy->Checked = true;
				this->rbVideoCopy->Location = System::Drawing::Point(6, 19);
				this->rbVideoCopy->Name = L"rbVideoCopy";
				this->rbVideoCopy->Size = System::Drawing::Size(48, 17);
				this->rbVideoCopy->TabIndex = 0;
				this->rbVideoCopy->TabStop = true;
				this->rbVideoCopy->Text = L"&copy";
				this->rbVideoCopy->UseVisualStyleBackColor = true;
				// 
				// rbVideoH265
				// 
				this->rbVideoH265->AutoSize = true;
				this->rbVideoH265->Location = System::Drawing::Point(6, 42);
				this->rbVideoH265->Name = L"rbVideoH265";
				this->rbVideoH265->Size = System::Drawing::Size(82, 17);
				this->rbVideoH265->TabIndex = 1;
				this->rbVideoH265->TabStop = true;
				this->rbVideoH265->Text = L"&hevc (h265)";
				this->rbVideoH265->UseVisualStyleBackColor = true;
				// 
				// groupAudioCodec
				// 
				this->groupAudioCodec->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
					| System::Windows::Forms::AnchorStyles::Right));
				this->groupAudioCodec->Controls->Add(this->rbAudioOpus);
				this->groupAudioCodec->Controls->Add(this->rbAudioAac);
				this->groupAudioCodec->Controls->Add(this->rbAudioCopy);
				this->groupAudioCodec->Location = System::Drawing::Point(12, 165);
				this->groupAudioCodec->Name = L"groupAudioCodec";
				this->groupAudioCodec->Size = System::Drawing::Size(289, 100);
				this->groupAudioCodec->TabIndex = 1;
				this->groupAudioCodec->TabStop = false;
				this->groupAudioCodec->Text = L"Audio Codec";
				// 
				// rbAudioOpus
				// 
				this->rbAudioOpus->AutoSize = true;
				this->rbAudioOpus->Location = System::Drawing::Point(6, 65);
				this->rbAudioOpus->Name = L"rbAudioOpus";
				this->rbAudioOpus->Size = System::Drawing::Size(48, 17);
				this->rbAudioOpus->TabIndex = 2;
				this->rbAudioOpus->TabStop = true;
				this->rbAudioOpus->Text = L"&opus";
				this->rbAudioOpus->UseVisualStyleBackColor = true;
				// 
				// rbAudioAac
				// 
				this->rbAudioAac->AutoSize = true;
				this->rbAudioAac->Location = System::Drawing::Point(6, 42);
				this->rbAudioAac->Name = L"rbAudioAac";
				this->rbAudioAac->Size = System::Drawing::Size(43, 17);
				this->rbAudioAac->TabIndex = 1;
				this->rbAudioAac->TabStop = true;
				this->rbAudioAac->Text = L"&aac";
				this->rbAudioAac->UseVisualStyleBackColor = true;
				// 
				// rbAudioCopy
				// 
				this->rbAudioCopy->AutoSize = true;
				this->rbAudioCopy->Checked = true;
				this->rbAudioCopy->Location = System::Drawing::Point(6, 19);
				this->rbAudioCopy->Name = L"rbAudioCopy";
				this->rbAudioCopy->Size = System::Drawing::Size(48, 17);
				this->rbAudioCopy->TabIndex = 0;
				this->rbAudioCopy->TabStop = true;
				this->rbAudioCopy->Text = L"c&opy";
				this->rbAudioCopy->UseVisualStyleBackColor = true;
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
				this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &TargetCodecDialog::TargetCodecDialog_FormClosing);
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
			System::Void TargetCodecDialog_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);

		};
	}
}