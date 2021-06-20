#pragma once

#include <string>
#include <iostream>

struct Contact {
    std::string name;
    std::string phone;
};

Contact random_contact();
std::ostream& operator<<(std::ostream& out, const Contact& contact);
bool operator==(const Contact& a, const Contact& b);

namespace std {
    template<> struct hash<Contact> {
        std::size_t operator()(const Contact& contact) const noexcept {
            auto h_name = std::hash<std::string>{}(contact.name);
            auto h_phone = std::hash<std::string>{}(contact.phone);
            return h_name ^ (h_phone << 1);
        }
    };
}
