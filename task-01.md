# Payment accounting statement

## Description  
This program is designed to record and manage the payment statement. It allows you to:  
- Display a list of all payouts from the file.  
- Add new payment records.  

The program uses a text file to store data. Each entry includes:  
- The name and surname of the recipient.  
- The date of issue of the payment in the format DD.MM.YYYY.  
- The amount of the payment in rubles.

---

## Functionality  

1. Reading data (`list')
Displays all entries from the file in a convenient format.  

2. Adding data (`add')  
   Allows the user to add a new entry to the statement by specifying:
- First and last name.  
   - The date of payment.  
   - The amount of the payment.  

---

## Usage  

1. Run the program.  
2. Enter the command:
- list — to display the current list.  
   - add — to add a new entry.  

---

## Data file format  
The data is stored in a text file vedomost.txt . Sample content:  
Ivan Ivanov 10.12.2024 5000
Peter Petrov 11.12.2024 4500

Each line contains:
1. First name.
2. Last name.  
3. The date of payment.  
4. The amount of the payment.  

---

## Example of work  

### Data display:  
Command:
list

Conclusion:
Ivan Ivanov 10.12.2024 5000 rubles.
Peter Petrov 11.12.2024 4500 rubles.

### Adding a new entry:  
Command:
add

Input:
Enter the first and last name: Anna Smirnova
Enter the date (DD.MM.YYYY): 12.12.2024
Enter the amount: 6000

Conclusion:
The entry has been added!

Updated file `vedomost.txt `:
Ivan Ivanov 10.12.2024 5000
Peter Petrov 11.12.2024 4500
Anna Smirnova 12.12.2024 6000

---

## Technical Details  

- Programming language: C++.  
- File format: a text file with data separated by spaces.  
- Functions:  
  - Reading data from a file.  
  - Writing data to a file.
- System requirements: Windows/Linux with the C++ compiler installed.

---

## Installation and launch  

1. Download the source code of the program.  
2. Compile the code using a compiler (for example, 'g++'):

g++ main.cpp -o vedomost
   
3. Run the program:

./vedomost
   
---

## Notes  

- If the file vedomost.txt if it is missing, the program will create it when the first record is added.  
- Dates must be entered in the format DD.MM.YYYY.  
- The program does not check the correctness of the amount (it is assumed that the input is always correct).  

---

## Contacts  

For questions and suggestions, write to:  
[example@email.com](mailto:example@email.com)