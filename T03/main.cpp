#include <iostream>
#include <vector>
#include <string>
#include <cmath>

namespace kernel
{
	enum str
	{
		nothing		= 0x00,
		spc_left	= 0x01 << 0,
		spc_right	= 0x01 << 1,
		spc_into	= 0x01 << 2,
		all_spcs	= 0b00000111,
		spec_line	= 0x01 << 4

	};

	bool format_str(std::string& value, __int8 _settings)
	{
		int idx = 0, _idx = 0;

		if (_settings & str::spc_left)
		{
			while (idx < value.length() && value[idx] == ' ')
				idx++;
			
			while ((_idx + idx) < value.length())
			{
				value[_idx] = value[_idx+idx];
				_idx++;
			}
		}
		value.resize(value.length() - idx);
		if (_settings & str::spc_into)
		{
			int _shift = 0;
			_idx = 0;

			while (_idx < value.length())
			{
				while (_idx > 0 && value[_idx] == ' ' && value[_idx - 1] == ' ')
					_idx++;
				
				if (_shift != _idx)
					value[_shift] = value[_idx];
				
				_shift++;
				_idx++;
			}
			idx = _idx - _shift;
			_shift = 0;
		}
		value.resize(value.length() - idx);
		
		if (_settings & str::spc_right)
		{
			_idx = value.length() - 1, idx = 0;

			while (value[_idx] == ' ')
			{
				_idx--;
				idx++;
			}
		}

		value.resize(value.length() - idx);
		//std::cout << '|' << value << '|' << std::endl;
		idx = 0, _idx = 0;
		return true;
	}

	bool split_str(std::string& value, std::string _delimiter, std::vector<std::string>& output, __int8 _settings = str::nothing)
	{
		std::string word;

		int idx = 0, _idx = 0;
		while (idx < value.length())
		{
			_idx = 0;
			bool is_delimiter = true;
			while (_idx < _delimiter.length())
			{
				if (value[idx + _idx] != _delimiter[_idx])
				{
					is_delimiter = false;
					break;
				}
				_idx++;
			}

			if (is_delimiter)
			{
				if (_settings & ~str::nothing)
					format_str(word, _settings);
				
				output.push_back(word);
				word.clear();
				idx = idx + _delimiter.length();
			}
			else
			{
				word += value[idx];
				idx++;
			}
		}

		if (_settings & ~str::nothing)
			format_str(word, _settings);

		output.push_back(word);
		word.clear();

		idx = 0, _idx = 0;

		return true;
	}

	int dec_power(__int8 _power)
	{
		int output = 1;
		while (_power > 0)
		{
			output *= 10;
			_power--;
		}
		return output;
	}

	bool get_int(std::string& _value, int& output)
	{
		int buffer = 0;
		__int8 position = 0;
		int idx = _value.length() - 1;
		while (idx >= 0)
		{
			if (_value[idx] >= '0' && _value[idx] <= '9')
			{
				buffer += int(_value[idx] - '0') * dec_power(position);
				position++;
			}
			else
			{
				std::cerr << "The value must be a positive integer!" << std::endl << std::endl;
				return false;
			}
			idx--;
		}
		output = buffer;
		return true;
	}

	bool get_double(std::string& value, double& output)
	{
		output = 0.0;
		bool dot = false;
		format_str(value, str::spc_left | str::spc_right);
		//std::cout << '|' << value << '|' << std::endl;
		std::string _val_dot;
		std::string _dot_val;
		bool minus = false;
		int idx = 0;

		if (!dot && value[0] == '-' && value.length() > 1)
		{
			minus = true;
			idx = 1;
		}

		while (idx < value.length())
		{
			
			if (!dot && value[idx] >= '0' && value[idx] <= '9')
				_val_dot += value[idx];

			else if (dot && value[idx] >= '0' && value[idx] <= '9')
				_dot_val += value[idx];

			else if (!dot && (value[idx] == '.' || value[idx] == ','))
				dot = true;

			else
			{
				//std::cerr << "The value must be a positive double!" << std::endl << std::endl;
				return false;
			}
			
			idx++;
		}

		if (_val_dot.length() > 0)
			output += std::stoi(_val_dot);

		if (_dot_val.length() > 0)
			output += double(double(std::stoi(_dot_val)) / dec_power(__int8(_dot_val.length())));

		if (minus)
			output *= (-1);

		return true;
	}


	struct terminal
	{
	private:
		std::string __cmd;
		std::vector<std::string> _buffer;
		
	public:
		bool get_line(__int8 _settings = kernel::str::all_spcs)
		{
			std::getline(std::cin, this->__cmd);
			kernel::format_str(this->__cmd, _settings);
			kernel::split_str(this->__cmd, " ", this->_buffer, kernel::str::all_spcs);

			return true;
		}

		bool remove_first()
		{
			if (this->_buffer.empty())
				return true;

			int size_a = this->_buffer.size();
			int idx = 0;
			while ((idx + 1) < this->_buffer.size())
			{
				this->_buffer[idx] = this->_buffer[idx + 1];
				idx++;
			}
			this->_buffer[this->_buffer.size() - 1].clear();
			this->_buffer.resize(this->_buffer.size() - 1);

			return (this->_buffer.size() == (size_a - 1));
		}

		inline bool clear()
		{
			this->_buffer.clear();
			this->__cmd.clear();

			return (_buffer.size() == 0 && __cmd.length() == 0);
		}

		inline std::string& current()
		{
			return this->_buffer[0];
		}

		inline bool empty()
		{
			return (this->_buffer.empty());
		}
	};
}

struct math_vector
{
	double x_value = 0;
	double y_value = 0;

	bool show(bool _new_line = true)
	{
		std::cout << "vector ("<< this->x_value << "; " << this->y_value << ")" << (_new_line ? "\n" : "");
		return true;
	}

	double length()
	{
		double output = pow(this->x_value, 2.0) + pow(this->y_value, 2.0);
		output = double(std::sqrt(output));
		return output;
	}

	bool initial(kernel::terminal& _term)
	{
		// req x_value
		do
		{
			if (_term.empty())
			{
				std::cout << "Enter the coordinate of the vector on the Ox axis: ";
				_term.get_line();
			}

			if (_term.current() == "stop" || _term.current() == "exit")
				return false;

			else if (kernel::get_double(_term.current(), this->x_value))
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be a positive decimal or integer." << std::endl
							<< "If you want to stop the program, then type \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.remove_first();

		} while (true);

		// req y_value
		do
		{
			if (_term.empty())
			{
				std::cout << "Enter the coordinate of the vector on the Oy axis: ";
				_term.get_line();
			}

			if (_term.current() == "stop" || _term.current() == "exit")
				return false;

			else if (kernel::get_double(_term.current(), this->y_value))
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be a positive decimal or integer." << std::endl
							<< "If you want to stop the program, then type \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.remove_first();

		} while (true);
		
		return true;
	}
};

namespace m_vect
{
	bool add(kernel::terminal& _term)
	{
		math_vector output;

		math_vector _vect_a;
		math_vector _vect_b;

		std::cout << "[vector a]" << std::endl;
		_vect_a.initial(_term);
		std::cout << "[vector b]" << std::endl;
		_vect_b.initial(_term);

		output.x_value = _vect_a.x_value + _vect_b.x_value;
		output.y_value = _vect_a.y_value + _vect_b.y_value;

		output.show();

		return true;
	}

	bool subtract(kernel::terminal& _term)
	{
		math_vector output;

		math_vector _vect_a;
		math_vector _vect_b;

		std::cout << "[vector a]" << std::endl;
		_vect_a.initial(_term);
		std::cout << "[vector b]" << std::endl;
		_vect_b.initial(_term);

		output.x_value = _vect_a.x_value - _vect_b.x_value;
		output.y_value = _vect_a.y_value - _vect_b.y_value;

		output.show();

		return true;
	}

	bool scale(kernel::terminal& _term)
	{
		math_vector _vect_a;
		double _scale;

		do
		{
			if (_term.empty())
			{
				std::cout << "Enter the scalar that the vector will be multiplied by: ";
				_term.get_line();
			}

			if (_term.current() == "stop" || _term.current() == "exit")
				return false;

			else if (kernel::get_double(_term.current(), _scale))
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be a positive decimal or integer." << std::endl
							<< "If you want to stop the program, then type \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.remove_first();

		} while (true);

		std::cout << "[vector]" << std::endl;
		_vect_a.initial(_term);

		_vect_a.x_value *= _scale;
		_vect_a.y_value *= _scale;

		_vect_a.show();

		return true;
	}

	bool len(kernel::terminal& _term)
	{
		math_vector _vect;

		std::cout << "[vector]" << std::endl;
		_vect.initial(_term);

		std::cout << _vect.length() << std::endl;

		return true;
	}

	bool normalize(kernel::terminal& _term)
	{
		math_vector _vect;
		math_vector _vect_norm;

		std::cout << "[vector]" << std::endl;
		_vect.initial(_term);

		_vect_norm.x_value = _vect.x_value / _vect.length();
		_vect_norm.y_value = _vect.y_value / _vect.length();

		_vect_norm.show();

		return true;
	}

}

int main()
{
	kernel::terminal xterm;

#ifdef _WIN32
	system("cls");
#elif defined(__linux__) || defined(__APPLE__)
	system("clear");
#endif
	
	std::cout	<< "MODULE 21 | TASK 03" << std::endl
				<< "Mathematical vector" << std::endl
				<< "----------------------------" << std::endl << std::endl;
	
	do {
		if (xterm.empty())
		{
			std::cout << "$ ";
			xterm.get_line();
		}

		if (xterm.current() == "cls" || xterm.current() == "clear")
		{
			xterm.remove_first();
#ifdef _WIN32
			system("cls");
#elif defined(__linux__) || defined(__APPLE__)
			system("clear");
#endif
		}
		else if (xterm.current() == "exit" || xterm.current() == "stop" || xterm.current() == "q")
		{
			xterm.clear();
			return 0;
		}
		else if (xterm.current() == "help")
		{
			xterm.remove_first();
			//				|	|	|	|	|
			std::cout	<< "COMMANDS LIST:" << std::endl
						<< "help\t\t\t- commands list" << std::endl
						<< "cls, clear\t\t- clear terminal" << std::endl
						<< "exit, stop, q\t\t- stop programm" << std::endl << std::endl

						<< "add, a\t\t\t- addition of two vectors" << std::endl
						<< "subtract, u\t\t- subtraction of two vectors" << std::endl
						<< "scale, s\t\t- multiplication of the vector by scalar" << std::endl
						<< "length, l\t\t- finding the length of the vector" << std::endl
						<< "normalize, n\t\t- vector normalization" << std::endl << std::endl;
		}
		else if (xterm.current().empty())
		{
			xterm.remove_first();
		}
		else if (xterm.current() == "add" || xterm.current() == "a" || xterm.current() == "A")
		{
			xterm.remove_first();
			m_vect::add(xterm);
		}
		else if (xterm.current() == "subtract" || xterm.current() == "u" || xterm.current() == "U")
		{
			xterm.remove_first();
			m_vect::subtract(xterm);
		}
		else if (xterm.current() == "scale" || xterm.current() == "s" || xterm.current() == "S")
		{
			xterm.remove_first();
			m_vect::scale(xterm);
		}
		else if (xterm.current() == "length" || xterm.current() == "l" || xterm.current() == "L")
		{
			xterm.remove_first();
			m_vect::len(xterm);
		}
		else if (xterm.current() == "normalize" || xterm.current() == "n" || xterm.current() == "N")
		{
			xterm.remove_first();
			m_vect::normalize(xterm);
		}
		else
		{
			std::cerr << "Command <" << xterm.current() << "> not availble!" << std::endl;
			xterm.remove_first();
		}

	} while (true);
		
	return 0;
}