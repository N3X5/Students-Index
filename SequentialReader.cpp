#include "SequentialReader.h"
#include "Definitions.h"

using namespace std;

SequentialReader::SequentialReader()
{
	input = new ifstream(STUDENT_GRADES_DB); //New input file stream
}

SequentialReader::~SequentialReader()
{
	closeReader(); //Close the stream (if not already closed)
	delete input; //Destroy input ifstream object
}

std::string SequentialReader::readRecord()
{
	string result = RECORD_NOT_FOUND; //Default return if there are no records || or if we reach EOF
	string line; // tmp line to store record

	if (getline(*input, line)) //If we have  a record =>
		result = line; //Assign that record to the result of the func
	else
		closeReader(); //Close the stream if there are no more records || if an error is encountered;

	//^ With referance to the above, please take note that once instance of this class only explores the file once!

	return result;
}

//Return the current get Pointer of the ifstream
std::streampos SequentialReader::tellGetPointer()
{
	return input->tellg();
}

void SequentialReader::closeReader()
{
	input->close(); //Close the stream
}