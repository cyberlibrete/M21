### Example of Issue for Repository `m21`
**Title:** `Add a function to process command chains`
---


**Description:**

A new function is required to process user input containing a chain of commands. The user should be able to input multiple commands separated by a delimiter (e.g., semicolon `;`), and the function should parse and execute each command in sequence

**Task Objective:**
Implement the function in the `m21t01` branch within the `T01` directory to process a chain of commands.

---
**Steps to Complete:**
1. In the `m21t01` branch, within the `T01` directory, create a function, e.g., `processCommandChain`. 2. Implement the following workflow:
	- Split the input string into individual commands using `;` as the delimiter.
	- Validate each command for correctness.
	- Execute each command in the given order.
3. Handle errors for the following cases:
	- Invalid commands.
	- Empty input string.
4. Add unit tests to ensure functionality.

---
**Expected Behavior:**
1. When the user inputs: ``` command1; command2; command3 ``` The function should execute each command in sequence.
2. If the user inputs an invalid command: ```command1 invalidCommand; command2``` The function should execute the first command, skip the invalid one with an error message, and continue with the remaining commands.

---
**Additional Information:**
- Example input for testing: ```run; stop; pause``` Here, `run`, `stop`, and `pause` are examples of possible commands.
- Input type: `std::string`.
---
**Labels:** `enhancement`, `feature`, `branch:m21t01`
---
**Checklist:**
- [ ] Implement the `processCommandChain` 
function.
- [ ] Add validation for each command.
- [ ] Handle errors gracefully.
- [ ] Write unit tests.
- [ ] Perform code review.

---
### Comments
If you have any questions, add them in the comments section of this issue. After completing the task, create a Pull Request from the `m21t01` branch to the `main` branch.  

