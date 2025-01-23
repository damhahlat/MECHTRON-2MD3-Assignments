#include <iostream>
#include <cstdlib>

// A class that generates a progression of numbers
class Progression {

	public:

		// Default constructor
		Progression() {
			current = 2;
			next = 200;
		}

		// Custom constructor
		Progression(int current, int next) {
			this->current = current;
			this->next = next;
		}

		// Prints the progression
		void printProgression(int n) {

			// Prints the current number before progressing and prints the next number
			for (int i = 0; i < n; i++) {
				std::cout << current << " ";
				nextProgression();
			}
			std::cout << std::endl;
		}

	private:

		// Private members to store the current and next number in the progression
		int current;
		int next;

		// To progress the numbers, set the current number to the next number and find their absolute difference for the next number
		void nextProgression() {
			int tmp = current;
			current = next;

			next = abs(next - tmp);
		}
};

int main() {

	// Test case fo the default constructor
	std::cout << "Absolute constructor with default constructor:" << std::endl;
	Progression p1;
	p1.printProgression(10);

	// Test case for the custom constructor
	std::cout << "Absolute constructor with custom constructor:" << std::endl;
	Progression p2(3, 300);
	p2.printProgression(10);

	return EXIT_SUCCESS;
}
