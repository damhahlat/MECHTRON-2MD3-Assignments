#include <cstdlib>
#include <iostream>

// Function prototypes
double BuyOnePotato(int price, int &potatoes);
double BuyNumPotatoes(int price, int &potatoes, int N);

int main() {

	// Initial amount of potatoes and their individual price
	int potatoes_available = 20;
	float potato_price = 30.0;

	// Buying one potato
	std::cout << "$" << BuyOnePotato(potato_price, potatoes_available) << std::endl;
	std::cout << potatoes_available << std::endl;

	// Buying three potatoes
	std::cout << "$" << BuyNumPotatoes(potato_price, potatoes_available, 3) << std::endl;
	std::cout << potatoes_available << std::endl;

	return EXIT_SUCCESS;
}

// If one potato is bought, the price is returned and the amount of potatoes is decremented by one
double BuyOnePotato(int price, int &potatoes) {
	potatoes--;
	return price * 1.13;
}

// If N potatoes are bought, the price of N potatoes is returned and the amount of potatoes is decremented by N
double BuyNumPotatoes(int price, int &potatoes, int N) {
	potatoes -= N;
	return price * N * 1.13;
}
