// Developer Al'meria

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;
namespace fs = std::filesystem;
using FileList = vector<string>;

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
    cout << "2. Open file" << endl;
    cout << "3. Delete file." << endl;
    cout << "0. Close program." << endl << endl;
}

FileList getFileList() {
    FileList fileList;
    for (const auto& entry : fs::directory_iterator("ToDoLists")){
        fileList.push_back(entry.path().filename().string());
    }
    sort(fileList.begin(), fileList.end());

    return fileList;
}

void displayLists() {
    FileList fileList = getFileList();
    for (size_t i = 0; i < fileList.size(); ++i) {
        cout << (i + 1) << ". " << fileList[i] << endl;
    }
}

void createList() {
    cout << "Enter your ToDoList name: ";
    cin.ignore(10000, '\n');
    FileList fileList = getFileList();
    string filename;
    getline(cin, filename);
    if (ranges::binary_search(fileList, filename)){
        cout << "This file already exist. Create another file." << endl << endl;
    }
    else {
        ofstream file("ToDoLists/" + filename);
        cout << "Your ToDoList has been successfully created." << endl << endl;
        file.close();
    }
    displayLists();
    listsActions();
}

void deleteList() {
    cout << "Enter file number: ";
    FileList fileList = getFileList();
    int i; cin >> i;
    cout << "Are you sure (yes/no): ";
    string sure; cin >> sure;
    if (sure == "yes"){
        if (fs::remove("ToDoLists/" + fileList[i - 1])){
            cout << "File has been successfully deleted." << endl << endl;
        }
        else {
            cout << "This file doesn't exist." << endl;
        }
    }
    displayLists();
    listsActions();
}

/* ------------------------------------ */

int main() {
    // creating file system
    fs::create_directory("ToDoLists");

    cout << "Hello! Your ToDoLists: " << endl << endl;

    if (fs::is_empty("ToDoLists")) {
        cout << "You don't have any ToDoLists" << endl << endl;
        cout << "1. Create file." << endl;
        cout << "0. Close program." << endl << endl;

        bool again = true;

        while (again){
            int a; cin >> a;
            switch (a) {
                case 1: createList(); again = false; break;
                case 0: cout << "Good luck!"; again = false; break;
                default: cout << "You enter wrong number. Try again." << endl;
            }
        }
    }
    else {
        displayLists();
        listsActions();

        bool again = true;
        while (again) {
            int a; cin >> a;

            switch (a) {
                case 1: createList(); break;
                // case 2: openList(); break;
                case 3: deleteList(); break;

                case 0: cout << "Good luck!" << endl; again = false; break;
                default: cout << endl << "Unknown comand." << endl << endl; displayLists(); listsActions(); break;
            }
        }
    }

    return 0;
}
