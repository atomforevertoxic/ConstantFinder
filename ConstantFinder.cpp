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
    //Инициализация массива ошибок
    vector<ErrorInfo> errors;
    bool isCommentedFlag = false;
    int stringCounter = 0;
    //Для каджой строки текста
    for (string codeString : codeText)
    {
        stringCounter++;
        //Если ранее был установлен флаг кооментированностит след. строки
        if (isCommentedFlag)
        {
            int commentEnd = getSubstrPositionOrDefaultValue(codeString, "*/", 0);
            //Если строка содержит конец многострочного комментария
            if (commentEnd != codeString.length())
            {
                //Вырезать участок строки до конца комментария
                cleanSubstrBySpaces(codeString, SubstrPos(0, commentEnd + 1));
            }
            //Иначе
            else
            {
                //Пропустить строку
                continue;
            }
        }
        //Начало поиска устанавливается на начало строки
        int startSearching = 0;
        //Пока значение начала поиска меньше длины строки
        while (startSearching < codeString.length())
        {
            //Удалить все комментарии из строки и распознать многострочный комментарий
            isCommentedFlag = removeAllCommentsFromString(codeString);
            
            //Найти позицию ключевого слова define
            int defineIndex = getSubstrPositionOrDefaultValue(codeString, "define ", startSearching);
            
            //Найти позицию ключевого слова typedef
            int typedefIndex = getSubstrPositionOrDefaultValue(codeString, "typedef ", startSearching);
            
            //Найти позицию ключевого слова template
            int templateIndex = getSubstrPositionOrDefaultValue(codeString, "template", startSearching);
            
            //Найти пару позиций строковой константы
            SubstrPos strConstPos = findPairStrConstPositions(codeString, startSearching);
            
            //Если в строке присутствует объявление define
            if (defineIndex != codeString.length() && isDefineReal(codeString, SubstrPos(defineIndex, (defineIndex + 6))))
            {
                //Сохранить соответствующую ошибку, включая номер строки, на которой она была обнаружена
                errors.push_back(ErrorInfo(stringCounter, ErrorMessage::THE_DEFINE_DIRECTIVE_IS_NOT_ALLOWED_IN_TEXT_CODE));
            }

            //Если в строке присутствует объявление псевдонимов (typedef)
            if (typedefIndex != codeString.length() && isTypedefReal(codeString, SubstrPos(defineIndex, (typedefIndex + 7))))
            {
                //Сохранить соответствующую ошибку, включая номер строки, на которой она была обнаружена
                errors.push_back(ErrorInfo(stringCounter, ErrorMessage::IT_IS_NOT_ALLOWED_TO_DECLARE_ALIASES_USING_A_TYPEDEF));
            }
            
            //Если в строке присутствует шаблон
            if (templateIndex != codeString.length() && isTemplateReal(codeString, SubstrPos(defineIndex, (templateIndex + 8))))
            {
                //Сохранить соответствующую ошибку, включая номер строки, на которой она была обнаружена
                errors.push_back(ErrorInfo(stringCounter, ErrorMessage::TEMPLATE_IS_NOT_ALLOWED_IN_TEXT_CODE));
            }
            //Сформировать список из всех найденных позиций
            list<int> indexes = { defineIndex, typedefIndex, templateIndex, strConstPos.right};

            //Поиск минимального значения из списка
            int minElemIndex = *min_element(indexes.begin(), indexes.end());

            //Начало поиска равно значению следующему после минимального найденного индекса
            startSearching = minElemIndex+1;
        }
    }
    //Вернуть набор найденных ошибок
    return errors;
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




int getSubstrPositionOrDefaultValue(const string& strToCheck, const string& substrToFind, int startSearching)
{
    //Найти позицию вхождения подстроки в строку
    int posOfSubstr = strToCheck.find(substrToFind, startSearching);

    //Если позиция недействительна, то присвоить ей значение длины строки, иначе оставить 
    posOfSubstr = posOfSubstr == -1 ? strToCheck.length() : posOfSubstr;

    //Вернуть вычисленную позицию
    return posOfSubstr;
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



bool isDefineReal(string codeString, SubstrPos pos)
{
    int substrStartIndex = pos.left;
    //Начиная с левой границы подстроки "двигаемся" назад до начала строки
    for (; substrStartIndex >= 0; substrStartIndex--)
    {
        //Если был найден символ #
        if (codeString[substrStartIndex] == '#')
        {
            //Вернуть результат функции, проверяющей подстроку на принадлежность строковой константе
            return !isSubstrInStrConst(pos, codeString);
        }
    }
    return false;
}



bool isTypedefReal(string codeString, SubstrPos pos)
{
    //Вернуть результат логической операции, проверяющей, что подстрока "typedef" не является частью другого слова и не находится внутри строковой константы
    return ((pos.left == 0 || !isalpha(codeString[pos.left - 1])) && !isSubstrInStrConst(pos, codeString));
}



bool isTemplateReal(string codeString, SubstrPos pos)
{
    //Вернуть результат логической операции, проверяющей, что подстрока "template" не является частью другого слова и не находится внутри строковой константы
    return ((pos.left == 0 || !isalpha(codeString[pos.left - 1])) && !isSubstrInStrConst(pos, codeString) && (pos.right == codeString.length() - 1 || codeString[pos.right] == ' ' || codeString[pos.right] == '\t' || codeString[pos.right] == '<'));
}



bool isSubstrInStrConst(SubstrPos& posToCheck, string& strToCheck)
{
    //Инициализация значения начала поиска
    int startSearching = 0;

    //Пока значение начала поиска не превышает длину строку
    while (startSearching <= strToCheck.length())
    {
        //Найти пару позиций строковой константы
        SubstrPos strConstPos = findPairStrConstPositions(strToCheck, startSearching);

        //Если позиции проверяемой подстроки находятся внутри строковой константы
        if (strConstPos.left<posToCheck.left && strConstPos.right>posToCheck.right)
        {
            return true;
        }
        //Начало поиска равно позиции следующей после конечной границу прошлой найденной строковой константы
        startSearching = strConstPos.right + 1;
    }
    return false;
}

void showAllErrors(vector<ErrorInfo> &errors)
{
    //Для каждой ошибки из вектора
    for (ErrorInfo error : errors)
    {
        //Начальный шаблон сообщения об ошибке
        string report = "Line - |" + to_string(error.Line) + "|: ";
        //В зависимости от типа ошибки в изначальный шаблон сообщения об ошибке добавляется разное пояснение
        switch (error.Message)
        {
        case INVALID_FILENAME_OR_FILEPATH_OR_FILE_DOES_NOT_EXIST:
            report += "INVALID FILENAME OR FILEPATH OR FILE DOES NOT EXIST";
            break;
        case LINES_COUNT_IS_OUT_OF_LIMIT:
            report += "LINES COUNT IS OUT OF LIMIT";
            break;
        case TEMPLATE_IS_NOT_ALLOWED_IN_TEXT_CODE:
            report += "TEMPLATE IS NOT ALLOWED IN TEXT CODE";
            break;
        case THE_DEFINE_DIRECTIVE_IS_NOT_ALLOWED_IN_TEXT_CODE:
            report += "THE DEFINE DIRECTIVE IS NOT ALLOWED IN TEXT CODE";
            break;
        case IT_IS_NOT_ALLOWED_TO_DECLARE_ALIASES_USING_A_TYPEDEF:
            report += "IT IS NOT ALLOWED TO DECLARE ALIASES USING A TYPEDEF";
            break;
        }
        //Вывод сформированного сообщения об ошибке
        cout << report << endl;
    }
}



multiset<Constant> findAllConstantsAndTheirLocation(list<string> codeText)
{
    multiset<Constant> cc;
    return cc;
}

SubstrPos getDeclarNamePosition(const string& strToCheck, const string& keyWord, int startSearching)
{
    return SubstrPos();
}