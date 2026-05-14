#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include <string>

class BaseEntity {
protected:
    int id;
    std::string name;
    std::string description;

public:
    BaseEntity(int id, const std::string& name, const std::string& description)
        : id(id), name(name), description(description) {}

    virtual ~BaseEntity() = default;

    // ── Pure virtual methods ──
    virtual void display() const = 0;
    virtual std::string toFileString() const = 0;

    // ── Getters ──
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

    // ── Setters ──
    void setName(const std::string& newName) { name = newName; }
    void setDescription(const std::string& newDesc) { description = newDesc; }
};

#endif