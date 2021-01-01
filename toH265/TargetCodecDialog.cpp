#include "stdafx.h"

#include "toH265.h"
#include "AVCodec.h"
#include "FormMain.h"

#include "TargetCodecDialog.h"
using namespace System::Text;

using namespace System::Collections::Generic;
using namespace System::IO;

namespace Ambiesoft {
	namespace toH265 {

		List<String^>^ MakeUnique(List<String^>^ inList)
		{
			List<String^>^ result = gcnew List<String^>();
			for each (String ^ s in inList)
			{
				if (result->Contains(s))
					continue;
				result->Add(s);
			}
			return result;
		}

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
			txtOtherDirectory->Enabled = !chkSameDirectory->Checked;
		}
		System::Void TargetCodecDialog::TargetCodecDialog_Load(System::Object^ sender, System::EventArgs^ e)
		{
			int iniComboIndex = -1;
			if (CanSerialize)
			{
				HashIni^ ini = Profile::ReadAll(iniPath_);
				Profile::GetInt(SECTION, KEY_ENCODE_TYPE, -1, iniComboIndex, ini);
				int v = -1;
				Profile::GetInt(SECTION, KEY_AUIDOCODEC, -1, v, ini);
				if (v != -1)
					AudioCodecInt = v;
				Profile::GetInt(SECTION, KEY_VIDEOCODEC, -1, v, ini);
				if (v != -1)
					VideoCodecInt = v;
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
		}
		System::Void TargetCodecDialog::CmbEncodeType_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
		{
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

		System::Void TargetCodecDialog::TargetCodecDialog_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
		{
			if (this->DialogResult == System::Windows::Forms::DialogResult::None)
			{
				e->Cancel = true;
				return;
			}
		}
		System::Void TargetCodecDialog::TargetCodecDialog_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
		{
			if (this->DialogResult != System::Windows::Forms::DialogResult::OK)
				return;
			if (!CanSerialize)
				return;
			HashIni^ ini = Profile::ReadAll(IniPath);
			Profile::WriteInt(SECTION, KEY_ENCODE_TYPE, cmbEncodeType->SelectedIndex, ini);
			if(AudioCodecIntEnabled)
				Profile::WriteInt(SECTION, KEY_AUIDOCODEC, AudioCodecInt, ini);
			if(VideoCodecIntEnabled)
				Profile::WriteInt(SECTION, KEY_VIDEOCODEC, VideoCodecInt, ini);

			if (!Profile::WriteAll(ini, IniPath))
			{
				CppUtils::Alert(I18N(L"Failed to save ini"));
			}
		}
		System::Void TargetCodecDialog::BtnOK_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// default is OK
			this->DialogResult = System::Windows::Forms::DialogResult::OK;

			if (cmbEncodeType->SelectedIndex == 0)
			{
				// LossLess
				// Copy must be 'checked'
				if (rbAudioCopy->Checked && rbVideoCopy->Checked)
				{
					// OK
					return;
				}
				CppUtils::Alert(I18N("'Copy' must be selected for 'Lossless concat'."));
				this->DialogResult = System::Windows::Forms::DialogResult::None;
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
				if (!String::IsNullOrEmpty(txtOtherDirectory->Text))
				{
					CppUtils::Alert(I18N("Other Directory is empty."));
					this->DialogResult = System::Windows::Forms::DialogResult::None;
					txtOtherDirectory->Focus();
					return;
				}
				if (!System::IO::Directory::Exists(txtOtherDirectory->Text))
				{
					CppUtils::Alert(String::Format(I18N("Directory '{0}' does not exist."),
						txtOtherDirectory->Text));
					this->DialogResult = System::Windows::Forms::DialogResult::None;
					txtOtherDirectory->Focus();
					return;
				}
			}


			if (!UpdateOutputFiles())
			{
				CppUtils::Alert(I18N("UpdateExtension failed"));
				this->DialogResult = System::Windows::Forms::DialogResult::None;
				return;
			}

			for each (String ^ outputFile in OutputFiles)
			{
				if (File::Exists(outputFile))
				{
					if (System::Windows::Forms::DialogResult::Yes != CppUtils::YesOrNo(String::Format(I18N("Output File '{0}' already exists. Do you want to override?"),
						outputFile)))
					{
						this->DialogResult = System::Windows::Forms::DialogResult::None;
						return;
					}
				}
			}
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

		bool TargetCodecDialog::UpdateOutputFiles()
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
				if (rbVideoCopy->Checked && rbAudioCopy->Checked)
				{
					if (System::Windows::Forms::DialogResult::Yes != CppUtils::CenteredMessageBox(
						this,
						I18N("Both audio and video codes are 'copy'. Are you sure to continue?"),
						Application::ProductName,
						MessageBoxButtons::YesNo,
						MessageBoxIcon::Question,
						MessageBoxDefaultButton::Button2))
					{
						return false;
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
					return false;
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
					return false;
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


			int countOutput = IsConcat ? 1 : InputMovies->Length;
			outputFiles_ = gcnew array<String^>(countOutput);
			
			for (int i = 0; i < countOutput; ++i)
			{
				// set basename
				String^ initialDir;
				String^ baseFileName = InputMovies[i];
				if(IsConcat)
					baseFileName = Ambiesoft::toH265Helper::GetCommonFilename(InputMovies);
				if (String::IsNullOrEmpty(baseFileName))
					baseFileName = "output";

				if (String::IsNullOrEmpty(initialDir))
					initialDir = Path::GetDirectoryName(InputMovies[i]);

				SaveFileDialog dlg;
				{
					StringBuilder sbFilter;
					for each (String ^ ae in outExtsNormalPriority)
					{
						sbFilter.AppendFormat("{0} (*{1})|*{2}|",
							ae, ae, ae);
					}
					sbFilter.Append(I18N("All File") + "(*.*)|*.*");
					dlg.Filter = sbFilter.ToString();
				}

				dlg.InitialDirectory = initialDir;

				String^ firstExt;
				for each (String ^ s in outExtsNormalPriority)
				{
					firstExt = s;
					break;
				}

				DASSERT(!String::IsNullOrEmpty(OutputVideoCodec->ToString()));
				dlg.FileName = String::Format(L"{0} [{1}]{2}",
					baseFileName,
					OutputVideoCodec->ToString(),
					firstExt);

				//if (System::Windows::Forms::DialogResult::OK != dlg.ShowDialog())
				//	return;

				if (IsFileOpen(getStdWstring(dlg.FileName).c_str()))
				{
					StringBuilder sb;
					sb.AppendLine(String::Format(I18N(FormMain::STR_0_ALREADY_OPENED), dlg.FileName));
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
				outputFiles_[i] = dlg.FileName;
			}

			return true;
		}
	}
}