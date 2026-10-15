#include "../include/ToDoList.h"
#include "../include/Validation.h"
#include "../include/Paths.h"

#include <fstream>
#include <string>
#include <vector>
#include <cstddef>

bool ToDoList::isMarked(int num) const {
    return tasks[num - 1][1] == 'X';
}

bool ToDoList::save(const std::string& filename) const {
    std::ofstream file(kListsDir + filename);
    if (file.is_open()) {
        for (const auto& task : tasks) {
            file << task << '\n';
        }
        file.close();
        return true;
    }
    return false;
}

bool ToDoList::remove(int num) {
    if (!validIndex(num, tasks.size())) return false;
    tasks.erase(tasks.begin() + num - 1);
    return true;
}

TReturnValues ToDoList::add(const std::string& text) {
    if (text.empty()) return TReturnValues::EmptyText;
    tasks.push_back("[ ] " + text);
    return TReturnValues::Ok;
}

TReturnValues ToDoList::mark(int num) {
    if (!validIndex(num, tasks.size())) return TReturnValues::InvalidIndex;
    else if (isMarked(num)) return TReturnValues::AlreadyDone;
    tasks[num - 1][1] = 'X';
    return TReturnValues::Ok;
}

TReturnValues ToDoList::unmark(int num) {
    if (!validIndex(num, tasks.size())) return TReturnValues::InvalidIndex;
    else if (!isMarked(num)) return TReturnValues::AlreadyDone;
    tasks[num - 1][1] = ' ';
    return TReturnValues::Ok;
}

TReturnValues ToDoList::load(const std::string& filename) {
    std::ifstream file(kListsDir + filename);

    if (!file.is_open()) return TReturnValues::FileProblem;

    tasks.clear();
    skippedLines.clear();
    std::string line;

    while (getline(file, line)) {
        if (line.length() >= 5 && (line.substr(0, 4) == "[ ] " || line.substr(0, 4) == "[X] ")) {
            tasks.push_back(line);
        }
        else skippedLines.push_back(line);
    }
    file.close();

    if (skippedLines.empty()) return TReturnValues::Ok;
    return TReturnValues::SkippedLines;
}

TReturnValues ToDoList::edit(int num, const std::string& text) {
    if (!validIndex(num, tasks.size())) return TReturnValues::InvalidIndex;
    else if (text.empty()) return TReturnValues::EmptyText;
    tasks[num - 1] = tasks[num - 1].substr(0, 4) + text;
    return TReturnValues::Ok;
}

std::size_t ToDoList::size() const {
    return tasks.size();
}

const std::vector<std::string>& ToDoList::getTasks() const {
    return tasks;
}

const std::vector<std::string>& ToDoList::getSkippedLines() const {
    return skippedLines;
}
