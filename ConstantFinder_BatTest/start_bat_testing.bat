set program=ConstantFinder.exe

echo. > ConstantFinderBatTestingResult.txt
echo 01 - (ОШИБКА) Файл не существует или путь до него некорректен >> ConstantFinderBatTestingResult.txt
%program% TestData1.cpp >> ConstantFinderBatTestingResult.txt


echo. >> ConstantFinderBatTestingResult.txt
echo 02 - (ОШИБКА) Кол-во строк в файле превышает допустимый лимит >> ConstantFinderBatTestingResult.txt
%program% TestData2.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 03 - (ОШИБКА) В коде объявлена директива define >> ConstantFinderBatTestingResult.txt
%program% TestData3.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 04 - (ОШИБКА) В коде объявлен псевдоним typedef >> ConstantFinderBatTestingResult.txt
%program% TestData4.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 05 - (ОШИБКА) В коде объявлен шаблон template>> ConstantFinderBatTestingResult.txt
%program% TestData5.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 06 - На вход поступил код, который не содержит строковые константы >> ConstantFinderBatTestingResult.txt
%program% TestData6.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 07 - На вход поступил код, который содержит НЕДУБЛИРУЮЩИЕСЯ строковые константы >> ConstantFinderBatTestingResult.txt
%program% TestData7.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 08 - На вход поступил код, который содержит ДУБЛИРУЮЩИЕСЯ строковые константы >> ConstantFinderBatTestingResult.txt
%program% TestData8.cpp >> ConstantFinderBatTestingResult.txt


