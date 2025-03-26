#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
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

		void insert(string key);
		int search(string key);
		void increment(string key);

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




int main() {
	HashTable table;
	string word;
	ifstream f1("f1.txt");

	// Read f1 and insert into the hash table
	while (f1 >> word) {
		table.insert(word);
	}

	// Print the hash table
	table.print();

	return 0;
}
