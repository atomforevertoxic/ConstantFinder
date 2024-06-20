#include "pch.h"
#include "CppUnitTest.h"
#include "../ConstantFinder/Func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testgetFuncNamePosition
{
	TEST_CLASS(testgetFuncNamePosition)
	{
	public:

		TEST_METHOD(baseTest)
		{
			string str = "int getInt(int a){";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(4, 10);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(soloSymbolFuncName)
		{
			string str = "void a(){";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(5, 6);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(userReturnTypeFunc)
		{
			string str = "SubstrPos getPos()";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(10, 16);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(userTypeObjectInArgs)
		{
			string str = "string getStrByPos(SubstrPos pos, int y)";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(7, 18);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(assignDefaultValueForArg)
		{
			string str = "int returnValue(int y = 0)";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(4, 15);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(assignDefaultStrValueForArg)
		{
			string str = "string str(string stttr = \"str\")";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(7, 10);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(severalArgsInFunc)
		{
			string str = "int y1(int y, int x, int z)";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(4, 6);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(noArgsFunc)
		{
			string str = "void getVoid()";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(5, 12);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(redefiningStandartOperator)
		{
			string str = "bool operator < (const Cl1 &other)";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(14, 15);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(staticFunc)
		{
			string str = "static void print()";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(12, 17);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(compoundOperatorInsteadFuncDeclaration)
		{
			string str = "for (int i =0; i<5; i++)";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(24, 24);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(semicolonInStrConstFuncDeclarArg)
		{
			string str = "void mainT(string S = \";\")";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(5, 10);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(funcDeclarInStrConst)
		{
			string str = "string str = \"void getP()\";";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(19, 23);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(extraSpacesBetweenReturnTypeAndFuncName)
		{
			string str = "void      getSomething(int number)";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(10, 22);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(extraSpacesBetweenFuncNameAndArgs)
		{
			string str = "int returnThat      (std::string strToRet)";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(4, 14);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(funcCallInsteadFuncDeclaration)
		{
			string str = "print();";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(8, 8);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(funcCallAndSpacesBetweenSemicolonAndCloseBracket)
		{
			string str = "getAndSet()    \t  ;";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(19, 19);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(noFuncDeclarInRow)
		{
			string str = "int number = 0;";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(15, 15);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(funcCallLikeDefaultValueInArgs)
		{
			string str = "int checkStr(int n = pick(1,2,6))";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(4, 12);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(funcDeclarLikeDefaultValueStringInArgs)
		{
			string str = "int getStr(string str = \"int g()\")";
			SubstrPos act = getFuncNamePosition(str, 0);
			SubstrPos exp(4, 10);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
	};
}
