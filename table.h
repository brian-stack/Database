#ifndef TABLE_H
#define TABLE_H

#include <cassert>
#include <fstream>
#include <iostream>
#include "multimap.h"
#include "map.h"
#include "set.h"
#include "record.h"
#include "vartype.h"
#include "field.h"
#include "token.h"

using namespace std;

class Table
{
public:
    //constructors
    Table(const string& tableName);
    Table(const string& tableName, const vector<string>& fieldNames, const vector<string>& dataTypes);

    //public accessors
    bool containsField(const string & fieldName) {return _fieldNameToIndex.contains(fieldName);}
    const vector<string>& getFieldNames() {return _fieldNames;}
    const string& getTableName(){return _tableName;}
    bool isValueValid(const Token &t, const size_t& index);
    Set<int>* createSet(string fieldName, typeID opType, string value);

    //public mutators
    void insertInto(vector<Token>& values);

    //print functions
    void printSelection(ostream& outs, const vector<string>& fields, Set<int>& selection, bool allFields = false);
    void printAll(ostream& outs, const vector<string>& fields, bool allFields = false);

private: 
    //File I/O:
    Record _rec;
    ofstream _fileOuts;
    ifstream _fileIns;

    //Field info
    vector<string> _fieldNames;
    vector<string> _dataTypes;
    vector<Field> _fields;

    //Table Info
    string _tableName;
    string _fileName;
    long _recordCount;
    Map<int, MMap<varType, int> > _indiciesMap;
    Map<string,int> _fieldNameToIndex;

    //private mutators
    varType createVarientType(const string& value, int fieldIndex);
    void updateIndiciesMaps(vector<string>& values);
    void populateIndiciesMaps();
};

#endif // TABLE_H
