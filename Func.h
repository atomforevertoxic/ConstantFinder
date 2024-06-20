#pragma once
#include <iostream>
#include <stack>
#include <set>
#include <list>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct Constant
{
public:
    string constantLocation;
    int constLine;
    string constString;

public:Constant(string Location, int Line, string cString) {
    constantLocation = Location;
    constLine = Line;
    constString = cString;
}
      bool operator < (const Constant& other) const {
          return constString < other.constString;
      }
};



struct SubstrPos
{
public:
    int left;
    int right;

    SubstrPos()
    {
        left = -1;
        right = -1;
    }

    SubstrPos(int l, int r)
    {
        left = l;
        right = r;
    }
};

enum ErrorMessage
{
    INVALID_FILENAME_OR_FILEPATH_OR_FILE_DOES_NOT_EXIST,
    LINES_COUNT_IS_OUT_OF_LIMIT,
    TEMPLATE_IS_NOT_ALLOWED_IN_TEXT_CODE,
    THE_DEFINE_DIRECTIVE_IS_NOT_ALLOWED_IN_TEXT_CODE,
    IT_IS_NOT_ALLOWED_TO_DECLARE_ALIASES_USING_A_TYPEDEF
};

struct ErrorInfo
{
public:
    int Line;
    ErrorMessage Message;
    ErrorInfo()
    {
        Line = -1;
    }
    ErrorInfo(int line, ErrorMessage message)
    {
        Line = line;
        Message = message;
    }
};

/*! Считывание текста из файла
* \param[in] pathAndName - путь и название вводимого .cpp файла
* \param[in] error - структура поиска ошибок
* \return - список строк, представляющий текст кода из файла .cpp
*/
list<string> getTextFromFile(const string pathAndName, ErrorInfo& error);



/*! Валидация входные данные
* \param[in] codeText - список строк, представляющий текст кода из файла .cpp
* \return - вектор всех найденных ошибок
*/
vector<ErrorInfo> isInputDataValid(const list<string>& codeText);



/*! Удаление всех комментариев из строки
* \param[in][out] strToExecute - строка, в которой производится удаление
* \return - логическое значение закомментированности след. строк
*/
bool removeAllCommentsFromString(string& strToExecute);



/*! Получение самого левого(первого) комментария в строке
* \param[in] strToResearch - строка, в которой производится поиск
* \return - позиции самого левого(первого) комментария
*/
SubstrPos getLeftmostComment(string& strToResearch);



/*! Определение, находится ли  стр. константа раньше комментариев
* \param[in] strToSearch - строка, в которой производится поиск
* \param[in] startSearching - позиция, с которой начинается поиск
* \return - результат логического произведения событий существования строковой константы
*           и значение первентсва значения стр. константы среди комментариев
*/
bool isConstFindsEarlierThanComments(const string& strToSearch, int startSearching);



/*! Затирание подстроки пробелами
* \param[in][out] strToExecute - строка, в которой производится затирание
* \param[in] pos - позиции затираемой подстроки
*/
void cleanSubstrBySpaces(string& strToClean, SubstrPos pos);



/*! Определение позиций строковой константы
* \param[in] strToCheck - строка, в которой определяются позиции строковой константы
* \param[in] startSearching - позиция, с которой начинается поиск
* \return - начальная и конечные позиции строковой константы
*/
SubstrPos findPairStrConstPositions(const string& strToCheck, int startSearching);



/*! Получение начальной позиции подстроки или значения по-умолчанию
* \param[in] strToCheck - строка, в которой производится поиск подстроки
* \param[in] substrToFind - подстрока, которую требуется найти в строке
* \param[in] startSearching - позиция, с которой начинается поиск
* \return - индекс начала подстроки или значение по-умолчанию, если подстрока не найдена в строке
*/
int getSubstrPositionOrDefaultValue(const string& strToCheck, const string& substrToFind, int startSearching);



/*! Функция, определяющая, действительно ли была объявлена директива define
* \param[in] codeString - строка, в которой предположительно объявлена директива define
* \param[in] pos - позиции подстроки define
* \return - логический результат поиска объявления директивы define
*/
bool isDefineReal(string codeString, SubstrPos pos);



/*! Функция, определяющая, действительно ли было объявлено ключевое слово typedef
* \param[in] codeString - строка, в которой предположительно объявлено ключевое слово typedef
* \param[in] pos - позиции подстроки typedef
* \return - логический результат поиска объявления ключевого слова typedef
*/
bool isTypedefReal(string codeString, SubstrPos pos);



/*! Функция, определяющая, действительно ли был объявлен шаблон templpate
* \param[in] codeString - строка, в которой предположительно объявлен шаблон templpate
* \param[in] pos - позиции подстроки templpate
* \return - логический результат поиска объявления шаблона templpate
*/
bool isTemplateReal(string codeString, SubstrPos pos);



/*! Функция определяющая, находится лии подстрока внутри строковой константы
* \param[in] pos - позиции подстроки, которая может находится в строковой константе
* \param[in] strToCheck - строка, в которой происходит проверка
* \return - логическое результат наличия подстроки в строковой константе
*/
bool isSubstrInStrConst(SubstrPos pos, string& strToCheck);


/*! Функция вывода найденных ошибок в консоль
* \param[in] errors - вектор всех найденных ошибок
*/
void showAllErrors(vector<ErrorInfo> &errors);



/*! Поиск константы и путь до них (главная вычислительная функция)
* \param[in] codeText - список строк, представляющий текст кода из файла .cpp
* \return - список строковых констант и их расположений
*/
multiset<Constant> findAllConstantsAndTheirLocation(list<string> codeText);



/*! Поиск всех ключевых объектов в строке
* \param[in] str - строка, в которой производится поиск
* \return - список позиций подстрок всех ключевых объектов
*/
list<SubstrPos>getAllKeyObjectsIndexes(const string& str);



/*! Поиск позиций имени пространства имен/объединения/класса/структуры
* \param[in] strToCheck - строка, в которой определяются позиции имени объявления
* \param[in] keyWord - ключевое слово для поиска
* \param[in] startSearching - позиция, с которой начинается поиск
* \return начальная и конечная позиции имени пространства имен/класса/структуры/объединения
*/
SubstrPos getDeclarNamePosition(string& strToCheck, const string& keyWord, int startSearching);



/*! Определение позиций имени функции
* \param[in] strToCheck - строка, в которой определяются позиции имени функции
* \param[in] startSearching - позиция, с которой начинается поиск
* \return - начальная и конечная позиции имени функции
*/
SubstrPos getFuncNamePosition(const string& strToCheck, int startSearching);



/*! Поиск позиций всех парных круглых скобок в строке
* \param[in] strToSearch - строка, в которой определяются позиции парных круглых скобок
* \param[in] startSearching - позиция, с которой начинается поиск
* \return - все позиции парных круглых скобок
*/
list<SubstrPos> getAllBracketsPos(const string& strToSearch, int startSearching);



/*! Определение позиций строковой константы
* \param[in] strToCheck - строка, в которой определяются позиции строковой константы
* \return - начальная и конечные позиции строковой константы
*/
SubstrPos findPairStrConstPositions(const string& strToCheck);

