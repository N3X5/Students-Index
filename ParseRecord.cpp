#include "ParseRecord.h"
#include "Definitions.h"

//A simple function to check if a record is found
bool recordIsValid(std::string & record)
{
	if (strcmp(record.c_str(), RECORD_NOT_FOUND))
		return true;

	return false;
}

//A simple function to check if a record matches a given FN
bool isMatchingRecord(std::string & record, unsigned long long & FN)
{
	if (stoll(record.substr(0, record.find(" "))) == FN)
		return true;

	return false;
}