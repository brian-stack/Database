#ifndef SELECTION_H
#define SELECTION_H
#include "set.h"
#include "myqueue.h"
#include "token.h"
#include "table.h"
#include <vector>

class Selection
{
public:
    //Constructor
    Selection(Set<int>* recNums, const string & tableName, Table * t, bool allRecords, bool allFields) :
        _recordNumbers(recNums), _table(t), _allFields(allFields),_allRecords(allRecords), _tableName(tableName) {}

    //Big 3
    ~Selection() { delete _recordNumbers;}
    Selection(const Selection& other)
    {
        _currentFields = other._currentFields;
        _allFields = other._allFields;
        _allRecords = other._allRecords;
        _table = other._table;
        _recordNumbers = new Set<int>(*other._recordNumbers);
        _tableName = other._tableName;
    }

    Selection& operator=(const Selection& rhs)
    {
        delete _recordNumbers;
        _allFields = rhs._allFields;
        _allRecords = rhs._allRecords;
        _table = rhs._table;
        _recordNumbers = new Set<int>(*rhs._recordNumbers);
        _tableName = rhs._tableName;
        return *this;
    }

    void setFields(const vector<string>& fields, bool allFields){ _currentFields = fields; _allFields = allFields;}
    const string& getTableName() {return _tableName;}

    friend ostream& operator<<(ostream& outs, const Selection& toPrint)
    {
        if(toPrint._allRecords)
            toPrint._table->printAll(outs,toPrint._currentFields,toPrint._allFields);
        else
            toPrint._table->printSelection(outs,toPrint._currentFields,*(toPrint._recordNumbers),toPrint._allFields);
        return outs;
    }

private:
    Set<int> * _recordNumbers;
    Table * _table;
    vector<string> _currentFields;
    bool _allFields;
    bool _allRecords;
    string _tableName;
};

#endif // SELECTION_H
