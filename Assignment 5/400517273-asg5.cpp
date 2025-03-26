#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

/*
	General assumptions for the project behaviour

	The assignment was fairly open-ended in terms of instructions,
	so I've taken the liberty of laying out some general criteria as per
	my understanding. 

	The hashmap stores the key, which is the word from f1,
	and the value is the amount of times that word appears in f2. For example,
	if the word "because" appears twice in f2, then it will be printed twice.

	For collision handling, the idea is to add the key to the next available slot.
	While this is not the most efficient way to handle collisions, it is the simplest
	in the context of this assignment.

	The size is assumed to be no more than 1000, since the sample files only go up to 800 words.

	The hash table consists of an array of Nodes, where each Node has a string (key) and an integer (value).

	When a string in f2 is found to be in the hash table, the value is incremented by 1.
	Then, that string is added to an array. When f2 has been completely read, teh string array
	is sorted and then printed, based on the value of the key in the hash table, into a file called "out.txt".
*/

#define SIZE 1000

struct Node {
	string key;
	int value;
};

// This hashtable only stores keys, not key-value pairs.
// Storing values can be implemented with ease but it is not required with the goals of this assignment.
class HashTable {

	private:
		Node table[SIZE];
		int size = SIZE;

		// Hash function. Adds them all up and mods by the size of the table which gives the index.
		int hash(string key) {
			int sum = 0;
			for (int i = 0; i < key.length(); i++) {
				sum += key[i];
			}
			return sum % size;
		}

	public:

		// Constructor
		HashTable() {
			for (int i = 0; i < SIZE; i++) {
				table[i].key = "";
				table[i].value = 0;
			}
		}

		// Function to get the index of a key in the table
		int getIndexOfKey(string key) {

			// Preliminary guess for a key is just the hash
			int index = hash(key);

			// Iterate in a circular manner until you get the actual key
			for (int i = index; i < size; i++) {
				if (table[i].key == key) {
					return i;
				}

				// If we reach the end of the table, start from the beginning.
				if (i == size - 1) {
					i = 0;
				}
			}
			return -1;
		}

		// Function to search for a key's value in the table
		// Resets the value to 0 after getting the value
		int getValue(string key) {
			int index = getIndexOfKey(key);
			int value = table[index].value;
			table[index].value = 0;
			return value;
		}

		// Big main functions for functionality
		void insert(string key); // Function to insert a key into the table
		bool increment(string key); // Function to increment the value of a key in the table

		// For testing purposes
		void print() {
			for (int i = 0; i < size; i++) {
				if (table[i].key != "") {
					cout << table[i].key << " " << table[i].value << endl;
				}
			}
		}
};


// Function which inserts the key into the table.
void HashTable::insert(string key) {
	int index = hash(key);
	if (table[index].key == "") {
		table[index].key = key;
		table[index].value = 0;
	} 

	// Collision handling
	else {

		// Iterate through the hash table in a circular manner until an empty area is found.
		for (int i = index; i < size; i++) {

			// If the key already exists we can ignore the insertion of a key
			if (table[i].key == key) {
				return;
			}

			// If an empty area is found, insert the key.
			if (table[i].key == "") {
				table[i].key = key;
				table[i].value = 0;
				break;
			}

			// If we reach the end of the table, start from the beginning.
			if (i == size - 1) {
				i = 0;
			}
		}
	}
}

// Function to increment the value of a key in the table
bool HashTable::increment(string key) {

	// Get the index based on the hash
	int index = hash(key);

	// If the key is found, increment the value by 1
	if (table[index].key == key) {
		table[index].value++;
		return true;
	}

	// Collision handling
	else {
		for (int i = index+1; i < size; i++) {
			if (table[i].key == key) {
				table[i].value++;
				return true;
			}

			// If we reach the end of the table, start from the beginning.
			if (i == size - 1) {
				i = 0;
			}

			if (i == index) {
				break;
			}
		}
	}

	return false;
}


int main() {

	// Initialize the hash table and read f1 while inserting into the hash table
	HashTable table;
	string word;
	ifstream f1("f1.txt");

	// Read f1 and insert into the hash table
	while (f1 >> word) {
		table.insert(word);
	}

	// Print the hash table
	table.print();

	// Read f2 and increment the value of the key in the hash table
	// Also store a vector of all the keys in f2
	ifstream f2("f2.txt");
	vector<string> keysInF2;

	// Read f2 and increment the value of the key in the hash table
	while (f2 >> word) {
		if (table.increment(word)) {
			keysInF2.push_back(word);
		}
	}

	// Bubble sort the keys in f2 based on alphabetical order
	for (int i = 0; i < keysInF2.size(); i++) {
		for (int j = 0; j < keysInF2.size() - 1; j++) {
			if (keysInF2[j] > keysInF2[j+1]) {
				string temp = keysInF2[j];
				keysInF2[j] = keysInF2[j+1];
				keysInF2[j+1] = temp;
			}
		}
	}

	// Print the keys in f2 based on the value in the hash table
	// To ensure uniqueness of the keys (repetitive printing), we will make the value 0 after printing (occurs in the getValue function)
	ofstream out("out.txt");

	for (int i = 0; i < keysInF2.size(); i++) {
		int value = table.getValue(keysInF2[i]);
		for (int j = 0; j < value; j++) {
			out << keysInF2[i] << endl;
		}
		table.getIndexOfKey(keysInF2[i]);
	}

	return 0;
}
