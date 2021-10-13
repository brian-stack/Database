#include "table.h"

//preconditions: tableName.txt must exist in the project directory.
//postconditions: constructs a table by opening the saved file,
// by reading the fields from the first record and the data types from the second.
Table::Table(const string& tableName)
{
    _recordCount = 0;
    string filePath = tableName + ".txt";
    ifstream fileIn(filePath.c_str(), ios_base::binary);
    assert(fileIn.is_open());
    _tableName = tableName;
    _fileName = filePath;

    //get the field names from the first record.
    _rec.readFromFile(fileIn,_fieldNames);

    //create the field name indicies map to save the
    // relative positions of fields in the file.
    int index = 0;
    for(string s : _fieldNames)
    {
        _fieldNameToIndex.insert(s,index);
        index++;
    }

    //get the data types from the second record.
    _rec.readFromFile(fileIn,_dataTypes);

    //create the fields vector from the data types and field names.
    for(unsigned i = 0; i < _fieldNames.size(); ++i)
    {
        Field f(_fieldNames.at(i),_dataTypes.at(i));
        _fields.push_back(f);
    }

    fileIn.close();
    populateIndiciesMaps();
}

//preconditions: tableName.txt must exist in the project directory.
//postconditions: constructs a new table with the specified fields,
// writes the field names to the first record, and data types to the second.
Table::Table(const string& tableName, const vector<string>& fieldNames, const vector<string>& dataTypes)
{
    _dataTypes = dataTypes;
    _fieldNames = fieldNames;
    _fileName = tableName + ".txt";
    _tableName = tableName;
    for(size_t i = 0; i < fieldNames.size(); ++i)
    {
        Field f(fieldNames.at(i),dataTypes.at(i));
        _fields.push_back(f);
    }

    int index = 0;
    for(string &s : _fieldNames)
    {
        _fieldNameToIndex.insert(s,index);
        index++;
    }
    //create a new file with tableName and write the fieldNames to the first record.
    _fileOuts.open(_fileName.c_str(),ios_base::binary);
    assert(_fileOuts.is_open());
    _rec.writeToFile(_fileOuts, _fieldNames);
    _rec.writeToFile(_fileOuts, _dataTypes);
    _fileOuts.close();
}

bool Table::isValueValid(const Token &t, const size_t& index)
{
    bool valid = false;

    if(index < _fields.size())
    {
        if(_fields.at(index).fieldType() == CHAR)
            valid = true;
        else if(_fields.at(index).fieldType() == FLOAT && (t.type() == FLOAT || t.type() == INT))
            valid = true;
        else if(_fields.at(index).fieldType() == INT && t.type() == INT)
            valid = true;
    }
    if(!valid)
    {
        cout << "In isValueValue: " << endl << "index = " << index << " _fields.at(index) = "
             << (_fields.at(index).fieldTypeAsString()) << endl;
        cout << "t.type() = " << t.type() << endl;
    }
    return valid;
}

//postconditions: the indicie maps of this table will be initially
// populated by reading records from disk.
void Table::populateIndiciesMaps()
{
    _fileIns.open(_fileName,ios_base::binary);
    _fileIns.seekg(_fileIns.beg + (2 * MAX_VALUES * MAX_CHARS));

    vector<string> recordValues;
    while(_rec.readFromFile(_fileIns,recordValues))
    {
        for(unsigned i = 0; i < recordValues.size(); i++)
        {
            varType temp = createVarientType(recordValues.at(i),_fieldNameToIndex[_fieldNames.at(i)]);
            _indiciesMap[_fieldNameToIndex[_fieldNames.at(i)]].insert(temp,_recordCount+2);
        }
        recordValues.clear();
        _recordCount++;
    }
    _fileIns.close();
}

//postconditions: the current value of _recordCount will be added to the indicie maps of this table,
// where the ith item in values is the key of the ith multimap in _indiciesMap.
void Table::updateIndiciesMaps(vector<string>& values)
{
    for(unsigned i = 0; i < values.size(); ++i)
    {
        varType temp = createVarientType(values.at(i),_fieldNameToIndex[_fieldNames.at(i)]);
        _indiciesMap[i][temp].insert(_recordCount-1);
    }
}

//preconditions: fieldIndex must specify a field in _fields,
// value must be convertable to the type of the field.
//postconditions: returns a varType object, whose active member is set,
// based on the type of the field, by possibly converting value to int or double.
varType Table::createVarientType(const string& value, int fieldIndex)
{
    typeID dataType = _fields.at(fieldIndex).fieldType();
    varType var;
    if(dataType == CHAR)
    {
        var._s = value;
        var._type = STRING;
    }
    else if(dataType == FLOAT)
    {
        var._d = stod(value);
        var._type = DOUBLE;
    }
    else if(dataType == INT)
    {
        var._i = stoi(value);
        var._type = NUMBER;
    }
    return var;
}

//preconditions: fieldName must exist in this table
//postconditions: returns a set containing the record numbers from the index map of fieldName field.
// such that: (fieldName RELATIONAL_OPERATOR value) is true.
// If there are no record where this condition is true, returns the empty set.
Set<int>* Table::createSet(string fieldName, typeID opType, string value)
{
    Set<int> * result = new Set<int>;
    MMap<varType,int>::Iterator endIt = _indiciesMap[_fieldNameToIndex[fieldName]].end();
    varType compare = createVarientType(value,_fieldNameToIndex[fieldName]);

    switch (opType)
    {
    case LESS:
    {
        MMap<varType,int>::Iterator beginIt = _indiciesMap[_fieldNameToIndex[fieldName]].begin();
        while(beginIt != endIt && ((*beginIt).key() < compare))
        {
            result->operator+=((*beginIt).values());
            ++beginIt;
        }
        break;
    }
    case GREATER:
    {
        MMap<varType,int>::Iterator atUpperBound = _indiciesMap[_fieldNameToIndex[fieldName]].upperBound(compare);
        while(atUpperBound != endIt)
        {
            *(result) += (*atUpperBound).values();
            ++atUpperBound;
        }
        break;
    }
    case EQUALS:
    {
        if(_indiciesMap[_fieldNameToIndex[fieldName]].contains(compare))
            *result = _indiciesMap[_fieldNameToIndex[fieldName]][compare];
        break;
    }
    case LESS_OR_EQUAL:
    {
        MMap<varType,int>::Iterator beginIt = _indiciesMap[_fieldNameToIndex[fieldName]].begin();
        while(beginIt != endIt && ((*beginIt).key() <= compare))
        {
            result->operator+=((*beginIt).values());
            ++beginIt;
        }
        break;
    }
    case GREATER_OR_EQUAL:
    {
        MMap<varType,int>::Iterator atLowerBound = _indiciesMap[_fieldNameToIndex[fieldName]].lowerBound(compare);
        while(atLowerBound != endIt)
        {
            result->operator+=((*atLowerBound).values());
            ++atLowerBound;
        }
        break;
    }
    default:
    {
        cout << "Error, operation not defined." << endl;
        break;
    }
    }
    return result;
}

//preconditions: fields must specify fields of this table.
//postconditions: if allFields is true, all fields of all records
// of this table will be printed, otherwise, the fields specified
// will be printed, for each record of this table.
void Table::printAll(ostream& outs, const vector<string>& fields, bool allFields)
{
    vector<string> fieldsToPrint = (allFields) ? _fieldNames : fields;
    _fileIns.open(_fileName,ios_base::binary);

    //seek to the first actual record since record 0 and 1 will be
    // used to store field names and data types.
    _fileIns.seekg(_fileIns.beg + (2 * MAX_VALUES * MAX_CHARS));

    for(const string &s : fieldsToPrint)
        outs << s << ", ";
    outs << endl << string(50,'-') << endl;

    vector<string> fromFile;
    unsigned index = 2;
    while(_rec.readFromFile(_fileIns,fromFile))
    {
        outs << index << " : ";
        for(const string &s : fieldsToPrint)
        {
            unsigned index = static_cast<unsigned>(_fieldNameToIndex[s]);
            outs << fromFile.at(index) << ", ";
        }
        outs << endl;
        fromFile.clear();
        ++index;
    }
    _fileIns.close();
}

//preconditions: selection is a valid set of positions in the file
//postconditions: if allFields is true, all fields of the records specified in selection
//  will be printed to the console, otherwise, for each record in selection,
//  the fields specified in the vector: fields will be printed.
void Table::printSelection(ostream& outs, const vector<string>& fields, Set<int>& selection, bool allFields)
{
    vector<string> fieldsToPrint = (allFields) ? _fieldNames : fields;
    _fileIns.open(_fileName,ios_base::binary);
    Set<int>::Iterator it = selection.begin();
    Set<int>::Iterator end = selection.end();

    for(string &s : fieldsToPrint)
        outs << s << ", ";
    outs << endl << string(50,'-') << endl;

    while(it != end)
    {
        vector<string> fromFile;
        if(_rec.readFromFile(_fileIns,fromFile,*it))
        {
            outs << *it << " : ";
            for(string &s : fieldsToPrint)
            {
                int index = _fieldNameToIndex[s];
                outs << fromFile.at(index) << ", ";
            }
            outs << endl;
        }
        ++it;
    }
    _fileIns.close();
}

//postconditions: the strings stored in values will be written to file.
// then, the indicie map will be updated.
void Table::insertInto(vector<Token>& values)
{
    vector<string> toFile;
    for(int i = 0; i < values.size(); ++i)
        toFile.push_back(values.at(i).token_str());

    _fileOuts.open(_fileName.c_str(),ios_base::binary | ios_base::app);
    _recordCount = _rec.writeToFile(_fileOuts, toFile);
    _fileOuts.close();

    updateIndiciesMaps(toFile);
}
