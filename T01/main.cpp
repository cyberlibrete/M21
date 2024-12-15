#include <iostream>
#include <string>
#include <vector>
#include <fstream>


namespace standart // standart for boot
{
	bool setProcces(); // for command input
	bool _programm_information(); // after start show programm information
	bool fileexist(std::fstream &file); // check file exist
	bool clear(); // terminal clearning
	bool exit(); // stop programm
}

namespace func // functional space
{
	struct data // personal data structure
	{
		std::string name;
		std::string surname;
		std::string date;
		int pay;
	};

	bool showHelp(); // show all commands description
	bool add(); // add new data into file
	bool showData(); // read file with data and print into terminal
}

bool func::showHelp()
{
	std::cout	<< "\nCOMMAND\tDESCRIPTION" << std::endl
				<< "-----------------------------------" << std::endl
				<< "clear\t- clear terminal" << std::endl
				<< "exit\t- stop programm and exit" << std::endl
				<< std::endl
				<< "add\t- add new data" << std::endl
				<< "list\t- show data list" << std::endl
				<< std::endl;
	
	return true;
}

bool standart::clear()
{

#ifdef _WIN32 // for WINDOWS
	system("cls");
#elif __unix__ // for UNIX
	system("clear");
#endif

	return true;
}

int main()
{
	/**
	 * 		1	Show programm information
	 * 		2	Show spacial commands `exit` and `clear`
	 * 		-<>- cycle
	 * 		3	Command request
	 * 		4	Check command distr.cmd() --> func
	 */
	return 0;
}