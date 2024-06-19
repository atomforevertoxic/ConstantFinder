#include "pch.h"
#include "CppUnitTest.h"
#include "../ConstantFinder/Func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testremoveAllCommentsFromString
{
	TEST_CLASS(testremoveAllCommentsFromString)
	{
	public:
		TEST_METHOD(baseTest)
		{
			string strToExecute = "int y = 0; // y = 0";
			bool nextStrCommented = removeAllCommentsFromString(strToExecute);
			string expectedAnswer = "int y = 0;         ";
			Assert::AreEqual(false, nextStrCommented);
			Assert::AreEqual(expectedAnswer, strToExecute);
		}
		TEST_METHOD(multLineNotEndedComment)
		{
			string strToExecute = "/* int x = 9;";
			bool nextStrCommented = removeAllCommentsFromString(strToExecute);
			string expectedAnswer = "             ";
			Assert::AreEqual(true, nextStrCommented);
			Assert::AreEqual(expectedAnswer, strToExecute);
		}
		TEST_METHOD(multLineEndedComment)
		{
			string strToExecute = "/* comment */int x = 9;";
			bool nextStrCommented = removeAllCommentsFromString(strToExecute);
			string expectedAnswer = "             int x = 9;";
			Assert::AreEqual(false, nextStrCommented);
			Assert::AreEqual(expectedAnswer, strToExecute);
		}

		TEST_METHOD(someNestedSingleComments)
		{
			string strToExecute = "string s = \"str\";//first//second";
			bool nextStrCommented = removeAllCommentsFromString(strToExecute);
			string expectedAnswer = "string s = \"str\";               ";
			Assert::AreEqual(false, nextStrCommented);
			Assert::AreEqual(expectedAnswer, strToExecute);
		}

		TEST_METHOD(someNestedMultLineEndlessComments)
		{
			string strToExecute = "/* first mult comma /* second";
			bool nextStrCommented = removeAllCommentsFromString(strToExecute);
			string expectedAnswer = "                             ";
			Assert::AreEqual(true, nextStrCommented);
			Assert::AreEqual(expectedAnswer, strToExecute);
		}

		TEST_METHOD(someNestedMultLineCommentsWhereBiggerCommentEnds)
		{
			string strToExecute = "/* first mult comma /* second */y=0;";
			bool nextStrCommented = removeAllCommentsFromString(strToExecute);
			string expectedAnswer = "                                y=0;";
			Assert::AreEqual(false, nextStrCommented);
			Assert::AreEqual(expectedAnswer, strToExecute);
		}

		TEST_METHOD(nestedSingleAndMultLineComments)
		{
			string strToExecute = "string str =\"...\";//comma/*anothercomma";
			bool nextStrCommented = removeAllCommentsFromString(strToExecute);
			string expectedAnswer = "string str =\"...\";                     ";
			Assert::AreEqual(false, nextStrCommented);
			Assert::AreEqual(expectedAnswer, strToExecute);
		}

		TEST_METHOD(nestedMultAndSingleLineComments)
		{
			string strToExecute = "int notStr = 0;/* comment//ddddddddd";
			bool nextStrCommented = removeAllCommentsFromString(strToExecute);
			string expectedAnswer = "int notStr = 0;                     ";
			Assert::AreEqual(true, nextStrCommented);
			Assert::AreEqual(expectedAnswer, strToExecute);
		}
		TEST_METHOD(someNotNestedComments)
		{
			string strToExecute = "/*comment*/int num = 0;//second";
			bool nextStrCommented = removeAllCommentsFromString(strToExecute);
			string expectedAnswer = "           int num = 0;        ";
			Assert::AreEqual(false, nextStrCommented);
			Assert::AreEqual(expectedAnswer, strToExecute);
		}
		TEST_METHOD(someNotNestedMultLineComments)
		{
			string strToExecute = "double dd = 9;/*comma*//*anothercomma";
			bool nextStrCommented = removeAllCommentsFromString(strToExecute);
			string expectedAnswer = "double dd = 9;                       ";
			Assert::AreEqual(true, nextStrCommented);
			Assert::AreEqual(expectedAnswer, strToExecute);
		}
	};
}
