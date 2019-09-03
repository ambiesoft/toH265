#include "stdafx.h"

#include "toH265.h"

#include "TargetCodecDialog.h"

namespace Ambiesoft {
	namespace toH265 {
		TargetCodecDialog::TargetCodecDialog(bool bLosslessable)
		{
			InitializeComponent();

			cmbEncodeType->Items->Add(I18N("Lossless concat"));
			cmbEncodeType->Items->Add(I18N("Reencode"));

			if (bLosslessable)
			{
				cmbEncodeType->SelectedIndex = 1;
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
		System::Void TargetCodecDialog::BtnOK_Click(System::Object^ sender, System::EventArgs^ e)
		{
			//if (!cmbEncodeType->Enabled)
			//	return;

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
				if (!rbVideoCopy->Checked && !rbVideoH265->Checked && !rbVideoVp9->Checked)
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