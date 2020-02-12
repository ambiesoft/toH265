#pragma once

namespace Ambiesoft {
	namespace toH265 {
		using namespace System;

		public ref class AVCodec
		{
		public:
			enum class VC {
				VC_NONE,
				VC_UNKNOWN,
				VC_AAC,
				VC_COPY,
				VC_H264,
				VC_H265,
				VC_OPUS,
				VC_VORBIS,
				VC_VP8,
				VC_VP9,
				VC_AV1,
				VC_WMAV,
				VC_WMV,
				VC_COOK,
				VC_RV30,
				VC_MIXED,
			};
		private:
			String^ unknownString_;
			VC vc_;
		public:

			AVCodec(String^ codec);
			AVCodec(VC vc) : vc_(vc){}
			AVCodec():vc_(VC::VC_NONE){}

			//void Clear() {
			//	vc_ = VC::VC_NONE;
			//	unknownString_ = String::Empty;
			//}
			property bool IsEmpty
			{
				bool get() { return vc_ == VC::VC_NONE; }
			}
			property bool IsCopy
			{
				bool get() { return vc_ == VC::VC_COPY; }
			}
			property bool IsVp9
			{
				bool get() { return vc_ == VC::VC_VP9; }
			}
			property bool IsVp8
			{
				bool get() { return vc_ == VC::VC_VP8; }
			}
			property bool IsH265
			{
				bool get() { return vc_ == VC::VC_H265; }
			}
			property bool IsH264
			{
				bool get() { return vc_ == VC::VC_H264; }
			}
			property bool IsOpus
			{
				bool get() { return vc_ == VC::VC_OPUS; }
			}
			property bool IsVorbis
			{
				bool get() { return vc_ == VC::VC_VORBIS; }
			}
			property bool IsMixed
			{
				bool get() { return vc_ == VC::VC_MIXED; }
			}
			String^ ToString() override;
			String^ ToFFMpegString();

			void Merge(AVCodec^ that);
		};

	}
}