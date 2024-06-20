#include "pch.h"
#include "CppUnitTest.h"
#include "../ConstantFinder/Func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testgetAllBracketsPos
{
	TEST_CLASS(testgetAllBracketsPos)
	{
	public:
		void arePosListsEqual(list<SubstrPos> expectedResult, list<SubstrPos> actuallResult)
		{
			Assert::AreEqual(expectedResult.size(), actuallResult.size());
			list<SubstrPos>::iterator expIt = expectedResult.begin();
			list<SubstrPos>::iterator actIt = actuallResult.begin();
			for (; expIt != expectedResult.end() && actIt != actuallResult.end(); ++expIt, ++actIt)
			{
				SubstrPos exp = *expIt;
				SubstrPos act = *actIt;
				Assert::AreEqual(exp.left, act.left);
				Assert::AreEqual(exp.right, act.right);
			}
		}
		TEST_METHOD(baseTest)
		{
			const string strToSearch = "(brackets)";
			list<SubstrPos> expectedResult = { SubstrPos(0, 9) };
			list<SubstrPos> actuallResult = getAllBracketsPos(strToSearch, 0);
			arePosListsEqual(expectedResult, actuallResult);
		}
		TEST_METHOD(someConsectivePairsOfBrackets)
		{
			const string strToSearch = "(brackets)()";
			list<SubstrPos> expectedResult = { SubstrPos(0,9), SubstrPos(10, 11) };
			list<SubstrPos> actuallResult = getAllBracketsPos(strToSearch, 0);
			arePosListsEqual(expectedResult, actuallResult);
		}

		TEST_METHOD(someNestedBrackets)
		{
			const string strToSearch = "(brac()kets)";
			list<SubstrPos> expectedResult = { SubstrPos(5,6), SubstrPos(0, 11) };
			list<SubstrPos> actuallResult = getAllBracketsPos(strToSearch, 0);
			arePosListsEqual(expectedResult, actuallResult);
		}
		TEST_METHOD(complexTest)
		{
			const string strToSearch = "(br()ckets())";
			list<SubstrPos> expectedResult = { SubstrPos(3, 4), SubstrPos(10, 11), SubstrPos(0, 12) };
			list<SubstrPos> actuallResult = getAllBracketsPos(strToSearch, 0);
			arePosListsEqual(expectedResult, actuallResult);
		}
		TEST_METHOD(noBrackets)
		{
			const string strToSearch = "no brackets";
			list<SubstrPos> expectedResult = { };
			list<SubstrPos> actuallResult = getAllBracketsPos(strToSearch, 0);
			Assert::AreEqual(expectedResult.size(), actuallResult.size());;
		}
	};
}
