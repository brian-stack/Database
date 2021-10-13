#include "token.h"
Map<typeID,int> Token::_priorityMap;

//preconditions: none
//postcondition: members are initialized.
Token::Token()
{
    initPriorityMap();
    _token = "";
    _type = EMPTY;
}

//preconditions: none
//postcondition: members are initialized to recieved values.
Token::Token(const string& str, typeID type) : _token(str), _type(type)
{
    initPriorityMap();
    if(type == LESS || type == GREATER || type == GREATER_OR_EQUAL
            || type == LESS_OR_EQUAL || type == EQUALS)
        _group = RELATIONAL_OP;
    else if(type == AND || type == OR)
        _group = LOGICAL_OP;
    else if(type == OPEN_P || type == CLOSE_P)
        _group = PARENTHESES;
    else if(type == VALUE || type == FIELD_NAME || type == RELATIONAL_FIELD
            || type == INT || type == FLOAT || type == CHAR)
        _group = OPERAND;
    else
        _group = OTHER;
}

//preconditions: none
//postcondition: if the _priorityMap was not yet initialized
// for an instance of token, initialize it now.
void Token::initPriorityMap()
{
    static bool init = false;
    if(!init)
    {
        _priorityMap.insert(OPEN_P,6);
        _priorityMap.insert(CLOSE_P,5);
        _priorityMap.insert(AND,2);
        _priorityMap.insert(OR,1);
        _priorityMap.insert(LESS,3);
        _priorityMap.insert(GREATER,3);
        _priorityMap.insert(GREATER_OR_EQUAL,3);
        _priorityMap.insert(LESS_OR_EQUAL,3);
        _priorityMap.insert(EQUALS,3);
    }
}

//preconditions: none
//postconditions: returns the priority of this token.
int Token::priority()
{
    if(_priorityMap.contains(_type))
        return _priorityMap[_type];
    else
        return -1;
}

//preconditions: none
//postcondition: outputs the type and token string to the recieved output stream.
ostream& operator <<(ostream& outs, const Token& t)
{
    outs << "[Type: \"" << t._type << "\" , Token: \"" << t._token << "\"]";
    return outs;
}

//preconditions: none
//postcondition: returns _type as an integer
typeID Token::type() const
{
    return _type;
}

//preconditions: none
//postcondition: returns the value of the token as a string.
string Token::token_str()
{
    return _token;
}
