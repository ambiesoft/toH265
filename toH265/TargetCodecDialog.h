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
			literal String^ KEY_ENCODE_TYPE = L"EncodeType";
			literal String^ KEY_AUIDOCODEC = L"AudioCodec";
			literal String^ KEY_VIDEOCODEC = L"VideoCodec";

			initonly String^ iniPath_;
			initonly String^ section_;
			initonly bool losslessable_;
		public:
			TargetCodecDialog(bool bLosslessable, String^ iniPath, String^ section);

			property bool IsReEncode
			{
				bool get() {
					//if (!cmbEncodeType->Enabled)
					//	return false;
					return cmbEncodeType->SelectedIndex == 1;
				}
			}
			property bool CanSerialize
			{
				bool get()
				{
					return (!String::IsNullOrEmpty(iniPath_) && !String::IsNullOrEmpty(section_));
				}
			}
			property String^ IniPath
			{
				String^ get()
				{
					return iniPath_;
				}
			}
			property String^ SECTION
			{
				String^ get()
				{
					return section_;
				}
			}
			property int AudioCodecInt
			{
				int get()
				{
					if (rbAudioCopy->Checked)
						return 0;
					else if (rbAudioAac->Checked)
						return 1;
					else if (rbAudioOpus->Checked)
						return 2;
					DASSERT(false);
					return 0;
				}
				void set(int value)
				{
					if (value == 0)
						rbAudioCopy->Checked = true;
					else if (value == 1)
						rbAudioAac->Checked = true;
					else if (value == 2)
						rbAudioOpus->Checked = true;
					else
						DASSERT(false);
				}
			}
			property int VideoCodecInt
			{
				int get()
				{
					if (rbVideoCopy->Checked)
						return 0;
					else if (rbVideoH265->Checked)
						return 1;
					else if (rbVideoVp9->Checked)
						return 2;
					else if (rbVideoAV1->Checked)
						return 3;
					DASSERT(false);
					return 0;
				}
				void set(int value)
				{
					if (value == 0)
						rbVideoCopy->Checked = true;
					else if (value == 1)
						rbVideoH265->Checked = true;
					else if (value == 2)
						rbVideoVp9->Checked = true;
					else if (value == 3)
						rbVideoAV1->Checked = true;
					else
						DASSERT(false);
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
internal: System::Windows::Forms::RadioButton^ rbVideoAV1;
private:

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
				System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(TargetCodecDialog::typeid));
				this->groupVideoCodec = (gcnew System::Windows::Forms::GroupBox());
				this->rbVideoAV1 = (gcnew System::Windows::Forms::RadioButton());
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
				resources->ApplyResources(this->groupVideoCodec, L"groupVideoCodec");
				this->groupVideoCodec->Controls->Add(this->rbVideoAV1);
				this->groupVideoCodec->Controls->Add(this->rbVideoVp9);
				this->groupVideoCodec->Controls->Add(this->rbVideoCopy);
				this->groupVideoCodec->Controls->Add(this->rbVideoH265);
				this->groupVideoCodec->Name = L"groupVideoCodec";
				this->groupVideoCodec->TabStop = false;
				// 
				// rbVideoAV1
				// 
				resources->ApplyResources(this->rbVideoAV1, L"rbVideoAV1");
				this->rbVideoAV1->Name = L"rbVideoAV1";
				this->rbVideoAV1->TabStop = true;
				this->rbVideoAV1->UseVisualStyleBackColor = true;
				// 
				// rbVideoVp9
				// 
				resources->ApplyResources(this->rbVideoVp9, L"rbVideoVp9");
				this->rbVideoVp9->Name = L"rbVideoVp9";
				this->rbVideoVp9->TabStop = true;
				this->rbVideoVp9->UseVisualStyleBackColor = true;
				// 
				// rbVideoCopy
				// 
				resources->ApplyResources(this->rbVideoCopy, L"rbVideoCopy");
				this->rbVideoCopy->Checked = true;
				this->rbVideoCopy->Name = L"rbVideoCopy";
				this->rbVideoCopy->TabStop = true;
				this->rbVideoCopy->UseVisualStyleBackColor = true;
				// 
				// rbVideoH265
				// 
				resources->ApplyResources(this->rbVideoH265, L"rbVideoH265");
				this->rbVideoH265->Name = L"rbVideoH265";
				this->rbVideoH265->TabStop = true;
				this->rbVideoH265->UseVisualStyleBackColor = true;
				// 
				// groupAudioCodec
				// 
				resources->ApplyResources(this->groupAudioCodec, L"groupAudioCodec");
				this->groupAudioCodec->Controls->Add(this->rbAudioOpus);
				this->groupAudioCodec->Controls->Add(this->rbAudioAac);
				this->groupAudioCodec->Controls->Add(this->rbAudioCopy);
				this->groupAudioCodec->Name = L"groupAudioCodec";
				this->groupAudioCodec->TabStop = false;
				// 
				// rbAudioOpus
				// 
				resources->ApplyResources(this->rbAudioOpus, L"rbAudioOpus");
				this->rbAudioOpus->Name = L"rbAudioOpus";
				this->rbAudioOpus->TabStop = true;
				this->rbAudioOpus->UseVisualStyleBackColor = true;
				// 
				// rbAudioAac
				// 
				resources->ApplyResources(this->rbAudioAac, L"rbAudioAac");
				this->rbAudioAac->Name = L"rbAudioAac";
				this->rbAudioAac->TabStop = true;
				this->rbAudioAac->UseVisualStyleBackColor = true;
				// 
				// rbAudioCopy
				// 
				resources->ApplyResources(this->rbAudioCopy, L"rbAudioCopy");
				this->rbAudioCopy->Checked = true;
				this->rbAudioCopy->Name = L"rbAudioCopy";
				this->rbAudioCopy->TabStop = true;
				this->rbAudioCopy->UseVisualStyleBackColor = true;
				// 
				// btnOK
				// 
				resources->ApplyResources(this->btnOK, L"btnOK");
				this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->btnOK->Name = L"btnOK";
				this->btnOK->UseVisualStyleBackColor = true;
				this->btnOK->Click += gcnew System::EventHandler(this, &TargetCodecDialog::BtnOK_Click);
				// 
				// btnCancel
				// 
				resources->ApplyResources(this->btnCancel, L"btnCancel");
				this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				this->btnCancel->Name = L"btnCancel";
				this->btnCancel->UseVisualStyleBackColor = true;
				// 
				// cmbEncodeType
				// 
				resources->ApplyResources(this->cmbEncodeType, L"cmbEncodeType");
				this->cmbEncodeType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
				this->cmbEncodeType->FormattingEnabled = true;
				this->cmbEncodeType->Name = L"cmbEncodeType";
				this->cmbEncodeType->SelectedIndexChanged += gcnew System::EventHandler(this, &TargetCodecDialog::CmbEncodeType_SelectedIndexChanged);
				// 
				// TargetCodecDialog
				// 
				this->AcceptButton = this->btnOK;
				resources->ApplyResources(this, L"$this");
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->CancelButton = this->btnCancel;
				this->Controls->Add(this->cmbEncodeType);
				this->Controls->Add(this->btnCancel);
				this->Controls->Add(this->btnOK);
				this->Controls->Add(this->groupAudioCodec);
				this->Controls->Add(this->groupVideoCodec);
				this->MaximizeBox = false;
				this->MinimizeBox = false;
				this->Name = L"TargetCodecDialog";
				this->ShowIcon = false;
				this->ShowInTaskbar = false;
				this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &TargetCodecDialog::TargetCodecDialog_FormClosing);
				this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &TargetCodecDialog::TargetCodecDialog_FormClosed);
				this->Load += gcnew System::EventHandler(this, &TargetCodecDialog::TargetCodecDialog_Load);
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
			System::Void TargetCodecDialog_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
			System::Void TargetCodecDialog_Load(System::Object^ sender, System::EventArgs^ e);

};
	}
}