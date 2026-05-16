CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
SRC_DIR = src
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Task.cpp \
          $(SRC_DIR)/Project.cpp \
          $(SRC_DIR)/User.cpp \
          $(SRC_DIR)/TaskManager.cpp
TARGET = task_manager

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
	rm -rf data

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run