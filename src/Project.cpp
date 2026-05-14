#include "Project.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// ── Constructor ──
Project::Project(int id, const std::string& name, const std::string& description)
    : BaseEntity(id, name, description) {}

// ── Display ──
void Project::display() const {
    std::cout << "=== Project #" << id << " ===" << std::endl;
    std::cout << "  Name:        " << name << std::endl;
    std::cout << "  Description: " << description << std::endl;

    std::cout << "  Members:     ";
    if (memberIds.empty()) {
        std::cout << "None";
    } else {
        for (size_t i = 0; i < memberIds.size(); i++) {
            std::cout << memberIds[i];
            if (i < memberIds.size() - 1) std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "  Tasks:       ";
    if (taskIds.empty()) {
        std::cout << "None";
    } else {
        for (size_t i = 0; i < taskIds.size(); i++) {
            std::cout << taskIds[i];
            if (i < taskIds.size() - 1) std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

// ── Serialization ──
// Format: id|name|description|member1,member2|task1,task2
std::string Project::toFileString() const {
    std::string result = std::to_string(id) + "|" +
                         name + "|" +
                         description + "|";

    for (size_t i = 0; i < memberIds.size(); i++) {
        result += std::to_string(memberIds[i]);
        if (i < memberIds.size() - 1) result += ",";
    }

    result += "|";

    for (size_t i = 0; i < taskIds.size(); i++) {
        result += std::to_string(taskIds[i]);
        if (i < taskIds.size() - 1) result += ",";
    }

    return result;
}

// ── Deserialization ──
Project Project::fromFileString(const std::string& line) {
    std::istringstream stream(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(stream, token, '|')) {
        tokens.push_back(token);
    }

    // Need at least id, name, description
    if (tokens.size() < 3) {
        throw std::invalid_argument("Invalid project line: " + line);
    }

    int id = std::stoi(tokens[0]);
    std::string name = tokens[1];
    std::string description = tokens[2];

    Project project(id, name, description);

    // Parse member IDs
    if (tokens.size() > 3 && !tokens[3].empty()) {
        std::istringstream memberStream(tokens[3]);
        std::string memberId;
        while (std::getline(memberStream, memberId, ',')) {
            if (!memberId.empty()) {
                project.addMember(std::stoi(memberId));
            }
        }
    }

    // Parse task IDs
    if (tokens.size() > 4 && !tokens[4].empty()) {
        std::istringstream taskStream(tokens[4]);
        std::string taskId;
        while (std::getline(taskStream, taskId, ',')) {
            if (!taskId.empty()) {
                project.addTask(std::stoi(taskId));
            }
        }
    }

    return project;
}

// ── Member management ──
void Project::addMember(int userId) {
    if (!hasMember(userId)) {
        memberIds.push_back(userId);
    }
}

void Project::removeMember(int userId) {
    auto it = std::find(memberIds.begin(), memberIds.end(), userId);
    if (it != memberIds.end()) {
        memberIds.erase(it);
    }
}

std::vector<int> Project::getMembers() const { return memberIds; }

bool Project::hasMember(int userId) const {
    return std::find(memberIds.begin(), memberIds.end(), userId) != memberIds.end();
}

// ── Task management ──
void Project::addTask(int taskId) {
    if (!hasTask(taskId)) {
        taskIds.push_back(taskId);
    }
}

void Project::removeTask(int taskId) {
    auto it = std::find(taskIds.begin(), taskIds.end(), taskId);
    if (it != taskIds.end()) {
        taskIds.erase(it);
    }
}

std::vector<int> Project::getTasks() const { return taskIds; }

bool Project::hasTask(int taskId) const {
    return std::find(taskIds.begin(), taskIds.end(), taskId) != taskIds.end();
}