#include "record.h"

//preconditions: none
//postconditions: fills _record with null terminators
void Record::initBuffer()
{
    for(int i = 0; i < MAX_VALUES; i++)
        for(int j = 0; j < MAX_CHARS; j++)
            _record[i][j] = '\0';
}

//preconditions: fouts must be open.
//postconditions: copy the contents of theFields to _record,
// and write to file; returns the record number.
int Record::writeToFile(ofstream& fouts, const vector<string>& theFields)
{
    initBuffer();
    assert(fouts.is_open());
    assert(theFields.size() < MAX_VALUES);

    for(unsigned i = 0; i < theFields.size(); i++)
        strncpy(_record[i],theFields.at(i).c_str(),MAX_CHARS);
    fouts.write(&_record[0][0],_charsPerRecord);

    return (fouts.tellp() / _charsPerRecord);
}

//preconditions: fins must be open.
//postconditions: returns the number of characters read with this call to read.
//  if we are not eof, returns theFields containing all values from the record.
int Record::readFromFile(ifstream& fins, vector<string>& theFields, int recordNum)
{
    initBuffer();
    assert(fins.is_open());

    //if recordNum < 0, continue reading from the
    // last position saved internally by the ifstream.
    if(recordNum >= 0)
        fins.seekg((recordNum * _charsPerRecord), ios_base::beg);

    if(fins.read(&_record[0][0], _charsPerRecord))
    {
        for(int i = 0; i < MAX_VALUES; i++)
        {
            string item(_record[i]);
            if(item.size() > 0) theFields.push_back(item);
        }
    }
    return fins.gcount();
}
