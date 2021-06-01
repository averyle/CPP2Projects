#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>
#include "MyAlgorithms.h"
#include "Util.h"
#include "Sort.h"
#include <numeric>

auto runtime = [](auto&& func, auto&&...param) {
	auto tick = std::chrono::steady_clock::now();
	std::forward<decltype(func)>(func)(std::forward<decltype(param)>(param)...);
	auto tock = std::chrono::steady_clock::now();
	std::chrono::duration<double> seconds = tock - tick;
	return seconds.count();
};

int main() {

	std::string results = "results.csv";
	std::ofstream Fout{ results, std::ios::out };
	Fout << "n, sort_time, sort_comp, sort_swap, sel_time, sel_comp, sel_swap, ins_time, ins_comp, ins_sawp, mg_time, mg_comp, mg_swap, qck_time, qck_comp, qck_swap, bub_time, bub_comp, bub_swap\n";
	for (int n = 1000; n <= 10000; n += 500) {
		std::mt19937 gen{ std::random_device{}() };
		std::uniform_int_distribution<int> range{ 0, 2 * n };
		auto data = generate_data<int>(n, [&gen, &range]() {
			return range(gen);
			});
		
		Fout << n << ", ";

		my::sortAnalytics analytics;
		std::vector<int> sort1 = clone(data);
		Fout << runtime(std::sort<decltype(sort1.begin())>, sort1.begin(), sort1.end())
			<< ", " << analytics << ", " ;
	
		std::vector<int> sort2 = clone(data);
		Fout << runtime(my::selection_sort<decltype(sort2.begin())>, sort2.begin(), sort2.end(), analytics)
			<< ", " << analytics << ", ";
		reset(analytics);

		std::vector<int> sort3 = clone(data);
		Fout << runtime(my::insertion_sort<decltype(sort3.begin())>, sort3.begin(), sort3.end(), analytics)
			<< ", " << analytics << ", ";
		reset(analytics);

		std::vector<int> sort4 = clone(data);
		Fout << runtime(my::merge_sort<decltype(sort4.begin())>, sort4.begin(), sort4.end(), analytics)
			<< ", " << analytics << ", ";
		reset(analytics);

		std::vector<int> sort5 = clone(data);
		Fout << runtime(my::quick_sort<decltype(sort5.begin())>, sort5.begin(), sort5.end(), analytics)
			<< ", " << analytics << ", ";

		std::vector<int> sort6 = clone(data);
		Fout << runtime(my::bubble_sort<decltype(sort6.begin())>, sort6.begin(), sort6.end(), analytics)
			<< ", " << analytics << "\n";
	}
}