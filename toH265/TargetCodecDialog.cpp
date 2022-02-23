#include "stdafx.h"

#include "toH265.h"
#include "AVCodec.h"
#include "FormMain.h"
#include "helper.h"

#include "TargetCodecDialog.h"
using namespace System::Text;

using namespace System::Collections::Generic;
using namespace System::IO;

namespace Ambiesoft {
	namespace toH265 {

	

		TargetCodecDialog::TargetCodecDialog(bool bLosslessable, 
			String^ iniPath,
			String^ section,
			array<String^>^ inputMovies,
			AVCodec^ defaultV, AVCodec^ defaultA):
			losslessable_(bLosslessable), 
			iniPath_(iniPath),
			section_(section),
			InputMovies(inputMovies),
			DefaultVideoCodec(defaultV),DefaultAudioCodec(defaultA)
		{
			InitializeComponent();

			cmbEncodeType->Items->Add(I18N("Lossless concat"));
			cmbEncodeType->Items->Add(I18N("Reencode"));

			UpdateEnableState();
		}
		void TargetCodecDialog::UpdateEnableState()
		{
			lblOtherDirectory->Enabled = !chkSameDirectory->Checked;
			lblOtherDirectory->Visible = !chkSameDirectory->Checked;

			txtOtherDirectory->Enabled = !chkSameDirectory->Checked;
			txtOtherDirectory->Visible = !chkSameDirectory->Checked;

			btnBrowseOtherDirectory->Enabled = !chkSameDirectory->Checked;
			btnBrowseOtherDirectory->Visible = !chkSameDirectory->Checked;
			
			chkFileByFile->Enabled = InputMovies->Length > 1 && cmbEncodeType->SelectedIndex == 1;
		}
		System::Void TargetCodecDialog::TargetCodecDialog_Load(System::Object^ sender, System::EventArgs^ e)
		{
			int iniComboIndex = -1;
			if (CanSerialize)
			{
				HashIni^ ini = Profile::ReadAll(iniPath_);
				Profile::GetInt(SECTION, KEY_ENCODE_TYPE, -1, iniComboIndex, ini);
				int v = -1;
				bool b;
				String^ s;
				cli::array<String^>^ a;

				Profile::GetInt(SECTION, KEY_AUIDOCODEC, -1, v, ini);
				if (v != -1)
					AudioCodecInt = v;
				Profile::GetInt(SECTION, KEY_VIDEOCODEC, -1, v, ini);
				if (v != -1)
					VideoCodecInt = v;
				
				Profile::GetBool(SECTION, KEY_SAME_DIRECTORY, true, b, ini);
				chkSameDirectory->Checked = b;

				Profile::GetString(SECTION, KEY_OTHER_DIRECTORY, String::Empty, s, ini);
				txtOtherDirectory->Text = s;

				Profile::GetBool(SECTION, KEY_FILE_BY_FILE, false, b, ini);
				chkFileByFile->Checked = b;

				AmbLib::LoadComboBox(cmbFilenameMacro, KEY_FILENAME_MACRO, FILENAME_MACRO_MAX, ini);

				Profile::GetStringArray(SECTION, KEY_FILENAME_MACRO, a, ini);
				cmbFilenameMacro->Items->AddRange(a);
				if (!cmbFilenameMacro->Items->Contains(FILENAME_MACRO_DEFAULT))
					cmbFilenameMacro->Items->Add(FILENAME_MACRO_DEFAULT);
				if(String::IsNullOrEmpty(cmbFilenameMacro->Text))
				{
					cmbFilenameMacro->Text = FILENAME_MACRO_DEFAULT;
				}
				
				//Profile::GetStringArray(SECTION, KEY_FILENAME_AFTER_ARRAY, a, ini);
				//cmbAfterFilename->Items->AddRange(a);
				//Profile::GetString(SECTION, KEY_FILENAME_AFTER, String::Empty, s, ini);
				//cmbAfterFilename->Text = s;



				Profile::GetStringArray(SECTION, KEY_ADDITIONALOPTIONS_BEFOREINPUT_ARRAY, a, ini);
				cmbAdditionalOptionsBeforeInput->Items->AddRange(a);
				Profile::GetString(SECTION, KEY_ADDITIONALOPTIONS_BEFOREINPUT, String::Empty, s, ini);
				cmbAdditionalOptionsBeforeInput->Text = s;

				Profile::GetStringArray(SECTION, KEY_ADDITIONALOPTIONS_AFTERINPUT_ARRAY, a, ini);
				cmbAdditionalOptionsAfterInput->Items->AddRange(a);
				Profile::GetString(SECTION, KEY_ADDITIONALOPTIONS_AFTERINPUT, String::Empty, s, ini);
				cmbAdditionalOptionsAfterInput->Text = s;

				Profile::GetBool(SECTION, KEY_ISMOVEENDEDINPUTFILES, false, b, ini);
				chkMoveFinishedInputs->Checked = b;
			}

			if (losslessable_)
			{
				cmbEncodeType->SelectedIndex = iniComboIndex != -1 ? iniComboIndex : 1;
			}
			else
			{
				// Fix 'Reencode'
				cmbEncodeType->SelectedIndex = 1;
				cmbEncodeType->Enabled = false;
			}

			UpdateFilename();
		}
		System::Void TargetCodecDialog::CmbEncodeType_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
		{
			UpdateEnableState();

			switch (cmbEncodeType->SelectedIndex)
			{
			case 0:
				// lossless
				rbAudioCopy->Checked = true;
				rbVideoCopy->Checked = true;
				groupAudioCodec->Enabled = false;
				groupVideoCodec->Enabled = false;
				return;
			}
			groupAudioCodec->Enabled = true;
			groupVideoCodec->Enabled = true;

			HashIni^ ini = Profile::ReadAll(iniPath_);
			int v = -1;
			Profile::GetInt(SECTION, KEY_AUIDOCODEC, -1, v, ini);
			if (v != -1)
				AudioCodecInt = v;
			Profile::GetInt(SECTION, KEY_VIDEOCODEC, -1, v, ini);
			if (v != -1)
				VideoCodecInt = v;
		}
		System::Void TargetCodecDialog::btnFilenameMacroMenu_Click(System::Object^ sender, System::EventArgs^ e)
		{
			//System::Drawing::Point p = btnFilenameMacroMenu->Location;
			//p.Y = p.Y + btnFilenameMacroMenu->Size.Width;
			//p = btnFilenameMacroMenu->Parent->PointToScreen(p);
			//cmFilenameMacro->Show(p.X, p.Y);

			
			mm_->InputString = cmbFilenameMacro->Text;
			if (System::Windows::Forms::DialogResult::OK != mm_->ShowDialog())
				return;
			cmbFilenameMacro->Text = mm_->InputString;
			txtResultFilename->Text = mm_->ResultString;
		}

		System::Void TargetCodecDialog::TargetCodecDialog_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
		{
			if (this->DialogResult == System::Windows::Forms::DialogResult::None)
			{
				e->Cancel = true;
				return;
			}
		}

		cli::array<String^>^ toStringArray(ComboBox::ObjectCollection^ collection)
		{
			List<String^> ret;
			for each (Object ^ o in collection)
				if(!String::IsNullOrEmpty(o->ToString()))
					ret.Add(o->ToString());
			return ret.ToArray();
		}
		System::Void TargetCodecDialog::btnSaveAsDefault_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (!CanSerialize)
			{
				CppUtils::Alert(I18N(L"Can not serialize"));
				return;
			}
			HashIni^ ini = Profile::ReadAll(IniPath);
			bool success = true;
			success &= Profile::WriteInt(SECTION, KEY_ENCODE_TYPE, cmbEncodeType->SelectedIndex, ini);
			if(AudioCodecIntEnabled)
				success &= Profile::WriteInt(SECTION, KEY_AUIDOCODEC, AudioCodecInt, ini);
			if(VideoCodecIntEnabled)
				success &= Profile::WriteInt(SECTION, KEY_VIDEOCODEC, VideoCodecInt, ini);

			success &= Profile::WriteBool(SECTION, KEY_SAME_DIRECTORY, chkSameDirectory->Checked, ini);
			success &= Profile::WriteString(SECTION, KEY_OTHER_DIRECTORY, txtOtherDirectory->Text, ini);
			success &= Profile::WriteBool(SECTION, KEY_FILE_BY_FILE, chkFileByFile->Checked, ini);

			success &= AmbLib::SaveComboBox(cmbFilenameMacro, KEY_FILENAME_MACRO, FILENAME_MACRO_MAX, ini);

			success &= Profile::WriteStringArray(SECTION, KEY_ADDITIONALOPTIONS_BEFOREINPUT_ARRAY,
				toStringArray(cmbAdditionalOptionsBeforeInput->Items), ini);
			success &= Profile::WriteString(SECTION, KEY_ADDITIONALOPTIONS_BEFOREINPUT,
				cmbAdditionalOptionsBeforeInput->Text, ini);

			success &= Profile::WriteStringArray(SECTION, KEY_ADDITIONALOPTIONS_AFTERINPUT_ARRAY,
				toStringArray(cmbAdditionalOptionsAfterInput->Items), ini);
			success &= Profile::WriteString(SECTION, KEY_ADDITIONALOPTIONS_AFTERINPUT,
				cmbAdditionalOptionsAfterInput->Text, ini);

			success &= Profile::WriteBool(SECTION, KEY_ISMOVEENDEDINPUTFILES,
				chkMoveFinishedInputs->Checked, ini);

			success &= Profile::WriteAll(ini, IniPath);
			if(!success)
			{
				CppUtils::Alert(I18N(L"Failed to save ini"));
				return;
			}

			lblSaved->Visible = true;
			timerSaved->Enabled = true;
		}
		System::Void TargetCodecDialog::timerSaved_Tick(System::Object^ sender, System::EventArgs^ e)
		{
			lblSaved->Visible = false;
			timerSaved->Enabled = false;
		}
		System::Void TargetCodecDialog::TargetCodecDialog_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
		{
			if (this->DialogResult != System::Windows::Forms::DialogResult::OK)
				return;
			if (!CanSerialize)
				return;

			HashIni^ ini = Profile::ReadAll(IniPath);
			bool success = true;

			// Save combobox items but not current text
			success &= Profile::WriteStringArray(SECTION, KEY_ADDITIONALOPTIONS_BEFOREINPUT_ARRAY,
				toStringArray(cmbAdditionalOptionsBeforeInput->Items), ini);

			success &= Profile::WriteStringArray(SECTION, KEY_ADDITIONALOPTIONS_AFTERINPUT_ARRAY,
				toStringArray(cmbAdditionalOptionsAfterInput->Items), ini);

			success &= Profile::WriteAll(ini, IniPath);
			if (!success)
			{
				CppUtils::Alert(I18N(L"Failed to save ini"));
			}
		}

		void StoreComboItem(ComboBox^ combo)
		{
			String^ text = combo->Text;
			if (String::IsNullOrEmpty(text))
				return;

			int index = combo->FindStringExact(text);
			if (index == 0)
			{
				return;
			}
			else if (index > 0)
			{
				combo->Items->RemoveAt(index);
			}
			combo->Items->Insert(0, text);
			combo->Text = text;
		}
		array<String^>^ TargetCodecDialog::GetTargetDirectories()
		{
			array<String^>^ dirs = gcnew array<String^>(InputMovies->Length);
			for (int i = 0; i < InputMovies->Length; ++i)
			{
				dirs[i] = Path::Combine(chkSameDirectory->Checked ?
					Path::GetDirectoryName(InputMovies[i]) :
					Path::Combine(Path::GetDirectoryName(InputMovies[i]),
						txtOtherDirectory->Text));
			}
			return dirs;
		}
		System::Collections::Generic::Dictionary<String^, String^>^ TargetCodecDialog::GetMacros(
			String^ inputmovie,
			String^ basename)
		{
			System::Collections::Generic::Dictionary<String^, String^>^ macros =
				gcnew System::Collections::Generic::Dictionary<String^, String^>();
			macros->Add("inputext", Path::GetExtension(inputmovie));
			macros->Add("basename", basename);
			macros->Add("targetext", GetTargetExt(false));
			macros->Add("basenamewithoutext", Path::GetFileNameWithoutExtension(basename));
			macros->Add("parentdirname", Path::GetFileName(Path::GetDirectoryName(inputmovie)));
			macros->Add("parentdirectory", Path::GetDirectoryName(inputmovie));

			return macros;
		}
		String^ TargetCodecDialog::GetBaseName(int i)
		{
			String^ baseFileName = IsConcat ?
				Ambiesoft::toH265Helper::GetCommonFilename(InputMovies) :
				Path::GetFileName(InputMovies[i]);
			if (String::IsNullOrEmpty(baseFileName))
				baseFileName = "output";
			return baseFileName;
		}
		System::Void TargetCodecDialog::BtnOK_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// default is OK
			this->DialogResult = System::Windows::Forms::DialogResult::OK;

			if (cmbEncodeType->SelectedIndex == 0)
			{
				// LossLess
				// Copy must be 'checked'
				if (!(rbAudioCopy->Checked && rbVideoCopy->Checked))
				{
					CppUtils::Alert(I18N("'Copy' must be selected for 'Lossless concat'."));
					this->DialogResult = System::Windows::Forms::DialogResult::None;
					return;
				}
			}
			else
			{
				// Reencode
				if (!rbAudioCopy->Checked && !rbAudioAac->Checked && !rbAudioOpus->Checked)
				{
					// No audio selected
					CppUtils::Alert(I18N("No audio selected"));
					this->DialogResult = System::Windows::Forms::DialogResult::None;
					return;
				}
				if (!rbVideoCopy->Checked && !rbVideoH265->Checked && !rbVideoVp9->Checked && !rbVideoAV1->Checked)
				{
					// No video selected
					CppUtils::Alert(I18N("No video selected"));
					this->DialogResult = System::Windows::Forms::DialogResult::None;
					return;
				}
			}


			if (!chkSameDirectory->Checked)
			{
				if (String::IsNullOrEmpty(txtOtherDirectory->Text))
				{
					CppUtils::Alert(I18N("Other Directory is empty."));
					this->DialogResult = System::Windows::Forms::DialogResult::None;
					txtOtherDirectory->Focus();
					return;
				}
			}

			// check and create target directory
			// relative path is regardted as relative from input files
			List<String^>^ dirsNotExists = gcnew List<String^>();
			for each (String ^ fulldir in GetTargetDirectories())
			{
				if (!System::IO::Directory::Exists(fulldir))
					dirsNotExists->Add(fulldir);
			}
			dirsNotExists = MakeUnique(dirsNotExists);
			if (dirsNotExists->Count != 0)
			{
				StringBuilder sb;
				sb.AppendLine(I18N("Following target directories does not exist. Do you want to create them?"));
				sb.AppendLine();
				for each (String ^ line in dirsNotExists)
					sb.AppendLine(line);

				if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(sb.ToString()))
				{
					this->DialogResult = System::Windows::Forms::DialogResult::None;
					txtOtherDirectory->Focus();
					return;
				}

				try
				{
					for each (String ^ dir in dirsNotExists)
						Directory::CreateDirectory(dir);
				}
				catch (Exception^ ex)
				{
					CppUtils::Alert(ex->Message);
					this->DialogResult = System::Windows::Forms::DialogResult::None;
					return;
				}
			}


			if (!UpdateOutputFiles())
			{
				// CppUtils::Alert(I18N("UpdateExtension failed"));
				this->DialogResult = System::Windows::Forms::DialogResult::None;
				return;
			}

			if (AmbLib::HasSameFile(OutputFiles))
			{
				CppUtils::Alert(I18N("Output files has same file."));
				this->DialogResult = System::Windows::Forms::DialogResult::None;
				return;
			}

			// check output file exists
			List<String^> exists;
			for each (String ^ outputFile in OutputFiles)
			{
				if (File::Exists(outputFile))
					exists.Add(outputFile);
			}
			if (exists.Count != 0)
			{
				StringBuilder sbMessage;
				sbMessage.AppendLine(I18N("Following output files already exist. Do you want to override them?"));
				sbMessage.AppendLine();
				for each (String ^ file in exists)
					sbMessage.AppendLine(file);
				if (System::Windows::Forms::DialogResult::Yes !=
					CppUtils::CenteredMessageBox(sbMessage.ToString(),
						Application::ProductName,
						MessageBoxButtons::YesNo,
						MessageBoxIcon::Warning,
						MessageBoxDefaultButton::Button2))
				{
					this->DialogResult = System::Windows::Forms::DialogResult::None;
						return;
				}
			}

			StoreComboItem(cmbFilenameMacro);
			// StoreComboItem(cmbAfterFilename);

			StoreComboItem(cmbAdditionalOptionsBeforeInput);
			StoreComboItem(cmbAdditionalOptionsAfterInput);
		}

		System::Void TargetCodecDialog::btnBrowseOtherDirectory_Click(System::Object^ sender, System::EventArgs^ e)
		{
			String^ folder = CppUtils::GetSelectedFolder(this, I18N("Select Folder"));
			if (String::IsNullOrEmpty(folder))
				return;
			txtOtherDirectory->Text = folder;
		}

		String^ TargetCodecDialog::OutputFile::get()
		{
			DASSERT(IsConcat);
			DASSERT(OutputFiles[0]);
			return OutputFiles[0];
		}

		bool TargetCodecDialog::IsSameAsInputMovies(String^ path)
		{
			for each (String ^ file in InputMovies)
			{
				if (AmbLib::IsSameFile(file, path))
					return true;
			}
			return false;
		}
		bool TargetCodecDialog::UpdateCodec()
		{
			return !String::IsNullOrEmpty(GetTargetExt());
		}
		String^ TargetCodecDialog::GetTargetExt()
		{
			return GetTargetExt(true);
		}
		String^ TargetCodecDialog::GetTargetExt(bool showWarning)
		{
			List<String^>^ outExtsNormalPriority = gcnew List<String^>();
			List<String^>^ outExtsHighPriority = gcnew List<String^>();

			bool bReEncode = IsReEncode;
			if (!bReEncode)
			{
				outExtsNormalPriority->Add(Path::GetExtension(InputMovies[0]));
				OutputAudioCodec = gcnew AVCodec(AVCodec::VC::VC_COPY);
				OutputVideoCodec = gcnew AVCodec(AVCodec::VC::VC_COPY);
			}
			else
			{
				// check if both codecs are 'copy'
				if (showWarning && rbVideoCopy->Checked && rbAudioCopy->Checked)
				{
					if (System::Windows::Forms::DialogResult::Yes != CppUtils::CenteredMessageBox(
						this,
						I18N("Both audio and video codes are 'copy'. Are you sure to continue?"),
						Application::ProductName,
						MessageBoxButtons::YesNo,
						MessageBoxIcon::Question,
						MessageBoxDefaultButton::Button2))
					{
						return nullptr;
					}
				}
				if (rbVideoH265->Checked)
				{
					OutputVideoCodec = gcnew AVCodec(AVCodec::VC::VC_H265);
					outExtsNormalPriority->Add(".mp4");
				}
				else if (rbVideoVp9->Checked)
				{
					OutputVideoCodec = gcnew AVCodec("vp9");
					outExtsNormalPriority->Add(".webm");
				}
				else if (rbVideoCopy->Checked)
				{
					if (DefaultVideoCodec->IsH264)
					{
						if (rbAudioOpus->Checked)
							outExtsNormalPriority->Add(".mkv");
						else
							outExtsNormalPriority->Add(".mp4");
					}
					OutputVideoCodec = gcnew AVCodec(AVCodec::VC::VC_COPY);
					outExtsNormalPriority->Add(Path::GetExtension(InputMovies[0]));
				}
				else if (rbVideoAV1->Checked)
				{
					OutputVideoCodec = gcnew AVCodec("av1");
					outExtsNormalPriority->Add(".mkv");
				}
				else
				{
					CppUtils::Alert(this, I18N(L"No video codec selected."));
					return nullptr;
				}

				if (rbAudioCopy->Checked)
					OutputAudioCodec = gcnew AVCodec(AVCodec::VC::VC_COPY);
				else if (rbAudioAac->Checked)
					OutputAudioCodec = gcnew AVCodec(AVCodec::VC::VC_AAC);
				else if (rbAudioOpus->Checked)
					OutputAudioCodec = gcnew AVCodec(AVCodec::VC::VC_OPUS);
				else
				{
					CppUtils::Alert(this, I18N(L"No audio codec selected."));
					return nullptr;
				}


				AVCodec^ targetVideoCodec = OutputVideoCodec->IsCopy ? DefaultVideoCodec : OutputVideoCodec;
				AVCodec^ targetAudioCodec = OutputAudioCodec->IsCopy ? DefaultAudioCodec : OutputAudioCodec;

				// vp9 can only hold audio of "vorbis" or "opus"
				if (OutputVideoCodec->IsVp9)
				{
					if (!targetAudioCodec->IsVorbis && !targetAudioCodec->IsOpus)
					{
						outExtsNormalPriority->Add(".mkv");
					}
				}
				if (outExtsNormalPriority->Contains(".mp4"))
				{
					if (targetVideoCodec->IsH265 && targetAudioCodec->IsOpus)
					{
						outExtsHighPriority->Add(".mkv");
					}
				}

				outExtsNormalPriority->Add(".mkv");
			}

			outExtsHighPriority = MakeUnique(outExtsHighPriority);
			outExtsNormalPriority->InsertRange(0, outExtsHighPriority);
			outExtsNormalPriority = MakeUnique(outExtsNormalPriority);

			for each (String ^ s in outExtsNormalPriority)
			{
				return s;
			}
			DASSERT(false);
			return nullptr;
		}
		bool TargetCodecDialog::UpdateOutputFiles()
		{
			try 
			{

				if (!UpdateCodec())
					return false;

				int countOutput = IsConcat ? 1 : InputMovies->Length;
				outputFiles_ = gcnew array<String^>(countOutput);

				for (int i = 0; i < countOutput; ++i)
				{
					// set basename
					String^ baseFileName = GetBaseName(i);

					// set extension
					//String^ firstExt;
					//for each (String ^ s in outExtsNormalPriority)
					//{
					//	firstExt = s;
					//	break;
					//}


					DASSERT(!String::IsNullOrEmpty(OutputVideoCodec->ToString()));
					String^ fullName = String::Empty;
					try
					{
						MacroManager^ mm = gcnew MacroManager(GetMacros(
							InputMovies[i],
							baseFileName));

						mm->InputString = cmbFilenameMacro->Text;
						String^ deployedFilename = mm->ResultString;
						if (String::IsNullOrEmpty(deployedFilename))
						{
							CppUtils::Alert(I18N(L"Filename is empty"));
							return false;
						}
						
						fullName = Path::Combine(GetTargetDirectories()[i],
							deployedFilename);
						if (IsSameAsInputMovies(fullName))
						{
							CppUtils::Alert(String::Format(I18N(L"Output file '{0}' is same path as one of input files."),
								fullName));
							return false;
						}
					}
					catch (Exception^ ex)
					{
						CppUtils::Alert(ex);
						return false;
					}
					
					if (IsFileOpen(getStdWstring(fullName).c_str()))
					{
						StringBuilder sb;
						sb.AppendLine(String::Format(I18N(FormMain::STR_0_ALREADY_OPENED), fullName));
						sb.AppendLine();
						sb.AppendLine(I18N(FormMain::STR_ARE_YOU_SURE_TO_CONTINUE));
						if (System::Windows::Forms::DialogResult::Yes != CppUtils::CenteredMessageBox(
							this,
							sb.ToString(),
							Application::ProductName,
							MessageBoxButtons::YesNo,
							MessageBoxIcon::Warning,
							MessageBoxDefaultButton::Button2))
						{
							return false;
						}
					}
					outputFiles_[i] = fullName;
				}

				return true;
			}
			catch (Exception^ ex)
			{
				CppUtils::Alert(ex->Message);
			}
			return false;
		}

		void TargetCodecDialog::UpdateFilename()
		{
			mm_ = gcnew MacroManager(GetMacros(
				InputMovies[0],
				GetBaseName(0)));
			mm_->InputString = cmbFilenameMacro->Text;
			txtResultFilename->Text = mm_->ResultString;
		}
		System::Void TargetCodecDialog::codec_CheckedChangedCommon(System::Object^ sender, System::EventArgs^ e)
		{
			UpdateFilename();
		}
		System::Void TargetCodecDialog::chkMoveFinishedInputs_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			if (chkMoveFinishedInputs->Checked)
				lblMoveFinishedInputFiles->Text = I18N("Finished Input Files will move to Folder 'old'.");
			else
				lblMoveFinishedInputFiles->Text = String::Empty;
		}
	}
}