#include <cstdlib>
#include <iostream>

double BuyOnePotato(int price, int *potatoes);
double BuyNumPotatoes(int price, int *potatoes, int N);

int main() {
	int potatoes_available = 20;
	float potato_price = 30.0;

	std::cout << "$" << BuyOnePotato(potato_price, &potatoes_available) << std::endl;
	std::cout << potatoes_available << std::endl;
	std::cout << "$" << BuyNumPotatoes(potato_price, &potatoes_available, 3) << std::endl;
	std::cout << potatoes_available << std::endl;
	return EXIT_SUCCESS;
}

double BuyOnePotato(int price, int *potatoes) {
	(*potatoes)--;
	return price * 1.13;
}

double BuyNumPotatoes(int price, int *potatoes, int N) {
	*potatoes -= N;
	return price * N * 1.13;
}
