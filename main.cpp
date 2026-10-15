// Developer Al'meria

#include "../include/ListStorage.h"
#include "../include/ToDoList.h"
#include "../include/Validation.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <limits>

namespace fs = std::filesystem;
using FileList = std::vector<std::string>;
using TaskFile = std::vector<std::string>;

bool readInt(int& out) {
    std::cin >> out;
    if (std::cin.fail()) {
        if(std::cin.eof()) {
            exit(0);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

bool confirm(const std::string& question) {
    std::cout << question;

    std::string answer;
    std::getline(std::cin, answer);

    return answer == "yes";
}

void displayLists(ListStorage& storage) {
    FileList fileList = storage.getFileList();
    std::cout << "\n------------------------------------\n";
    for (size_t i = 0; i < fileList.size(); ++i) {
        std::cout << (i + 1) << ". " << fileList[i] << '\n';
    }
}

void listsActions() {
    std::cout << "\n-----------------\n";
    std::cout << "1. Create file.\n";
    std::cout << "2. Open file.\n";
    std::cout << "3. Close program.\n";
    std::cout << "0. Delete file.\n";
    std::cout << "-----------------\n\n";
}

void listActions() {
    std::cout << "\n1. Create task.\n";
    std::cout << "2. Edit task.\n";
    std::cout << "3. Mark task.\n";
    std::cout << "4. Unmark task.\n";
    std::cout << "5. Exit task.\n";
    std::cout << "0. Delete task.\n";
    std::cout << "------------------------------------\n\n";
}

void createList(ListStorage& storage) {
    std::cout << "Enter your ToDoList name: ";
    std::string filename;
    getline(std::cin, filename);

    switch (storage.create(filename)) {
        case LReturnValues::InvalidInput: std::cout << "\nWrite the correct name for the file!"; break;
        case LReturnValues::AlreadyExist: std::cout << "\nFile with this name already exist!"; break;
        case LReturnValues::Ok: std::cout << "\nThe file has been successfully created!"; break;
        case LReturnValues::Error: std::cout << "\nThe file was not created!"; break;
    }
    displayLists(storage);
    listsActions();
}

void deleteList(ListStorage& storage) {
    std::cout << "Enter file number: ";
    int num;
    if (!readInt(num) || !validIndex(num, storage.getFileList().size())) {
        std::cout << "\nEnter correct file number!";
        return;
    }
    if(confirm("\nAre you sure (yes/no): ")) {
        switch (storage.remove(num)) {
            case LReturnValues::InvalidInput: std::cout << "\nEnter correct file number!"; break;
            case LReturnValues::Ok: std::cout << "\nThe file has been successfully deleted!"; break;
            case LReturnValues::Error: std::cout << "\nThe file was not deleted!"; break;
            default: ;
        }

    }
    displayLists(storage);
    listsActions();
}

void displayTask(ToDoList& list) {
    std::cout << "\n------------------------------------\n";
    TaskFile taskFile = list.getTasks();
    int count = 0;
    for (const auto& task : taskFile) {
        count++;
        std::cout << count << ". " << task << '\n';
    }
    std::cout << "------------------------------------\n";
}

void displaySkippedLines(const std::vector<std::string>& skippedLines) {
    for (std::string line : skippedLines) {
        std::cout << line << '\n';
    }
}

void createTask(ToDoList& list) {
    std::cout << "\nEnter your task: ";
    std::string task; getline(std::cin, task);
    switch (list.add(task)) {
        case TReturnValues::EmptyText: std::cout << "\nTask can not be empty!"; break;
        case TReturnValues::Ok: std::cout << "\nTask has been added!"; break;
        default: ;
    }
    displayTask(list);
    listActions();
}

void editTask(ToDoList& list) {
    std::cout << "\nEnter task number you want to edit: ";
    int num;
    if (!readInt(num) || !validIndex(num, list.size())) std::cout << "\nEnter correct task number!";
    else {
        std::cout << "\nEnter new task: ";
        std::string task; getline(std::cin, task);
        if (confirm("\nAre you sure you want to edit task (yes/no): ")) {
            switch (list.edit(num, task)) {
                case TReturnValues::InvalidIndex: std::cout << "\nEnter correct task number!"; break;
                case TReturnValues::EmptyText: std::cout << "\nTask can not be empty!"; break;
                case TReturnValues::Ok: std::cout << "\nTask has been successfully edited!"; break;
                default: ;
            }
        }
    }
    displayTask(list);
    listActions();
}

void markTask(ToDoList& list) {
    std::cout << "\nEnter task number you want to mark: ";
    int num;
    if (!readInt(num) || !validIndex(num, list.size())) std::cout << "\nEnter correct task number!";
    else {
        switch (list.mark(num)) {
            case TReturnValues::InvalidIndex: std::cout << "\nEnter correct task number!"; break;
            case TReturnValues::AlreadyDone: std::cout << "\nTask already marked!"; break;
            case TReturnValues::Ok: std::cout << "\nTask has been marked!"; break;
            default: ;
        }
    }
    displayTask(list);
    listActions();
}

void unmarkTask(ToDoList& list) {
    std::cout << "\nEnter task number you want to unmark: ";
    int num;
    if (!readInt(num) || !validIndex(num, list.size())) std::cout << "\nEnter correct task number!";
    else {
        switch (list.unmark(num)) {
            case TReturnValues::InvalidIndex: std::cout << "\nEnter correct task number!"; break;
            case TReturnValues::AlreadyDone: std::cout << "\nTask already unmarked!"; break;
            case TReturnValues::Ok: std::cout << "\nTask has been unmarked!"; break;
            default: ;
        }
    }
    displayTask(list);
    listActions();
}

void deleteTask(ToDoList& list) {
    std::cout << "\nEnter task you want to delete: ";
    int num;
    if (!readInt(num) || !validIndex(num, list.size())) std::cout << "\nEnter correct task number!";
    else if (confirm("\nAre you sure you want to delete task (yes/no): ")) {
        if (list.remove(num)) std::cout << "\nTask was successfully deleted!";
        else std::cout << "\nEnter correct task number!";
    }
}

void openList(ListStorage& storage, ToDoList& list) {
    std::cout << "\nEnter file number: ";
    FileList fileList = storage.getFileList();
    int i;
    if (!readInt(i) || !validIndex(i, fileList.size())) {
        std::cout << "\nEnter correct file number.";
        displayLists(storage);
        listsActions();
        return;
    }

    switch (list.load(fileList[i - 1])) {
        case TReturnValues::FileProblem: std::cout << "\nThe file was not opened!"; return;
        case TReturnValues::SkippedLines:
            std::cout << "\nSome lines were omitted because they do not conform to the format:\n";
            displaySkippedLines(list.getSkippedLines());
            break;
        case TReturnValues::Ok: break;
        default: ;
    }

    displayTask(list);
    listActions();

    while (true) {
        int num;
        if (!readInt(num)) {
            std::cout << "\nEnter correct number!\n\n";
            displayTask(list);
            listActions();
            continue;
        }
        switch (num) {
            case 0: deleteTask(list); break;
            case 1: createTask(list); break;
            case 2: editTask(list); break;
            case 3: markTask(list); break;
            case 4: unmarkTask(list); break;
            case 5: displayLists(storage); listsActions(); return;
            default: std::cout << "\nUnknown command\n\n"; continue;
        }
        list.save(storage.getFileList()[i - 1]);
    }
}

/* ------------------------------------ */

int main() {
    fs::create_directory("ToDoLists");

    ToDoList list;
    ListStorage storage;


    if (fs::is_empty("ToDoLists")) {
        std::cout << "Hello! You don't have any ToDoLists.\n\n";
        std::cout << "1. Create file.\n";
        std::cout << "0. Close program.\n\n";

        bool running = true;
        while (running) {
            int num;
            if (!readInt(num)){
                std::cout << "\nEnter correct number!\n\n";
                continue;
            }
            switch (num) {
                case 0: std::cout << "Good luck!\n"; return 0;
                case 1: createList(storage); running = false; break;
                default: std::cout << "You enter wrong number! Try again!\n";
            }
        }
    }
    else {
        std::cout << "Hello! Your ToDoLists:";
        displayLists(storage);
        listsActions();
    }
    bool repeat = true;
    while (repeat) {
        int num;
        if (!readInt(num)) {
            std::cout << "\nEnter correct number!\n\n";
            continue;
        }
        switch (num) {
            case 0: deleteList(storage); break;
            case 1: createList(storage); break;
            case 2: openList(storage, list); break;
            case 3: std::cout << "Good luck!\n"; repeat = false; break;
            default: std::cout << "\nUnknown command.\n\n"; displayLists(storage); listsActions(); break;
        }
    }

    return 0;
}
