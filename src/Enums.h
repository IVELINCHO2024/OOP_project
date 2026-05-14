#ifndef ENUMS_H
#define ENUMS_H

#include <string>
#include <stdexcept>

enum class Status {
    ToDo,
    InProgress,
    Done
};

enum class Priority {
    Low,
    Medium,
    High,
    Critical
};

enum class Role {
    Admin,
    Member
};

// ── Status conversions ──

inline std::string statusToString(Status s) {
    switch (s) {
        case Status::ToDo:       return "ToDo";
        case Status::InProgress: return "InProgress";
        case Status::Done:       return "Done";
    }
    return "Unknown";
}

inline Status stringToStatus(const std::string& s) {
    if (s == "ToDo")       return Status::ToDo;
    if (s == "InProgress") return Status::InProgress;
    if (s == "Done")       return Status::Done;
    throw std::invalid_argument("Unknown status: " + s);
}

// ── Priority conversions ──

inline std::string priorityToString(Priority p) {
    switch (p) {
        case Priority::Low:      return "Low";
        case Priority::Medium:   return "Medium";
        case Priority::High:     return "High";
        case Priority::Critical: return "Critical";
    }
    return "Unknown";
}

inline Priority stringToPriority(const std::string& s) {
    if (s == "Low")      return Priority::Low;
    if (s == "Medium")   return Priority::Medium;
    if (s == "High")     return Priority::High;
    if (s == "Critical") return Priority::Critical;
    throw std::invalid_argument("Unknown priority: " + s);
}

// ── Role conversions ──

inline std::string roleToString(Role r) {
    switch (r) {
        case Role::Admin:  return "Admin";
        case Role::Member: return "Member";
    }
    return "Unknown";
}

inline Role stringToRole(const std::string& s) {
    if (s == "Admin")  return Role::Admin;
    if (s == "Member") return Role::Member;
    throw std::invalid_argument("Unknown role: " + s);
}

#endif