#pragma once

#include "AVCodec.h"

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

			literal String^ KEY_SAME_DIRECTORY = L"IsSameDirectory";
			literal String^ KEY_OTHER_DIRECTORY = L"OtherDirectory";
			literal String^ KEY_FILE_BY_FILE = L"FileByFile";

			literal String^ KEY_FILENAME_MACRO = L"FilenameMacro";
			literal String^ FILENAME_MACRO_DEFAULT = L"${basename}${targetext}";
			literal int FILENAME_MACRO_MAX = 64;
			//literal String^ KEY_FILENAME_MACRO_ARRAY = L"FilenameMacroArray";
			//literal String^ KEY_FILENAME_AFTER = L"FilenameAfter";
			//literal String^ KEY_FILENAME_AFTER_ARRAY = L"FilenameAfterArray";

			literal String^ KEY_ADDITIONALOPTIONS_BEFOREINPUT = L"AdditionalOptionsBeforeInput";
			literal String^ KEY_ADDITIONALOPTIONS_BEFOREINPUT_ARRAY = L"AdditionalOptionsBeforeInputArray";
			literal String^ KEY_ADDITIONALOPTIONS_AFTERINPUT = L"AdditionalOptionsAfterInput";
			literal String^ KEY_ADDITIONALOPTIONS_AFTERINPUT_ARRAY = L"AdditionalOptionsAfterInputArray";

			initonly String^ iniPath_;
			initonly String^ section_;
		private: System::Windows::Forms::GroupBox^ groupTargetDirectory;
		private: System::Windows::Forms::Button^ btnBrowseOtherDirectory;
		private: System::Windows::Forms::Label^ lblOtherDirectory;
		private: System::Windows::Forms::TextBox^ txtOtherDirectory;
		private: System::Windows::Forms::CheckBox^ chkSameDirectory;
		private: System::Windows::Forms::GroupBox^ groupFilename;

		private: System::Windows::Forms::CheckBox^ chkFileByFile;


			   initonly bool losslessable_;
			   initonly array<String^>^ InputMovies;
			   initonly AVCodec^ DefaultVideoCodec;
			   MacroManager^ mm_;
		private: System::Windows::Forms::ComboBox^ cmbFilenameMacro;


		private: System::Windows::Forms::GroupBox^ groupAdditionalOptions;

		private: System::Windows::Forms::Label^ lblAdditonalOptionAfterInput;

		private: System::Windows::Forms::Label^ lblBeforeInputFile;
		private: System::Windows::Forms::ComboBox^ cmbAdditionalOptionsAfterInput;
		private: System::Windows::Forms::ComboBox^ cmbAdditionalOptionsBeforeInput;

		private: System::Windows::Forms::Label^ lblBeforeFilename;
		private: System::Windows::Forms::Button^ btnFilenameMacroMenu;
		private: System::Windows::Forms::ContextMenuStrip^ cmFilenameMacro;
		private: System::Windows::Forms::ToolStripMenuItem^ basenameToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^ basenamewithoutextToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^ targetextToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^ originalextToolStripMenuItem;
		private: System::Windows::Forms::TextBox^ txtResultFilename;
		private: System::Windows::Forms::Label^ lblFilenameResult;

			   initonly AVCodec^ DefaultAudioCodec;
		public:
			TargetCodecDialog(bool bLosslessable, 
				String^ iniPath, 
				String^ section,
				array<String^>^ inputMovies,
				AVCodec^ defaultV, AVCodec^ defaultA);

			void UpdateFilename();
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
			property bool AudioCodecIntEnabled
			{
				bool get()
				{
					return groupAudioCodec->Enabled;
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
			property bool VideoCodecIntEnabled
			{
				bool get()
				{
					return groupVideoCodec->Enabled;
				}
			}
			property bool IsEachFile
			{
				bool get()
				{
					return IsReEncode && chkFileByFile->Enabled && chkFileByFile->Checked;
				}
			}
			property bool IsConcat
			{
				bool get()
				{
					if (InputMovies->Length == 1)
						return false;
					return !IsEachFile;
				}
			}
			property String^ OutputFile
			{
				String^ get();
			}
			array<String^>^ outputFiles_;
			property array<String^>^ OutputFiles
			{
				array<String^>^ get() {
					DASSERT(outputFiles_ && outputFiles_->Length != 0);
					return outputFiles_;
				}
			}
			property AVCodec^ OutputVideoCodec;
			property AVCodec^ OutputAudioCodec;


			property String^ AdditonalOptionsBeforeInput
			{
				String^ get() { return cmbAdditionalOptionsBeforeInput->Text; }
			}
			property String^ AdditonalOptionsAfterInput
			{
				String^ get() { return cmbAdditionalOptionsAfterInput->Text; }
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
private: System::ComponentModel::IContainer^ components;
internal:
private:

		internal:
		private:









		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>


#pragma region Windows Form Designer generated code
			/// <summary>
			/// Required method for Designer support - do not modify
			/// the contents of this method with the code editor.
			/// </summary>
			void InitializeComponent(void)
			{
				this->components = (gcnew System::ComponentModel::Container());
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
				this->groupTargetDirectory = (gcnew System::Windows::Forms::GroupBox());
				this->btnBrowseOtherDirectory = (gcnew System::Windows::Forms::Button());
				this->lblOtherDirectory = (gcnew System::Windows::Forms::Label());
				this->txtOtherDirectory = (gcnew System::Windows::Forms::TextBox());
				this->chkSameDirectory = (gcnew System::Windows::Forms::CheckBox());
				this->groupFilename = (gcnew System::Windows::Forms::GroupBox());
				this->txtResultFilename = (gcnew System::Windows::Forms::TextBox());
				this->btnFilenameMacroMenu = (gcnew System::Windows::Forms::Button());
				this->lblFilenameResult = (gcnew System::Windows::Forms::Label());
				this->lblBeforeFilename = (gcnew System::Windows::Forms::Label());
				this->cmbFilenameMacro = (gcnew System::Windows::Forms::ComboBox());
				this->chkFileByFile = (gcnew System::Windows::Forms::CheckBox());
				this->groupAdditionalOptions = (gcnew System::Windows::Forms::GroupBox());
				this->cmbAdditionalOptionsAfterInput = (gcnew System::Windows::Forms::ComboBox());
				this->cmbAdditionalOptionsBeforeInput = (gcnew System::Windows::Forms::ComboBox());
				this->lblAdditonalOptionAfterInput = (gcnew System::Windows::Forms::Label());
				this->lblBeforeInputFile = (gcnew System::Windows::Forms::Label());
				this->cmFilenameMacro = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
				this->basenameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->basenamewithoutextToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->targetextToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->originalextToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->groupVideoCodec->SuspendLayout();
				this->groupAudioCodec->SuspendLayout();
				this->groupTargetDirectory->SuspendLayout();
				this->groupFilename->SuspendLayout();
				this->groupAdditionalOptions->SuspendLayout();
				this->cmFilenameMacro->SuspendLayout();
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
				this->rbVideoAV1->CheckedChanged += gcnew System::EventHandler(this, &TargetCodecDialog::codec_CheckedChangedCommon);
				// 
				// rbVideoVp9
				// 
				resources->ApplyResources(this->rbVideoVp9, L"rbVideoVp9");
				this->rbVideoVp9->Name = L"rbVideoVp9";
				this->rbVideoVp9->TabStop = true;
				this->rbVideoVp9->UseVisualStyleBackColor = true;
				this->rbVideoVp9->CheckedChanged += gcnew System::EventHandler(this, &TargetCodecDialog::codec_CheckedChangedCommon);
				// 
				// rbVideoCopy
				// 
				resources->ApplyResources(this->rbVideoCopy, L"rbVideoCopy");
				this->rbVideoCopy->Checked = true;
				this->rbVideoCopy->Name = L"rbVideoCopy";
				this->rbVideoCopy->TabStop = true;
				this->rbVideoCopy->UseVisualStyleBackColor = true;
				this->rbVideoCopy->CheckedChanged += gcnew System::EventHandler(this, &TargetCodecDialog::codec_CheckedChangedCommon);
				// 
				// rbVideoH265
				// 
				resources->ApplyResources(this->rbVideoH265, L"rbVideoH265");
				this->rbVideoH265->Name = L"rbVideoH265";
				this->rbVideoH265->TabStop = true;
				this->rbVideoH265->UseVisualStyleBackColor = true;
				this->rbVideoH265->CheckedChanged += gcnew System::EventHandler(this, &TargetCodecDialog::codec_CheckedChangedCommon);
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
				this->rbAudioOpus->CheckedChanged += gcnew System::EventHandler(this, &TargetCodecDialog::codec_CheckedChangedCommon);
				// 
				// rbAudioAac
				// 
				resources->ApplyResources(this->rbAudioAac, L"rbAudioAac");
				this->rbAudioAac->Name = L"rbAudioAac";
				this->rbAudioAac->TabStop = true;
				this->rbAudioAac->UseVisualStyleBackColor = true;
				this->rbAudioAac->CheckedChanged += gcnew System::EventHandler(this, &TargetCodecDialog::codec_CheckedChangedCommon);
				// 
				// rbAudioCopy
				// 
				resources->ApplyResources(this->rbAudioCopy, L"rbAudioCopy");
				this->rbAudioCopy->Checked = true;
				this->rbAudioCopy->Name = L"rbAudioCopy";
				this->rbAudioCopy->TabStop = true;
				this->rbAudioCopy->UseVisualStyleBackColor = true;
				this->rbAudioCopy->CheckedChanged += gcnew System::EventHandler(this, &TargetCodecDialog::codec_CheckedChangedCommon);
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
				// groupTargetDirectory
				// 
				resources->ApplyResources(this->groupTargetDirectory, L"groupTargetDirectory");
				this->groupTargetDirectory->Controls->Add(this->btnBrowseOtherDirectory);
				this->groupTargetDirectory->Controls->Add(this->lblOtherDirectory);
				this->groupTargetDirectory->Controls->Add(this->txtOtherDirectory);
				this->groupTargetDirectory->Controls->Add(this->chkSameDirectory);
				this->groupTargetDirectory->Name = L"groupTargetDirectory";
				this->groupTargetDirectory->TabStop = false;
				// 
				// btnBrowseOtherDirectory
				// 
				resources->ApplyResources(this->btnBrowseOtherDirectory, L"btnBrowseOtherDirectory");
				this->btnBrowseOtherDirectory->Name = L"btnBrowseOtherDirectory";
				this->btnBrowseOtherDirectory->UseVisualStyleBackColor = true;
				this->btnBrowseOtherDirectory->Click += gcnew System::EventHandler(this, &TargetCodecDialog::btnBrowseOtherDirectory_Click);
				// 
				// lblOtherDirectory
				// 
				resources->ApplyResources(this->lblOtherDirectory, L"lblOtherDirectory");
				this->lblOtherDirectory->Name = L"lblOtherDirectory";
				// 
				// txtOtherDirectory
				// 
				resources->ApplyResources(this->txtOtherDirectory, L"txtOtherDirectory");
				this->txtOtherDirectory->Name = L"txtOtherDirectory";
				// 
				// chkSameDirectory
				// 
				resources->ApplyResources(this->chkSameDirectory, L"chkSameDirectory");
				this->chkSameDirectory->Checked = true;
				this->chkSameDirectory->CheckState = System::Windows::Forms::CheckState::Checked;
				this->chkSameDirectory->Name = L"chkSameDirectory";
				this->chkSameDirectory->UseVisualStyleBackColor = true;
				this->chkSameDirectory->CheckedChanged += gcnew System::EventHandler(this, &TargetCodecDialog::chkSameDirectory_CheckedChanged);
				// 
				// groupFilename
				// 
				resources->ApplyResources(this->groupFilename, L"groupFilename");
				this->groupFilename->Controls->Add(this->txtResultFilename);
				this->groupFilename->Controls->Add(this->btnFilenameMacroMenu);
				this->groupFilename->Controls->Add(this->lblFilenameResult);
				this->groupFilename->Controls->Add(this->lblBeforeFilename);
				this->groupFilename->Controls->Add(this->cmbFilenameMacro);
				this->groupFilename->Name = L"groupFilename";
				this->groupFilename->TabStop = false;
				// 
				// txtResultFilename
				// 
				resources->ApplyResources(this->txtResultFilename, L"txtResultFilename");
				this->txtResultFilename->Name = L"txtResultFilename";
				this->txtResultFilename->ReadOnly = true;
				// 
				// btnFilenameMacroMenu
				// 
				resources->ApplyResources(this->btnFilenameMacroMenu, L"btnFilenameMacroMenu");
				this->btnFilenameMacroMenu->Name = L"btnFilenameMacroMenu";
				this->btnFilenameMacroMenu->UseVisualStyleBackColor = true;
				this->btnFilenameMacroMenu->Click += gcnew System::EventHandler(this, &TargetCodecDialog::btnFilenameMacroMenu_Click);
				// 
				// lblFilenameResult
				// 
				resources->ApplyResources(this->lblFilenameResult, L"lblFilenameResult");
				this->lblFilenameResult->Name = L"lblFilenameResult";
				// 
				// lblBeforeFilename
				// 
				resources->ApplyResources(this->lblBeforeFilename, L"lblBeforeFilename");
				this->lblBeforeFilename->Name = L"lblBeforeFilename";
				// 
				// cmbFilenameMacro
				// 
				resources->ApplyResources(this->cmbFilenameMacro, L"cmbFilenameMacro");
				this->cmbFilenameMacro->Name = L"cmbFilenameMacro";
				this->cmbFilenameMacro->SelectedIndexChanged += gcnew System::EventHandler(this, &TargetCodecDialog::cmbFilenameMacro_SelectedIndexChanged);
				this->cmbFilenameMacro->TextUpdate += gcnew System::EventHandler(this, &TargetCodecDialog::cmbFilenameMacro_TextUpdate);
				// 
				// chkFileByFile
				// 
				resources->ApplyResources(this->chkFileByFile, L"chkFileByFile");
				this->chkFileByFile->Name = L"chkFileByFile";
				this->chkFileByFile->UseVisualStyleBackColor = true;
				// 
				// groupAdditionalOptions
				// 
				resources->ApplyResources(this->groupAdditionalOptions, L"groupAdditionalOptions");
				this->groupAdditionalOptions->Controls->Add(this->cmbAdditionalOptionsAfterInput);
				this->groupAdditionalOptions->Controls->Add(this->cmbAdditionalOptionsBeforeInput);
				this->groupAdditionalOptions->Controls->Add(this->lblAdditonalOptionAfterInput);
				this->groupAdditionalOptions->Controls->Add(this->lblBeforeInputFile);
				this->groupAdditionalOptions->Name = L"groupAdditionalOptions";
				this->groupAdditionalOptions->TabStop = false;
				// 
				// cmbAdditionalOptionsAfterInput
				// 
				resources->ApplyResources(this->cmbAdditionalOptionsAfterInput, L"cmbAdditionalOptionsAfterInput");
				this->cmbAdditionalOptionsAfterInput->FormattingEnabled = true;
				this->cmbAdditionalOptionsAfterInput->Name = L"cmbAdditionalOptionsAfterInput";
				// 
				// cmbAdditionalOptionsBeforeInput
				// 
				resources->ApplyResources(this->cmbAdditionalOptionsBeforeInput, L"cmbAdditionalOptionsBeforeInput");
				this->cmbAdditionalOptionsBeforeInput->FormattingEnabled = true;
				this->cmbAdditionalOptionsBeforeInput->Name = L"cmbAdditionalOptionsBeforeInput";
				// 
				// lblAdditonalOptionAfterInput
				// 
				resources->ApplyResources(this->lblAdditonalOptionAfterInput, L"lblAdditonalOptionAfterInput");
				this->lblAdditonalOptionAfterInput->Name = L"lblAdditonalOptionAfterInput";
				// 
				// lblBeforeInputFile
				// 
				resources->ApplyResources(this->lblBeforeInputFile, L"lblBeforeInputFile");
				this->lblBeforeInputFile->Name = L"lblBeforeInputFile";
				// 
				// cmFilenameMacro
				// 
				this->cmFilenameMacro->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
					this->basenameToolStripMenuItem,
						this->basenamewithoutextToolStripMenuItem, this->targetextToolStripMenuItem, this->originalextToolStripMenuItem
				});
				this->cmFilenameMacro->Name = L"cmFilenameMacro";
				resources->ApplyResources(this->cmFilenameMacro, L"cmFilenameMacro");
				// 
				// basenameToolStripMenuItem
				// 
				this->basenameToolStripMenuItem->Name = L"basenameToolStripMenuItem";
				resources->ApplyResources(this->basenameToolStripMenuItem, L"basenameToolStripMenuItem");
				// 
				// basenamewithoutextToolStripMenuItem
				// 
				this->basenamewithoutextToolStripMenuItem->Name = L"basenamewithoutextToolStripMenuItem";
				resources->ApplyResources(this->basenamewithoutextToolStripMenuItem, L"basenamewithoutextToolStripMenuItem");
				// 
				// targetextToolStripMenuItem
				// 
				this->targetextToolStripMenuItem->Name = L"targetextToolStripMenuItem";
				resources->ApplyResources(this->targetextToolStripMenuItem, L"targetextToolStripMenuItem");
				// 
				// originalextToolStripMenuItem
				// 
				this->originalextToolStripMenuItem->Name = L"originalextToolStripMenuItem";
				resources->ApplyResources(this->originalextToolStripMenuItem, L"originalextToolStripMenuItem");
				// 
				// TargetCodecDialog
				// 
				this->AcceptButton = this->btnOK;
				resources->ApplyResources(this, L"$this");
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->CancelButton = this->btnCancel;
				this->Controls->Add(this->groupAdditionalOptions);
				this->Controls->Add(this->chkFileByFile);
				this->Controls->Add(this->groupFilename);
				this->Controls->Add(this->groupTargetDirectory);
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
				this->groupTargetDirectory->ResumeLayout(false);
				this->groupTargetDirectory->PerformLayout();
				this->groupFilename->ResumeLayout(false);
				this->groupFilename->PerformLayout();
				this->groupAdditionalOptions->ResumeLayout(false);
				this->groupAdditionalOptions->PerformLayout();
				this->cmFilenameMacro->ResumeLayout(false);
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion
			String^ GetTargetExt();
			String^ GetTargetExt(bool showWarning);
			bool UpdateOutputFiles();
			void UpdateEnableState();
			array<String^>^ GetTargetDirectories();
			System::Collections::Generic::Dictionary<String^, String^>^ GetMacros(
				String^ inputmovie,
				String^ basename);
			String^ GetBaseName(int i);
			bool IsSameAsInputMovies(String^ path);
			bool UpdateCodec();
		private:
			System::Void CmbEncodeType_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
			System::Void BtnOK_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void TargetCodecDialog_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
			System::Void TargetCodecDialog_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
			System::Void TargetCodecDialog_Load(System::Object^ sender, System::EventArgs^ e);
			System::Void btnBrowseOtherDirectory_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void chkSameDirectory_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
				UpdateEnableState();
			}
			System::Void btnFilenameMacroMenu_Click(System::Object^ sender, System::EventArgs^ e);
			System::Void codec_CheckedChangedCommon(System::Object^ sender, System::EventArgs^ e);
			System::Void cmbFilenameMacro_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
				UpdateFilename();
			}
			System::Void cmbFilenameMacro_TextUpdate(System::Object^ sender, System::EventArgs^ e) {
				UpdateFilename();
			}
};
	}
}