#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define TEST

struct record
{
	std::string		name;
	std::string		surname;
	std::string		paymantDate;
	std::string		amount;

	record(){};
	record(const char* _name, const char* _surname, const char* _paymantDate, int _amount)
	{
		this->name = _name;
		this->surname = _surname;
		this->paymantDate = _paymantDate;
	}

	void printData()
	{
		std::cout	<< this->name << ' '
					<< this->surname << ' '
					<< this->paymantDate << ' '
					<< this->amount << " RUB"
					<< std::endl;
	}
};

struct statement
{
private:
	std::vector<record> _journal;
	
public:
	bool loadData()
	{
		std::fstream file("report.txt", std::ios::in);
		
		if (file.is_open())
		{
			this->_journal.clear();
			record CurrentRecord;
			while (!file.eof())
			{
				file >> CurrentRecord.name >> CurrentRecord.surname >> CurrentRecord.paymantDate >> CurrentRecord.amount;
				this->_journal.push_back(CurrentRecord);
			}
			std::cout << ">>> SIZE OF JOURNAL: " << this->_journal.size() << std::endl;
			return true;
		}
		return false;
	}

	statement()
	{
		loadData();
	}

	void printAnalytics()
	{
		loadData();
		std::cout	<< "Number of payout records: " << this->_journal.size() << std::endl;
	}
	
	bool printList()
	{
		int idx = 0;
		while (idx < this->_journal.size())
		{
			std::cout << idx << '\t';
			_journal[idx].printData();
			idx++;
		}
		return true;
	}
};

void printHelp()
{
	std::cout	<< "list\t- show a list of payouts" << std::endl
				<< "add\t- add a new entry" << std::endl << std::endl;
	
}

bool getCommand(std::string& _cline)
{

	std::cout	<< "Enter the command" << std::endl
				<< "$ ";
	
	std::getline(std::cin, _cline);
	
	if (_cline == "stop" || _cline == "exit")
		return false;

	else if (_cline == "help")
		printHelp();

#ifdef _WIN32
	else if (_cline == "clear" || _cline == "cls")
		system("cls");
#elif defined(__linux__) || defined(__APPLE__)
	else if (_cline == "clear" || _cline == "cls")
		system("clear");
#endif
	
	return true;
}

int main ()
{
	// ---------------------------
	statement LOADED_DATA;
	
	std::string _cmd;
	// ---------------------------

	std::cout	<< "MODULE 21 | TASK 01" << std::endl
				<< "Payment accounting statement" << std::endl
				<< "----------------------------" << std::endl << std::endl;
	
	while (getCommand(_cmd))
	{
		if (_cmd == "list")
		{
			LOADED_DATA.printAnalytics();	
			LOADED_DATA.printList();
		}
		/*else if (_cmd == "add")
		{
		//enter name

		//enter surname
		
		//enter date
		//		+ CTRL

		//enter amount
		//		+ CTRL
			
		}*/
	}
	
}

