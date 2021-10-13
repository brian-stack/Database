#include "sql.h"

//preconditions: any table whose path is in SAVED_TABLES must exist.
//postconditions: any existing tables saved on disk will be loaded.
//                a table object will be constructed for each table.
void SQL::loadSavedTables()
{
    ifstream savedTables(SAVED_TABLES,ios_base::binary);

    if(savedTables.is_open())
    {
        Record paths;
        vector<string> tableInfo;
        while(paths.readFromFile(savedTables,tableInfo) > 0 && tableInfo.size() == 1)
        {
            _tables.insert(tableInfo.at(0),new Table(tableInfo.at(0)));
            tableInfo.clear();
        }
        savedTables.close();
    }
}

//preconditions: none
//postconditions: the adjacency matricies used in the SQL state machine will be initialized.
void SQL::initAdjacencyMatrix()
{
    //Select SQL Command
    initTable(_SQLAdjacencyMatrix);
    markFail(_SQLAdjacencyMatrix,SELECT_START);
    markFail(_SQLAdjacencyMatrix,SELECT_START+1);
    markFail(_SQLAdjacencyMatrix,SELECT_START+2);
    markFail(_SQLAdjacencyMatrix,SELECT_START+3);
    markFail(_SQLAdjacencyMatrix,SELECT_START+4);
    markFail(_SQLAdjacencyMatrix,SELECT_START+5);
    markSuccess(_SQLAdjacencyMatrix,SELECT_START+6);
    markCell(SELECT_START,_SQLAdjacencyMatrix,SELECT,SELECT_START+1);
    markCell(SELECT_START+1,_SQLAdjacencyMatrix,ASTERISK,SELECT_START+2);
    markCell(SELECT_START+1,_SQLAdjacencyMatrix,UNKNOWN,SELECT_START+3);
    markCell(SELECT_START+2,_SQLAdjacencyMatrix,FROM,SELECT_START+5);
    markCell(SELECT_START+3,_SQLAdjacencyMatrix,COMMA,SELECT_START+4);
    markCell(SELECT_START+3,_SQLAdjacencyMatrix,FROM,SELECT_START+5);
    markCell(SELECT_START+4,_SQLAdjacencyMatrix,UNKNOWN,SELECT_START+3);
    markCell(SELECT_START+5,_SQLAdjacencyMatrix,UNKNOWN,SELECT_START+6);

    //Select where clause
    markFail(_SQLAdjacencyMatrix,SELECT_START+7);
    markFail(_SQLAdjacencyMatrix,SELECT_START+8);
    markFail(_SQLAdjacencyMatrix,SELECT_START+9);
    markSuccess(_SQLAdjacencyMatrix,SELECT_START+10);
    markCell(SELECT_START+6,_SQLAdjacencyMatrix,WHERE,SELECT_START+7);
    markCell(SELECT_START+7,_SQLAdjacencyMatrix,UNKNOWN,SELECT_START+8);
    markCell(SELECT_START+8,_SQLAdjacencyMatrix,LESS,SELECT_START+9);
    markCell(SELECT_START+8,_SQLAdjacencyMatrix,GREATER,SELECT_START+9);
    markCell(SELECT_START+8,_SQLAdjacencyMatrix,GREATER_OR_EQUAL,SELECT_START+9);
    markCell(SELECT_START+8,_SQLAdjacencyMatrix,LESS_OR_EQUAL,SELECT_START+9);
    markCell(SELECT_START+8,_SQLAdjacencyMatrix,EQUALS,SELECT_START+9);
    markCell(SELECT_START+7,_SQLAdjacencyMatrix,OPEN_P,SELECT_START+7);
    markCell(SELECT_START+10,_SQLAdjacencyMatrix,CLOSE_P,SELECT_START+10);
    markCell(SELECT_START+9,_SQLAdjacencyMatrix,UNKNOWN,SELECT_START+10);
    markCell(SELECT_START+9,_SQLAdjacencyMatrix,DIGIT,SELECT_START+10);
    markCell(SELECT_START+9,_SQLAdjacencyMatrix,INT,SELECT_START+10);
    markCell(SELECT_START+9,_SQLAdjacencyMatrix,FLOAT,SELECT_START+10);
    markCell(SELECT_START+9,_SQLAdjacencyMatrix,CHAR,SELECT_START+10);
    markCell(SELECT_START+9,_SQLAdjacencyMatrix,VALUE,SELECT_START+10);
    markCell(SELECT_START+10,_SQLAdjacencyMatrix,AND,SELECT_START+7);
    markCell(SELECT_START+10,_SQLAdjacencyMatrix,OR,SELECT_START+7);

    //Second 2d array will map row(state), col(type) to the SQL type.
    initTable(_rowColSQLStates);
    markCell(SELECT_START+1,_rowColSQLStates,SELECT,SELECT);
    markCell(SELECT_START+2,_rowColSQLStates,ASTERISK,ASTERISK);
    markCell(SELECT_START+3,_rowColSQLStates,UNKNOWN,FIELD_NAME);
    markCell(SELECT_START+4,_rowColSQLStates,COMMA,COMMA);
    markCell(SELECT_START+5,_rowColSQLStates,FROM,FROM);
    markCell(SELECT_START+6,_rowColSQLStates,UNKNOWN,DB_NAME);
    markCell(SELECT_START+7,_rowColSQLStates,WHERE,WHERE);
    markCell(SELECT_START+8,_rowColSQLStates,UNKNOWN,RELATIONAL_FIELD);
    markCell(SELECT_START+9,_rowColSQLStates,LESS,LESS);
    markCell(SELECT_START+9,_rowColSQLStates,GREATER,GREATER);
    markCell(SELECT_START+9,_rowColSQLStates,EQUALS,EQUALS);
    markCell(SELECT_START+9,_rowColSQLStates,GREATER_OR_EQUAL,GREATER_OR_EQUAL);
    markCell(SELECT_START+9,_rowColSQLStates,LESS_OR_EQUAL,LESS_OR_EQUAL);
    markCell(SELECT_START+10,_rowColSQLStates,UNKNOWN,VALUE);
    markCell(SELECT_START+10,_rowColSQLStates,VALUE,VALUE);
    markCell(SELECT_START+10,_rowColSQLStates,INT,INT);
    markCell(SELECT_START+10,_rowColSQLStates,FLOAT,FLOAT);
    markCell(SELECT_START+10,_rowColSQLStates,DIGIT,VALUE);
    markCell(SELECT_START+7,_rowColSQLStates,AND,AND);
    markCell(SELECT_START+7,_rowColSQLStates,OR,OR);
    markCell(SELECT_START+10,_rowColSQLStates,CLOSE_P,CLOSE_P);
    markCell(SELECT_START+7,_rowColSQLStates,OPEN_P,OPEN_P);

    //Make SQL Command
    markFail(_SQLAdjacencyMatrix,CREATE_START);
    markFail(_SQLAdjacencyMatrix,CREATE_START+1);
    markFail(_SQLAdjacencyMatrix,CREATE_START+2);
    markFail(_SQLAdjacencyMatrix,CREATE_START+3);
    markFail(_SQLAdjacencyMatrix,CREATE_START+4);
    markFail(_SQLAdjacencyMatrix,CREATE_START+6);
    markSuccess(_SQLAdjacencyMatrix,CREATE_START+5);
    markCell(CREATE_START,_SQLAdjacencyMatrix,MAKE,CREATE_START+1);
    markCell(CREATE_START+1,_SQLAdjacencyMatrix,TABLE,CREATE_START+2);
    markCell(CREATE_START+2,_SQLAdjacencyMatrix,UNKNOWN,CREATE_START+3);
    markCell(CREATE_START+3,_SQLAdjacencyMatrix,FIELDS,CREATE_START+4);
    markCell(CREATE_START+4,_SQLAdjacencyMatrix,UNKNOWN,CREATE_START+5);
    markCell(CREATE_START+5,_SQLAdjacencyMatrix,COMMA,CREATE_START+6);
    markCell(CREATE_START+6,_SQLAdjacencyMatrix,UNKNOWN,CREATE_START+5);

    //testing
    markCell(CREATE_START+5,_SQLAdjacencyMatrix,INT,CREATE_START+7);
    markCell(CREATE_START+5,_SQLAdjacencyMatrix,FLOAT,CREATE_START+7);
    markCell(CREATE_START+5,_SQLAdjacencyMatrix,CHAR,CREATE_START+7);
    markCell(CREATE_START+7,_SQLAdjacencyMatrix,COMMA,CREATE_START+4);
    markSuccess(_SQLAdjacencyMatrix,CREATE_START+7);

    markCell(CREATE_START+7,_rowColSQLStates,INT,INT);
    markCell(CREATE_START+7,_rowColSQLStates,FLOAT,FLOAT);
    markCell(CREATE_START+7,_rowColSQLStates,CHAR,CHAR);
    markCell(CREATE_START+4,_rowColSQLStates,COMMA,COMMA);

    //Second 2d array will map row(state), col(type) to the SQL type.
    markCell(CREATE_START+1,_rowColSQLStates,MAKE,MAKE);
    markCell(CREATE_START+2,_rowColSQLStates,TABLE,TABLE);
    markCell(CREATE_START+3,_rowColSQLStates,UNKNOWN,DB_NAME);
    markCell(CREATE_START+4,_rowColSQLStates,FIELDS,FIELDS);
    markCell(CREATE_START+5,_rowColSQLStates,UNKNOWN,FIELD_NAME);
    markCell(CREATE_START+6,_rowColSQLStates,COMMA,COMMA);

    //Insert SQL Command:
    markFail(_SQLAdjacencyMatrix,INSERT_START);
    markFail(_SQLAdjacencyMatrix,INSERT_START+1);
    markFail(_SQLAdjacencyMatrix,INSERT_START+2);
    markFail(_SQLAdjacencyMatrix,INSERT_START+3);
    markFail(_SQLAdjacencyMatrix,INSERT_START+4);
    markFail(_SQLAdjacencyMatrix,INSERT_START+6);
    markSuccess(_SQLAdjacencyMatrix,INSERT_START+5);
    markCell(INSERT_START,_SQLAdjacencyMatrix,INSERT,INSERT_START+1);
    markCell(INSERT_START+1,_SQLAdjacencyMatrix,INTO,INSERT_START+2);
    markCell(INSERT_START+2,_SQLAdjacencyMatrix,UNKNOWN,INSERT_START+3);
    markCell(INSERT_START+3,_SQLAdjacencyMatrix,VALUES,INSERT_START+4);
    markCell(INSERT_START+4,_SQLAdjacencyMatrix,UNKNOWN,INSERT_START+5);
    markCell(INSERT_START+4,_SQLAdjacencyMatrix,INT,INSERT_START+5);
    markCell(INSERT_START+4,_SQLAdjacencyMatrix,FLOAT,INSERT_START+5);
    markCell(INSERT_START+4,_SQLAdjacencyMatrix,VALUE,INSERT_START+5);
    markCell(INSERT_START+5,_SQLAdjacencyMatrix,COMMA,INSERT_START+6);
    markCell(INSERT_START+6,_SQLAdjacencyMatrix,UNKNOWN,INSERT_START+5);
    markCell(INSERT_START+6,_SQLAdjacencyMatrix,INT,INSERT_START+5);
    markCell(INSERT_START+6,_SQLAdjacencyMatrix,FLOAT,INSERT_START+5);
    markCell(INSERT_START+6,_SQLAdjacencyMatrix,VALUE,INSERT_START+5);

    //Second 2d array will map row(state), col(type) to the SQL type.
    markCell(INSERT_START+1,_rowColSQLStates,INSERT,INSERT);
    markCell(INSERT_START+2,_rowColSQLStates,INTO,INTO);
    markCell(INSERT_START+3,_rowColSQLStates,UNKNOWN,DB_NAME);
    markCell(INSERT_START+4,_rowColSQLStates,VALUES,VALUES);
    markCell(INSERT_START+5,_rowColSQLStates,UNKNOWN,VALUE);
    markCell(INSERT_START+5,_rowColSQLStates,FLOAT,FLOAT);
    markCell(INSERT_START+5,_rowColSQLStates,INT,INT);
    markCell(INSERT_START+5,_rowColSQLStates,VALUE,VALUE);
    markCell(INSERT_START+6,_rowColSQLStates,COMMA,COMMA);
}

//preconditions: isStream must be an open input stream.
// commands recieved from inStream must be deliminated by a new line character.
//postconditions: the stokenizer will be initialized to the recieved command,
// then, the command token will be obtained, (SELECT, INSERT, MAKE)
//       the command token will determine the state state in which
//       a state machine will read subsequent tokens and create the PTree.
void SQL::recieveCommands(bool fromFile, istream& inStream)
{
    bool exit = false;
    while(!exit && !inStream.eof())
    {
        bool isSuccess = false;
        cout << endl << string(50,'-') << endl << "Enter a command " << endl;
        getCommand(_command,INT_MAX,inStream);
        _command[inStream.gcount()] = '\0';

        if(fromFile)
            cout << _command << endl;

        if(strcmp(_command,"exit") == 0 || inStream.gcount() == 0)
            exit = true;
        else
        {
            _preProcessor.setString(_command);
            Token commandToken;
            vector<string> commandVector;

            _preProcessor >> commandToken;
            commandVector.push_back(commandToken.token_str());

            _currentAction = commandToken.type();
            if(commandToken.type() == SELECT)
            {
                _PTree.insert(SELECT,commandVector);
                isSuccess = getPTree(SELECT_START,SELECT);
            }
            else if(commandToken.type() == INSERT)
            {
                _PTree.insert(INSERT,commandVector);
                isSuccess = getPTree(INSERT_START,INSERT);
            }
            else if(commandToken.type() == MAKE)
            {
                _PTree.insert(MAKE,commandVector);
                isSuccess = getPTree(CREATE_START,MAKE);
            }
            else
                cout << "Error, SQL command not recognized as SELECT, INSERT, or MAKE." <<endl;

            if(isSuccess)
            {
                _currentTableName = _PTree[DB_NAME].at(0);
                if(verifyPTree())
                {
                    if(!_infix.empty())
                    {
                        if(!toPostfix())
                        {
                            isSuccess = false;
                            cout << "Error when converting expression to postfix: " << endl << _postfix << endl;
                        }
                    }
                    if(isSuccess)
                    {
                        executeCommand();
                    }
                }
                else
                    cout << "Error, no such table or fields exist." << endl;
            }
            else
            {
                cout << "Error, invalid SQL grammer " << endl;
                cout << endl << _PTree << endl;
            }
            _PTree.clear();
            _infix.clear();
            _postfix.clear();
            _values.clear();
            _currentAction = EMPTY;
            _currentTable = nullptr;
            _currentTableName = "";
        }
    }
}

//preconditions: ins must be open.
//postconditions: read a command from file, here we define a command to be
// a line read from the stream that does NOT start with "\" and is NOT an empty line.
void SQL::getCommand(char arr [], unsigned n, istream& ins)
{
    bool hasValid = false;
    while(!hasValid && !ins.eof())
    {
        ins.getline(arr,n,'\n');
        arr[ins.gcount()] = '\0';

        if(arr[0] != '/' && arr[0] != '\n' && arr[0] != '\0')
            hasValid = true;
    }
}

//preconditions: none
//postconditions: a PTree will be constructed by traversing the adjacency matrix starting
// from (row, col): startState, startType. At this level, parentheses after the where clause,
// do not have any impact on the validity of an expression.
// Any tokens found after the where keyword will be pushed to
bool SQL::getPTree(int startState, typeID startType)
{
    startState = _SQLAdjacencyMatrix[startState][static_cast<int>(startType)];
    bool isSuccess = false;
    bool foundWhere = false;
    Map<int,vector<string> > Ptree = _PTree;
    Token lastNonParentheses;
    Token nextToken;
    Queue<Token> tempInfixQueue;

    //pre-read while loop
    _preProcessor >> nextToken;
    startState = _SQLAdjacencyMatrix[startState][static_cast<int>(nextToken.type())];
    while(startState != -1 && nextToken.type() != EMPTY)
    {
        //determine what the tokenType should be based on the current row / col in the adjacency matrix
        // For example, this allows us to classify UNKNOWN as DB_NAME, based on the current state / token.
        int tokenType = _rowColSQLStates[startState][nextToken.type()];

        if(!Ptree.contains(tokenType))
        {
            vector<string> temp;
            temp.push_back(nextToken.token_str());
            Ptree.insert(tokenType,temp);
        }
        else
            Ptree[tokenType].push_back(nextToken.token_str());

        if(foundWhere)
        {
            Token t(nextToken.token_str(),static_cast<typeID>(tokenType));
            tempInfixQueue.push(t);
        }

        //if a success state is found, save the current state of the PTree and Infixqueue
        if(_SQLAdjacencyMatrix[startState][0] == 1)
        {
            isSuccess = true;
            _PTree = Ptree;
            _infix = tempInfixQueue;
        }

        if(tokenType == CHAR || tokenType == INT || tokenType == FLOAT || tokenType == VALUE)
            _values.push_back(nextToken);

        //Special case where 2 tokens get parsed in 1 iteration of the while loop
        //Here things get a bit complicated because if the current token is type field_name,
        // and no data type identifier follows as the next token, a default type of CHAR will be
        // associated with that token.
        if(_currentAction == MAKE && tokenType == FIELD_NAME)
        {
            if(!Ptree.contains(DATA_TYPE))
            {
                vector<string> temp;
                Ptree.insert(DATA_TYPE,temp);
            }

            bool successAtEnd = false;
            //if a success state is found, make a note of it.
            if(_SQLAdjacencyMatrix[startState][0] == 1)
                successAtEnd = true;

            _preProcessor >> nextToken;
            startState = _SQLAdjacencyMatrix[startState][static_cast<int>(nextToken.type())];
            tokenType = _rowColSQLStates[startState][nextToken.type()];

            switch(tokenType)
            {
            case INT:
            {
                Ptree[DATA_TYPE].push_back("INT");
                break;
            }
            case FLOAT:
            {
                Ptree[DATA_TYPE].push_back("FLOAT");
                break;
            }
            default:
            {
                Ptree[DATA_TYPE].push_back("CHAR");
            }
            }

            //if a success state is found, save the current state of the PTree and Infixqueue
            if(_SQLAdjacencyMatrix[startState][0] == 1 || (successAtEnd && tokenType == -1))
            {
                isSuccess = true;
                _PTree = Ptree;
                _infix = tempInfixQueue;
            }
        }

        //If a where clause is encountered, set this flag to true.
        // any subsequent tokens will be pushed to the infix queue.
        if(nextToken.type() == WHERE)
            foundWhere = true;

        _preProcessor >> nextToken;
        startState = _SQLAdjacencyMatrix[startState][static_cast<int>(nextToken.type())];
    }
    return isSuccess;
}

//preconditions: none
//postconditions>: if _currentAction is MAKE and a table with the name provided already exists,
//                    return false, otherwise true.
//                 if _currentAction is SELECT, and a table with the provided name does exist,
//                    and an asterisk is entered, or all fields entered exist in the table
//                    and, if a where clause was provided, all relational fields must also exist in the table
//                    return true, otherwise false.
//                 if _currentAction is INSERT, and a table with the provided name does exist,
//                    and, the number of values entered is the same as the number of fields in the target table,
//                    return true, otherwise false.
bool SQL::verifyPTree()
{
    bool isValid = true;
    if(_currentAction == MAKE)
    {
        if(_tables.contains(_currentTableName))
        {
            cout << "Error, Table: " << _currentTableName << " already exists." << endl;
            isValid = false;
        }
        else
            isValid = true;

        if(_PTree[FIELD_NAME].size() != _PTree[DATA_TYPE].size())
        {
            cout << "Cannot make table. Error when matching data types and field names." << endl;
            isValid = false;
        }
        else
            isValid = true;
    }
    else if(_currentAction == SELECT)
    {
        if(_tables.contains(_PTree[DB_NAME].at(0)))
        {
            _currentTable = _tables[_currentTableName];
            if(!_PTree.contains(ASTERISK) && !verifyFields(_PTree[FIELD_NAME]))
                isValid = false;

            if(_PTree.contains(WHERE) && !verifyFields(_PTree[RELATIONAL_FIELD]))
                isValid = false;
        }
        else
        {
            cout << "Error, cannot select from : " << _PTree[DB_NAME] << ", because no such table exists." << endl;
            isValid = false;
        }
    }
    else if(_currentAction == INSERT)
    {
        if(_tables.contains(_PTree[DB_NAME].at(0)))
        {
            _currentTable = _tables[_currentTableName];
            if(_values.size() !=_currentTable->getFieldNames().size())
            {
                cout << "Error, Table: " << _currentTableName
                     << " contains " <<  _currentTable->getFieldNames().size()
                     << " fields, but only " << _values.size()  << " values were specified." << endl;
                isValid = false;
            }
            else
            {
                for(unsigned i = 0; i < _values.size(); ++i)
                {
                    if(!_currentTable->isValueValid(_values.at(i),i))
                        isValid = false;
                }
            }
        }
        else
        {
            cout << "Error, cannot insert into : " << _currentTableName << ", because no such table exists." << endl;
            isValid = false;
        }
    }
    else
    {
        cout << "Error, command invalid." << endl;
        isValid = false;
    }
    return isValid;
}

//preconditions: none
//postconditions: if the table corresponding to that of the current query contains
// all fields in the provided vector, return true, otherwise return false.
bool SQL::verifyFields(vector<string>& fields)
{
    bool fieldsExist = true;
    for(unsigned i = 0; i < fields.size() && fieldsExist; ++i)
        if(!_tables[_PTree[DB_NAME].at(0)]->containsField(fields.at(i)))
            fieldsExist = false;
    return fieldsExist;
}

//preconditions: none
//postconditions: the _infix queue will be converted to postfix notation and placed in _postfix,
// using the conventional shunting yard algorithm outlined below. If there was a parentheses mismatch,
// return false, otherwise, if the conversion was successful, return true.
bool SQL::toPostfix()
{
    /* This implementation does not implement composite functions,functions with variable number of arguments, and unary operators. */
//    while there are tokens to be read:
//        read a token.
//        if the token is a number, then:
//            push it to the output queue.
//        if the token is a function then:
//            push it onto the operator stack
//        if the token is an operator, then:
//            while ((there is a function at the top of the operator stack)
//                   or (there is an operator at the top of the operator stack with greater or equal precedence)
//                  and (the operator at the top of the operator stack is not a left bracket):
//                pop operators from the operator stack onto the output queue.
//            push it onto the operator stack.
//        if the token is a left bracket (i.e. "("), then:
//            push it onto the operator stack.
//        if the token is a right bracket (i.e. ")"), then:
//            while the operator at the top of the operator stack is not a left bracket:
//                pop the operator from the operator stack onto the output queue.
//            pop the left bracket from the stack.
//            /* if the stack runs out without finding a left bracket, then there are mismatched parentheses. */
//    if there are no more tokens to read:
//        while there are still operator tokens on the stack:
//            /* if the operator token on the top of the stack is a bracket, then there are mismatched parentheses. */
//            pop the operator from the operator stack onto the output queue.
//    exit.

    Stack<Token> operatorStack;
    bool isValid = true;

    while(!_infix.empty())
    {
        Token current = _infix.pop();
        if(current.priority() == -1)
        {
            _postfix.push(current);
        }
        else if(current.type() != OPEN_P && current.type() != CLOSE_P)
        {
            while((!operatorStack.empty()) && (operatorStack.top().priority() >= current.priority())
                  && (operatorStack.top().type() != OPEN_P))
            {
                _postfix.push(operatorStack.pop());
            }
            operatorStack.push(current);
        }
        else if(current.type() == OPEN_P)
        {
            operatorStack.push(current);
        }
        else if(current.type() == CLOSE_P)
        {
            while(!operatorStack.empty() && operatorStack.top().type() != OPEN_P)
            {
                _postfix.push(operatorStack.pop());
            }
            if(operatorStack.empty())
                isValid = false;
            else
                operatorStack.pop();
        }
    }
    while(!operatorStack.empty())
    {
        Token temp = operatorStack.pop();
        if(temp.type() == OPEN_P || temp.type() == CLOSE_P)
            isValid = false;
        else
        {
            _postfix.push(temp);
        }
    }
    return isValid;
}

//preconditions: the Ptree is valid, any required fields or tables exist.
//postconditions>: if _currentAction is MAKE, create a new table with the fields entered and insert it to _tables
//                 if _currentAction is SELECT, process the selection, additonal work is required here.
//                 if _currentAction is INSERT, insert the specified values to the matching fields of the target table.
void SQL::executeCommand()
{
    if(_currentAction == MAKE)
    {
        _tables.insert(_currentTableName,new Table(_currentTableName,_PTree[FIELD_NAME],_PTree[DATA_TYPE]));
    }
    else if(_currentAction == INSERT)
    {
        _currentTable->insertInto(_values);
    }
    else if(_currentAction == SELECT)
    {
        processSelect();
    }
}

//preconditions: the current action must be SELECT
//postconditions: if there are items in the _postfix queue, i.e., there was a where clause.
// the following algorithm will be used to obtain a set where all contained record numbers
// meet the conditions in the where clause.
void SQL::processSelect()
{
    Set<int> *result = nullptr;

    //the selection contains a where clause
    if(_PTree.contains(WHERE))
    {
        //while _postFix queue is not yet empty:
        //  pop the front token:
        //  if the front token is NOT an operator
        //      push it to the operand stack.
        //  else if, the token IS a RELATIONAL_OPERATOR,
        //      a) pop 2 tokens from the operand stack.
        //         notice that the SECOND token popped will be the field_name.
        //      b) obtain a set from the (field, RELATIONAL_OPERATOR, value)
        //         and push the newly created set to setStack.
        //  else, the token IS a LOGICAL_OPERATOR
        //      a) pop 2 sets from the setStack
        //      b) obtain a new set that is either the intersection or
        //         union of the 2 sets, and push that set back to the setStack.

        //postconditions, one set will be obtained that meets all of the
        // conditions specified in the where clause.

        Stack<Token> operandStack;
        Stack<Set<int>*> setStack; //use a stack of pointers to prevent potentially expensive B+Tree copying
        while(!_postfix.empty())
        {
            Token current = _postfix.pop();
            if(current.group() == OPERAND)
            {
                operandStack.push(current);
            }
            else if(current.group() == RELATIONAL_OP)
            {
                Token rhs = operandStack.pop();
                Token lhs = operandStack.pop();
                setStack.push(_currentTable->createSet(lhs.token_str(),current.type(),rhs.token_str()));
            }
            else if(current.group() == LOGICAL_OP)
            {
                Set<int> *rhs = setStack.pop();
                Set<int> *lhs = setStack.pop();

                //Either find union or intersection and save to result
                if(current.type() == AND)
                {
                    //The intersection of lhs / rhs will be stored in result
                    Set<int> *intersection = new Set<int>;
                    lhs->intersection(*rhs, *intersection);
                    delete rhs;
                    delete lhs;
                    setStack.push(intersection);
                }
                else if(current.type() == OR)
                {
                    //rhs will be merged into lhs
                    lhs->operator+=(*rhs);
                    delete rhs;
                    setStack.push(lhs);
                }
            }
        }
        result = setStack.pop();
    }
    //Print the selection object.
    Selection *s = new Selection(result,_currentTableName, _currentTable,
                                 !_PTree.contains(WHERE),_PTree.contains(ASTERISK));
    s->setFields(_PTree[FIELD_NAME],_PTree.contains(ASTERISK));
    cout << *s << endl;
    delete s;
}
