#include "pch.h"
#include "CppUnitTest.h"
#include "../ConstantFinder/Func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testgetDeclarNamePosition
{
	TEST_CLASS(testgetDeclarNamePosition)
	{
	public:

		TEST_METHOD(baseTest)
		{
			string str = "class Class1";
			SubstrPos act = getDeclarNamePosition(str, "class", 0);
			SubstrPos exp(6, 12);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(structDeclaration)
		{
			string str = "struct Str1";
			SubstrPos act = getDeclarNamePosition(str, "struct", 0);
			SubstrPos exp(7, 11);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(unionDeclaration)
		{
			string str = "union Un1";
			SubstrPos act = getDeclarNamePosition(str, "union", 0);
			SubstrPos exp(6, 9);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(namespaceDeclaration)
		{
			string str = "namespace NM1";
			SubstrPos act = getDeclarNamePosition(str, "namespace", 0);
			SubstrPos exp(10, 13);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(extraSpacesBetweenDeclAndName)
		{
			string str = "class        Cl1";
			SubstrPos act = getDeclarNamePosition(str, "class", 0);
			SubstrPos exp(13, 16);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(extraSpacesAfterName)
		{
			string str = "struct Str2    ";
			SubstrPos act = getDeclarNamePosition(str, "struct", 0);
			SubstrPos exp(7, 11);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(severalDeclarationInOneRow)
		{
			string str = "class cl1{ class cl2 {";
			SubstrPos act = getDeclarNamePosition(str, "class", 0);
			SubstrPos exp(6, 9);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(keyWordIsPartOfOtherVariable)
		{
			string str = "int soloclass = 0;";
			SubstrPos act = getDeclarNamePosition(str, "class", 0);
			SubstrPos exp(18, 18);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(noDeclarationInRow)
		{
			string str = "int y = 0;";
			SubstrPos act = getDeclarNamePosition(str, "class", 0);
			SubstrPos exp(10, 10);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
		TEST_METHOD(tabSignBeforeKeyWord)
		{
			string str = "\tclass Cl1";
			SubstrPos act = getDeclarNamePosition(str, "class", 0);
			SubstrPos exp(7, 10);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(tabSignBetweenKeyWordAndDeclarName)
		{
			string str = "class\tCl1";
			SubstrPos act = getDeclarNamePosition(str, "class", 0);
			SubstrPos exp(6, 9);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(classDeclarInStrConst)
		{
			string str = "string str = \"class\tCl1\";";
			SubstrPos act = getDeclarNamePosition(str, "class", 0);
			SubstrPos exp(25, 25);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(classDeclarationAfterStrConst)
		{
			string str = "string str = \"\";class Cl4 {";
			SubstrPos act = getDeclarNamePosition(str, "class", 0);
			SubstrPos exp(22, 25);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}

		TEST_METHOD(structDeclarAfterSpecSymbol)
		{
			string str = "}struct Str33";
			SubstrPos act = getDeclarNamePosition(str, "struct", 0);
			SubstrPos exp(8, 13);
			Assert::AreEqual(exp.left, act.left);
			Assert::AreEqual(exp.right, act.right);
		}
	};
}
