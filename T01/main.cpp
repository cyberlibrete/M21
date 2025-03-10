#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct record
{
	std::string		name;
	std::string		surname;
	std::string		paymantDate;
	std::string		amount;

	bool set_data()
	{
		
		std::cout << "Name: ";
		std::cin >> this->name;
		//std::cout << std::endl;

		std::cout << "Surname: ";
		std::cin >> this->surname;
		//std::cout << std::endl;

		do {
			std::cout << "Payment date (DD.MM.YYYY): ";
			std::cin >> this->paymantDate;
		} while (!DATE_FORMATTING());

		do {
			std::cout << "Amount: ";
			std::cin >> this->amount;

			if (!is_integer(this->amount))
				std::cerr << "! Incorrect value !" << std::endl;

		} while (true);
		
		return true;
	}

	void printData()
	{
		std::cout	<< this->name << ' '
					<< this->surname << ' '
					<< this->paymantDate << ' '
					<< this->amount << " RUB"
					<< std::endl;
	}
	bool clear()
	{
		this->name.clear();
		this->surname.clear();
		this->paymantDate.clear();
		this->amount.clear();

		return true;
	}

protected:

	int DEC_POWER(__int8 _power)
	{
		int output = 1;
		while (_power > 0)
		{
			output *= 10;
			_power--;
		}
		return output;
		
	}

	std::string set_format(__int8 fill, int _value)
	{
		std::string output;
		int count = 0;

		while (_value > 0)
		{
			output = char((_value % 10) + '0') + output;
			count++;
			_value = _value / 10;
		}

		while (output.length() < fill)
			output = '0' + output;
		
		return output;
	}

	bool DATE_FORMATTING()
	{
		__int8 days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		int _date[3] = {0, 0, 0};
		__int8 d_position = 2;
		__int8 _dec = 0;

		int idx = this->paymantDate.length() - 1; // from RIGHT to LEFT
		while (idx >=0)
		{
			if (this->paymantDate[idx] >= '0' && this->paymantDate[idx] <= '9')
			{
				_date[d_position] += (this->paymantDate[idx] - '0') * DEC_POWER(_dec);
				_dec++;
			} else if (_dec > 0)
			{
				d_position--;
				_dec = 0;
			}

			if (d_position < 0) break;

			idx--;
		}

		if (_date[1] == 2 && ((_date[2] % 4 == 0 && _date[2] % 100 != 0) || (_date[2] % 400 == 0)))
			days[1] = 29;
		
		if (_date[1] < 1 || _date[1] > 12)
		{
			std::cerr	<< "| INVALID DATE ! " << std::endl
						<< "| The valid range for the month should be from 1 to 12" << std::endl << std::endl;
			return false;
		}

		if (_date[0] > int(days[_date[1] - 1]))
		{
			std::cerr	<< "| INVALID DATE ! " << std::endl
						<< "| The valid range for the day should be from 1 to " << days[_date[1] - 1] << std::endl << std::endl;
			return false;
		}

		if (_date[2] < 1000 || _date[2] > 2026)
		{
			std::cerr	<< "| INVALID DATE ! " << std::endl
						<< "| The valid range for the year should be from 1000 to 2026" << std::endl << std::endl;
			return false;
		}

		this->paymantDate = set_format(2, _date[0]) + '.' + set_format(2, _date[1]) + '.' + set_format(4, _date[2]);

		return true;
	}

	bool is_integer(std::string& _value)
	{
		int output = 0;
		int idx = _value.length() - 1;

		while (idx >= 0)
		{
			if (_value[idx] < '0' || _value[idx] > '9')
				return false;

			output += int(_value[idx] - '0') * DEC_POWER(_value.length() - idx);

			idx--;
		}
		
		return true;
	}

public:
	bool writeData(const char* _filename)
	{
		std::fstream file(_filename, std::ios::app);
		if (!file.is_open())
		{
			std::cerr << "! FILE OPENING ERROR !" << std::endl << std::endl;
			return false;
		}

		file << this->name << ' ' << this->surname << ' ' << this->paymantDate << ' ' << this->amount << std::endl;
		file.close();
		return true;
	}

};

struct statement
{
private:
	std::vector<record> _journal;
	
public:
	bool loadData()
	{
		std::ifstream file("report.txt");
		
		if (file.is_open())
		{
			this->_journal.clear();
			record CurrentRecord;
			while (file >> CurrentRecord.name >> CurrentRecord.surname >> CurrentRecord.paymantDate >> CurrentRecord.amount)
			{
				this->_journal.push_back(CurrentRecord);
				CurrentRecord.clear();
			}
			std::cout << ">>> SIZE OF JOURNAL: " << this->_journal.size() << std::endl;
			file.close();
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
				<< "add\t- add a new entry" << std::endl
				<< "clear\t- clear teminal" << std::endl
				<< "exit\t- exit program" << std::endl << std::endl;
	
}

bool getCommand(std::string& _cline)
{
	do {
		std::cout	<< "M21T01 $ ";
		
		std::cin >> _cline;
		
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
	} while (_cline == "help" || _cline == "clear" || _cline == "cls" || _cline.length() == 0);

	return true;
}



int main ()
{
	// ---------------------------
	statement LOADED_DATA;
	record newRecord;
	std::string _cmd;
	// ---------------------------

#ifdef _WIN32
		system("cls");
#elif defined(__linux__) || defined(__APPLE__)
		system("clear");
#endif

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
		else if (_cmd == "add")
		{
			newRecord.set_data();
			newRecord.writeData("report.txt");
		}
		else
		{
			std::cerr << "Command \"" << _cmd << "\" not found\nTry command \"help\" to print available commands for this program" << std::endl << std::endl;
		}
	}
	
}

