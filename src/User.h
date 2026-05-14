#ifndef USER_H
#define USER_H

#include "BaseEntity.h"
#include "Enums.h"
#include <string>

class User : public BaseEntity {
private:
    Role role;

public:
    User(int id, const std::string& name, const std::string& description, Role role);

    // ── Overrides from BaseEntity ──
    void display() const override;
    std::string toFileString() const override;

    // ── Static factory method ──
    static User fromFileString(const std::string& line);

    // ── Getters ──
    Role getRole() const;

    // ── Setters ──
    void setRole(Role newRole);
};

#endif