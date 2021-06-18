//#include <string>
#include <iostream>
#include <algorithm>
#include <random>
#include "associative.h"
#include "contact.hpp"

using namespace std::string_literals;

constexpr std::size_t Capacity = 16;

int main() {
    my::lookup_table<int, 64> hotel_room_occupancy;

    std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<size_t> room_range{ 0,63 };

    for (int i = 0; i < 30; ++i) {
        auto room = room_range(rng);
        std::cout << "guest" << i << " has arrived and wants to stay in room" << room << "\n";

        auto occupied = hotel_room_occupancy.lookup(room);
        std::cout << "\tthe room is " << (occupied.has_value() ? "occupied" : "vacant") << "\n";

        if (occupied.has_value()) {
            std::cout << "\tsorry guest, " << occupied.value() << " is already here :(\n\n";
        }
        else {
            hotel_room_occupancy.set(room, i);
            std::cout << "\tWelcome to the hotel :)\n\n";
        }
    }
}
