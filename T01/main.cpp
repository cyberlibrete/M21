#include <iostream>
#include <string>
#include <vector>
#include <fstream>


namespace standart							// standart for boot
{
	bool _programm_information();			// after start show programm information
	void clear();							// terminal clearning
	bool _exit();							// stop programm
}

namespace func								// functional space
{
	struct data								// personal data structure
	{
		std::string name;
		std::string surname;
		std::string date;
		int pay;
	};

	std::vector<func::data> dataList;

	bool showHelp();						// show all commands description
	bool add();								// add new data into file
	bool showData();						// read file with data and print into terminal
}

bool func::showHelp()
{
	std::cout	<< "\nCOMMAND\tDESCRIPTION" << std::endl
				<< "-----------------------------------" << std::endl
				<< "clear\t- Clear the terminal screen" << std::endl
				<< "exit\t- Exit the shell" << std::endl
				<< "help\t- Display information about builtin commands" << std::endl
				<< std::endl
				<< "add\t- Add new data" << std::endl
				<< "list\t- Show data list" << std::endl
				<< std::endl;
	
	return true;
}

void standart::clear()
{
#ifdef _WIN32								// for WINDOWS
	system("cls");
#elif __unix__								// for UNIX
	system("clear");
#endif
}


// ----------------------------------------------------------------------------------------------------- //
int main()
{
	bool FILE_EXISTS = false;
	const char* _FILEWITHDATAPATH = "data.txt";
	/**
	 * 		1	Show programm information
	 * 		2	Show spacial commands `exit` and `clear`
	 * 		-<>- cycle
	 * 		3	Command request
	 * 		4	Check command distr.cmd() --> func
	 */

	std::fstream dataFile(_FILEWITHDATAPATH, std::ios::in);
	FILE_EXISTS = dataFile.is_open();


	std::string _command;
	standart::clear();
	standart::_programm_information();

	// 1. print programm information
	while (true)
	{
		// get command | input command
		std::cout	<< "_$> ";
		
		std::getline(std::cin, _command);

		if (_command == "exit")
			break;
		else if (_command == "clear")
			standart::clear();
		else if (_command == "help")
			func::showHelp();
		//else if (_command == "add")
			//func::add();
		else if (FILE_EXISTS && (_command == "list"))
			func::showData();
		else
			std::cout << "[Command not found. Enter \"help\" to print available command list]\n";
	}
	return 0;
}

// ===================================================================================================== //

bool standart::_programm_information()
{

	std::cout	<< "Track payments" << std::endl
				<< "Version 0.1.0" << std::endl
				<< "Code on GitHub: https://github.com/cyberlibrete/M21" << std::endl;


	return true;
}