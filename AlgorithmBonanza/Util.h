#pragma once

#include <random>
#include <vector>

template<size_t Count = 20, typename T, typename Generator>
std::vector<T> generate_data(Generator nextElement) {
	std::vector<T> result;

	for (size_t i = 1; i < Count; i++) {
		result.push_back(nextElement());
	}

	return result;

}

template<size_t Count>
std::vector<int> generate_integers() {
	std::mt19937 gen{ std::random_device{}() };
	std::uniform_int_distribution<int> range{ 0, 100 };

	auto result = generate_data<Count, int>([&gen, &range]() {
		return range(gen);
		});

	return result;
}

std::ostream& operator<<(std::ostream& out, const std::pair<int, int>& data) {
	out << "(" << data.first << "," << data.second << ")";
	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& data) {
	out << "[";
	for (const T& item : data) {
		out << item << ",";
	}
	out << "]";
	return out;
}