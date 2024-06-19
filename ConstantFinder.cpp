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