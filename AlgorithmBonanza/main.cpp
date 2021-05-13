#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>
#include "MyAlgorithms.h"
#include "Util.h"
#include <numeric>

int main() {
	/*auto runtimeMeasure = [](auto& func, auto&...param) {
		auto tick = std::chrono::steady_clock::now();
		std::forward<decltype(func)>(func)(std::forward<decltype(param)>(param)...);
		auto tock = std::chrono::steady_clock::now();
		std::chrono::duration<double> seconds = tock - tick;
		return seconds;
	};*/

	std::string runtime = "runtime.csv";
	std::string comparison = "comparison.csv";
	std::ofstream runtimeFout{ runtime, std::ios::out };
	std::ofstream comparisonFout{ comparison, std::ios::out };
	std::vector<double> measurements;
	int count = 0;
	for (int n = 10000; n <= 1000000; n += 10000) {
		std::mt19937 gen{ std::random_device{}() };
		std::uniform_int_distribution<int> range{ 0, 2*n };
		auto data = generate_data<int>(n,[&gen, &range]() {
			return range(gen);
			});

		auto val = range(gen);
		auto func1 = [&val, &count](const int& item) { count++;  return item == val; };
		auto func2 = [&val, &count](const int& i, const int& j) { count++;  return (i < j); };
		auto func3 = [&count](const int& i) { count++; return (i % 2 == 0); };

		for (int i = 0; i < 3; i++) {
			auto tick = std::chrono::steady_clock::now();
			auto it = std::find_if(std::cbegin(data), std::cend(data), func1);
			auto tock = std::chrono::steady_clock::now();
			std::chrono::duration<double> seconds = tock - tick;
			measurements.push_back(seconds.count());
		}
		auto average1 = std::accumulate(std::cbegin(measurements), std::cend(measurements), 0.0) / std::size(measurements);
		runtimeFout << n << "," << average1 << ",";
		comparisonFout << n << "," << count << ",";
		measurements.clear();
		count = 0;

		for (int i = 0; i < 3; i++) {
			auto tick = std::chrono::steady_clock::now();
			auto it = my::find_if(std::cbegin(data), std::cend(data), func1);
			auto tock = std::chrono::steady_clock::now();
			std::chrono::duration<double> seconds = tock - tick;
			measurements.push_back(seconds.count());
		}
		auto average2 = std::accumulate(std::cbegin(measurements), std::cend(measurements), 0.0) / std::size(measurements);
		runtimeFout << average2 << ",";
		comparisonFout << count << ",";
		measurements.clear();
		count = 0;

		for (int i = 0; i < 3; i++) {
			std::vector<int> clone;
			std::copy(std::begin(data), std::end(data), std::back_inserter(clone));
			std::sort(clone.begin(), clone.end());
			auto tick = std::chrono::steady_clock::now();
			auto it = std::binary_search(std::cbegin(clone), std::cend(clone), val, func2);
			auto tock = std::chrono::steady_clock::now();
			std::chrono::duration<double> seconds = tock - tick;
			measurements.push_back(seconds.count());
		}
		auto average3 = std::accumulate(std::cbegin(measurements), std::cend(measurements), 0.0) / std::size(measurements);
		runtimeFout << average3 << ",";
		comparisonFout << count << ",";
		measurements.clear();
		count = 0;

		for (int i = 0; i < 3; i++) {
			std::vector<int> clone;
			std::copy(std::begin(data), std::end(data), std::back_inserter(clone));
			std::sort(clone.begin(), clone.end());
			auto tick = std::chrono::steady_clock::now();
			auto it = my::binary_search(std::cbegin(clone), std::cend(clone), val, func2);
			auto tock = std::chrono::steady_clock::now();
			std::chrono::duration<double> seconds = tock - tick;
			measurements.push_back(seconds.count());
		}
		auto average4 = std::accumulate(std::cbegin(measurements), std::cend(measurements), 0.0) / std::size(measurements);
		runtimeFout << average4 << ",";
		comparisonFout << count << ",";
		measurements.clear();

		for (int i = 0; i < 3; i++) {
			auto tick = std::chrono::steady_clock::now();
			auto it = std::count_if(std::cbegin(data), std::cend(data), func3);
			auto tock = std::chrono::steady_clock::now();
			std::chrono::duration<double> seconds = tock - tick;
			measurements.push_back(seconds.count());
		}
		auto average5 = std::accumulate(std::cbegin(measurements), std::cend(measurements), 0.0) / std::size(measurements);
		runtimeFout << average5 << ",";
		comparisonFout << count << ",";
		measurements.clear(); 
		count = 0;

		for (int i = 0; i < 3; i++) {
			auto tick = std::chrono::steady_clock::now();
			auto it = my::count_if(std::cbegin(data), std::cend(data), func3);
			auto tock = std::chrono::steady_clock::now();
			std::chrono::duration<double> seconds = tock - tick;
			measurements.push_back(seconds.count());
		}
		auto average6 = std::accumulate(std::cbegin(measurements), std::cend(measurements), 0.0) / std::size(measurements);
		runtimeFout << average6 << "\n";
		comparisonFout << count << "\n";
		measurements.clear();
		count = 0;
	}
}