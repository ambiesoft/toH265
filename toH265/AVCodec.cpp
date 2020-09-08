#include "stdafx.h"
#include "AVCodec.h"


namespace Ambiesoft {
	namespace toH265 {
		using namespace System;

		AVCodec::AVCodec(String^ codec)
		{
			if (String::IsNullOrEmpty(codec))
			{
				vc_ = VC::VC_NONE;
				return;
			}

			codec = codec->ToLower();
			if (codec == "copy")
				vc_ = VC::VC_COPY;
			else if (codec == "h264")
				vc_ = VC::VC_H264;
			else if (codec == "h265" || codec == "hevc")
				vc_ = VC::VC_H265;
			else if (codec == "opus")
				vc_ = VC::VC_OPUS;
			else if (codec == "vorbis")
				vc_ = VC::VC_VORBIS;
			else if (codec == "vp8")
				vc_ = VC::VC_VP8;
			else if (codec == "vp9")
				vc_ = VC::VC_VP9;
			else if (codec == "aac")
				vc_ = VC::VC_AAC;
			else if (codec == "av1")
				vc_ = VC::VC_AV1;
			else if (codec->StartsWith("wmav"))
				vc_ = VC::VC_WMAV;
			else if (codec->StartsWith("wmv"))
				vc_ = VC::VC_WMV;
			else if (codec == "cook")
				vc_ = VC::VC_COOK;
			else if (codec == "rv30")
				vc_ = VC::VC_RV30;
			else
			{
				DASSERT(false);
				vc_ = VC::VC_UNKNOWN;
				unknownString_ = codec;
			}
		}
		
		String^ AVCodec::ToString() 
		{
			switch (vc_)
			{
			case VC::VC_NONE:return String::Empty;
			case VC::VC_UNKNOWN:return unknownString_;
			case VC::VC_AAC:return "aac";
			case VC::VC_COPY:return "copy";
			case VC::VC_H264:return "h264";
			case VC::VC_H265:return "h265";
			case VC::VC_OPUS:return "opus";
			case VC::VC_VORBIS:return "vorbis";
			case VC::VC_VP8:return "vp8";
			case VC::VC_VP9:return "vp9";
			case VC::VC_AV1:return "av1";
			case VC::VC_WMV:return "wmv";
			case VC::VC_WMAV:return "wmav";
			case VC::VC_COOK:return "cook";
			case VC::VC_RV30:return "rv30";
			case VC::VC_MIXED:return "mixed";
			default:
				DASSERT(false);
			}
			return String::Empty;
		}
		String^ AVCodec::ToFFMpegString() {
			switch (vc_)
			{
			case VC::VC_NONE:return String::Empty;
			case VC::VC_UNKNOWN:return unknownString_;
			case VC::VC_AAC:return "aac";
			case VC::VC_COPY:return "copy";
			case VC::VC_H264:return "h264";
			case VC::VC_H265:return "libx265";
			case VC::VC_OPUS:return "libopus";
			case VC::VC_VORBIS:return "libvorbis";
			case VC::VC_VP8:return "vp8";
			case VC::VC_VP9:return "vp9";
			case VC::VC_AV1:return "av1 -strict -2";
			default:
				DASSERT(false);
			}
			return String::Empty;
		}
		void AVCodec::Merge(AVCodec^ that)
		{
			unknownString_ = String::Empty;
			if (vc_ == VC::VC_NONE)
			{
				vc_ = that->vc_;
				unknownString_ = that->unknownString_;
				return;
			}

			if (this->vc_ == that->vc_)
				return;

			this->vc_ = VC::VC_MIXED;
			unknownString_ = String::Empty;
		}
	}
}