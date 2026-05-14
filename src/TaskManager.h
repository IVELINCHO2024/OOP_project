#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "Task.h"
#include "Project.h"
#include "User.h"
#include <vector>
#include <string>

class TaskManager {
private:
    std::vector<Task> tasks;
    std::vector<Project> projects;
    std::vector<User> users;

    // ── ID generators ──
    int nextTaskId;
    int nextProjectId;
    int nextUserId;

    // ── File paths ──
    std::string dataDir;
    std::string tasksFile;
    std::string projectsFile;
    std::string usersFile;

    // ── Private helpers ──
    Task* findTaskById(int id);
    Project* findProjectById(int id);
    User* findUserById(int id);

    void printSeparator() const;
    void pauseScreen() const;

public:
    TaskManager(const std::string& dataDir = "data");

    // ── Main loop ──
    void run();

    // ── CRUD: Tasks ──
    void createTask();
    void listTasks() const;
    void updateTask();
    void deleteTask();

    // ── CRUD: Projects ──
    void createProject();
    void listProjects() const;
    void updateProject();
    void deleteProject();

    // ── CRUD: Users ──
    void createUser();
    void listUsers() const;
    void updateUser();
    void deleteUser();

    // ── Feature: Assign/Unassign ──
    void assignTask();
    void unassignTask();

    // ── Feature: Status workflow ──
    void changeTaskStatus();

    // ── Feature: Filtering & Search ──
    void filterTasks() const;

    // ── Feature: Sort by priority ──
    void sortTasksByPriority();

    // ── Feature: Statistics ──
    void showStatistics() const;

    // ── Feature: Overdue tasks ──
    void showOverdueTasks() const;

    // ── Feature: Tags ──
    void manageTags();

    // ── Persistence ──
    void saveToFile() const;
    void loadFromFile();
};

#endif