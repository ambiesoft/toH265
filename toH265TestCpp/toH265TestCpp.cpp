#include "pch.h"
#include "CppUnitTest.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Ambiesoft;

namespace toH265TestCpp
{
	TEST_CLASS(toH265TestCpp)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::IsTrue(AmbLib::GetRatioString(821874467, 4017871354)->StartsWith("20"));
			Assert::IsTrue(AmbLib::GetRatioString((double)821874467, (double)4017871354)->StartsWith("20"));
			Assert::IsTrue(AmbLib::GetRatioString(821874467LL, 4017871354LL)->StartsWith("20"));
		}
	};
}
