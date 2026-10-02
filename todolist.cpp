// Developer Al'meria

#include <cstddef>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>

using namespace std;
namespace fs = std::filesystem;
using FileDict = std::map<string, size_t>;

enum OpenedList {
    deleteTask,
    createTask,
    editTask,
    markTask,
    unmarkTask
};

/* ------------------------------------ */

struct toDoList {
    string fileName;
};

/* ------------------------------------ */

void listsActions() {
    cout << endl <<  "1. Create file." << endl;
    cout << "2. Delete file." << endl;
    cout << "3. Next page." << endl;
    cout << "4. Previous page." << endl;
    cout << "0. Close program." << endl << endl;
}

FileDict getFileDict() {
    int count = 0;
    FileDict fileDict;
    for (const auto& entry : fs::directory_iterator("ToDoLists")){
        count++;
        fileDict[entry.path().filename().string()] = count;
    }
    return fileDict;
}

void displayLists(FileDict fileDict) {
    for (auto& [filename, number] : fileDict) {
        cout << number << ". " << filename << endl;
    }
}

void createList(FileDict fileDict) {
    cout << "Enter your ToDoList name: ";
    bool again = true;
    while (again) {
        string filename; cin >> filename;
        if (fileDict.contains(filename)){
            cout << "This file already exist. Create another file." << endl;
        }
        else {
            ofstream file("ToDoLists/" + filename);
            cout << "Your ToDoList has been successfully created." << endl;
            file.close();
            again = false;
        }
    }
}

void closeProgram() {
    cout << "Good luck!" << endl;
}

/* ------------------------------------ */

int main() {
    // creating file system
    fs::create_directory("ToDoLists");

    FileDict fileDict;

    cout << "Hello! Your ToDoLists: " << endl << endl;

    if (fs::is_empty("ToDoLists")) {
        cout << "You don't have any ToDoLists" << endl << endl;
        cout << "1. Create file." << endl;
        cout << "0. Close program." << endl << endl;

        bool again = true;

        while (again){
            int a; cin >> a;
            switch (a) {
                case 1: createList(fileDict); again = false; break;
                case 0: closeProgram(); again = false; break;
                default: cout << "You enter wrong number. Try again." << endl;
            }
        }
    }
    else {
        fileDict = getFileDict();
        displayLists(fileDict);
        listsActions();
    }











    return 0;
}
