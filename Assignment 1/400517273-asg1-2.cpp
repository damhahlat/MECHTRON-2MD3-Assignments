#include <iostream>
#include <cstdlib>
#include <ctime>

// Class to represent a birthday
class Birthday {

	public:

		// Constructor
		Birthday() {
			month = rand() % 12 + 1; // Anything between 1 (JAN) and 12 (DEC) inclusive

			// Check for which months have different number of days
			if (month == 4 || month == 6 || month == 9 || month == 11) {
				day = rand() % 30 + 1; // Anything between 1 and 30 inclusive
			} else if (month == 2) {
				day = rand() % 28 + 1; // Anything between 1 and 28 inclusive
			} else {
				day = rand() % 31 + 1; // Anything between 1 and 31 inclusive
			}
		}

		// Overload the == operator
		bool operator==(const Birthday &otherBirthday) {
			return (day == otherBirthday.day && month == otherBirthday.month);
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
