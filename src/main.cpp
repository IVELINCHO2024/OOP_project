#include "TaskManager.h"
#include <iostream>

int main() {
    try {
        TaskManager manager;
        manager.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}