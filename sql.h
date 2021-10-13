#ifndef SQL_H
#define SQL_H
#include <fstream>
#include <iostream>
#include <cassert>
#include "stokenizer.h"
#include "table.h"
#include "arrayutil.h"
#include "myqueue.h"
#include "mystack.h"
#include "set.h"
#include "selection.h"

using namespace std;

class SQL
{
public:
    //Default Constructor
    SQL() : _currentTable(nullptr), _currentTableName(""),
        _currentAction(EMPTY)
    {
        loadSavedTables();
        initAdjacencyMatrix();
        recieveCommands();
    }

    //Batch constructor
    SQL(const char* batchFileName) : _currentTable(nullptr),
        _currentTableName(""), _currentAction(EMPTY)
    {
        _batchInput.open(batchFileName,ios_base::binary);
        assert(_batchInput.is_open());

        //loadSavedTables();
        initAdjacencyMatrix();
        recieveCommands(true,_batchInput);
    }

    //Destructor
    ~SQL()
    {
        ofstream savedTables(SAVED_TABLES,ios_base::binary);
        assert(savedTables.is_open());
        Record rec;
        Map<string,Table*>::Iterator it  = _tables.begin();
        while(it != _tables.end())
        {
            vector<string> temp;
            temp.push_back((*it).value()->getTableName());
            rec.writeToFile(savedTables,temp);
            delete (*it).value();
            ++it;
        }
        savedTables.close();
    }

private:
    void recieveCommands(bool fromFile = false, istream& inStream = cin);   //Main loop for the program, can accept batch input.
    void getCommand(char arr [], unsigned n, istream& ins = cin);
    bool getPTree(int startState, typeID startType); //Verify that the command is structured correctly.
    bool verifyPTree();                              //Additional command specific verification, check DB name, fields, num values, etc.
    bool verifyFields(vector<string>& fields);       //Used by verifyPTree to check that fields match those of the current table.
    void executeCommand();                           //Execute the command, using the current table, or constructing a new one.
    bool toPostfix();                                //Convert _infix to _postfix and return if parentheses match.
    void processSelect();                            //Process the selection query, obtain a set if where clause

    //SQL Initialization
    void initAdjacencyMatrix();
    void loadSavedTables();

    //Where clause
    Queue<Token> _postfix;
    Queue<Token> _infix;

    //Current command
    Table * _currentTable;
    string _currentTableName;
    typeID _currentAction;

    //Command parser
    Map<int,vector<string> > _PTree;
    vector<Token> _values;
    char _command[200];
    STokenizer _preProcessor;
    ifstream _batchInput;

    //For state machine
    int _SQLAdjacencyMatrix[MAX_ROWS][MAX_COLUMNS];
    int _rowColSQLStates[MAX_ROWS][MAX_COLUMNS];

    Map<string,Table*> _tables;
};

#endif //SQL_H
