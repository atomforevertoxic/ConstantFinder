set program=ConstantFinder.exe

echo. > ConstantFinderBatTestingResult.txt
echo 01 - (������) ���� �� ���������� ��� ���� �� ���� ����������� >> ConstantFinderBatTestingResult.txt
%program% TestData1.cpp >> ConstantFinderBatTestingResult.txt


echo. >> ConstantFinderBatTestingResult.txt
echo 02 - (������) ���-�� ����� � ����� ��������� ���������� ����� >> ConstantFinderBatTestingResult.txt
%program% TestData2.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 03 - (������) � ���� ��������� ��������� define >> ConstantFinderBatTestingResult.txt
%program% TestData3.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 04 - (������) � ���� �������� ��������� typedef >> ConstantFinderBatTestingResult.txt
%program% TestData4.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 05 - (������) � ���� �������� ������ template>> ConstantFinderBatTestingResult.txt
%program% TestData5.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 06 - �� ���� �������� ���, ������� �� �������� ��������� ��������� >> ConstantFinderBatTestingResult.txt
%program% TestData6.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 07 - �� ���� �������� ���, ������� �������� ��������������� ��������� ��������� >> ConstantFinderBatTestingResult.txt
%program% TestData7.cpp >> ConstantFinderBatTestingResult.txt

echo. >> ConstantFinderBatTestingResult.txt
echo 08 - �� ���� �������� ���, ������� �������� ������������� ��������� ��������� >> ConstantFinderBatTestingResult.txt
%program% TestData8.cpp >> ConstantFinderBatTestingResult.txt


