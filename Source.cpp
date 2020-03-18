// Boyer-Moore algorithm implementation created by Jonah McElfatrick for CMP201 Data structures and Algorithms 1
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <chrono>
#include <thread>
using namespace std;

//Clock for benchmarking
using chrono::duration_cast;
using chrono::milliseconds;
using this_thread::sleep_for;
typedef chrono::steady_clock the_clock;

list<int> find_int_occurrence(const string& pattern, const string& txt) {
	//Holds the number of position jumps that can be made for each character
	int jumps[256];

	//Stores the number of times that the search loop is run
	int operations = 0;

	//Length of the text contents and the pattern
	int pat_len = pattern.size();
	int txt_len = txt.size();

	//List of positions for each found position the pattern is at
	list<int> found_positions;

	//Fills the jumps array with the number of jumps needed for each character that might appear
	for (int i = 0; i < 256; i++) {
		//Fils the entire jumps array with the length of the pattern as a base point to then calculate the number of jumps needed for each character
		jumps[i] = pat_len;
	}

	for (int i = 0; i < pat_len; i++) {
		//Calculates the number of jumps required for each character that is in the pattern
		//and overwrites the corresponding 8-bit character with the number of jumps
		jumps[int(pattern[i])] = (pat_len - 1) - i;
	}

	//Prints out the comparred values and compares them together
	for (int i = 0; i < txt_len - pat_len; ++i) {
		operations++;
		//Initialise variable j for 'jumps' 
		int j = jumps[int(txt[i + pat_len - 1])];
		if (j != 0) {
			//Jumps forwards the number of positions required to 
			i += j - 1;
			continue;
		}
		int h;
		//Loop to check to see if the pattern is the same as the comparrison value
		for (h = 0; h < pat_len; h++) {
			if (txt[i + h] != pattern[h]) {
				//Pattern is not found and breaks from the loop
				break;
			}
		}
		if (h == pat_len) {
			//Pattern is found and then added onto the found_positions list
			found_positions.push_back(i);
		}
	}
	return found_positions;
}

string inputFile() {
	//Variables for the contents of the file
	string contents;

	//Opens the text file for reading
	ifstream file("100000names.txt", ios_base::binary);

	//Check to see if the TextFile.txt file was sucessfully opened
	if (!file.good()) {
		cout << "Unable to open Text File";
		system("pause");
		exit(1);
	}

	//Finds end of text file
	file.seekg(0, ios::end);

	contents.reserve(file.tellg());

	//Finds beginning of text file
	file.seekg(0, ios::beg);

	//Assigns the contents of the text file to the string variable contents
	contents.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());

	//Closes the input textfile
	file.close();

	return contents;
}

int main() {
	//Variables for holding the text contents of a file, and the pattern that is being searched for
	string txt;
	string pattern = "logan5@gmail.co.uk";

	//Call the inputFile() function which fills the variable text with the contents of the file
	txt = inputFile();

	// Start timing
	the_clock::time_point start = the_clock::now();

	//list<Position> pos = find_occurrences(pattern, text);
	list<int> Found_Positions = find_int_occurrence(pattern, txt);

	// Stop timing
	the_clock::time_point end = the_clock::now();

	if (Found_Positions.size() == 0) {
		//If there was no found occurrences of the email then a message stating no emails were found is displayed
		cout << "No occurrences of " << pattern << " were found in the file." << endl;
	}
	else {
		//The number of found emails and there positions will be displayed
		for (auto p : Found_Positions) {
			cout << p << "\n";
		}
		cout << "Found " << Found_Positions.size() << " occurrences of '" << pattern << "' at the positions above" << endl;
	}

	//Calculate the time taken for the search function to finish
	auto time_taken = duration_cast<milliseconds>(end - start).count();
	cout << "It took " << time_taken << " ms to find all occurrences of " << pattern << "." << endl;

	//Pauses the interface when the results have been outputted to the screen
	system("pause");
	return 0;
}