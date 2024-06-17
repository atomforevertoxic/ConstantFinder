#pragma once
#include <iostream>
#include <stack>
#include <set>
#include <list>
#include <vector>


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


struct ErrorInfo
{
public:
    int Line;
    enum ErrorMessage
    {
        INVALID_FILENAME_OR_FILEPATH_OR_FILE_DOES_NOT_EXIST,
        LINES_COUNT_IS_OUT_OF_LIMIT,
        TEMPLATE_IS_NOT_ALLOWED_IN_CODE_TEXT,
        THE_DEFINE_DIRECTIVE_IS_NOT_ALLOWED_IN_CODE_TEXT,
        IT_IS_NOT_ALLOWED_TO_DECLARE_ALIASES_USING_A_TYPEDEF
    };

};

/*!
* \���������� ������ �� �����
* \param[in] pathAndName - ���� � �������� ��������� .cpp �����
* \return - ������ �����, �������������� ����� ���� �� ����� .cpp
*/
list<string> getTextFromFile(const string pathAndName);

/*!
* ��������� ������� ������
* \param[in] codeText - ������ �����, �������������� ����� ���� �� ����� .cpp
* \param[in] error - ��������� ������ ������
* \return - ������ ���� ��������� ������
*/
vector<string> isInputDataValid(const list<string>& codeText, ErrorInfo& error);

/*!
* ����� ��������� � ���� �� ��� (������� �������������� �������)
* \param[in] codeText - ������ �����, �������������� ����� ���� �� ����� .cpp
* \return - ������ ��������� �������� � �� ������������
*/
multiset<Constant> findAllConstantsAndTheirLocation(list<string> codeText);

/*!
* �������� ���� ������������ �� ������
* \param[in][out] strToExecute - ������, � ������� ������������ ��������
* \return - ���������� �������� �������������������� ����. �����
*/
bool removeAllCommentsFromString(string& strToExecute);


/*!
* ��������� ������ ������(�������) ����������� � ������
* \param[in] strToResearch - ������, � ������� ������������ �����
* \return - ������� ������ ������(�������) �����������
*/
SubstrPos getLeftmostComment(string& strToResearch);

/*!
* �����������, ��������� ��  ���. ��������� ������ ������������
* \param[in] strToSearch - ������, � ������� ������������ �����
* \return - ��������� ����������� ������������ ������� ������������� ��������� ���������
*           � �������� ���������� �������� ���. ��������� ����� ������������
*/
bool isConstFindsEarlierThanComments(const string& strToSearch);

/*!
* ��������� ��������� ������� ��������� ��� �������� ��-���������
* \param[in] strToCheck - ������, � ������� ������������ ����� ���������
* \param[in] substrToFind - ���������, ������� ��������� ����� � ������
* \param[in] startSearching - �������, � ������� ���������� �����
* \return - ������ ������ ��������� ��� �������� ��-���������, ���� ��������� �� ������� � ������
*/
int getSubstrPositionOrDefaultValue(const string& strToCheck, const string& substrToFind, int startSearching);

/*!
* ��������� ��������� ���������
* \param[in][out] strToExecute - ������, � ������� ������������ ���������
* \param[in] pos - ������� ���������� ���������
*/
void cleanSubstrBySpaces(string& strToClean, SubstrPos pos);

/*!
* ����� ���� �������� �������� � ������
* \param[in] str - ������, � ������� ������������ �����
* \return - ������ ������� �������� ���� �������� ��������
*/
list<SubstrPos>getAllKeyObjectsIndexes(const string& str);

/*!
* ����� ������� ����� ������������ ����/�����������/������/���������
* \param[in] strToCheck - ������, � ������� ������������ ������� ����� ����������
* \param[in] keyWord - �������� ����� ��� ������
* \return ��������� � �������� ������� ����� ������������ ����/������/���������/�����������
*/
SubstrPos getDeclarNamePosition(const string& strToCheck, const string& keyWord);

/*!
* ����������� ������� ����� �������
* \param[in] strToCheck - ������, � ������� ������������ ������� ����� �������
* \return - ��������� � �������� ������� ����� �������
*/
SubstrPos getFuncNamePosition(const string& strToCheck);

/*!
* ����� ������� ���� ������ ������� ������ � ������
* \param[in] strToSearch - ������, � ������� ������������ ������� ������ ������� ������
* \return - ��� ������� ������ ������� ������
*/
list<SubstrPos> getAllBracketsPos(const string& strToSearch);

/*!
* ����������� ������� ��������� ���������
* \param[in] strToCheck - ������, � ������� ������������ ������� ��������� ���������
* \return - ��������� � �������� ������� ��������� ���������
*/
SubstrPos findPairStrConstPositions(const string& strToCheck);

