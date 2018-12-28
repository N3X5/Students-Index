#ifndef PARSE_RECORD_H
#define PARSE_RECORD_H

#include <string>

//A simple function to check if a record is found
bool recordIsValid(std::string &record);

//A simple function to check if a record matches a given FN
bool isMatchingRecord(std::string &record, unsigned long long &FN);

#endif