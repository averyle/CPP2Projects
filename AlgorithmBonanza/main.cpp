#include <iostream>
#include <algorithm>
#include <string>
#include "MyAlgorithms.h"
#include "Util.h"

int main() {
	auto test = generate_integers<10>();
	/*auto testResult = my::minmax_element(std::cbegin(test), std::cend(test));
	std::cout << "the minmax pair is " << testResult << "\n";
	std::mt19937 gen{ std::random_device{}() };
	std::uniform_int_distribution<int> range{ 0, 100 };
	auto data = generate_data<20, std::pair<int, int>>([&range, &gen]() {
		return std::make_pair(range(gen), range(gen));
	});
	std::cout << data << "\n";
	auto min = my::min_element_when(std::cbegin(data), std::cend(data), [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a < b; });
	auto max = my::max_element_when(std::cbegin(data), std::cend(data), [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a > b; });
	if (min != std::cend(data)) {
		std::cout << "the minimum element is " << *min << "\n";
	}
	if (max != std::cend(data)) {
		std::cout << "the maximum element is " << *max << "\n";
	}*/
	std::cout << "Test vector: " << test << "\n";
	int countEven = my::count_if(std::cbegin(test), std::cend(test), [](const int& val) { return ((val % 2) == 0); });
	std::cout << "Total even elements: " << countEven << "\n";
	std::cout << test << ": ";;
	if (my::is_sorted(std::begin(test), std::end(test)) == 0) {
		std::cout << "false\n";
	}
	else { std::cout << "true\n"; }
	std::sort(std::begin(test), std::end(test));
	std::cout << test << ": ";;
	if (my::is_sorted(std::begin(test), std::end(test)) == 0) {
		std::cout << "false";
	}
	else { std::cout << "true"; }
	




}