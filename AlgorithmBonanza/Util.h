#pragma once

#include <random>
#include <vector>

template< typename T, typename Generator>
std::vector<T> generate_data(size_t count, Generator nextElement) {
	std::vector<T> result;

	for (size_t i = 0; i < count; i++) {
		result.push_back(nextElement());
	}

	return result;

}

template<typename T>
std::vector<T> generate_integers(size_t count) {
	std::mt19937 gen{ std::random_device{}() };
	std::uniform_int_distribution range{ 0, 20000 };

	auto result = generate_data<T>(count, [&gen, &range]() {
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
