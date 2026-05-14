#include "User.h"
#include <iostream>
#include <sstream>
#include <vector>

// ── Constructor ──
User::User(int id, const std::string& name, const std::string& description, Role role)
    : BaseEntity(id, name, description), role(role) {}

// ── Display ──
void User::display() const {
    std::cout << "=== User #" << id << " ===" << std::endl;
    std::cout << "  Name:        " << name << std::endl;
    std::cout << "  Description: " << description << std::endl;
    std::cout << "  Role:        " << roleToString(role) << std::endl;
}

// ── Serialization ──
// Format: id|name|description|role
std::string User::toFileString() const {
    return std::to_string(id) + "|" +
           name + "|" +
           description + "|" +
           roleToString(role);
}

// ── Deserialization ──
User User::fromFileString(const std::string& line) {
    std::istringstream stream(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(stream, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 4) {
        throw std::invalid_argument("Invalid user line: " + line);
    }

    int id = std::stoi(tokens[0]);
    std::string name = tokens[1];
    std::string description = tokens[2];
    Role role = stringToRole(tokens[3]);

    return User(id, name, description, role);
}

// ── Getters ──
Role User::getRole() const { return role; }

// ── Setters ──
void User::setRole(Role newRole) { role = newRole; }