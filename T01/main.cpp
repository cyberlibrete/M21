#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

// INIT
bool add_data();
bool show_data();
bool processCommandChain(std::vector<std::string>& _cmd);


struct data
{
	std::string name;
	std::string surnmae;
	int bdate[3] = {1, 1, 2024};
};



int main (int argc, char* argv[])
{
	
	std::vector<std::string>	__cmd;
	int							idx = 1;
	std::string value;
	while (idx < argc)
	{
		value = *(argv + idx);
		//std::cout << idx << '\t' << *(argv + idx) << std::endl;
		__cmd.push_back(value);
		//std::cout << idx << '\t' << value << '\t' << value.length() << std::endl;
		idx++;
	}

	while (true)
	{
		

		idx = 0;
		while (idx < __cmd.size())
		{
			std::cout << "check :: " << __cmd[idx] << std::endl;
			for (std::string _token : {"stop", "exit"})
			{
				if (__cmd[idx] == _token)
				{
					std::cout << _token << " [CHECKED]" << std::endl;
					return 0;
				}
			}
			// if (__cmd[idx] == "exit" || __cmd[idx] == "stop")
			if (__cmd[idx] == "add")
				add_data();
			else if (__cmd[idx] == "list")
				show_data();
			else
				std::cout << "[COMMNAD \'" << __cmd[idx] << "\' NOT FOUND!]" << std::endl;
			
			
			idx++;
		}
		__cmd.clear();
		processCommandChain(__cmd);
	}
	#ifdef unix
	std::cout << "unix OS" << std::endl;
	system("ls");
	#elif _WIN32
	std::cout << "WINDOWS OS" << std::endl;
	system("DIR");
	#endif
	return 0;
};

bool add_data()
{
	std::cout << ">> ADD DATA" << std::endl;
	return true;
}

bool show_data()
{
	std::cout << ">> SHOW DATA" << std::endl;
	return true;
}


bool processCommandChain(std::vector<std::string>& _cmd)
{
	char _ch;
	std::string _command;
	bool _space = false;
	while (true)
	{
		_ch = std::cin.get();
		if (_ch == ';')
		{
			_space = true;
			if (_command.length() > 0)
				_cmd.push_back(_command);
			_command.clear();
		}
		else if (_ch == 32)
		{
			//std::cout << _command << '\t' << _command.length() << std::endl;
			_space = true;
		}
		else if (_ch == 10)
		{
			//std::cout << _command << '\t' << _command.length() << std::endl << "[ENTER]" << std::endl;
			_cmd.push_back(_command);
			_command.clear();
			break;
		}
		else if ((_command.length() > 0) && _space)
		{
			_space = false;
			_command += (char)32;
			_command += (char)_ch;
		}
		else
		{
			_space = false;
			_command += (char)_ch;
		}
	}
	return true;
}