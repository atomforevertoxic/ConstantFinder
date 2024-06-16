#pragma once
#include <iostream>
//#include <stack>
//#include <set>
//#include <list>
//#include <vector>


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

