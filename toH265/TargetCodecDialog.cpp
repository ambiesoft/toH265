#include "stdafx.h"

#include "toH265.h"

#include "TargetCodecDialog.h"

namespace Ambiesoft {
	namespace toH265 {
		TargetCodecDialog::TargetCodecDialog(bool bLosslessable)
		{
			InitializeComponent();

			if (bLosslessable)
			{
				cmbEncodeType->Items->Add(I18N("Lossless concat"));
				cmbEncodeType->Items->Add(I18N("Reencode"));
				cmbEncodeType->SelectedIndex = 1;
			}
			else
			{
				cmbEncodeType->Enabled = false;
			}
		}

		System::Void TargetCodecDialog::CmbEncodeType_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
		{
			switch (cmbEncodeType->SelectedIndex)
			{
			case 0:
				// lossless
				rbCopyAudio->Checked = true;
				rbCopyVideo->Checked = true;
				groupAudioCodec->Enabled = false;
				groupVideoCodec->Enabled = false;
				return;
			}
			groupAudioCodec->Enabled = true;
			groupVideoCodec->Enabled = true;
		}
		System::Void TargetCodecDialog::BtnOK_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (!cmbEncodeType->Enabled)
				return;

			if (cmbEncodeType->SelectedIndex != 0)
			{
				// if not lossless, any selection is OK
				return;
			}

			// Here below is lossless
			if (rbCopyAudio->Checked && rbCopyVideo->Checked)
			{
				// OK
				return;
			}

			CppUtils::Alert(I18N("'Copy' must be selected for 'Lossless concat'."));
			btnOK->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		}
	}
}