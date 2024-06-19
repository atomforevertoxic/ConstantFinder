#include "pch.h"
#include "CppUnitTest.h"
#include "../ConstantFinder/Func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testgetLeftmostComment
{
	TEST_CLASS(testgetLeftmostComment)
	{
	public:

		void substrPosAreEqual(SubstrPos expected, SubstrPos result)
		{
			Assert::AreEqual(expected.left, result.left);
			Assert::AreEqual(expected.right, result.right);
		}
		TEST_METHOD(baseTest)
		{
			string strToResearch = "int y = 0; //comment";
			SubstrPos expected(11, strToResearch.length() - 1);
			SubstrPos result = getLeftmostComment(strToResearch);
			substrPosAreEqual(expected, result);
		}
		TEST_METHOD(nestedSingleLineComments)
		{
			string strToResearch = "int y = 0; //comment//nested comment";
			SubstrPos expected(11, strToResearch.length() - 1);
			SubstrPos result = getLeftmostComment(strToResearch);
			substrPosAreEqual(expected, result);
		}
		TEST_METHOD(nestedMultLineComments)
		{
			string strToResearch = "int y; /*comment/*nested mult line comment";
			SubstrPos expected(7, -1);
			SubstrPos result = getLeftmostComment(strToResearch);
			substrPosAreEqual(expected, result);
		}
		TEST_METHOD(nestedDifferentTypeComments)
		{
			string strToResearch = "string str; /*comment //nested single line comment";
			SubstrPos expected(12, -1);
			SubstrPos result = getLeftmostComment(strToResearch);
			substrPosAreEqual(expected, result);
		}
		TEST_METHOD(multLineCommentEndsOnCurrentLine)
		{
			string strToResearch = "/*multline com*/int y = 0;";
			SubstrPos expected(0, 15);
			SubstrPos result = getLeftmostComment(strToResearch);
			substrPosAreEqual(expected, result);
		}
		TEST_METHOD(soloMultLineComment)
		{
			string strToResearch = "/*comment";
			SubstrPos expected(0, -1);
			SubstrPos result = getLeftmostComment(strToResearch);
			substrPosAreEqual(expected, result);
		}
		TEST_METHOD(commentInStrConst)
		{
			string strToResearch = "string str = \"/.comment\";";
			SubstrPos expected(-1, -1);
			SubstrPos result = getLeftmostComment(strToResearch);
			substrPosAreEqual(expected, result);
		}
		TEST_METHOD(constantAfterMultLineCommentEnd)
		{
			string strToResearch = "/*com*/string str=\"after comma\";";
			SubstrPos expected(0, 6);
			SubstrPos result = getLeftmostComment(strToResearch);
			substrPosAreEqual(expected, result);
		}
		TEST_METHOD(noCommentsString)
		{
			string strToResearch = "int y = 0;";
			SubstrPos expected(-1, -1);
			SubstrPos result = getLeftmostComment(strToResearch);
			substrPosAreEqual(expected, result);
		}
	};
}
