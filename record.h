#ifndef RECORD_H
#define RECORD_H
#include "constants.h"
#include <string>
#include <vector>
#include <fstream>
class Record
{
public:
    Record() : _charsPerRecord(MAX_CHARS * MAX_VALUES) {}

    //init _record to null
    void initBuffer();

    //return pos in file.
    int writeToFile(ofstream& fouts, const vector<string>& theFields);

    //return gcount.
    int readFromFile(ifstream& fins, vector<string>& theFields, int recordNum = -1);

private:
    char _record[MAX_CHARS][MAX_VALUES];
    int _charsPerRecord;
};

#endif // RECORD_H
