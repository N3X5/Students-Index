#ifndef INDEX_H
#define INDEX_H

#include <fstream>
#include <vector>
#include <string>

/*Index class for building and searching and index for a given students DB file.
Please take note that this class is meant to be used only for either searching or building an
index file in a given instance. Trying to do both at once may cause strange behavious due to both an ofstream
and ifstream being opened at the same time. This program is build to avoid such behaviour.*/
class Index 
{
private:
	std::ofstream* index_write; //Write stream for building an index
	std::ifstream* index_read; //Read stream for exploring the index
	std::vector<std::vector<long long>> memory_index; //Memory index used when building the index
	/*Const size of an index record in bytes; one record contains two LLONG_MAX data elements for the FN and the getP in the DB
	file. Additionally, there is a delimiter (1 byte) and two new line bytes.*/
	const size_t student_index_size = std::to_string(LLONG_MAX).size() * 2 + 3;

	//Search for a given index by the corresponding FN and return the line on which that record is located
	long long FN_index_binary_search(long long left, long long right, unsigned long long FN);
public:
	Index(); //Constructor to init index_write & index_read
	~Index(); //Destructor to destroy index_write & index_read streams if the are opened
	 
	void build(); //Build an index
	/*Search for the get pointer of a given student record in the DB file. Returns -1 if no such pointer is found.
	A new index is built if one does not exist previously. In such a case, the func returns -1 as well.*/
	std::streampos search(unsigned long long); //
};

#endif