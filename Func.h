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

/*! ���������� ������ �� �����
* \param[in] pathAndName - ���� � �������� ��������� .cpp �����
* \param[in] error - ��������� ������ ������
* \return - ������ �����, �������������� ����� ���� �� ����� .cpp
*/
list<string> getTextFromFile(const string pathAndName, ErrorInfo& error);



/*! ��������� ������� ������
* \param[in] codeText - ������ �����, �������������� ����� ���� �� ����� .cpp
* \return - ������ ���� ��������� ������
*/
vector<ErrorInfo> isInputDataValid(const list<string>& codeText);



/*! �������� ���� ������������ �� ������
* \param[in][out] strToExecute - ������, � ������� ������������ ��������
* \return - ���������� �������� �������������������� ����. �����
*/
bool removeAllCommentsFromString(string& strToExecute);



/*! ��������� ������ ������(�������) ����������� � ������
* \param[in] strToResearch - ������, � ������� ������������ �����
* \return - ������� ������ ������(�������) �����������
*/
SubstrPos getLeftmostComment(string& strToResearch);



/*! �����������, ��������� ��  ���. ��������� ������ ������������
* \param[in] strToSearch - ������, � ������� ������������ �����
* \param[in] startSearching - �������, � ������� ���������� �����
* \return - ��������� ����������� ������������ ������� ������������� ��������� ���������
*           � �������� ���������� �������� ���. ��������� ����� ������������
*/
bool isConstFindsEarlierThanComments(const string& strToSearch, int startSearching);



/*! ��������� ��������� ���������
* \param[in][out] strToExecute - ������, � ������� ������������ ���������
* \param[in] pos - ������� ���������� ���������
*/
void cleanSubstrBySpaces(string& strToClean, SubstrPos pos);



/*! ����������� ������� ��������� ���������
* \param[in] strToCheck - ������, � ������� ������������ ������� ��������� ���������
* \param[in] startSearching - �������, � ������� ���������� �����
* \return - ��������� � �������� ������� ��������� ���������
*/
SubstrPos findPairStrConstPositions(const string& strToCheck, int startSearching);



/*! ��������� ��������� ������� ��������� ��� �������� ��-���������
* \param[in] strToCheck - ������, � ������� ������������ ����� ���������
* \param[in] substrToFind - ���������, ������� ��������� ����� � ������
* \param[in] startSearching - �������, � ������� ���������� �����
* \return - ������ ������ ��������� ��� �������� ��-���������, ���� ��������� �� ������� � ������
*/
int getSubstrPositionOrDefaultValue(const string& strToCheck, const string& substrToFind, int startSearching);



/*! �������, ������������, ������������� �� ���� ��������� ��������� define
* \param[in] codeString - ������, � ������� ���������������� ��������� ��������� define
* \param[in] pos - ������� ��������� define
* \return - ���������� ��������� ������ ���������� ��������� define
*/
bool isDefineReal(string codeString, SubstrPos pos);



/*! �������, ������������, ������������� �� ���� ��������� �������� ����� typedef
* \param[in] codeString - ������, � ������� ���������������� ��������� �������� ����� typedef
* \param[in] pos - ������� ��������� typedef
* \return - ���������� ��������� ������ ���������� ��������� ����� typedef
*/
bool isTypedefReal(string codeString, SubstrPos pos);



/*! �������, ������������, ������������� �� ��� �������� ������ templpate
* \param[in] codeString - ������, � ������� ���������������� �������� ������ templpate
* \param[in] pos - ������� ��������� templpate
* \return - ���������� ��������� ������ ���������� ������� templpate
*/
bool isTemplateReal(string codeString, SubstrPos pos);



/*! ������� ������������, ��������� ��� ��������� ������ ��������� ���������
* \param[in] pos - ������� ���������, ������� ����� ��������� � ��������� ���������
* \param[in] strToCheck - ������, � ������� ���������� ��������
* \return - ���������� ��������� ������� ��������� � ��������� ���������
*/
bool isSubstrInStrConst(SubstrPos pos, string& strToCheck);


/*! ������� ������ ��������� ������ � �������
* \param[in] errors - ������ ���� ��������� ������
*/
void showAllErrors(vector<ErrorInfo> &errors);



/*! ����� ��������� � ���� �� ��� (������� �������������� �������)
* \param[in] codeText - ������ �����, �������������� ����� ���� �� ����� .cpp
* \return - ������ ��������� �������� � �� ������������
*/
multiset<Constant> findAllConstantsAndTheirLocation(list<string> codeText);



/*! ����� ���� �������� �������� � ������
* \param[in] str - ������, � ������� ������������ �����
* \return - ������ ������� �������� ���� �������� ��������
*/
list<SubstrPos>getAllKeyObjectsIndexes(const string& str);



/*! ����� ������� ����� ������������ ����/�����������/������/���������
* \param[in] strToCheck - ������, � ������� ������������ ������� ����� ����������
* \param[in] keyWord - �������� ����� ��� ������
* \param[in] startSearching - �������, � ������� ���������� �����
* \return ��������� � �������� ������� ����� ������������ ����/������/���������/�����������
*/
SubstrPos getDeclarNamePosition(string& strToCheck, const string& keyWord, int startSearching);



/*! ����������� ������� ����� �������
* \param[in] strToCheck - ������, � ������� ������������ ������� ����� �������
* \param[in] startSearching - �������, � ������� ���������� �����
* \return - ��������� � �������� ������� ����� �������
*/
SubstrPos getFuncNamePosition(const string& strToCheck, int startSearching);



/*! ����� ������� ���� ������ ������� ������ � ������
* \param[in] strToSearch - ������, � ������� ������������ ������� ������ ������� ������
* \param[in] startSearching - �������, � ������� ���������� �����
* \return - ��� ������� ������ ������� ������
*/
list<SubstrPos> getAllBracketsPos(const string& strToSearch, int startSearching);



/*! ����������� ������� ��������� ���������
* \param[in] strToCheck - ������, � ������� ������������ ������� ��������� ���������
* \return - ��������� � �������� ������� ��������� ���������
*/
SubstrPos findPairStrConstPositions(const string& strToCheck);

