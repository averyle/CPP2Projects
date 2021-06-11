#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <random>
#include <numeric>
#include <chrono>
#include "containers.h"


auto runtime = [](auto&& func, auto&&...param) {
	auto tick = std::chrono::steady_clock::now();
	std::forward<decltype(func)>(func)(std::forward<decltype(param)>(param)...);
	auto tock = std::chrono::steady_clock::now();
	std::chrono::duration<double> seconds = tock - tick;
	return seconds.count();
};

auto pushIt = [](auto&& list, int size, int random) {
	for (int i = 0; i < size; ++i)
	{
		list.push(random);
	}
};

auto popIt = [](auto&& list, int size) {
	for (int i = 0; i < size; ++i)
	{
		list.pop();
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T>& data) {
	out << "[";
	for (const T& item : data) {
		out << item << ",";
	}
	out << "]";
	return out;
}

int main() {
	std::string results = "push_pop.csv";
	std::ofstream Fout{ results, std::ios::out };

	my::array_queue<int> array_queue;
	my::ring_queue<int> ring_queue;
	my::linked_queue<int> linked_queue;
	my::array_stack<int> array_stack;
	my::linked_stack<int> linked_stack;

	Fout << "n, aq push, aq pop, rq push, rq pop, lq push, lq pop, as push, as pop, ls push, ls pop \n";
	for (int i = 10000; i <= 100000; i += 5000) {

		std::mt19937 gen{ std::random_device{}() };
		std::uniform_int_distribution<int> range{ 0, 10000 };
		auto random = range(gen);

		Fout << i << ",";

		Fout << runtime(pushIt, array_queue, i, random)
			<< ","
			<< runtime(popIt, array_queue, i)
			<< ",";
		array_queue.clear();

		Fout << runtime(pushIt, ring_queue, i, random)
			<< ","
			<< runtime(popIt, ring_queue, i)
			<< ",";
		ring_queue.clear();
			
		Fout << runtime(pushIt, linked_queue, i, random)
			<< ","
			<< runtime(popIt, linked_queue, i)
			<< ",";
		linked_queue.clear();

		Fout << runtime(pushIt, array_stack, i, random)
			<< ","
			<< runtime(popIt, array_stack, i)
			<< ",";
		array_stack.clear();

		Fout << runtime(pushIt, linked_stack, i, random)
			<< ","
			<< runtime(popIt, linked_stack, i)
			<< "\n";
		linked_stack.clear();
	}
}