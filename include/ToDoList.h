#pragma once
#include <string>
#include <vector>
#include <cstddef>

enum class TReturnValues { Ok, AlreadyDone, InvalidIndex, EmptyText, FileProblem, SkippedLines};

class ToDoList {
private:
    bool isMarked(int num) const;
    std::vector<std::string> tasks;
    std::vector<std::string> skippedLines;
public:
    bool save(const std::string& filename) const;
    bool remove(int num);
    TReturnValues add(const std::string& text);
    TReturnValues mark(int num);
    TReturnValues unmark(int num);
    TReturnValues load(const std::string& filename);
    TReturnValues edit(int num, const std::string& text);
    std::size_t size() const;
    const std::vector<std::string>& getTasks() const;
    const std::vector<std::string>& getSkippedLines() const;
};
