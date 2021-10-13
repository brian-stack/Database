#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include "constants.h"
#include "map.h"
using namespace std;

class Token
{
    //prints the type and token to the output stream.
    friend ostream& operator <<(ostream& outs, const Token& t);

public:
    Token();                                   //sets _type to 0 and token to "".
    Token(const string& str, typeID _type);    //sets this->_type to _type and token to str
    void setType(typeID type) {_type = type; }

    //public accessors
    int priority();
    groupID group() {return _group;}
    typeID type() const;             //returns the value of _type
    string type_string();            //returns the name of the the type as a string.
    string token_str();              //returns the value of token.

private:
    void initPriorityMap();
    static Map<typeID,int> _priorityMap;

    string _token;
    typeID _type;
    groupID _group;
};

#endif //TOKEN_H
