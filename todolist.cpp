// Developer Al'meria

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>


using namespace std;
namespace fs = std::filesystem;
using FileList = vector<string>;
using TaskFile = vector<string>;

/* ------------------------------------ */

void listsActions();
void listActions();
FileList getFileList();
void displayLists();
void createList();
void deleteList();
void createTask(TaskFile& taskFile);
void editTask(TaskFile& taskFile);
bool isMarked(TaskFile& taskFile, int& taskNum);
void markTask(TaskFile& taskFile);
void unmarkTask(TaskFile& taskFile);
void displayList(TaskFile& taskFile, string& filename);
void openList();

/* ------------------------------------ */

bool readInt(int& out) {
    cin >> out;
    if (cin.fail()) {
        if(cin.eof()) {
            exit(0);
        }
        cin.clear();
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    return true;
}

bool validIndex(int n, size_t size) {
    return n >= 1 && static_cast<std::size_t>(n) <= size;
}

void listsActions() {
    cout << endl << "-----------------" << endl;
    cout << "1. Create file." << endl;
    cout << "2. Open file" << endl;
    cout << "3. Close program." << endl;
    cout << "0. Delete file." << endl;
    cout << "-----------------" << endl << endl;

}

void listActions() {
    cout << endl << "1. Create task." << endl;
    cout << "2. Edit task." << endl;
    cout << "3. Mark task." << endl;
    cout << "4. Unmark task." << endl;
    cout << "5. Exit task." << endl;
    cout << "0. Delete task." << endl;
    cout << "------------------------------------" << endl << endl;
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
    string filename;
    getline(cin, filename);

    if (filename.empty() || filename.find_first_of("/\\*?:;\"\'><|") != string::npos){
        cout << "Invalid input.\n\n";
        displayLists();
        listsActions();
        return;
    }

    if (fs::exists("ToDoLists/" + filename)){
        cout << "This file already exist. Create another file." << endl << endl;
    }
    else {
        ofstream file("ToDoLists/" + filename);
        if(file.is_open()){
            cout << "\nYour ToDoList has been successfully created." << endl << endl;
            file.close();
        }
        else {
            cout << "\nFile has not been created.";
        }
    }
    displayLists();
    listsActions();
}

void deleteList() {
    cout << "Enter file number: ";
    FileList fileList = getFileList();
    int i;
    if (!readInt(i) || !validIndex(i, fileList.size())) {
        cout << "Enter correct file number.\n\n";
    }
    else {
        cout << "Are you sure (yes/no): ";
        string sure;
        getline(cin, sure);
        if (sure == "yes"){
            if (fs::remove("ToDoLists/" + fileList[i - 1])){
                cout << "File has been successfully deleted." << endl << endl;
            }
            else {
                cout << "This file doesn't exist." << endl;
            }
        }
    }
    displayLists();
    listsActions();
}

void saveTasksInFile(string& filename, TaskFile& taskFile) {
    ofstream file("ToDoLists/" + filename);
    if (file.is_open()) {
        for (const auto& task : taskFile) {
            file << task << endl;
        }
        file.close();
    }
    else {
        cout << "File has not been saved." << endl;
    }
}

void deleteTask(TaskFile& taskFile) {
    cout << "Enter task number: ";
    int num;
    if (!readInt(num) || !validIndex(num, taskFile.size())) {
        cout << endl << "Enter correct number." << endl;
        return;
    }
    cout << "Are you sure (yes/no): ";
    string sure;
    getline(cin, sure);
    if (sure == "yes") {
        taskFile.erase(taskFile.begin() + num - 1);
        cout << endl << "Task has been successfully deleted." << endl;
    }
}

void createTask(TaskFile& taskFile) {
    cout << "Enter your task: ";
    string task;
    getline(cin, task);
    if (task.empty()){
        cout << "Task can't be empty." << endl;
        return;
    }
    taskFile.push_back("[ ] " + task );
}

void editTask(TaskFile& taskFile) {
    cout << endl << "Enter task number: ";
    int num;
    if (!readInt(num) || !validIndex(num, taskFile.size())){
        cout << "\nEnter correct number!\n";
        return;
    }
    cout << endl << "Enter changed task: ";
    string task;
    getline(cin, task);
    if (task.empty()) {
        cout << "\nTask can not be empty!\n";
        return;
    }
    cout << endl << "Are you sure (yes/no): ";
    string sure;
    getline(cin, sure);
    if (sure == "yes") {
        taskFile[num - 1] = taskFile[num - 1].substr(0, 4) + task;
    }
}

bool isMarked(TaskFile& taskFile, int& taskNum) {
    return taskFile[taskNum - 1][1] == 'X';
}

void markTask(TaskFile& taskFile) {
    cout << "Enter task number: ";
    int num;
    if (!readInt(num) || !validIndex(num, taskFile.size())) {
        cout << "\nEnter correct number.\n";
        return;
    }
    if (isMarked(taskFile, num)) {
        cout << endl << "Task already marked." << endl;
    }
    else {
        taskFile[num - 1][1] = 'X';
        cout << endl << "Successfully marked." << endl;
    }
}

void unmarkTask(TaskFile& taskFile) {
    cout << "Enter task number: ";
    int num;
    if (!readInt(num) || !validIndex(num, taskFile.size())) {
        cout << "\nEnter correct number.\n";
        return;
    }
    if (!isMarked(taskFile, num)) {
        cout << endl << "Task already unmarked." << endl;
    }
    else {
        taskFile[num - 1][1] = ' ';
        cout << endl << "Successfully unmarked" << endl;
    }
}

void displayList(TaskFile& taskFile, string& filename) {
    cout << endl << endl << "ToDoList: " << filename << endl;
    cout << "----------------------------------------------" << endl;
    string line;
    int count = 0;
    for (const auto& task : taskFile) {
        count++;
        cout << count << ". " << task << endl;
    }
    cout << "------------------------------------" << endl;
}

void openList() {
    cout << "Enter file number: ";
    FileList fileList = getFileList();
    int i;
    if (!readInt(i) || !validIndex(i, fileList.size())) {
        cout << "Enter correct file number.\n\n";
        displayLists();
        listsActions();
        return;
    }
    ifstream file("ToDoLists/" + fileList[i - 1]);
    if (!file.is_open()) {
        cout << endl << "Problem with opening file " << fileList[i - 1] << '.' << endl;
        return;
    }
    TaskFile taskFile;
    string line;
    while (getline(file, line)) {
        taskFile.push_back(line);
    }
    file.close();
    displayList(taskFile, fileList[i - 1]);
    listActions();

    while (true) {
        int cs;
        if (!readInt(cs)) {
            cout << "\nEnter correct number!\n\n";
            continue;
        }
        switch (cs) {
            case 0:
                deleteTask(taskFile);
                break;
            case 1:
                createTask(taskFile);
                break;
            case 2:
                editTask(taskFile);
                break;
            case 3:
                markTask(taskFile);
                break;
            case 4:
                unmarkTask(taskFile);
                break;
            case 5:
                displayLists();
                listsActions();
                return;
            default: cout << "\nUnknown command\n\n"; continue;
        }
        saveTasksInFile(fileList[i - 1], taskFile);
        displayList(taskFile, fileList[i - 1]);
        listActions();
    }
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
            int a;
            if (!readInt(a)) {
                cout << "\nEnter correct number!\n\n";
                continue;
            }

            switch (a) {
                case 1: createList(); again = false; break;
                case 0: cout << "Good luck!\n"; return 0;
                default: cout << "You enter wrong number. Try again." << endl;
            }
        }
    }
    else {
        displayLists();
        listsActions();
    }
    bool again = true;
    while (again) {
        int a;
        if (!readInt(a)) {
            cout << "\nEnter correct number!\n\n";
            continue;
        }

        switch (a) {
            case 1: createList(); break;
            case 2: openList(); break;
            case 0: deleteList(); break;
            case 3: cout << "Good luck!" << endl; again = false; break;
            default: cout << endl << "Unknown command." << endl << endl; displayLists(); listsActions(); break;
        }
    }

    return 0;
}
