#include "Definitions.h"
#include "Index.h"
#include "SequentialReader.h"
#include "ParseRecord.h"

#include <algorithm>
#include <iostream>

using namespace std;

//Binary search for the line on which a given FN index record is located. Returns -1 if no such index record is found.
long long Index::FN_index_binary_search(long long left, long long right, unsigned long long FN)
{
	if (right >= left) {
		long long middle = left + (right - left) / 2; //Calculate the middle index record line
		index_read->seekg(student_index_size*middle, ios::beg); //Seek to the start of that line in the file
		string record; //Container for a record
		getline(*index_read, record); //Read the record into the string
		unsigned long long parsed_FN = stoll(record.substr(0, record.find(":"))); //Parse the FN of the record

		if (parsed_FN == FN) //If we have a match, return the middle line number
			return middle;

		if (parsed_FN > FN) //If our FN is bigger than the one in the middle, search in the first half of the file
			return FN_index_binary_search(left, middle - 1, FN);
 
		//Else search in the second half of the file
		return FN_index_binary_search(middle + 1, right, FN);
	}
	//No student index record is found
	return -1;
}

//Init our streams as nullptr initially
Index::Index()
{
	index_write = nullptr;
	index_read = nullptr;
}

//If the streams have been opened, close & delete them
Index::~Index()
{
	if (index_write != nullptr)
	{
		index_write->close();
		delete index_write;
	}

	if (index_read != nullptr)
	{
		index_read->close();
		delete index_read;
	}
}

//Build an index file
void Index::build()
{
	index_write = new ofstream(FACULTY_NUMBER_INDEX, ios_base::out); //Init our ofstream

	SequentialReader reader; //New sequential reader

	string record; //Container for a student record
	
	streampos getPointer = reader.tellGetPointer(); //Initial get pointer

	while (recordIsValid(record = reader.readRecord())) //Read the students DB line by line
	{
		//Push the FN and corresponding line get Pointer into our memory index
		memory_index.push_back({stoll(record.substr(0, record.find(" "))), getPointer});

		getPointer = reader.tellGetPointer(); //Acquire the get Pointer of the start of the line of the next student record
	}
	
	//Sort our memory index by the FN of each index
	sort(memory_index.begin(), memory_index.end(), [](const vector< long long >& a, const std::vector< long long >& b) { return a[0] < b[0]; });

	//Write the memory index to a file
	for (vector<long long> student_index : memory_index)
	{
		string FN = to_string(student_index[0]); //Get the FN as a str
		string getPointer_str = to_string(student_index[1]); //Parse the get Pointer as a str
		
		*index_write << FN; //Write the FN

		//Write blank space chars for the remaining difference between the str length of the FN and the LLONG_MAX
		for (size_t i = 0; i < to_string(LLONG_MAX).size() - FN.size(); i++)
			*index_write << ' ';
		
		//Write a ':' delim between the FN and corresponding get Pointer
		*index_write << ':' << getPointer_str;

		//Write blank space chars for the remaining difference between the str length of the get Pointer and the LLONG_MAX
		for (size_t i = 0; i < to_string(LLONG_MAX).size() - getPointer_str.size(); i++)
			*index_write << ' ';

		//End the line of the given student index record
		*index_write << endl;
	}
}

//Search for the get Pointer of a student record in the index file
std::streampos Index::search(unsigned long long FN)
{
	index_read = new ifstream(FACULTY_NUMBER_INDEX, ios_base::in); //Open the index file for reading
	index_read->seekg(0, ios::end); //Seek to the end of the index file
	 
	long long index_size = index_read->tellg(); //Get the length of the index file in bytes

	//If the index does not exist, build a new one and return a record not found failure
	if (index_size == -1)
	{
		build();

		return -1;
	}

	long long number_of_records = index_size / student_index_size; //Calculate the number of index records

	//Binary search for the line on which a given index record is located
	long long result = FN_index_binary_search(0, number_of_records - 1, FN);

	//No such line found => we can assume that an index record for the given FN does not exist
	if (result == -1)
		return -1;

	//Seek to the start of the line of the found student index record
	index_read->seekg(result*student_index_size, ios::beg);

	string index_record; //Container for the sturent index record
	getline(*index_read, index_record); //Read the line into the container

	//Parse only the str of the get Pointer of the given student index record
	index_record.erase(0, index_record.find(":")+1);

	//Parse the get Pointer from the str and return it
	return stoll(index_record);
}
