#ifndef FIELD_H
#define FIELD_H
#include "constants.h"
#include <string>
class Field
{
public: 
    //constructors
    Field(const string& fieldName, const typeID& fieldType = CHAR) : _fieldName(fieldName), _fieldType(fieldType){}
    Field(const string& fieldName, const string& fieldTypeAsString) : _fieldName(fieldName)
    {
        if(fieldTypeAsString == "CHAR")
            _fieldType = CHAR;
        else if(fieldTypeAsString == "FLOAT")
            _fieldType = FLOAT;
        else if(fieldTypeAsString == "INT")
            _fieldType = INT;
    }

    //public accessors
    const string& fieldName() { return _fieldName; }
    typeID fieldType() { return _fieldType; }
    string fieldTypeAsString()
    {
        string s;
        if(_fieldType == CHAR)
            s = "CHAR";
        else if(_fieldType == FLOAT)
            s = "FLOAT";
        else if(_fieldType == INT)
            s = "INT";
        return s;
    }

private:
    std::string _fieldName;
    typeID _fieldType;
};

#endif // FIELD_H
