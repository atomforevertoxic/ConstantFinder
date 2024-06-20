#include "Func.h"

int main(int argc, char* argv[])
{
    //Инициализировать одиную ошибку для функции считывания текста из файла
    ErrorInfo inputError = ErrorInfo();

    //Получить список строк из файла
    list<string> codeText = getTextFromFile(argv[1], inputError);

    //Инициализировать вектор ошибок
    vector<ErrorInfo> ErrorVector;

    //Инициализировать набор объектов типа Constant
    multiset<Constant> constantAndTheirLocations;

    //Получить имя(путь и имя) входного файла
    string responseFileName = argv[1];

    //Сформировать имя выходного файла
    responseFileName.replace((responseFileName.length() - 4), 4, "_response.txt");

    //Если одиночная ошибка содержит значение не по-умолчанию
    if (inputError.Line != -333)
    {
        //Вывести соответствующую ошибку в консоль
        ErrorVector = { inputError };
        showAllErrors(ErrorVector);

        //Создать пустой файл
        makeResponseFile(constantAndTheirLocations, responseFileName);
        
        //Вернуть код неккоректного завершения программы
        return -1;
    }

    //Протестировать входные данные на корректность
    ErrorVector = isInputDataValid(codeText);

    //Если в ходе проверки были выявлены ошибки
    if (!ErrorVector.empty())
    {
        //Вывести ошибки в консоль
        showAllErrors(ErrorVector);

        //Создать пустой файл
        makeResponseFile(constantAndTheirLocations, responseFileName);
        
        //Вернуть код некорректного завершения программы
        return -2;
    }

    //Найти все константы из текста и их расположение
    constantAndTheirLocations = findAllConstantsAndTheirLocation(codeText);

    //Сформировать выходной файл на основе полученного результата главной вычислительной функции
    makeResponseFile(constantAndTheirLocations, responseFileName);
    
    //Вернуть код корректного завершения программы
    return 0;
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



bool isSubstrInStrConst(SubstrPos posToCheck, string& strToCheck)
{
    //Инициализация значения начала поиска
    int startSearching = 0;

    //Пока значение начала поиска не превышает длину строку
    while (startSearching <= strToCheck.length())
    {
        //Найти пару позиций строковой константы
        SubstrPos strConstPos = findPairStrConstPositions(strToCheck, startSearching);

        //Если позиции проверяемой подстроки находятся внутри строковой константы
        if (strConstPos.left<=posToCheck.left && strConstPos.right>posToCheck.right)
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
    multiset<Constant> allConstantAndTheirLocation;
    int startSearching = 0;
    bool isCommentedFlag = false;

    int curlOpenBracketsCounter = 0;

    //Инициализировать стэк для хранения "пути" до константы (кроме функции)
    stack<string> globalConstantPathStack;

    //Функция, в которой объявлена константа
    string constantFunction = "";

    int stringCounter = 0;


    //Для каждой строки из списка
    for (string codeString : codeText)
    {
        stringCounter++;
        int startSearching = 0;

        //Пока начало поиска не больше длины строки
        while (startSearching <= codeString.length() && codeString.length()!=0)
        {
            //Сформировать список из пар позиций всех ключевых объектов
            list<SubstrPos> allKeyObjectPositions = getAllKeyObjectsIndexes(codeString, startSearching);

            //Найти минимальную пару позиций из списка
            SubstrPos minPos;
            int minSubstrIndex = minSubstrPosFromList(allKeyObjectPositions, minPos);

            //Начало поиска равно позиции следующей после минимальной пары позиций
            startSearching = minPos.right + 1;

            //Если минимальная пара позиций не указывает на конец строки
            if (minPos.left < codeString.length())
            {
                switch (minSubstrIndex)
                {
                    //Если минимальная пара значение - позиции открывающей фигурной скобки
                    case 0:
                        //Инкрементировать счетчик открывающих фигурных скобок
                        curlOpenBracketsCounter++;
                        break;

                    //Если минимальная пара значение - позиции закрывающей фигурной скобки
                    case 1:
                        //Декрементировать счетчик открывающих фигурных скобок
                        curlOpenBracketsCounter--;

                        //Если кол-во открывающих фигурных скобок меньше суммы кол-ва классов и функции, которым принадлежит строка, и функция определена
                        if (curlOpenBracketsCounter < (globalConstantPathStack.size() + 1) && constantFunction != "")
                        {
                            //Удалить имя функции
                            constantFunction = "";
                        }
                        //Иначе если кол-во открывающих скобко менше суммы кол-ва классов, которым принадлежит строка
                        else if (curlOpenBracketsCounter < (globalConstantPathStack.size() + 1))
                        {
                            //Удалить имя последнего сохраненного класса из стэка
                            globalConstantPathStack.pop();
                        }
                        break;

                    //Если минимальная пара значение - позиции имени класса//структуры/объединения/пространства имен
                    case 2:
                        //Сохранить имя класса/структуры/объединения/пространства имен в стэк пути
                        globalConstantPathStack.push(codeString.substr(minPos.left, (minPos.right - minPos.left)));
                        break;

                    //Если минимальная пара значение - позиции имени функции
                    case 3:
                        //Сохранить имя функции в соответствующую переменную
                        constantFunction = codeString.substr(minPos.left, (minPos.right - minPos.left));
                        break;

                    //Если минимальная пара значение - позиции строковой константы
                    case 4:
                    {
                        string fullConstantPath = "";
                        //Сформировать путь до константы, используя стэк и имя функции
                        if (!constantFunction.empty())
                        {
                            fullConstantPath = " - " + constantFunction + ":";
                        }
                        else
                        {
                            fullConstantPath = ":";
                        }

                        if (globalConstantPathStack.empty())
                        {
                            fullConstantPath = "global" + fullConstantPath;
                        }
                        else
                        {
                            stack<string> stackToMakePath = globalConstantPathStack;
                            while (!stackToMakePath.empty())
                            {
                                fullConstantPath = " - " + stackToMakePath.top() + fullConstantPath;
                                stackToMakePath.pop();
                            }
                            fullConstantPath.erase(0, 3);
                        }

                        //Получить строковую константу из строки
                        string foundConstString = codeString.substr(minPos.left, (minPos.right - minPos.left));
                        
                        //Сохранить строковую константу и путь до нее в обекст Constant
                        Constant fullConstantObject(fullConstantPath, stringCounter, foundConstString);

                        //Добавить объект Constant в multiset
                        allConstantAndTheirLocation.insert(fullConstantObject);
                    }
                    break;
                }
            }
        }
    }
    //Вернуть multiset объектов Constant
    return allConstantAndTheirLocation;
}



list<SubstrPos>getAllKeyObjectsIndexes(string& str, int startSearching)
{
    //Найти позиции имени объявляемого класса
    SubstrPos classDeclarPos = getDeclarNamePosition(str, "class", startSearching); 

    //Найти позиции имени объявляемой структуры
    SubstrPos structDeclarPos = getDeclarNamePosition(str, "struct", startSearching);

    //Найти позиции имени объявляемого объединения
    SubstrPos unionDeclarPos = getDeclarNamePosition(str, "union", startSearching);

    //Найти позиции имени объявляемого пространства имен
    SubstrPos namespaceDeclarPos = getDeclarNamePosition(str, "namespace", startSearching); 
    
    //Найти минимальную пару позиций из списка всех найденных пар
    SubstrPos minGlobalPathPartPos;
    minSubstrPosFromList({ classDeclarPos, structDeclarPos, unionDeclarPos, namespaceDeclarPos }, minGlobalPathPartPos);

    //Найти позиции имени объявляемой функции
    SubstrPos funcNamePos = getFuncNamePosition(str, startSearching);

    //Найти позиции строковой константы
    SubstrPos constMarkPos = findPairStrConstPositions(str, startSearching);
    
    //Найти позиции открывающей фигурной скобки
    int openFigIndex = getSubstrPositionOrDefaultValue(str, "{", startSearching);
    SubstrPos openFigBracket = SubstrPos(openFigIndex, openFigIndex + 1);
    
    //Найти позиции закрывающей фигурной скобки
    int closeFigIndex = getSubstrPositionOrDefaultValue(str, "}", startSearching);
    SubstrPos closeFigBracket = SubstrPos(closeFigIndex, closeFigIndex + 1);
    
    //Составить список найденных пар позиций
    list<SubstrPos> allPositions = { openFigBracket, closeFigBracket, minGlobalPathPartPos, funcNamePos, constMarkPos };
    
    //Вернуть список пар позиций
    return allPositions;
}


SubstrPos getDeclarNamePosition(string& strToCheck, const string& keyWord, int startSearching)
{
    //Если ключевое слово не равно "class" и "struct" и "namespace" и "union"
    if (keyWord != "class" && keyWord != "struct" && keyWord != "union" && keyWord != "namespace")
    {
        //Вернуть значение позиций по-умолчанию
        return SubstrPos(strToCheck.length(), strToCheck.length());
    }

    //Найти индекс ключевого слова
    int keyWordStartIndex = getSubstrPositionOrDefaultValue(strToCheck, keyWord, startSearching);
    
    //Если не удалось найти ключевое слово
    if (keyWordStartIndex == strToCheck.length())
    {
        //Вернуть значение по-умолчанию
        return SubstrPos(strToCheck.length(), strToCheck.length());
    }

    //Начало ключевого слова не совпадает с началом строки
    if (keyWordStartIndex != 0)
    {
        //Пока символ до ключевого слова - буква
        while (isalpha(strToCheck[keyWordStartIndex - 1]))
        {
            //Начало поиска - следующая позиция от начала ключевого слова
            startSearching = keyWordStartIndex + 1;

            //Найти новый индекс ключевого слова
            keyWordStartIndex = getSubstrPositionOrDefaultValue(strToCheck, keyWord, startSearching);
            //Если ключевое слово не было найдено
            if (keyWordStartIndex == strToCheck.length())
            {
                //Вернуть значение позиций по-умолчанию
                return SubstrPos(strToCheck.length(), strToCheck.length());
            }
        }
    }
    startSearching = 0;
    
    //Определить конец ключевого слова
    int keyWordEndIndex = keyWordStartIndex + keyWord.length();

    //Пока символ после ключевого - буква
    while (isalpha(strToCheck[keyWordEndIndex]))
    {
        //Начало поиска - следующая позиция от конца ключевого слова
        startSearching = keyWordStartIndex + 1;

        //Найти новый индекс ключевого слова
        keyWordStartIndex = getSubstrPositionOrDefaultValue(strToCheck, keyWord, startSearching);

        //Если ключевое слово не было найдено
        if (keyWordStartIndex == strToCheck.length())
        {
            //Вернуть значение позиций по-умолчанию
            return SubstrPos(strToCheck.length(), strToCheck.length());
        }
    }

    //Если ключевое слово заключено в строковую константу
    if (isSubstrInStrConst(SubstrPos(keyWordStartIndex, keyWordEndIndex), strToCheck))
    {
        //Вернуть значение позиций по-умолчанию
        return SubstrPos(strToCheck.length(), strToCheck.length());
    }

    //Начало поиска - следующая позиция от конца ключевого слова
    startSearching = keyWordEndIndex + 1;

    //Найти позицию открывающей фигурной скобки
    int figBracketIndex = getSubstrPositionOrDefaultValue(strToCheck, "{", startSearching) - 1;

    //Минимальное значение будем считать за конец имени объявления
    int endNameIndex = min(figBracketIndex, (int)strToCheck.length()-1);

    //Перемещать позицию конца ключевого слова к началу имени объявления
    for (; strToCheck[keyWordEndIndex] == ' ' || strToCheck[keyWordEndIndex] == '\t'; keyWordEndIndex++);

    //Перемещать позицию конца объявления к концу имени объявления
    for (; strToCheck[endNameIndex] == ' ' || strToCheck[endNameIndex] == '\t'; endNameIndex--);

    //Найти позицию точки с запятой
    int semicolonIndex = getSubstrPositionOrDefaultValue(strToCheck, ";", startSearching);

    //Если точка с запятой была найдена в строке
    if (semicolonIndex != strToCheck.length())
    {
        //Получить подстроку в границах между имени объявления и точки с запятой
        string afterDeclarSubstr = strToCheck.substr(endNameIndex + 1, semicolonIndex - endNameIndex-1);

        //Регулярное выржание, которое определет, относится ли точка с запятой к объявлению
        regex r(R"(^\s*$)");

        //Если регулярное выражение выполняется
        if (regex_match(afterDeclarSubstr, r))
        {
            //Вернуть значение позиций по-умолчанию
            return SubstrPos(strToCheck.length(), strToCheck.length());
        }
    }

    //Вернуть позиции имени объявления
    return SubstrPos(keyWordEndIndex, endNameIndex + 1);
}



int minSubstrPosFromList(list<SubstrPos> allPositions, SubstrPos& minPos)
{
    int index = 0;
    int minIndex = 0;

    //По-умолчанию минимальная пара позиций равна первой пары из списка
    minPos = *allPositions.begin();

    //Для каждой пары позиций
    for (SubstrPos pos : allPositions)
    {
        //Если левая граница предложенной пары позиций меньше левой границы минимальной пары позиций
        if (pos.left < minPos.left)
        {
            //Предложенная пара становится минимальной
            minPos = pos;

            //Запоминается позиция минимальной пары в списке
            minIndex = index;
        }
        index++;
    }
    return minIndex;
}



SubstrPos getFuncNamePosition(const string& strToCheck, int startSearching)
{

    //Найти список всех круглых скобок в строке
    list<SubstrPos> bracketsPosList = getAllBracketsPos(strToCheck, startSearching);
    bool argsFoundFlag = false;
    SubstrPos foundArgsBracketPos;

    //Для каждой пары позиций круглых скобок
    for (list<SubstrPos>::reverse_iterator listItr = bracketsPosList.rbegin(); listItr != bracketsPosList.rend(); ++listItr)
    {
        SubstrPos pos(listItr->left, listItr->right);

        //Получить подстроку заключенную в круглые скобки
        string argSubstr = strToCheck.substr(pos.left, (pos.right - pos.left) + 1);

        //Найти позицию точки с запятой
        int semicolonIndex = getSubstrPositionOrDefaultValue(strToCheck, ";", pos.right);

        //Получить подстроку между закрывающей фигурной скобкой и позицией точки с запятой
        string afterArgSubstr = strToCheck.substr(pos.right, semicolonIndex - pos.right + 1);

        //Регулярное выражение, которое распознает, что между закрывающей круглой скобкой и точкой с запятой находятся только пробелы или знаки табуляции
        regex r(R"(^\)[ \s]*\;$)");

        //Если подстрока между круглыми скобками содержит аргументы и (в строке нет точки с запятой или точка с запятой не относится к предположительному объявлению функции)
        if (isSubstrContainsFuncArgs(argSubstr) && (semicolonIndex == strToCheck.length() || !regex_match(afterArgSubstr, r)))
        {

            //Установить флаг, что аргументы были найдены между круглыми скобками
            argsFoundFlag = true;

            //Сохраненить позиции этих скобок
            foundArgsBracketPos = pos;
            break;
        }
    }
    //Если аргументы были найдены
    if (argsFoundFlag)
    {
        //Конец имени функции равен позиции открывающей круглой скобки
        int nameEndPos = foundArgsBracketPos.left - 1;
        
        //Передвигать позицию конца имени функции влево, пока не встретится буква
        for (; strToCheck[nameEndPos] == ' ' || strToCheck[nameEndPos] == '\t'; nameEndPos--);

        //Начао имени функции равен позиции конца имени функции
        int nameStartPos = nameEndPos;
        
        //Передвигать позицию начала имени функции, пока не закончится слово
        for (; strToCheck[nameStartPos] != ' ' && strToCheck[nameStartPos] != '\t'; nameStartPos--);

        //Сохранить позиции имени функции
        return SubstrPos(nameStartPos + 1, nameEndPos + 1);
    }

    //Вернуть значение позиций по-умолчанию
    return SubstrPos(strToCheck.length(), strToCheck.length());
}



list<SubstrPos> getAllBracketsPos(const string& strToSearch, int startSearching)
{
    //Инициализация списка позиций пар круглых скобок
    list<SubstrPos> allBracketsPosList;

    //Инициализация стэка позиций открывающих круглых скобок
    stack<int> openBracketsPos;

    //Пока строка содержит открывающую скобку
    while (strToSearch.find("(", startSearching) != string::npos)
    {
        //Найти индекс открывающей круглой скобки
        int openBracketsIndex = strToSearch.find("(", startSearching);

        //Сохранить найденную позицию в стэк
        openBracketsPos.push(openBracketsIndex);

        //Установить начало поиска равное позиции следующей после индекса открывающей скобки
        startSearching = openBracketsIndex + 1;

        //Пока индекс новой открывающей скобки меньше индекса закрывающей скобки
        while (strToSearch.find("(", startSearching) != string::npos && strToSearch.find("(", startSearching) < strToSearch.find(")", startSearching))
        {
            //Найти позицию новой открывающей скобки
            openBracketsIndex = strToSearch.find("(", startSearching);

            //Сохранить найденную позицию в стэк
            openBracketsPos.push(openBracketsIndex);

            //Установить начало поиска равное позиции следующей после индекса открывающей скобки
            startSearching = openBracketsIndex + 1;
        }

        //Пока индекс новой закрывающей скобки меньше индекса открывающей скобки
        while (strToSearch.find(")", startSearching) != string::npos && (strToSearch.find("(", startSearching) > strToSearch.find(")", startSearching)))
        {
            //Найти позицию новой закрывающей скобки
            int closedBracketIndex = strToSearch.find(")", startSearching);

            //Сохранить позиции пары открывающей и закрывающей скобок
            allBracketsPosList.push_back(SubstrPos(openBracketsPos.top(), closedBracketIndex));

            //Удалить последнюю позицию открывающей скобки из стэка
            openBracketsPos.pop();

            //Установить начало поиска равное позиции следующей после индекса закрывающей скобки
            startSearching = closedBracketIndex + 1;
        }
    }
    //Вернуть список позиций всех пар правильно расставленных круглых скобок
    return allBracketsPosList;
}



bool isSubstrContainsFuncArgs(string substr)
{
    //Регулярное выражение определяющая аргумент или несколько аргументов функции в строке
    const regex r(R"(\([^\)\n]*?[a-zA-Z_][a-zA-Z0-9_]*[ \t]+[a-zA-Z_][a-zA-Z0-9_]*.*?\))");

    //Инициализация списка позиций строковых констант
    list<SubstrPos> marksIndexes;


    int startSearching = 0;

    //Пока в строке найдена кавычка
    while (getSubstrPositionOrDefaultValue(substr, "\"", startSearching) != substr.length())
    {
        //Найти позиции строковой константы
        SubstrPos pos = findPairStrConstPositions(substr, startSearching);

        //Сохранить найденные позиции
        marksIndexes.push_back(pos);

        //Установить начало поиска равное позиции следующей после индекса закрывающей кавычки
        startSearching = pos.right + 1;
    }

    startSearching = 0;
    //Поиск позиции точки с запятой в строке
    int semicolonIndex = getSubstrPositionOrDefaultValue(substr, ";", startSearching);

    //Пока в строке найдена точка с запятой
    while (semicolonIndex != substr.length())
    {
        //Если точка с запятой не находится внутри строковой константы
        if (!isSemicolonInStrConst(marksIndexes, semicolonIndex))
        {
            return false;
        }
        //Установить начало поиска равное позиции следующей после индекса точки с запятой
        startSearching = semicolonIndex + 1;

        //Найти новую позицию точки с запятой
        semicolonIndex = getSubstrPositionOrDefaultValue(substr, ";", startSearching);
    }

    //Результат функции положителен если длина подстркои с аргументами равна 2 или соответствует маски регулярного выражения
    return substr.length() == 2 || regex_match(substr, r);
}



bool isSemicolonInStrConst(list<SubstrPos> strConstIndexes, int semicolonIndex)
{
    //Для каждой пары позиций из списка
    for (SubstrPos pos : strConstIndexes)
    {
        //Если позиция точки с запятой находится в строковой константе
        if (semicolonIndex >= pos.left && semicolonIndex < pos.right)
        {
            return true;
        }
    }
    return false;
}


void makeResponseFile(multiset<Constant> constants, string responseFileName)
{
    //Открытие поток записи в файл
    ofstream MyFile(responseFileName);

    //Если набор объектов Constant пуст
    if (constants.empty())
    {
        //Вывести соответствующее сообщение
        MyFile << "no solution" << endl;
    }
    //Иначе
    else
    {
        bool isMultisetContainsDupl = false;
        string lastUsedPath = "";
        //Для каждого объекта Constant
        for (Constant constant : constants)
        {
            //Если данный путь до константы еще не был записан
            if (lastUsedPath != constant.constantLocation)
            {
                //Записать это путь как заголовок для констант
                MyFile << endl;
                MyFile << constant.constantLocation << endl;

                lastUsedPath = constant.constantLocation;
            }

            //Сформировать строку, содержащую информацию о константе
            string response = to_string(constant.constLine) + " \"" + constant.constString + "\"";

            //Записать строку в файл
            MyFile << response << endl;
            
            //Если данная константа дублируется
            if (constants.count(constant) > 1)
            {
                //Установить соответствующий флаг
                isMultisetContainsDupl = true;
            }
        }

        //Если набор содержит дублирующиеся константы
        if (isMultisetContainsDupl)
        {
            //Записать в файл соответствующий заголовок
            MyFile << endl;
            MyFile << endl;
            MyFile << "#duplicates:" << endl;

            //Инициализировать набор уже записанных дубликатов
            multiset<Constant> lastUsedConstants;

            //Для каждого объекта Constant из набора
            for (Constant duplicate : constants)
            {
                //Если константа дублируется и еще не была записана в файл
                if (lastUsedConstants.count(duplicate) == 0 && constants.count(duplicate) > 1)
                {
                    //Записать информацию о дубликате в специальной форме
                    string duplResponse = duplicate.constString + " - " + to_string(constants.count(duplicate));
                    MyFile << duplResponse << endl;

                    //Добавить объект в набор использованных
                    lastUsedConstants.insert(duplicate);
                }
            }
        }

    }

    //Закрыть поток записис в файл
    MyFile.close();
}