#include "Definitions.h"
#include "ParseRecord.h"
#include "CommandParser.h"
#include "SequentialReader.h"
#include "Index.h"

#include <iostream>
#include <string>

using namespace std;

int main()
{
	CommandParser parser; //New command parser; to be used while the program is running

	while (unsigned short command = parser.parseCommand()) //Parse command, which is converted to an integer; terminates when a 0 is received from the parser
	{
		if (command == 1) //Build an index
		{
			Index index; //New index instance
			index.build(); //Build an index

			cout << INDEX_BUILT << endl; //Return a message that the index has been built to the user
		}
		else if (command == 2) //Search for a student via an index
		{
			unsigned long long FN; //Container for a FN
			cin >> FN; //Read a FN from the console

			Index index; //New index instance

			streampos record_location = index.search(FN); //Acquire the get Pointer to the start of the line of a given student record

			//If we don't have a valid get Pointer, return that no record has been found and continue to the next command
			if ((long long)record_location == -1)
			{
				cout << RECORD_NOT_FOUND << endl;
				continue;
			}

			//New reader for the Students DB file
			ifstream student_grades(STUDENT_GRADES_DB, ios_base::in);

			//Seek to the acquire get Pointer location
			student_grades.seekg(record_location, ios::beg);

			string record; //Container for a student record
			getline(student_grades, record); //Read the record into the container

			cout << record << endl; //Print the acquired record

			student_grades.close(); //All is done, close the reader of the Students DB file
		}
		else if (command == 3) //Sequential search for a student
		{
			unsigned long long FN;
			cin >> FN;

			SequentialReader reader; //New sequential reader

			string record; //Container for a student record
			bool recordFound = 0; //To make sure that we have actually found something

			while (recordIsValid(record = reader.readRecord())) //Read the file line by line
			{
				if (isMatchingRecord(record, FN)) //Check for a match
				{
					cout << record << endl; //Print the matching record
					recordFound = 1; //Found a match

					break; //No need for further read operations, so break the loop; The sequential explorer will be closed as soon as we leave scope
				}
			}

			if (!recordFound) //No such record found for the given FN || or an error occured
				cout << RECORD_NOT_FOUND << endl;
		}
	}

	return 0;
}