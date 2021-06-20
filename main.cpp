#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

#include "associative.h"
#include "contact.hpp"

auto runtime = [](auto&& func, auto&&...param) {
    auto tick = std::chrono::steady_clock::now();
    std::forward<decltype(func)>(func)(std::forward<decltype(param)>(param)...);
    auto tock = std::chrono::steady_clock::now();
    std::chrono::duration<double> seconds = tock - tick;
    return seconds.count();
};

auto insertFunc = [](auto&& table, int size, int random) {
    for (int i = 0; i < size; ++i)
    {
        table.insert(random);
    }
};

auto eraseFunc = [](auto&& table, int size,auto& it) {
    for (int i = 0; i < size; ++i)
    {
        table.erase(it);
    }
};

auto findFunc = [](auto&& table, int size, int value) {
    for (int i = 0; i < size; ++i)
    {
        table.find(value);
    }
};

int main() {
    std::string results = "results.csv";
    std::ofstream Fout{ results , std::ios::out };
    my::chaining_hash_table<int> chaining_table;
    my::probing_hash_table<int> probing_table;

    Fout << "n, chaining insert, chaining remove, chaining find, probing insert, probing remove, probing find\n";
    for (int i = 100; i <= 1000; i += 50) {
        std::mt19937 gen{ std::random_device{}() };
        std::uniform_int_distribution<int> range{ 0, 10000 };
        auto random = range(gen);

        Fout << i << ",";

        Fout << runtime(insertFunc, chaining_table, i, random) << ",";
        auto it = chaining_table.begin();
        Fout << runtime(eraseFunc, chaining_table, i, it) << ","
             << runtime(findFunc, chaining_table, i, random) << ",";
        chaining_table.clear();

        Fout << runtime(insertFunc, probing_table, i, random) << ",";
        auto it2 = probing_table.begin();
        Fout << runtime(eraseFunc, probing_table, i, it2) << ","
             << runtime(findFunc, probing_table, i, random) << "\n";
        probing_table.clear();
    }

}
