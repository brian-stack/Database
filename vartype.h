#ifndef VARTYPE_H
#define VARTYPE_H

#include <ostream>
#include <cstdlib>
#include <string>
#include <iostream>
#include "constants.h"

using namespace std;

struct varType
{
    fieldId _type;
    union
    {
        string _s;
        double _d;
        int _i;
    };

    //constructors
    varType() : _type(DOUBLE), _d(0) {}
    varType(string sData) : _type(STRING), _s(sData) {}
    varType(double dData) : _type(DOUBLE), _d(dData) {}
    varType(int iData) : _type(NUMBER), _d(iData) {}

    //Big 3
    ~varType()
    {
        if(_type == STRING)
            _s.~basic_string();
    }
    varType(const varType& other)
    {
        _type = other._type;
        if(_type == STRING)
            new (&this->_s) auto(other._s);
        else if(_type == DOUBLE)
            this->_d = other._d;
        else if(_type == NUMBER)
            this->_i = other._i;
    }
    varType& operator=(const varType& rhs)
    {
        _type = rhs._type;
        if(_type == DOUBLE)
            _d = rhs._d;
        else if(_type == STRING)
            new (&this->_s) auto(rhs._s);
        else if(_type == NUMBER)
            _i = rhs._i;
        return *this;
    }

    //insertion operator
    friend ostream& operator<<(ostream& outs, const varType& data)
    {
        switch(data._type)
        {
            case STRING:
            {
                outs << data._s;
                break;
            }
            case DOUBLE:
            {
                outs << data._d;
                break;
            }
            case NUMBER:
            {
                outs << data._i;
                break;
            }
        }
        return outs;
    }

    //comparison operators
    friend bool operator<(const varType& lhs, const varType& rhs)
    {
        bool result = false;
        if((lhs._type == rhs._type))
        {
            if(lhs._type == DOUBLE)
                result = (lhs._d < rhs._d);
            else if(lhs._type == STRING)
                result = (lhs._s < rhs._s);
            else if(lhs._type == NUMBER)
                result = (lhs._i < rhs._i);
        }
        return result;
    }
    friend bool operator>(const varType& lhs, const varType& rhs)
    {
        bool result = false;
        if((lhs._type == rhs._type))
        {
            if(lhs._type == DOUBLE)
                result = (lhs._d > rhs._d);
            else if(lhs._type == STRING)
                result = (lhs._s > rhs._s);
            else if(lhs._type == NUMBER)
                result = (lhs._i > rhs._i);
        }
        return result;
    }
    friend bool operator==(const varType& lhs, const varType& rhs)
    {
        bool result = false;
        if((lhs._type == rhs._type))
        {
            if(lhs._type == DOUBLE)
                result = (lhs._d == rhs._d);
            else if(lhs._type == STRING)
                result = (lhs._s == rhs._s);
            else if(lhs._type == NUMBER)
                result = (lhs._i == rhs._i);
        }
        return result;
    }
    friend bool operator!=(const varType& lhs, const varType& rhs)
    {
        bool result = false;
        if((lhs._type == rhs._type))
        {
            if(lhs._type == DOUBLE)
                result = (lhs._d != rhs._d);
            else if(lhs._type == STRING)
                result = (lhs._s != rhs._s);
            else if(lhs._type == NUMBER)
                result = (lhs._i != rhs._i);
        }
        return result;
    }
    friend bool operator>=(const varType& lhs, const varType& rhs)
    {
        bool result = false;
        if((lhs._type == rhs._type))
        {
            if(lhs._type == DOUBLE)
                result = (lhs._d >= rhs._d);
            else if(lhs._type == STRING)
                result = (lhs._s >= rhs._s);
            else if(lhs._type == NUMBER)
                result = (lhs._i >= rhs._i);
        }
        return result;
    }
    friend bool operator<=(const varType& lhs, const varType& rhs)
    {
        bool result = false;
        if((lhs._type == rhs._type))
        {
            if(lhs._type == DOUBLE)
                result = (lhs._d <= rhs._d);
            else if(lhs._type == STRING)
                result = (lhs._s <= rhs._s);
            else if(lhs._type == NUMBER)
                result = (lhs._i <= rhs._i);
        }
        return result;
    }
};

#endif // VARTYPE_H
