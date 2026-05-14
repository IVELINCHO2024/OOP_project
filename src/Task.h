#ifndef TASK_H
#define TASK_H

#include "BaseEntity.h"
#include "Enums.h"
#include <string>
#include <vector>

class Task : public BaseEntity {
private:
    Status status;
    Priority priority;
    std::string deadline;
    int assigneeId;
    int projectId;
    std::vector<std::string> tags;

public:
    Task(int id, const std::string& name, const std::string& description,
         Priority priority, const std::string& deadline,
         int projectId = -1, int assigneeId = -1);

    // ── Overrides from BaseEntity ──
    void display() const override;
    std::string toFileString() const override;

    // ── Static factory method for deserialization ──
    static Task fromFileString(const std::string& line);

    // ── Getters ──
    Status getStatus() const;
    Priority getPriority() const;
    std::string getDeadline() const;
    int getAssigneeId() const;
    int getProjectId() const;
    std::vector<std::string> getTags() const;

    // ── Setters ──
    void setStatus(Status newStatus);
    void setPriority(Priority newPriority);
    void setDeadline(const std::string& newDeadline);
    void setAssigneeId(int newAssigneeId);
    void setProjectId(int newProjectId);

    // ── Tag operations ──
    void addTag(const std::string& tag);
    void removeTag(const std::string& tag);

    // ── Utility ──
    bool isOverdue() const;
};

#endif