/*************************************************************************************************************************
 * Author: Brian Stack
 * Assignment: SQL Database
 * Date: 12/10/18
 * Class: CS 8
 * CRN: 74231
 * ***********************************************************************************************************************
 * This SQL Database allows the user to create, save and load, saved tables from previous sessions.
 *  Additionally, the user may insert a new record into an existing table,
 *  or select a number of records where a certain condition is true.
 *
 *  The following commands are defined:
 *      <MAKE TABLE> : {  make table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME>...]  }
 *      <INSERT> : {  insert <INTO> <TABLE_NAME> values <VALUE> [, <VALUE>...]      }
 *      <SELECT> : {  select <* | FIELD_NAME> [, ,FIELD_NAME>...]
 *                    from <TABLE_NAME>
 *                    where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>
 * 						[<LOGICAL_OPERATOR>
 * 						<FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>...]
 ************************************************************************************************************************/
#include "sql.h"
using namespace std;
int main()
{
    const char thePath[] = "batch_file.txt";
    SQL sql(thePath);
    //SQL sql2;
    return 0;
}
