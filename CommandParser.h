#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

class CommandParser
{
public:
	CommandParser(); //Dummy constructor

	//0 - Exit, 1 - Build Index, 2 - Index Search, 3 - Sequential Search
	unsigned short parseCommand();
};

#endif