#include "pch.h"
#include "CppUnitTest.h"
#include "../ConstantFinder/Func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testisInputDataValid
{
	TEST_CLASS(testisInputDataValid)
	{
	public:
		void checkAnswerVector(vector<ErrorInfo> expErrors, vector<ErrorInfo> actErrors)
		{
			Assert::AreEqual(expErrors.size(), actErrors.size());
			vector<ErrorInfo>::iterator expIterator;
			vector<ErrorInfo>::iterator actIterator;
			for (expIterator = expErrors.begin(), actIterator = actErrors.begin(); expIterator != expErrors.end() && actIterator != actErrors.end(); ++expIterator, ++actIterator)
			{
				Assert::AreEqual(expIterator->Line, actIterator->Line);
				Assert::AreEqual(expIterator->Message == actIterator->Message, true);
			}
		}
		TEST_METHOD(baseTest)
		{
			list<string> Text = {
				"int main() {",
				"std::string str=\"str\";",
				"",
				"}"
			};
			vector<ErrorInfo> errors = isInputDataValid(Text);
			vector<ErrorInfo> expectedErrors = {};
			checkAnswerVector(expectedErrors, errors);
		}

		TEST_METHOD(templatePresenceError)
		{
			list<string> Text = {
				"template<typname T> T solve();",
				"void main() {",
				"\tsolve()",
				"}",
				"tempate<typename T> T solve()",
				"{"
				"\t return 1;",
				"}"
			};
			vector<ErrorInfo> errors = isInputDataValid(Text);
			vector<ErrorInfo> expectedErrors = { ErrorInfo(1, ErrorMessage::TEMPLATE_IS_NOT_ALLOWED_IN_TEXT_CODE) };
			checkAnswerVector(expectedErrors, errors);
		}
		TEST_METHOD(definePresenceError)
		{
			list<string> Text = {
				"#define MAX 80",
				"",
				"int main() {",
				"std::string str=\"str\";",
				"\r return 0;",
				"}"
			};
			vector<ErrorInfo> errors = isInputDataValid(Text);
			vector<ErrorInfo> expectedErrors = { ErrorInfo(1, ErrorMessage::THE_DEFINE_DIRECTIVE_IS_NOT_ALLOWED_IN_TEXT_CODE) };
			checkAnswerVector(expectedErrors, errors);
		}
		TEST_METHOD(typedefPresenceError)
		{
			list<string> Text = {
				"int main() {",
				"typedef std::vector<int> Vint;",
				"}"
			};
			vector<ErrorInfo> errors = isInputDataValid(Text);
			vector<ErrorInfo> expectedErrors = { ErrorInfo(2, ErrorMessage::IT_IS_NOT_ALLOWED_TO_DECLARE_ALIASES_USING_A_TYPEDEF) };
			checkAnswerVector(expectedErrors, errors);
		}
		TEST_METHOD(severalErrorsInText)
		{
			list<string> Text = {
				"#define MAX 80",
				"void main()",
				"{",
				"\ttypedef std::vector<int> Vint;",
				"}"
			};
			vector<ErrorInfo> errors = isInputDataValid(Text);
			vector<ErrorInfo> expectedErrors = { ErrorInfo(1, ErrorMessage::THE_DEFINE_DIRECTIVE_IS_NOT_ALLOWED_IN_TEXT_CODE), ErrorInfo(4, ErrorMessage::IT_IS_NOT_ALLOWED_TO_DECLARE_ALIASES_USING_A_TYPEDEF) };
			checkAnswerVector(expectedErrors, errors);
		}
		TEST_METHOD(variableNameLooksLikeErrorKeyObject)
		{
			list<string> Text = {
				"void main() {",
				"int templateValue = 32;",
				"}"
			};
			vector<ErrorInfo> errors = isInputDataValid(Text);
			vector<ErrorInfo> expectedErrors = {};
			checkAnswerVector(expectedErrors, errors);
		}
		TEST_METHOD(defineDeclarInComment)
		{
			list<string> Text = {
				"//#define Min 3",
				"void main() {",
				"int y = 3;",
				"}"
			};
			vector<ErrorInfo> errors = isInputDataValid(Text);
			vector<ErrorInfo> expectedErrors = {};
			checkAnswerVector(expectedErrors, errors);
		}
		TEST_METHOD(typedefDeclarInStrConst)
		{
			list<string> Text = {
				"void main() {",
				"std::string str = \"template<typename T>T solution();\";",
				"}"
			};
			vector<ErrorInfo> errors = isInputDataValid(Text);
			vector<ErrorInfo> expectedErrors = {};
			checkAnswerVector(expectedErrors, errors);
		}

		TEST_METHOD(multlineCommentedDefine)
		{
			list<string> Text = {
				"/*start comment",
				"#define Min 3*/",
				"void main() {",
				"int y = 3;",
				"}"
			};
			vector<ErrorInfo> errors = isInputDataValid(Text);
			vector<ErrorInfo> expectedErrors = {};
			checkAnswerVector(expectedErrors, errors);
		}
	};
}
