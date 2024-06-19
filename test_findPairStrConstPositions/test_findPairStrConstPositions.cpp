#include "pch.h"
#include "CppUnitTest.h"
#include "../ConstantFinder/Func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testfindPairStrConstPositions
{
	TEST_CLASS(testfindPairStrConstPositions)
	{
	public:
		TEST_METHOD(baseTest)
		{
			const string testString = "string str = \"str\";";
			SubstrPos act = findPairStrConstPositions(testString, 0);
			SubstrPos exp = SubstrPos(14, 17);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(someStrConstInOneRow)
		{
			const string testString = "string str = \"str1\" + \"str2\"";
			SubstrPos act = findPairStrConstPositions(testString, 0);
			SubstrPos exp = SubstrPos(14, 18);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(screenedMarkInStrConst)
		{
			const string testString = "string strSpec = \"str\\\"strstr\";";
			SubstrPos act = findPairStrConstPositions(testString, 0);
			SubstrPos exp = SubstrPos(18, 29);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(someScreenedMarksInStrConst)
		{
			const string testString = "string str1 = \"str\\\"strs\\\"tr\";";
			SubstrPos act = findPairStrConstPositions(testString, 0);
			SubstrPos exp = SubstrPos(15, 28);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(otherScreenedSymbolsInStrConst)
		{
			const string testString = "string s = \"str\\\\st\\\trstr\";";
			SubstrPos act = findPairStrConstPositions(testString, 0);
			SubstrPos exp = SubstrPos(12, 25);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(screenedMarkAfterScreenedBackslash)
		{
			const string testString = "string str1 = \"str\\\\\\\"strs\\\\\\\"tr\";";
			SubstrPos act = findPairStrConstPositions(testString, 0);
			SubstrPos exp = SubstrPos(15, 32);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(multipleScreenedBackSlashBeforeMark)
		{
			const string testString = "string str1 = \"str\\\\\\\\\";";
			SubstrPos act = findPairStrConstPositions(testString, 0);
			SubstrPos exp = SubstrPos(15, 22);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(noStrConstInRow)
		{
			const string testString = "int a = 0;";
			SubstrPos act = findPairStrConstPositions(testString, 0);
			SubstrPos exp = SubstrPos(11, 10);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(strConstInFuncCalling)
		{
			const string testString = "getStr(\"solution\")";
			SubstrPos act = findPairStrConstPositions(testString, 0);
			SubstrPos exp = SubstrPos(8, 16);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(strConstInCompoundOperator)
		{
			const string testString = "for (string str = \"C\";;++str)";
			SubstrPos act = findPairStrConstPositions(testString, 0);
			SubstrPos exp = SubstrPos(19, 20);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
	};
}
