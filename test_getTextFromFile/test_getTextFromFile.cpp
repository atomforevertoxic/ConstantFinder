#include "pch.h"
#include "CppUnitTest.h"
#include "../ConstantFinder/Func.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testgetTextFromFile
{
	TEST_CLASS(testgetTextFromFile)
	{
	public:
		void checkFuncResult(list<string> expText, list<string> actText, ErrorInfo expError, ErrorInfo actError)
		{
			Assert::AreEqual((expText == actText), true);
			Assert::AreEqual(expError.Line, actError.Line);
			Assert::AreEqual(expError.Message == actError.Message, true);
		}
		TEST_METHOD(baseTest)
		{
			ErrorInfo error;
			list<string> Text = getTextFromFile("D:\\testGetTextFromFile_files\\example1.cpp", error);
			list<string> expectedText = {
				"#include <iostream>",
				"",
				"int main()",
				"{",
				"",
				"}"
			};
			ErrorInfo expError;
			checkFuncResult(expectedText, Text, expError, error);
		}
		TEST_METHOD(incorrectFilePath)
		{
			ErrorInfo error;
			list<string> Text = getTextFromFile("D:\\testGetTextFromFile_file\\example2.cpp", error);
			list<string> expectedText = {};
			ErrorInfo expError(-1, ErrorMessage::INVALID_FILENAME_OR_FILEPATH_OR_FILE_DOES_NOT_EXIST);
			Assert::AreEqual(((Text == expectedText)), true);
			checkFuncResult(expectedText, Text, expError, error);
		}
		TEST_METHOD(incorrectFilename)
		{
			ErrorInfo error;
			list<string> Text = getTextFromFile("D:\\testGetTextFromFile_files\\example33.cpp", error);
			list<string> expectedText = {};
			ErrorInfo expError(-1, ErrorMessage::INVALID_FILENAME_OR_FILEPATH_OR_FILE_DOES_NOT_EXIST);
			Assert::AreEqual(((Text == expectedText)), true);
			checkFuncResult(expectedText, Text, expError, error);
		}
		TEST_METHOD(emptyInputFile)
		{
			ErrorInfo error;
			list<string> Text = getTextFromFile("D:\\testGetTextFromFile_files\\example4.cpp", error);
			list<string> expectedText = { "" };
			ErrorInfo expError;
			checkFuncResult(expectedText, Text, expError, error);
		}
		TEST_METHOD(singleStringFile)
		{
			ErrorInfo error;
			list<string> Text = getTextFromFile("D:\\testGetTextFromFile_files\\example5.cpp", error);
			list<string> expectedText = {
				"int main(){ }"
			};
			ErrorInfo expError;
			checkFuncResult(expectedText, Text, expError, error);

		}

		TEST_METHOD(strLimitError)
		{
			ErrorInfo error;
			list<string> Text = getTextFromFile("D:\\testGetTextFromFile_files\\example6.cpp", error);
			list<string> expectedText = {};
			ErrorInfo expError(-1, ErrorMessage::LINES_COUNT_IS_OUT_OF_LIMIT);
			checkFuncResult(expectedText, Text, expError, error);

		}
	};
}
