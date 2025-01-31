#include <iostream>
#include <string>

namespace acc
{
	// personal data
	struct personalData
	{
		char			_CHECK_STATUS = 0x00;
		std::string		name;
		std::string		surname;
		std::string		issueDate;
		int				amount;

		personalData();
		personalData(const char* _name, const char* _surname, const char* _issueDate, int _amount);
	};
}

int main ()
{
	std::cout   << "| MODULE 21 | TASK 01" << std::endl
				<< "| Accounting statement" << std::endl;
	

	return 0;
}

// -----------------------------------------------------------
// namespace: acc

acc::personalData::personalData()
{
	this->issueDate = "01.01.2010";
}