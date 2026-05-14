#ifndef PROJECT_H
#define PROJECT_H

#include "BaseEntity.h"
#include <string>
#include <vector>

class Project : public BaseEntity {
private:
    std::vector<int> memberIds;
    std::vector<int> taskIds;

public:
    Project(int id, const std::string& name, const std::string& description);

    // ── Overrides from BaseEntity ──
    void display() const override;
    std::string toFileString() const override;

    // ── Static factory method ──
    static Project fromFileString(const std::string& line);

    // ── Member management ──
    void addMember(int userId);
    void removeMember(int userId);
    std::vector<int> getMembers() const;
    bool hasMember(int userId) const;

    // ── Task management ──
    void addTask(int taskId);
    void removeTask(int taskId);
    std::vector<int> getTasks() const;
    bool hasTask(int taskId) const;
};

#endif