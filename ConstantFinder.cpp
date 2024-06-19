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

    //Вернуть текст в виде списка строк
    return textCode;
}


vector<ErrorInfo> isInputDataValid(const list<string>& codeText)
{
    vector<ErrorInfo> vect;
    return vect;
}

bool removeAllCommentsFromString(string& strToExecute)
{
    //Найти позицию самого первого комментария
    SubstrPos commaPos = getLeftmostComment(strToExecute);
    //Пока позиции комментария - действительные
    while (commaPos.left != -1)
    {
        //Если позиции принадлежат многострочному комментарию, который не заканчивается на этой же строке
        if (commaPos.right == -1)
        {
            //Очистить строку от начала комментария и до конца строки
            commaPos.right = strToExecute.length() - 1;
            cleanSubstrBySpaces(strToExecute, commaPos);
            //Вернуть флаг комментированности следующих строк
            return true;
        }
        //Иначе
        else
        {
            //Очистить построку между полученными позициями комментария
            cleanSubstrBySpaces(strToExecute, commaPos);
            //Найти позиции нового самого первого комментария
            commaPos = getLeftmostComment(strToExecute);
        }
    }
    //Вернуть флаг комментированности следующих строк
    return false;
}

SubstrPos getLeftmostComment(string& strToResearch)
{
    SubstrPos leftmostCommaPos;
    int startSearchingForLeftmostComma = 0;
    //Пока индекс строковой константы меньше индексов комментариев
    while (isConstFindsEarlierThanComments(strToResearch, startSearchingForLeftmostComma))
    {
        //Начало поиска равна позиции конца строковой константы +1
        int newStartPos = findPairStrConstPositions(strToResearch, startSearchingForLeftmostComma).right+2;
        startSearchingForLeftmostComma = newStartPos;
    }

    int singleLineCommentIndex = getSubstrPositionOrDefaultValue(strToResearch, "//", startSearchingForLeftmostComma);
    int multLineCommentIndex = getSubstrPositionOrDefaultValue(strToResearch, "/*", startSearchingForLeftmostComma);

    //Если индекс многострочного комментария меньше однострочного комментария
    if (multLineCommentIndex < singleLineCommentIndex)
    {
        leftmostCommaPos.left = multLineCommentIndex;
    
        //Найти индекс конца многострочного комментария
        int multLineCommaEndIndex = getSubstrPositionOrDefaultValue(strToResearch, "*/", startSearchingForLeftmostComma);
        
        //Если конца многострочного комментария нет
        if (multLineCommaEndIndex == strToResearch.length())
        {
            //Установить правую границу комментария равную -1
            leftmostCommaPos.right = -1;
        }
        //Иначе
        else
        {
            //Установить найденную позицию конца многострочного комментария
            leftmostCommaPos.right = multLineCommaEndIndex + 1;
        }
    }
    //Иначе если индекс однострочного комментария меньше многострочного комментария
    else if (singleLineCommentIndex < multLineCommentIndex)
    {
        //Установить позиции однострочного комментария
        leftmostCommaPos.left = singleLineCommentIndex;
        leftmostCommaPos.right = strToResearch.length() - 1;
    }
    //Вернуть позиции комментария
    return leftmostCommaPos;
}

void cleanSubstrBySpaces(string& strToClean, SubstrPos pos)
{
    //Заменить на пробел все символы в строке от начальной до конечной позиций подстроки
    for (int i = pos.left; i <= pos.right; i++)
    {
        strToClean[i] = ' ';
    }
}

bool isConstFindsEarlierThanComments(const string& strToSearch, int startSearching)
{
    //Найти позицию начала строковой константы или получить значение равное длине строки
    int strConstIndex = getSubstrPositionOrDefaultValue(strToSearch, "\"", startSearching);

    //Найти позицию начала однострочного комментария или получить значение равное длине строки
    int singleLineCommentIndex = getSubstrPositionOrDefaultValue(strToSearch, "//", startSearching);

    //Найти позицию начала многострочного комментария или получить значение равное длине строки
    int multLinesCommentIndex = getSubstrPositionOrDefaultValue(strToSearch, "/*", startSearching);

    //Составить список из найденных позиций
    list<int> indexes = { strConstIndex, singleLineCommentIndex, multLinesCommentIndex };

    //Найти минимальное значение списка
    int minElem = *min_element(indexes.begin(), indexes.end());
    
    //Вернуть результат логической операции: строковая константа была найдена в строке и позиция ее начала - минимальная из всех найденных
    return minElem == strConstIndex && minElem != strToSearch.length();
}

SubstrPos findPairStrConstPositions(const string& strToCheck, int startSearching)
{
    //Найти позицию начала строковой константы
    int startStrConstIndex = getSubstrPositionOrDefaultValue(strToCheck, "\"", startSearching);
    
    //Найти позицию конца строковой константы
    int endStrConstIndex = getSubstrPositionOrDefaultValue(strToCheck, "\"", startStrConstIndex + 1);
    
    //Пока конец строковой константы указывает на экранированный обратный слэш
    while (strToCheck[endStrConstIndex - 1] == *"\\")
    {
        //Счетчик знаков обратного слэша
        int backSlashCounter = 0;
        
        //Начало подсчета начинается с обратного слэша экранированной кавычки
        int newBackCounter = endStrConstIndex - 1;

        //Пока символ - обратный слэш
        while (strToCheck[newBackCounter] == *"\\")
        {
            //Увеличить счетчик обратных слэшей
            backSlashCounter++;
            
            //Переход к предыдущему символу
            newBackCounter--;
        }

        //Если кол-во обратных слэшей - нечетное, значит на позиции конца строковой константы была экранированная кавычка
        if (backSlashCounter % 2 != 0)
        {
            //Ищем новый конец строковой константы
            endStrConstIndex = getSubstrPositionOrDefaultValue(strToCheck, "\"", endStrConstIndex + 1);
        }
        //Иначе значит, что знак обратного слэша был часть экранированного обратного слэша
        else
        {
            break;
        }
    }
    //Вернуть позиции строковой константы
    return SubstrPos(startStrConstIndex + 1, endStrConstIndex);
}

int getSubstrPositionOrDefaultValue(const string& strToCheck, const string& substrToFind, int startSearching)
{
    //Найти позицию вхождения подстроки в строку
    int posOfSubstr = strToCheck.find(substrToFind, startSearching);

    //Если позиция недействительна, то присвоить ей значение длины строки, иначе оставить 
    posOfSubstr = posOfSubstr == -1 ? strToCheck.length() : posOfSubstr;

    //Вернуть вычисленную позицию
    return posOfSubstr;
}

