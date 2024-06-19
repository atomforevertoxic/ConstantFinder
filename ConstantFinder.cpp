#include "Func.h"

int main()
{
    std::cout << "Hello World!\n";
}

list<string> getTextFromFile(const string pathAndName, ErrorInfo& error)
{
    //Открытие файлового потока
    ifstream file(pathAndName);

    list<string> textCode;

    //Если файл не получилоось открыть 
    if (!file.is_open())
    {
        //Выдать соответствующее сообщение об ошибке
        error = ErrorInfo(-1, ErrorMessage::INVALID_FILENAME_OR_FILEPATH_OR_FILE_DOES_NOT_EXIST);
    }

    string line;
    bool wasStrErased = false;
    int lineCounter = 0;
    //Пока удается получить строку
    while (getline(file, line))
    {
        lineCounter++;

        //Если превышен лимит строк
        if (lineCounter > 300)
        {
            //Очистить список строк
            textCode.clear();
            //Выдать соответствующее сообщение об ошибке
            error = ErrorInfo(-1, ErrorMessage::LINES_COUNT_IS_OUT_OF_LIMIT);
            break;
        }
        //Если строка еще не была обрезана
        if (!wasStrErased)
        {
            //Вырезаются первые 3 "мусорных" символа из первой строки текста
            line.erase(0, 3);
            //Устанавливается флаг совершенного удаления
            wasStrErased = true;
        }
        //Добавление строки в конец списка
        textCode.push_back(line);
    }
    //Закрытие файлового потока
    file.close();

    return textCode;
}


vector<ErrorInfo> isInputDataValid(const list<string>& codeText)
{
    vector<ErrorInfo> vect;
    return vect;
}

bool removeAllCommentsFromString(string& strToExecute)
{
    SubstrPos commaPos = getLeftmostComment(strToExecute);
    while (commaPos.left != -1)
    {
        if (commaPos.right == -1)
        {
            commaPos.right = strToExecute.length() - 1;
            cleanSubstrBySpaces(strToExecute, commaPos);
            return true;
        }
        else
        {
            cleanSubstrBySpaces(strToExecute, commaPos);
            commaPos = getLeftmostComment(strToExecute);
        }
    }
    return false;
}

SubstrPos getLeftmostComment(string& strToResearch)
{
    SubstrPos leftmostCommaPos;
    int startSearchingForLeftmostComma = 0;
    while (isConstFindsEarlierThanComments(strToResearch, startSearchingForLeftmostComma))
    {
        int newStartPos = getSubstrPositionOrDefaultValue(strToResearch, "\"", startSearchingForLeftmostComma) + 2;
        startSearchingForLeftmostComma = newStartPos;
    }

    int singleLineCommentIndex = getSubstrPositionOrDefaultValue(strToResearch, "//", startSearchingForLeftmostComma);
    int multLineCommentIndex = getSubstrPositionOrDefaultValue(strToResearch, "/*", startSearchingForLeftmostComma);
    if (multLineCommentIndex < singleLineCommentIndex)
    {
        leftmostCommaPos.left = multLineCommentIndex;
        int multLineCommaEndIndex = getSubstrPositionOrDefaultValue(strToResearch, "*/", startSearchingForLeftmostComma);
        if (multLineCommaEndIndex == strToResearch.length())
        {
            leftmostCommaPos.right = -1;
        }
        else
        {
            leftmostCommaPos.right = multLineCommaEndIndex + 1;
        }
    }
    else if (singleLineCommentIndex < multLineCommentIndex)
    {
        leftmostCommaPos.left = singleLineCommentIndex;
        leftmostCommaPos.right = strToResearch.length() - 1;
    }
    return leftmostCommaPos;
}

void cleanSubstrBySpaces(string& strToClean, SubstrPos pos)
{
    for (int i = pos.left; i <= pos.right; i++)
    {
        strToClean[i] = ' ';
    }
}

bool isConstFindsEarlierThanComments(const string& strToSearch, int startSearching)
{
    int strConstIndex = getSubstrPositionOrDefaultValue(strToSearch, "\"", startSearching);
    int singleLineCommentIndex = getSubstrPositionOrDefaultValue(strToSearch, "//", startSearching);
    int multLinesCommentIndex = getSubstrPositionOrDefaultValue(strToSearch, "/*", startSearching);
    list<int> indexes = { strConstIndex, singleLineCommentIndex, multLinesCommentIndex };
    int minElem = *min_element(indexes.begin(), indexes.end());
    return minElem == strConstIndex && minElem != strToSearch.length();
}

int getSubstrPositionOrDefaultValue(const string& strToCheck, const string& substrToFind, int startSearching)
{
    int posOfSubstr = strToCheck.find(substrToFind, startSearching);
    posOfSubstr = posOfSubstr == -1 ? strToCheck.length() : posOfSubstr;
    return posOfSubstr;
}