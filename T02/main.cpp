#include <iostream>
#include <string>
#include <vector>


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
				buffer += int(_value[idx] - '0') * kernel::dec_power(position);
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
			/*int idx = 0;
			while (idx < this->_buffer.size())
			{
				std::cout << '[' << this->_buffer[idx] << "] ";
				idx++;
			}*/
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



enum room_type
{
	living,			// гостиная
	childrens,		// детская
	bedroom,		// спальня
	kitchen,		// кухня
	bathroom		// ванная
};

enum building_type
{
	garage,			// гараж
	shed,			// бытовка
	sauna			// баня
};

struct room
{
	room_type roomType;
	double square;

	bool initial(kernel::terminal& _term)
	{
		// req room type
		int room_type_id;
		do
		{
			if (_term.empty())
			{
				std::cout	<< "BUILDING TYPE:" << std::endl
							<< "1.\tLiving room" << std::endl
							<< "2.\tChildrens room" << std::endl
							<< "3.\tBedroom" << std::endl
							<< "4.\tKitchen" << std::endl
							<< "5.\tBathroom" << std::endl << std::endl
							<< "Enter the item number that describes the purpose of the room: ";
				_term.get_line();
			}

			if (_term.current() == "stop" || _term.current() == "exit")
				return false;

			else if (kernel::get_int(_term.current(), room_type_id) && room_type_id >= 1 && room_type_id <= 5)
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be an integer in the range from 1 to 5." << std::endl
							<< "If you want to stop the program, then enter \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.clear();
		} while (true);
		
		switch (room_type_id)
		{
		case 1:
			this->roomType = room_type::living;
			break;
		
		case 2:
			this->roomType = room_type::childrens;
			break;
		
		case 3:
			this->roomType = room_type::bedroom;
			break;

		case 4:
			this->roomType = room_type::kitchen;
			break;

		case 5:
			this->roomType = room_type::bathroom;
			break;
		
		default:
			break;
		}

		// req square
		do
		{
			if (_term.empty())
			{
				std::cout << "Enter the area of the room: ";
				_term.get_line();
			}

			if (_term.current() == "stop" || _term.current() == "exit")
				return false;

			else if (kernel::get_double(_term.current(), this->square) && this->square >= 5 && this->square <= 30)
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be a positive decimal or integer. Values in the range from 5 to 30 are allowed." << std::endl
							<< "If you want to stop the program, then type \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.clear();

		} while (true);

		return true;
	}

	bool show()
	{
		std::cout << "\t\t\t\t";
		switch (this->roomType)
		{
		case room_type::living :
			std::cout << "Living room\t";
			break;
		
		case room_type::kitchen :
			std::cout << "Kitchen\t\t";
			break;
		
		case room_type::childrens :
			std::cout << "Children's room\t";
			break;
		
		case room_type::bedroom :
			std::cout << "Bedroom\t\t";
			break;

		case room_type::bathroom :
			std::cout << "Bathroom\t\t";
			break;
		
		default:
			break;
		}

		std::cout << " (S= " << this->square << ')' << std::endl;
		return true;
	}
};

struct floor
{
	std::vector<room> rooms;
	double ceiling_h = 2.5;				// default 2.5

	bool initial(kernel::terminal& _term, int _floor_level = 1)
	{
		// req ceiling height
		do
		{
			if (_term.empty())
			{
				std::cout << "Enter the ceiling height on floor " << _floor_level <<": ";
				_term.get_line();
			}

			if (_term.current() == "stop" || _term.current() == "exit")
				return false;

			else if (kernel::get_double(_term.current(), this->ceiling_h) && this->ceiling_h >= 0.5 && this->ceiling_h <= 10)
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be a positive decimal or integer. Values in the range from 0.5 to 10 are allowed." << std::endl
							<< "If you want to stop the program, then type \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.clear();

		} while (true);
		

		// req rooms number (2...4 rooms)
		int count = 0;
		do
		{
			if (_term.empty())
			{
				std::cout << "Enter the number of rooms per floor: ";
				_term.get_line();
			}

			if (_term.current() == "stop" || _term.current() == "exit")
				return false;

			else if (kernel::get_int(_term.current(), count) && count >= 2 && count <= 4)
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be an integer in the range from 2 to 4." << std::endl
							<< "If you want to stop the program, then enter \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.clear();

		} while (true);
		// initial EVERY room

		while (count > 0)
		{
			room _cur_room;
			//if (!_cur_room.initial(_term))
			if(!_cur_room.initial(_term))
				return false;
			else
				rooms.push_back(_cur_room);
			count--;
		}
		return true;
	}
	
	bool show(int _level)
	{
		std::cout << "\t\t\tFloor " << _level << " (Ceiling = " << this->ceiling_h << ')' << std::endl;

		int idx = 0;
		while (idx < rooms.size())
		{
			rooms[idx].show();
			idx++;
		}
		return true;
	}
};

struct OtherBuilding
{
	building_type buildingType;
	double square;
	bool stove = true;

	bool initial(kernel::terminal& _term)
	{
		// req building type
		int building_type_id;
		do
		{
			if (_term.empty())
			{
				std::cout	<< "BUILDING TYPE:" << std::endl
							<< "1.\tGarage" << std::endl
							<< "2.\tShed" << std::endl
							<< "3.\tSauna" << std::endl << std::endl
							<< "Enter the item number that describes the building type: ";
				_term.get_line();
			}

			if (_term.current() == "stop" || _term.current() == "exit")
				return false;

			else if (kernel::get_int(_term.current(), building_type_id) && building_type_id >= 1 && building_type_id <= 3)
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be an integer in the range from 1 to 3." << std::endl
							<< "If you want to stop the program, then enter \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.clear();
		} while (true);
		
		switch (building_type_id)
		{
		case 1:
			this->buildingType = building_type::garage;
			break;
		
		case 2:
			this->buildingType = building_type::shed;
			break;
		
		case 3:
			this->buildingType = building_type::sauna;
			break;
		
		default:
			break;
		}

		// req square of this building
		do
		{
			if (_term.empty())
			{
				std::cout << "Enter the area occupied by the building: ";
				_term.get_line();
			}

			if (_term.current() == "stop" || _term.current() == "exit")
				return false;

			else if (kernel::get_double(_term.current(), this->square) && this->square >= 20 && this->square <= 150)
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be a positive decimal or integer. Values in the range from 20 to 150 are allowed." << std::endl
							<< "If you want to stop the program, then type \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.clear();

		} while (true);

		// req stove [if it's sauna]
		if (this->buildingType == building_type::sauna)
		{
			if (_term.empty())
			{
				std::cout << "Is there a fireplace with a chimney in the building? [Y/n] ";
				_term.get_line();
			}

			if (_term.current() == "n" || _term.current() == "N")
			{
				this->stove = false;
			}
			_term.remove_first();
		}

		return true;
	}

	bool show(double _sum_square)
	{
		std::cout << "\t\t";
		switch (this->buildingType)
		{
		case building_type::garage :
			std::cout << "Garage\t";
			break;
		
		case building_type::sauna :
			std::cout << "Sauna\t";
			break;
		
		case building_type::shed :
			std::cout << "Shed\t";
			break;
		
		default:
			break;
		}

		std::cout	<< "(S=" << this->square << " | "
					<< (double(this->square * 100) / _sum_square) << "%) "
					<< (stove ? "[FIREPLACE AND CHIMNEY]" : "") << std::endl;
		return true;
	}
};

struct ResidentialBuilding
{
	std::vector<floor> floors;
	double square;
	bool stove = true;				// default TRUE

	int get_square_floor(int _level)
	{
		int output = 0;

		int idx = 0;
		while (idx < floors[_level].rooms.size())
		{
			output += floors[_level].rooms[idx].square;
			idx++;
		}

		return output;
	}

	bool initial(kernel::terminal& _term)
	{
		// req number of floors [1...3 floors]
		int count = 0;
		do
		{
			if (_term.empty())
			{
				std::cout << "Enter the number of floors in the house: ";
				_term.get_line();
			}
			
			if (_term.current() == "stop" || _term.current() == "exit")
				return false;

			else if (kernel::get_int(_term.current(), count) && count >= 1 && count <= 3)
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be an integer in the range from 1 to 3." << std::endl
							<< "If you want to stop the program, then enter \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.clear();

		} while (true);
		
		
		// initial EVERY floor
		while (count > 0)
		{
			floor _cur_floor;

			if(!_cur_floor.initial(_term, this->floors.size() + 1))
				return false;
			else
				floors.push_back(_cur_floor);
			count--;
		}

		// *SET squeare by floor-1
		this->square = get_square_floor(0);
		
		// req stove in the house
		if (_term.empty())
		{
			std::cout << "Is there a fireplace with a chimney in the building? [Y/n] ";
			_term.get_line();
		}

		if (_term.current() == "n" || _term.current() == "N")
		{
			this->stove = false;
		}
		_term.remove_first();
		
		return true;
	}

	bool show(double _sum_square)
	{
		std::cout	<< "\t\tRESIDENTIAL HOUSE (S=" << this->square << " | "
					<< (double(this->square * 100) / _sum_square) << "%) "
					<< (stove ? "[FIREPLACE AND CHIMNEY]" : "") << std::endl;

		int idx = 0;
		while (idx < floors.size())
		{
			floors[idx].show(idx + 1);
			idx++;
		}

		return true;
	}
};

struct area
{
	int area_id;
	bool house_on_territory = false;
	ResidentialBuilding house;
	std::vector<OtherBuilding> anybuildings;

	double get_square_sum()
	{
		double output = 0;

		if (house_on_territory)
			output += house.get_square_floor(0);
		
		int idx = 0;
		while (idx < anybuildings.size())
		{
			output += anybuildings[idx].square;
			idx++;
		}

		return  output;
	}

	bool initial(kernel::terminal& _term, int _area_id = 1)
	{
		// set areaID (automaticly)
		this->area_id = _area_id;
		std::cout << "> AREA " << this->area_id << std::endl;

		if (_term.empty())
		{
			std::cout << "Is there an residential house on the site? [y/N] ";
			_term.get_line();
		}

		if (_term.current() == "y" || _term.current() == "Y")
		{
			_term.remove_first();
			this->house_on_territory = true;
			house.initial(_term);
		}
		else
			_term.remove_first();

		// req othe buildings number
		int count = 0;
		do
		{
			if (_term.empty())
			{
				std::cout	<< "\nHow many other buildings are there in the territory?" << std::endl
							<< "$ ";
				_term.get_line();
			}

			if (_term.current() == "stop" || _term.current() == "exit")
				return false;
			
			else if (kernel::get_int(_term.current(), count) && count >= 0)
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value." << std::endl
							<< "The value must be integer and positive. The value 0 is taken as the absence of initialization elements." << std::endl
							<< "If you want to stop the program, then type \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.clear();

		} while (true);

		
		// initial every building
		while (count > 0)
		{
			OtherBuilding _cur_build;
			if (!_cur_build.initial(_term))
				return false;
			else
				anybuildings.push_back(_cur_build);
			count--;
		}

		return true;
	}

	bool show(double _sum_square)
	{
		std::cout	<< "\tArea No." << this->area_id << std::endl;

		int idx = 0;
		while (idx < anybuildings.size())
		{
			anybuildings[idx].show(_sum_square);
			idx++;
		}

		if (house_on_territory)
		{
			house.show(_sum_square);
		}

		return true;	
	}
};

struct village
{
	std::vector<area> areas;

	double get_sum_square()
	{
		double output = 0;

		int idx = 0;
		while (idx < areas.size())
		{
			output += areas[idx].get_square_sum();
			idx++;
		}

		return output;
	}

	// add new areas into village
	bool initial(kernel::terminal& _term)
	{
		int count = 0;
		// request number of areas
		do
		{
			if (_term.empty())
			{
				std::cout << "Enter the number of plots: ";
				_term.get_line();
			}
				
			if (_term.current() == "stop" || _term.current() == "exit")
				return false;
			
			else if (kernel::get_int(_term.current(), count) && count >= 1)
			{
				_term.remove_first();
				break;
			}
			else
			{
				std::cerr	<< "Invalid value."<< std::endl
							<< "The value must be integer and positive. Initialize at least one section, to do this, enter the number 1." << std::endl
							<< "If you want to stop the program, then enter \"stop\" or \"exit\"." << std::endl << std::endl;
			}
			_term.clear();

		} while (true);


		// intial EVERY area
		while (count > 0)
		{
			area _cur_area;
			//std::cout << "INITIAL AREA " << count << std::endl;
			if (!_cur_area.initial(_term, areas.size() + 1))
				return false;
			else
				areas.push_back(_cur_area);
			count--;
		}
		return true;
	}

	bool show()
	{
		std::cout	<< "--------------------------------------------------" << std::endl
					<< "VILLAGE STRUCTURE (" << "AREAS: " << areas.size()  << ')' << std::endl;
		
		int idx = 0;
		while (idx < areas.size())
		{
			areas[idx].show(this->get_sum_square());
			idx++;
		}

		return true;	
	}
};

int main ()
{
	kernel::terminal xterm;

	village CurrentVillage;
	//std::string _cmd;

#ifdef _WIN32
		system("cls");
#elif defined(__linux__) || defined(__APPLE__)
		system("clear");
#endif

	std::cout	<< "MODULE 21 | TASK 02" << std::endl
				<< "Data model for a village" << std::endl
				<< "----------------------------" << std::endl << std::endl;

	do {
		//_cmd.clear();
		
		if (xterm.empty())
		{
			std::cout << "VILLAGE $ ";
			xterm.get_line();
		}
		
		//std::cout << xterm.current() << std::endl;
		//std::cin >> _cmd;
		
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
			std::cout	<< "COMMANDS LIST:" << std::endl
						<< "help\t\t-commands list" << std::endl
						<< "cls, clear\t-clear terminal" << std::endl
						<< "add\t\t-initialization of the settlement" << std::endl
						<< "show\t\t-show full structure" << std::endl << std::endl
						<< "exit, stop, q\t- stop programm" << std::endl << std::endl;
		}
		else if (xterm.current() == "add")
		{
			xterm.remove_first();
			if(!CurrentVillage.initial(xterm))
				return 1;
		}
		else if (xterm.current().empty())
		{
			xterm.remove_first();
		}
		else if (xterm.current() == "show")
		{
			xterm.remove_first();
			CurrentVillage.show();
		}
		else
		{
			std::cerr << "Command <" << xterm.current() << "> not availble!" << std::endl;
			xterm.remove_first();
		}
		//xterm.remove_first();
	} while (true);
	return 0;
}
