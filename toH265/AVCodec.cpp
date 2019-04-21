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
			else if (codec == "h265")
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
			default:
				DASSERT(false);
			}
			return String::Empty;
		}

	}
}