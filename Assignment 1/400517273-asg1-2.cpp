#include <iostream>
#include <cstdlib>
#include <ctime>

class Birthday {
	public:
		int day;
		int month;
		Birthday() {
			day = rand() % 30 + 1; // Anything between 1 and 30 inclusive
			month = rand() % 12 + 1; // Anything between 1 (JAN) and 12 (DEC) inclusive
		}
};

class BirthdayParadox {
	public:
		Birthday *birthdays;
		int num_people;

		BirthdayParadox(int n) {
			num_people = n;
			birthdays = new Birthday[n];
		}

		~BirthdayParadox() {
			delete[] birthdays;
		}

		bool CheckDuplicates();
};

// Check for duplicates
bool BirthdayParadox::CheckDuplicates() {
	for (int i = 0; i < num_people; i++) {
		for (int j = i + 1; j < num_people; j++) {
			if (birthdays[i].day == birthdays[j].day && birthdays[i].month == birthdays[j].month) {
				//std::cout << "Duplicate found: " << birthdays[i].day << "/" << birthdays[i].month << std::endl;
				return 1;
			}
		}
	}
	return 0;
}

int main() {
	srand(time(0)); // Seed the random number generator

	for (int n = 5; n <= 100; n += 5) {

		int duplicates = 0;

		for (int i = 0; i < 100; i++) {
			BirthdayParadox bp(n);
			if (bp.CheckDuplicates()) {
				duplicates++;
			}
		}

		std::cout << n << "," << duplicates << "," << duplicates / 100.0 << std::endl;
	}
	return EXIT_SUCCESS;
}
