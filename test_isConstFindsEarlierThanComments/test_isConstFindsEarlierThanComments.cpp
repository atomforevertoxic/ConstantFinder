#include "pch.h"
#include "CppUnitTest.h"
#include "../ConstantFinder/Func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testisConstFindsEarlierThanComments
{
	TEST_CLASS(testisConstFindsEarlierThanComments)
	{
	public:
		TEST_METHOD(baseTest)
		{
			string strToResearch = "int y = 0; // y";
			int startSearching = 0;
			bool result = isConstFindsEarlierThanComments(strToResearch, startSearching);
			bool expected = false;
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(commentedStrConst)
		{
			string strToResearch = "int y = 0; // \"y - not const\"";
			int startSearching = 4;
			bool result = isConstFindsEarlierThanComments(strToResearch, startSearching);
			bool expected = false;
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(strConstEarlierThanComments)
		{
			string strToResearch = "string str = \"const\";//comment";
			int startSearching = 2;
			bool result = isConstFindsEarlierThanComments(strToResearch, startSearching);
			bool expected = true;
			Assert::AreEqual(expected, result);
		}

		TEST_METHOD(strConstBetweenTwoComments)
		{
			string strToResearch = "/*com*/string s = \"s\"/*moment";
			int startSearching = 0;
			bool result = isConstFindsEarlierThanComments(strToResearch, startSearching);
			bool expected = false;
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(multLineCommentsEarlier)
		{
			string strToResearch = "/*string str = \"str\"";
			int startSearching = 0;
			bool result = isConstFindsEarlierThanComments(strToResearch, startSearching);
			bool expected = false;
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(singleAndMultLineCommentInRow)
		{
			string strToResearch = "/*int y = 0; // y";
			int startSearching = 0;
			bool result = isConstFindsEarlierThanComments(strToResearch, startSearching);
			bool expected = false;
			Assert::AreEqual(expected, result);
		}
		TEST_METHOD(allObjectInTheRow)
		{
			string strToResearch = "/*string str = \"row\"; // y";
			int startSearching = 0;
			bool result = isConstFindsEarlierThanComments(strToResearch, startSearching);
			bool expected = false;
			Assert::AreEqual(expected, result);
		}
	};
}
