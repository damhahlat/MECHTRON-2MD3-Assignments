#include <iostream>
#include <cstdlib>
#include <ctime>

// Class to represent a birthday
class Birthday {

	public:

		// Constructor
		Birthday() {
			day = rand() % 30 + 1; // Anything between 1 and 30 inclusive
			month = rand() % 12 + 1; // Anything between 1 (JAN) and 12 (DEC) inclusive
		}

		// Overload the == operator
		bool operator==(const Birthday &b) {
			return (day == b.day && month == b.month);
		}

	// Store the day and month as private members
	private:
		int day;
		int month;
};

// Class to represent the birthday paradox
class BirthdayParadox {

	public:

		// Constructor generates n random birthdays
		BirthdayParadox(int n) {
			num_people = n;
			birthdays = new Birthday[n];
		}

		// Destructor deletes the array of birthdays
		~BirthdayParadox() {
			delete[] birthdays;
		}

		// Method to check for duplicates in the array of birthdays
		bool CheckDuplicates();

	// Store the array of birthdays and the number of people as private members
	private:
		Birthday *birthdays;
		int num_people;

};

// Check for duplicates
bool BirthdayParadox::CheckDuplicates() {

	// Iterate through the array of birthdays and check for duplicates
	for (int i = 0; i < num_people; i++) {
		for (int j = i + 1; j < num_people; j++) {

			// If a duplicate is found, return true
			if (birthdays[i] == birthdays[j]) return 1;
		}
	}

	// If no duplicates are found, return false
	return 0;
}

int main() {

	// Seed the random number generator with the current time to ensure randomness
	srand(time(0)); 

	// Run the simulation for n = 5, 10, 15, ..., 100
	for (int n = 5; n <= 100; n += 5) {

		// Count the number of duplicates
		int duplicates = 0;

		// Run each simulation 100 times
		for (int i = 0; i < 100; i++) {

			// Create a BirthdayParadox object with n people
			BirthdayParadox bp(n);

			// Check if a duplicate exists in this simulation
			if (bp.CheckDuplicates()) duplicates++;
		}

		// Output the results
		std::cout << n << "," << duplicates << "," << duplicates / 100.0 << std::endl;
	}

	return EXIT_SUCCESS;
}
