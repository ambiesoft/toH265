#include "stdafx.h"

#include "toH265.h"

#include "TargetCodecDialog.h"

namespace Ambiesoft {
	namespace toH265 {
		TargetCodecDialog::TargetCodecDialog(bool bLosslessable, String^ iniPath, String^ section):
			losslessable_(bLosslessable), iniPath_(iniPath), section_(section)
		{
			InitializeComponent();

			cmbEncodeType->Items->Add(I18N("Lossless concat"));
			cmbEncodeType->Items->Add(I18N("Reencode"));
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
			Profile::WriteInt(SECTION, KEY_AUIDOCODEC, AudioCodecInt, ini);
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
		}
	}
}