#include <iostream>
#include <cstdlib>

// Create a class template Pair that can store two values of any type.
template <typename T1, typename T2> class Pair {
	public:

		// Constructor
		Pair(T1 first, T2 second) {
			this->first = first;
			this->second = second;
		}

		// Print the pair
		void print() {
			std::cout << "<" << first << ", " << second << ">" << std::endl;
		}

	private:
		T1 first;
		T2 second;
};


int main() {

	// Test cases
	Pair<int, double> p1(1, 7.3);
	p1.print();

	Pair<std::string, double> p2("hello", 7.7);
	p2.print();

	Pair<float, long> p3(1.2, 777777773);
	p3.print();
}
