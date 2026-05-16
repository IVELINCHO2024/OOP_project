#include "TaskManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <direct.h>

// ══════════════════════════════════════════
// Constructor & Helpers
// ══════════════════════════════════════════

TaskManager::TaskManager(const std::string& dataDir)
    : nextTaskId(1), nextProjectId(1), nextUserId(1),
      dataDir(dataDir),
      tasksFile(dataDir + "/tasks.txt"),
      projectsFile(dataDir + "/projects.txt"),
      usersFile(dataDir + "/users.txt") {

    // Create data directory if it doesn't exist
    _mkdir(dataDir.c_str());
    loadFromFile();
}

Task* TaskManager::findTaskById(int id) {
    for (auto& task : tasks) {
        if (task.getId() == id) return &task;
    }
    return nullptr;
}

Project* TaskManager::findProjectById(int id) {
    for (auto& project : projects) {
        if (project.getId() == id) return &project;
    }
    return nullptr;
}

User* TaskManager::findUserById(int id) {
    for (auto& user : users) {
        if (user.getId() == id) return &user;
    }
    return nullptr;
}

void TaskManager::printSeparator() const {
    std::cout << "----------------------------------------" << std::endl;
}

void TaskManager::pauseScreen() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

// ══════════════════════════════════════════
// Main Menu Loop
// ══════════════════════════════════════════

void TaskManager::run() {
    int choice;

    while (true) {
        std::cout << "\n";
        printSeparator();
        std::cout << "   TASK & PROJECT MANAGER" << std::endl;
        printSeparator();
        std::cout << "  1.  Create Task" << std::endl;
        std::cout << "  2.  List Tasks" << std::endl;
        std::cout << "  3.  Update Task" << std::endl;
        std::cout << "  4.  Delete Task" << std::endl;
        printSeparator();
        std::cout << "  5.  Create Project" << std::endl;
        std::cout << "  6.  List Projects" << std::endl;
        std::cout << "  7.  Update Project" << std::endl;
        std::cout << "  8.  Delete Project" << std::endl;
        printSeparator();
        std::cout << "  9.  Create User" << std::endl;
        std::cout << "  10. List Users" << std::endl;
        std::cout << "  11. Update User" << std::endl;
        std::cout << "  12. Delete User" << std::endl;
        printSeparator();
        std::cout << "  13. Assign Task to User" << std::endl;
        std::cout << "  14. Unassign Task" << std::endl;
        std::cout << "  15. Change Task Status" << std::endl;
        std::cout << "  16. Filter Tasks" << std::endl;
        std::cout << "  17. Sort Tasks by Priority" << std::endl;
        std::cout << "  18. Manage Tags" << std::endl;
        std::cout << "  19. Show Overdue Tasks" << std::endl;
        std::cout << "  20. Show Statistics" << std::endl;
        printSeparator();
        std::cout << "  0.  Save & Exit" << std::endl;
        printSeparator();
        std::cout << "  Choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 1:  createTask();         break;
            case 2:  listTasks();          break;
            case 3:  updateTask();         break;
            case 4:  deleteTask();         break;
            case 5:  createProject();      break;
            case 6:  listProjects();       break;
            case 7:  updateProject();      break;
            case 8:  deleteProject();      break;
            case 9:  createUser();         break;
            case 10: listUsers();          break;
            case 11: updateUser();         break;
            case 12: deleteUser();         break;
            case 13: assignTask();         break;
            case 14: unassignTask();       break;
            case 15: changeTaskStatus();   break;
            case 16: filterTasks();        break;
            case 17: sortTasksByPriority();break;
            case 18: manageTags();         break;
            case 19: showOverdueTasks();   break;
            case 20: showStatistics();     break;
            case 0:
                saveToFile();
                std::cout << "Data saved. Goodbye!" << std::endl;
                return;
            default:
                std::cout << "Invalid choice. Try again." << std::endl;
        }
    }
}

// ══════════════════════════════════════════
// CRUD: Tasks
// ══════════════════════════════════════════

void TaskManager::createTask() {
    std::cin.ignore();
    std::string name, description, deadline;
    int priorityChoice;

    std::cout << "\n-- Create Task --" << std::endl;
    std::cout << "Name: ";
    std::getline(std::cin, name);
    std::cout << "Description: ";
    std::getline(std::cin, description);
    std::cout << "Deadline (YYYY-MM-DD): ";
    std::getline(std::cin, deadline);
    std::cout << "Priority (1=Low, 2=Medium, 3=High, 4=Critical): ";
    std::cin >> priorityChoice;

    Priority priority;
    switch (priorityChoice) {
        case 1: priority = Priority::Low;      break;
        case 2: priority = Priority::Medium;   break;
        case 3: priority = Priority::High;     break;
        case 4: priority = Priority::Critical; break;
        default:
            std::cout << "Invalid priority. Defaulting to Low." << std::endl;
            priority = Priority::Low;
    }

    Task task(nextTaskId++, name, description, priority, deadline);
    tasks.push_back(task);

    std::cout << "Task #" << task.getId() << " created successfully!" << std::endl;
}

void TaskManager::listTasks() const {
    if (tasks.empty()) {
        std::cout << "\nNo tasks found." << std::endl;
        return;
    }

    std::cout << "\n-- All Tasks (" << tasks.size() << ") --" << std::endl;
    for (const auto& task : tasks) {
        task.display();
        std::cout << std::endl;
    }
}

void TaskManager::updateTask() {
    int id;
    std::cout << "\n-- Update Task --" << std::endl;
    std::cout << "Enter Task ID: ";
    std::cin >> id;

    Task* task = findTaskById(id);
    if (!task) {
        std::cout << "Task not found." << std::endl;
        return;
    }

    task->display();
    std::cin.ignore();

    std::string input;
    std::cout << "New name (Enter to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) task->setName(input);

    std::cout << "New description (Enter to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) task->setDescription(input);

    std::cout << "New deadline (Enter to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) task->setDeadline(input);

    std::cout << "New priority (1=Low, 2=Med, 3=High, 4=Critical, 0=skip): ";
    int p;
    std::cin >> p;
    switch (p) {
        case 1: task->setPriority(Priority::Low);      break;
        case 2: task->setPriority(Priority::Medium);   break;
        case 3: task->setPriority(Priority::High);     break;
        case 4: task->setPriority(Priority::Critical); break;
        default: break;
    }

    std::cout << "Task updated!" << std::endl;
}

void TaskManager::deleteTask() {
    int id;
    std::cout << "\n-- Delete Task --" << std::endl;
    std::cout << "Enter Task ID: ";
    std::cin >> id;

    auto it = std::find_if(tasks.begin(), tasks.end(),
        [id](const Task& t) { return t.getId() == id; });

    if (it == tasks.end()) {
        std::cout << "Task not found." << std::endl;
        return;
    }

    // Remove task from its project
    int projectId = it->getProjectId();
    if (projectId != -1) {
        Project* project = findProjectById(projectId);
        if (project) project->removeTask(id);
    }

    tasks.erase(it);
    std::cout << "Task #" << id << " deleted." << std::endl;
}

// ══════════════════════════════════════════
// CRUD: Projects
// ══════════════════════════════════════════

void TaskManager::createProject() {
    std::cin.ignore();
    std::string name, description;

    std::cout << "\n-- Create Project --" << std::endl;
    std::cout << "Name: ";
    std::getline(std::cin, name);
    std::cout << "Description: ";
    std::getline(std::cin, description);

    Project project(nextProjectId++, name, description);
    projects.push_back(project);

    std::cout << "Project #" << project.getId() << " created!" << std::endl;
}

void TaskManager::listProjects() const {
    if (projects.empty()) {
        std::cout << "\nNo projects found." << std::endl;
        return;
    }

    std::cout << "\n-- All Projects (" << projects.size() << ") --" << std::endl;
    for (const auto& project : projects) {
        project.display();
        std::cout << std::endl;
    }
}

void TaskManager::updateProject() {
    int id;
    std::cout << "\n-- Update Project --" << std::endl;
    std::cout << "Enter Project ID: ";
    std::cin >> id;

    Project* project = findProjectById(id);
    if (!project) {
        std::cout << "Project not found." << std::endl;
        return;
    }

    project->display();
    std::cin.ignore();

    std::string input;
    std::cout << "New name (Enter to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) project->setName(input);

    std::cout << "New description (Enter to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) project->setDescription(input);

    std::cout << "Project updated!" << std::endl;
}

void TaskManager::deleteProject() {
    int id;
    std::cout << "\n-- Delete Project --" << std::endl;
    std::cout << "Enter Project ID: ";
    std::cin >> id;

    auto it = std::find_if(projects.begin(), projects.end(),
        [id](const Project& p) { return p.getId() == id; });

    if (it == projects.end()) {
        std::cout << "Project not found." << std::endl;
        return;
    }

    // Unlink all tasks from this project
    for (auto& task : tasks) {
        if (task.getProjectId() == id) {
            task.setProjectId(-1);
        }
    }

    projects.erase(it);
    std::cout << "Project #" << id << " deleted." << std::endl;
}

// ══════════════════════════════════════════
// CRUD: Users
// ══════════════════════════════════════════

void TaskManager::createUser() {
    std::cin.ignore();
    std::string name, description;
    int roleChoice;

    std::cout << "\n-- Create User --" << std::endl;
    std::cout << "Name: ";
    std::getline(std::cin, name);
    std::cout << "Description: ";
    std::getline(std::cin, description);
    std::cout << "Role (1=Admin, 2=Member): ";
    std::cin >> roleChoice;

    Role role = (roleChoice == 1) ? Role::Admin : Role::Member;

    User user(nextUserId++, name, description, role);
    users.push_back(user);

    std::cout << "User #" << user.getId() << " created!" << std::endl;
}

void TaskManager::listUsers() const {
    if (users.empty()) {
        std::cout << "\nNo users found." << std::endl;
        return;
    }

    std::cout << "\n-- All Users (" << users.size() << ") --" << std::endl;
    for (const auto& user : users) {
        user.display();
        std::cout << std::endl;
    }
}

void TaskManager::updateUser() {
    int id;
    std::cout << "\n-- Update User --" << std::endl;
    std::cout << "Enter User ID: ";
    std::cin >> id;

    User* user = findUserById(id);
    if (!user) {
        std::cout << "User not found." << std::endl;
        return;
    }

    user->display();
    std::cin.ignore();

    std::string input;
    std::cout << "New name (Enter to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) user->setName(input);

    std::cout << "New description (Enter to skip): ";
    std::getline(std::cin, input);
    if (!input.empty()) user->setDescription(input);

    std::cout << "New role (1=Admin, 2=Member, 0=skip): ";
    int r;
    std::cin >> r;
    if (r == 1) user->setRole(Role::Admin);
    else if (r == 2) user->setRole(Role::Member);

    std::cout << "User updated!" << std::endl;
}

void TaskManager::deleteUser() {
    int id;
    std::cout << "\n-- Delete User --" << std::endl;
    std::cout << "Enter User ID: ";
    std::cin >> id;

    auto it = std::find_if(users.begin(), users.end(),
        [id](const User& u) { return u.getId() == id; });

    if (it == users.end()) {
        std::cout << "User not found." << std::endl;
        return;
    }

    // Unassign all tasks from this user
    for (auto& task : tasks) {
        if (task.getAssigneeId() == id) {
            task.setAssigneeId(-1);
        }
    }

    // Remove user from all projects
    for (auto& project : projects) {
        project.removeMember(id);
    }

    users.erase(it);
    std::cout << "User #" << id << " deleted." << std::endl;
}

// ══════════════════════════════════════════
// Feature: Assign / Unassign
// ══════════════════════════════════════════

void TaskManager::assignTask() {
    int taskId, userId;
    std::cout << "\n-- Assign Task --" << std::endl;
    std::cout << "Task ID: ";
    std::cin >> taskId;
    std::cout << "User ID: ";
    std::cin >> userId;

    Task* task = findTaskById(taskId);
    if (!task) {
        std::cout << "Task not found." << std::endl;
        return;
    }

    User* user = findUserById(userId);
    if (!user) {
        std::cout << "User not found." << std::endl;
        return;
    }

    task->setAssigneeId(userId);
    std::cout << "Task #" << taskId << " assigned to "
              << user->getName() << "!" << std::endl;
}

void TaskManager::unassignTask() {
    int taskId;
    std::cout << "\n-- Unassign Task --" << std::endl;
    std::cout << "Task ID: ";
    std::cin >> taskId;

    Task* task = findTaskById(taskId);
    if (!task) {
        std::cout << "Task not found." << std::endl;
        return;
    }

    task->setAssigneeId(-1);
    std::cout << "Task #" << taskId << " unassigned." << std::endl;
}

// ══════════════════════════════════════════
// Feature: Status Workflow
// ══════════════════════════════════════════

void TaskManager::changeTaskStatus() {
    int taskId, statusChoice;
    std::cout << "\n-- Change Task Status --" << std::endl;
    std::cout << "Task ID: ";
    std::cin >> taskId;

    Task* task = findTaskById(taskId);
    if (!task) {
        std::cout << "Task not found." << std::endl;
        return;
    }

    std::cout << "Current status: " << statusToString(task->getStatus()) << std::endl;
    std::cout << "New status (1=ToDo, 2=InProgress, 3=Done): ";
    std::cin >> statusChoice;

    switch (statusChoice) {
        case 1: task->setStatus(Status::ToDo);       break;
        case 2: task->setStatus(Status::InProgress);  break;
        case 3: task->setStatus(Status::Done);         break;
        default:
            std::cout << "Invalid status." << std::endl;
            return;
    }

    std::cout << "Status updated to " << statusToString(task->getStatus())
              << "!" << std::endl;
}

// ══════════════════════════════════════════
// Feature: Filter Tasks
// ══════════════════════════════════════════

void TaskManager::filterTasks() const {
    int choice;
    std::cout << "\n-- Filter Tasks --" << std::endl;
    std::cout << "  1. By Status" << std::endl;
    std::cout << "  2. By Priority" << std::endl;
    std::cout << "  3. By Assignee" << std::endl;
    std::cout << "  4. By Project" << std::endl;
    std::cout << "  5. By Tag" << std::endl;
    std::cout << "  Choice: ";
    std::cin >> choice;

    std::vector<const Task*> results;

    switch (choice) {
        case 1: {
            int s;
            std::cout << "Status (1=ToDo, 2=InProgress, 3=Done): ";
            std::cin >> s;
            Status status;
            switch (s) {
                case 1: status = Status::ToDo;       break;
                case 2: status = Status::InProgress;  break;
                case 3: status = Status::Done;         break;
                default:
                    std::cout << "Invalid status." << std::endl;
                    return;
            }
            for (const auto& t : tasks) {
                if (t.getStatus() == status) results.push_back(&t);
            }
            break;
        }
        case 2: {
            int p;
            std::cout << "Priority (1=Low, 2=Med, 3=High, 4=Critical): ";
            std::cin >> p;
            Priority priority;
            switch (p) {
                case 1: priority = Priority::Low;      break;
                case 2: priority = Priority::Medium;   break;
                case 3: priority = Priority::High;     break;
                case 4: priority = Priority::Critical; break;
                default:
                    std::cout << "Invalid priority." << std::endl;
                    return;
            }
            for (const auto& t : tasks) {
                if (t.getPriority() == priority) results.push_back(&t);
            }
            break;
        }
        case 3: {
            int userId;
            std::cout << "User ID: ";
            std::cin >> userId;
            for (const auto& t : tasks) {
                if (t.getAssigneeId() == userId) results.push_back(&t);
            }
            break;
        }
        case 4: {
            int projectId;
            std::cout << "Project ID: ";
            std::cin >> projectId;
            for (const auto& t : tasks) {
                if (t.getProjectId() == projectId) results.push_back(&t);
            }
            break;
        }
        case 5: {
            std::cin.ignore();
            std::string tag;
            std::cout << "Tag: ";
            std::getline(std::cin, tag);
            for (const auto& t : tasks) {
                auto tags = t.getTags();
                if (std::find(tags.begin(), tags.end(), tag) != tags.end()) {
                    results.push_back(&t);
                }
            }
            break;
        }
        default:
            std::cout << "Invalid choice." << std::endl;
            return;
    }

    if (results.empty()) {
        std::cout << "No tasks match the filter." << std::endl;
    } else {
        std::cout << "\n-- Results (" << results.size() << ") --" << std::endl;
        for (const auto* t : results) {
            t->display();
            std::cout << std::endl;
        }
    }
}

// ══════════════════════════════════════════
// Feature: Sort by Priority
// ══════════════════════════════════════════

void TaskManager::sortTasksByPriority() {
    if (tasks.empty()) {
        std::cout << "\nNo tasks to sort." << std::endl;
        return;
    }

    std::sort(tasks.begin(), tasks.end(),
        [](const Task& a, const Task& b) {
            return static_cast<int>(a.getPriority()) > static_cast<int>(b.getPriority());
        });

    std::cout << "Tasks sorted by priority (Critical first)." << std::endl;
    listTasks();
}

// ══════════════════════════════════════════
// Feature: Manage Tags
// ══════════════════════════════════════════

void TaskManager::manageTags() {
    int taskId;
    std::cout << "\n-- Manage Tags --" << std::endl;
    std::cout << "Task ID: ";
    std::cin >> taskId;

    Task* task = findTaskById(taskId);
    if (!task) {
        std::cout << "Task not found." << std::endl;
        return;
    }

    task->display();

    int choice;
    std::cout << "  1. Add Tag" << std::endl;
    std::cout << "  2. Remove Tag" << std::endl;
    std::cout << "  Choice: ";
    std::cin >> choice;
    std::cin.ignore();

    std::string tag;
    std::cout << "Tag name: ";
    std::getline(std::cin, tag);

    if (choice == 1) {
        task->addTag(tag);
        std::cout << "Tag '" << tag << "' added!" << std::endl;
    } else if (choice == 2) {
        task->removeTag(tag);
        std::cout << "Tag '" << tag << "' removed!" << std::endl;
    } else {
        std::cout << "Invalid choice." << std::endl;
    }
}

// ══════════════════════════════════════════
// Feature: Overdue Tasks
// ══════════════════════════════════════════

void TaskManager::showOverdueTasks() const {
    std::cout << "\n-- Overdue Tasks --" << std::endl;

    bool found = false;
    for (const auto& task : tasks) {
        if (task.isOverdue()) {
            task.display();
            std::cout << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No overdue tasks!" << std::endl;
    }
}

// ══════════════════════════════════════════
// Feature: Statistics
// ══════════════════════════════════════════

void TaskManager::showStatistics() const {
    std::cout << "\n-- Statistics --" << std::endl;
    printSeparator();

    int total = tasks.size();
    if (total == 0) {
        std::cout << "No tasks to analyze." << std::endl;
        return;
    }

    int toDo = 0, inProgress = 0, done = 0, overdue = 0;
    for (const auto& t : tasks) {
        switch (t.getStatus()) {
            case Status::ToDo:       toDo++;       break;
            case Status::InProgress: inProgress++; break;
            case Status::Done:       done++;       break;
        }
        if (t.isOverdue()) overdue++;
    }

    std::cout << "  Total tasks:    " << total << std::endl;
    std::cout << "  ToDo:           " << toDo << std::endl;
    std::cout << "  In Progress:    " << inProgress << std::endl;
    std::cout << "  Done:           " << done << std::endl;
    std::cout << "  Overdue:        " << overdue << std::endl;
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "  Completion:     " << (done * 100.0 / total) << "%" << std::endl;

    printSeparator();
    std::cout << "  Total projects: " << projects.size() << std::endl;
    std::cout << "  Total users:    " << users.size() << std::endl;

    // Workload per user
    if (!users.empty()) {
        printSeparator();
        std::cout << "  Workload per user:" << std::endl;
        for (const auto& user : users) {
            int count = 0;
            for (const auto& t : tasks) {
                if (t.getAssigneeId() == user.getId() && t.getStatus() != Status::Done) {
                    count++;
                }
            }
            std::cout << "    " << user.getName() << ": "
                      << count << " active task(s)" << std::endl;
        }
    }
}

// ══════════════════════════════════════════
// Persistence: Save & Load
// ══════════════════════════════════════════

void TaskManager::saveToFile() const {
    // Save tasks
    std::ofstream taskOut(tasksFile);
    if (taskOut.is_open()) {
        taskOut << nextTaskId << std::endl;
        for (const auto& task : tasks) {
            taskOut << task.toFileString() << std::endl;
        }
        taskOut.close();
    }

    // Save projects
    std::ofstream projOut(projectsFile);
    if (projOut.is_open()) {
        projOut << nextProjectId << std::endl;
        for (const auto& project : projects) {
            projOut << project.toFileString() << std::endl;
        }
        projOut.close();
    }

    // Save users
    std::ofstream userOut(usersFile);
    if (userOut.is_open()) {
        userOut << nextUserId << std::endl;
        for (const auto& user : users) {
            userOut << user.toFileString() << std::endl;
        }
        userOut.close();
    }
}

void TaskManager::loadFromFile() {
    // Load tasks
    std::ifstream taskIn(tasksFile);
    if (taskIn.is_open()) {
        std::string line;
        if (std::getline(taskIn, line)) {
            nextTaskId = std::stoi(line);
        }
        while (std::getline(taskIn, line)) {
            if (!line.empty()) {
                try {
                    tasks.push_back(Task::fromFileString(line));
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Skipping invalid task line: "
                              << e.what() << std::endl;
                }
            }
        }
        taskIn.close();
    }

    // Load projects
    std::ifstream projIn(projectsFile);
    if (projIn.is_open()) {
        std::string line;
        if (std::getline(projIn, line)) {
            nextProjectId = std::stoi(line);
        }
        while (std::getline(projIn, line)) {
            if (!line.empty()) {
                try {
                    projects.push_back(Project::fromFileString(line));
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Skipping invalid project line: "
                              << e.what() << std::endl;
                }
            }
        }
        projIn.close();
    }

    // Load users
    std::ifstream userIn(usersFile);
    if (userIn.is_open()) {
        std::string line;
        if (std::getline(userIn, line)) {
            nextUserId = std::stoi(line);
        }
        while (std::getline(userIn, line)) {
            if (!line.empty()) {
                try {
                    users.push_back(User::fromFileString(line));
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Skipping invalid user line: "
                              << e.what() << std::endl;
                }
            }
        }
        userIn.close();
    }
}