#include "contact.hpp"

#include <random>
#include <algorithm>

Contact random_contact() {
    static std::mt19937 rng{ std::random_device{}() };
    static std::uniform_int_distribution <unsigned int> triple_range{0,999};
    static std::uniform_int_distribution <unsigned int> quad_range{ 0,9999 };
    static std::uniform_int_distribution <unsigned int> name_size{ 2,10 };
    static std::uniform_int_distribution <char> ascii_range{65,90};

    auto ascii_gen = []() { return ascii_range(rng);};

    std::string first_name;
    std::generate_n(std::back_inserter(first_name), name_size(rng), ascii_gen);
    std::string last_name;
    std::generate_n(std::back_inserter(last_name), name_size(rng), ascii_gen);
    unsigned int tel_area = triple_range(rng);
    unsigned int tel_a = triple_range(rng);
    unsigned int tel_b = quad_range(rng);

    return Contact{
        first_name + " " + last_name,
        std::to_string(tel_area) + "-" + std::to_string(tel_a) + "-" + std::to_string(tel_b)
    };
}

std::ostream& operator<<(std::ostream& out, const Contact& contact) {
    out << "Contact: { " << contact.name << ", " << contact.phone << " }";
    return out;
}

bool operator==(const Contact& a, const Contact& b) {
    return a.name == b.name && a.phone == b.phone;
}
