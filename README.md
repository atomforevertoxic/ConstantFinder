# ConstantFinder
 
Программа предназначена для выявления строковых констант в компилируемом коде, написанном на языке C++. Также указывается их расположение т.е. класс(ы), стуктура(ы), объединение(я), пространства имен и функция, внутри которых они находятся. Строковые константы, которые встречаются больше одного раза попадают, помимо основного ответа, в блок #duplicates. Он содержит дубликаты среди констант вместе с количеством их повторений в коде.

Для запуска программы необходимо при помощи командной строки найти и вызвать исполняемый файл. При запуске программы вторым аргументом передаем расположение и имя внешнего файла, в котором хранится компилируемый код на языке C++ и из которого требуется выявить строковые константы вместе с их расположением.
