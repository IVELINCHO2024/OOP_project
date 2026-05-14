#include "Task.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>

// ── Constructor ──
Task::Task(int id, const std::string& name, const std::string& description,
           Priority priority, const std::string& deadline,
           int projectId, int assigneeId)
    : BaseEntity(id, name, description),
      status(Status::ToDo),
      priority(priority),
      deadline(deadline),
      assigneeId(assigneeId),
      projectId(projectId) {}

// ── Display ──
void Task::display() const {
    std::cout << "=== Task #" << id << " ===" << std::endl;
    std::cout << "  Name:        " << name << std::endl;
    std::cout << "  Description: " << description << std::endl;
    std::cout << "  Status:      " << statusToString(status) << std::endl;
    std::cout << "  Priority:    " << priorityToString(priority) << std::endl;
    std::cout << "  Deadline:    " << deadline << std::endl;
    std::cout << "  Assignee ID: " << (assigneeId == -1 ? "None" : std::to_string(assigneeId)) << std::endl;
    std::cout << "  Project ID:  " << (projectId == -1 ? "None" : std::to_string(projectId)) << std::endl;

    std::cout << "  Tags:        ";
    if (tags.empty()) {
        std::cout << "None";
    } else {
        for (size_t i = 0; i < tags.size(); i++) {
            std::cout << tags[i];
            if (i < tags.size() - 1) std::cout << ", ";
        }
    }
    std::cout << std::endl;

    if (isOverdue()) {
        std::cout << "  *** OVERDUE ***" << std::endl;
    }
}

// ── Serialization ──
// Format: id|name|description|status|priority|deadline|assigneeId|projectId|tag1,tag2,tag3
std::string Task::toFileString() const {
    std::string result = std::to_string(id) + "|" +
                         name + "|" +
                         description + "|" +
                         statusToString(status) + "|" +
                         priorityToString(priority) + "|" +
                         deadline + "|" +
                         std::to_string(assigneeId) + "|" +
                         std::to_string(projectId) + "|";

    for (size_t i = 0; i < tags.size(); i++) {
        result += tags[i];
        if (i < tags.size() - 1) result += ",";
    }

    return result;
}

// ── Deserialization ──
Task Task::fromFileString(const std::string& line) {
    std::istringstream stream(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(stream, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 8) {
        throw std::invalid_argument("Invalid task line: " + line);
    }

    int id = std::stoi(tokens[0]);
    std::string name = tokens[1];
    std::string description = tokens[2];
    Status status = stringToStatus(tokens[3]);
    Priority priority = stringToPriority(tokens[4]);
    std::string deadline = tokens[5];
    int assigneeId = std::stoi(tokens[6]);
    int projectId = std::stoi(tokens[7]);

    Task task(id, name, description, priority, deadline, projectId, assigneeId);
    task.setStatus(status);

    // Parse tags if present
    if (tokens.size() > 8 && !tokens[8].empty()) {
        std::istringstream tagStream(tokens[8]);
        std::string tag;
        while (std::getline(tagStream, tag, ',')) {
            if (!tag.empty()) {
                task.addTag(tag);
            }
        }
    }

    return task;
}

// ── Getters ──
Status Task::getStatus() const { return status; }
Priority Task::getPriority() const { return priority; }
std::string Task::getDeadline() const { return deadline; }
int Task::getAssigneeId() const { return assigneeId; }
int Task::getProjectId() const { return projectId; }
std::vector<std::string> Task::getTags() const { return tags; }

// ── Setters ──
void Task::setStatus(Status newStatus) { status = newStatus; }
void Task::setPriority(Priority newPriority) { priority = newPriority; }
void Task::setDeadline(const std::string& newDeadline) { deadline = newDeadline; }
void Task::setAssigneeId(int newAssigneeId) { assigneeId = newAssigneeId; }
void Task::setProjectId(int newProjectId) { projectId = newProjectId; }

// ── Tag operations ──
void Task::addTag(const std::string& tag) {
    // Prevent duplicates
    auto it = std::find(tags.begin(), tags.end(), tag);
    if (it == tags.end()) {
        tags.push_back(tag);
    }
}

void Task::removeTag(const std::string& tag) {
    auto it = std::find(tags.begin(), tags.end(), tag);
    if (it != tags.end()) {
        tags.erase(it);
    }
}

// ── Utility ──
// Deadline format: YYYY-MM-DD
bool Task::isOverdue() const {
    if (status == Status::Done || deadline.empty()) {
        return false;
    }

    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    int currentYear = localTime->tm_year + 1900;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;

    // Parse deadline
    int dYear, dMonth, dDay;
    char dash1, dash2;
    std::istringstream ds(deadline);
    ds >> dYear >> dash1 >> dMonth >> dash2 >> dDay;

    if (ds.fail()) return false;

    // Compare as single integer: YYYYMMDD
    int currentDate = currentYear * 10000 + currentMonth * 100 + currentDay;
    int deadlineDate = dYear * 10000 + dMonth * 100 + dDay;

    return currentDate > deadlineDate;
}