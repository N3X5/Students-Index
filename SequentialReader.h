#ifndef SEQUENTIAL_READER_H
#define SEQUENTIAL_READER_H

#include <string>
#include <fstream>

/*
* The sequential reader has two main functions:
*	1) Read the file line by line and return the relevant student record
*	2) Provide access to the current get Pointer of the ifstream used by the class
*/
class SequentialReader
{
private:
	std::ifstream* input; //Our current input file
	void closeReader(); //Close the reader; Called either on destruction of the instance || when we've read  through the whole file
public:
	SequentialReader(); //Open the current DB file
	~SequentialReader(); //Close the stream and delete input ifstream

	std::string readRecord(); //Read a single record
	std::streampos tellGetPointer(); //Return the current get Pointer location of our ifstream
};

#endif