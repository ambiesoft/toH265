#pragma once

#include "EncodeJob.h"
#include "EncodeTask.h"


namespace Ambiesoft {
	namespace toH265 {

		using namespace System;

		ref class EncodeJob;

		ref class Paper
		{
			bool hasWarning_;
			String^ text_;
			EncodeJob^ job_;

		public:
			Paper(bool hasWaring, String^ text, EncodeJob^ job) :
				hasWarning_(hasWaring),
				text_(text),
				job_(job) {}
			property String^ OutputtedMovie
			{
				String^ get() { return job_->OutputtedMovie; }
			}
			property bool IsWarning
			{
				bool get() { return hasWarning_; }
			}
			property String^ Message
			{
				String^ get() { return (IsWarning ? I18N(L"WARNING_EMOJI") : I18N(L"OK_EMOJI")) + L"\r\n"+  text_; }
			}
		};

		// Plan (pseudocode):
		// 1. Mark the class with DataContract so DataContractJsonSerializer can serialize/deserialize.
		// 2. Expose `Text` and `IsWarning` as properties decorated with DataMember and provide setters so deserialization works.
		// 3. Add a parameterless constructor required by the serializer and keep the existing convenience constructor.
		// 4. Implement `ToJsonString()` using System::Runtime::Serialization::Json::DataContractJsonSerializer:
		//    - create serializer for SummaryText::typeid
		//    - write object to MemoryStream
		//    - convert MemoryStream->ToArray() to UTF8 string and return
		// 5. Implement `SaveToFile(String^ path)`:
		//    - call `ToJsonString()` and write to file with UTF8 encoding
		// 6. Implement static `FromJsonString(String^ json)`:
		//    - get UTF8 bytes from string
		//    - read object from MemoryStream using the same serializer and return casted SummaryText^
		// 7. Implement static `LoadFromFile(String^ path)`:
		//    - read file content as UTF8 string and call `FromJsonString`
		// 8. Use fully-qualified type names to avoid adding new using directives.

		[System::Runtime::Serialization::DataContractAttribute]
		public ref class SummaryText
		{
		private:
			String^ text_;
			bool isWarning_;
		public:
			SummaryText() : text_(String::Empty), isWarning_(false) {}
			SummaryText(String^ text, bool isWarning) :
				text_(text),
				isWarning_(isWarning) {}

			[System::Runtime::Serialization::DataMemberAttribute]
			property String^ Text
			{
				String^ get() { return text_; }
				void set(String^ value) { text_ = value; }
			}

			[System::Runtime::Serialization::DataMemberAttribute]
			property bool IsWarning
			{
				bool get() { return isWarning_; }
				void set(bool value) { isWarning_ = value; }
			}

			// Serialize this instance to a JSON string (UTF-8)
			String^ ToJsonString()
			{
				auto serializer = gcnew System::Runtime::Serialization::Json::DataContractJsonSerializer(SummaryText::typeid);
				auto ms = gcnew System::IO::MemoryStream();
				serializer->WriteObject(ms, this);
				array<Byte>^ bytes = ms->ToArray();
				return System::Text::Encoding::UTF8->GetString(bytes);
			}

			// Save JSON to file using UTF-8 encoding
			void SaveToFile(String^ path)
			{
				System::IO::File::WriteAllText(path, ToJsonString(), System::Text::Encoding::UTF8);
			}

			// Deserialize from a JSON string
			static SummaryText^ FromJsonString(String^ json)
			{
				array<Byte>^ bytes = System::Text::Encoding::UTF8->GetBytes(json);
				auto ms = gcnew System::IO::MemoryStream(bytes);
				auto serializer = gcnew System::Runtime::Serialization::Json::DataContractJsonSerializer(SummaryText::typeid);
				return dynamic_cast<SummaryText^>(serializer->ReadObject(ms));
			}

			// Load JSON from file and deserialize
			static SummaryText^ LoadFromFile(String^ path)
			{
				String^ json = System::IO::File::ReadAllText(path, System::Text::Encoding::UTF8);
				return FromJsonString(json);
			}

			void Show(System::Windows::Forms::Form^ form);
		};
		public ref class Summary
		{
			String^ overallResult_ = String::Empty;
			System::Collections::Generic::List<Paper^> papers_;

		public:
			void AddOverallResult(EncodeTask^ encodeTask);
			void AddPaper(bool hasWarning, String^ text, EncodeJob^ job)
			{
				papers_.Add(gcnew Paper(hasWarning, text, job));
			}

			void Show(System::Windows::Forms::Form^ form);

			property String^ LastOutputMovie
			{
				String^ get() {
					if (papers_.Count == 0)
						return nullptr;
					for (int i = papers_.Count - 1; i >= 0; --i)
						if (!String::IsNullOrEmpty(papers_[i]->OutputtedMovie))
							return papers_[i]->OutputtedMovie;
					return nullptr;
				}
			}
			SummaryText^ GetSummaryText();
		};

	}
}