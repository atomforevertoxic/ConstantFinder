#include "pch.h"
#include "CppUnitTest.h"
#include "../ConstantFinder/Func.h"
#include <set>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testfindAllConstantsAndTheirLocation
{
	TEST_CLASS(testfindAllConstantsAndTheirLocation)
	{
	public:
		void areMultisetsEqual(multiset<Constant> exp, multiset<Constant> act)
		{
			Assert::AreEqual(exp.size(), act.size());
			multiset<Constant>::iterator expIterator;
			multiset<Constant>::iterator actIterator;
			for (expIterator = exp.begin(), actIterator = act.begin(); expIterator != exp.end() && actIterator != act.end(); ++expIterator, ++actIterator)
			{
				Assert::AreEqual(expIterator->constantLocation, actIterator->constantLocation);
				Assert::AreEqual(expIterator->constLine, actIterator->constLine);
				Assert::AreEqual(expIterator->constString, actIterator->constString);
			}
		}
		TEST_METHOD(baseTest)
		{
			list<string> codeText = {
				"using namespace std;",
				"void main() {",
				"\tstd::string str = \"constant\";",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("global - main:", 3, "constant") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(globalConstantInCode)
		{
			list<string> codeText = {
				"using namespace std;",
				"std::string str = \"globalConstant\";",
				"void main() {",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("global:", 2, "globalConstant") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(strConstIsClassVariable)
		{
			list<string> codeText = {
				"using namespace std;",
				"class Cl1 {std::string c = \"C\";}",
				"void main() {",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("Cl1:", 2, "C") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(strConstIsNestedClassVariable)
		{
			list<string> codeText = {
				"using namespace std;",
				"class Cl1 {",
				"class cl2{std::string clStr = \"CL2\";",
				"}",
				"}",
				"void main() {",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("Cl1 - cl2:", 3, "CL2") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(strConstIsStructVariable)
		{
			list<string> codeText = {
				"using namespace std;",
				"struct s1 {std::string strC = \"S\";",
				"void main() {",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("s1:", 2, "S") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(strConstIsUserNamespaceGlobalVariable)
		{
			list<string> codeText = {
				"using namespace std;",
				"namespace user {",
				"\tstd::string str = \"user\";",
				"\tvoid main() {",
				"}",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("user:", 3, "user") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(strConstIsUnionVariable)
		{
			list<string> codeText = {
				"using namespace std;",
				"union u2{std::string uniC = \"un\";}",
				"void main() {",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("u2:", 2, "un") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(strConstIsInClassFunction)
		{
			list<string> codeText = {
				"using namespace std;",
				"class Cl1 {",
				"\tvoid cl1Func() {",
				"\t\tstd::string funcConst=\"func\";",
				"}",
				"void main() {",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("Cl1 - cl1Func:", 4, "func") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(severalStrConstsInOneString)
		{
			list<string> codeText = {
				"using namespace std;",
				"void main() {",
				"     std::string str = \"first\"+\"second\";",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("global - main:", 3, "first"), Constant("global - main:", 3, "second") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(strConstInCompoundOperator)
		{
			list<string> codeText = {
				"using namespace std;",
				"void main() {",
				"\tfor (std::string str=\"forA\";;str++)",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("global - main:", 3, "forA") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(setStrConstToDefaultFuncArg)
		{
			list<string> codeText = {
				"void set(std::string s=\"Solve\");",
				"using namespace std;",
				"void main() {",
				"}",
				"void set(std::string s=\"Solve\")",
				"{",
				"\ts=\"notSolve\";",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("global:", 1, "Solve"), Constant("global - set:", 5, "Solve"), Constant("global - set:", 7, "notSolve") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(strConstPassedToArgWhenFuncIsCalled)
		{
			list<string> codeText = {
				"void get(std::string s);",
				"using namespace std;",
				"void main() {",
				"\tget(\"Solve\");",
				"}",
				"void get(std::string s)",
				"{",
				"\t s = \"notSolve\";",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("global - main:", 4, "Solve"), Constant("global - get:", 8, "notSolve") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(duplStrConst)
		{
			list<string> codeText = {
				"using namespace std;",
				"void main() {",
				"\tstd::string str = \"const\"+\"const\";",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("global - main:", 3, "const"), Constant("global - main:", 3, "const") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(strConstContsinatsScreenedSymbols)
		{
			list<string> codeText = {
				"using namespace std;",
				"void main() {",
				"\tstd::string str = \"cons\\tant\";",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { Constant("global - main:", 3, "cons\\tant") };
			areMultisetsEqual(expAnswer, answer);
		}

		TEST_METHOD(noStrConstInCode)
		{
			list<string> codeText = {
				"using namespace std;",
				"void main() {",
				"}"
			};
			multiset<Constant> answer = findAllConstantsAndTheirLocation(codeText);
			multiset<Constant> expAnswer = { };
			areMultisetsEqual(expAnswer, answer);
		}
	};
}
