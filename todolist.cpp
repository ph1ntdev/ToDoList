// Developer Al'meria

#include <ios>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>


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
TaskFile createTask(TaskFile& taskFile);
TaskFile editTask(TaskFile& taskFile);
void displayList(TaskFile& taskFile);
void openList();

/* ------------------------------------ */

void listsActions() {
    cout << endl << "-----------------" << endl;
    cout << "1. Create file." << endl;
    cout << "2. Open file" << endl;
    cout << "3. Delete file." << endl;
    cout << "0. Close program." << endl;
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
    if (i <= fileList.size()) {
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
    }
    else {
        cout << "Enter correct file number." << endl << endl;
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

TaskFile createTask(TaskFile& taskFile) {
    cin.ignore(10000, '\n');
    cout << "Enter your task: ";
    string task;
    getline(cin, task);
    if (task.empty()){
        cout << "Task can't be empty." << endl;
        return taskFile;
    }
    taskFile.push_back("[ ] " + task );

    return taskFile;
}

TaskFile editTask(TaskFile& taskFile) {

}

void displayList(TaskFile& taskFile) {
    cout << endl << "------------------------------------" << endl;
    string line;
    int count = 0;
    for (const auto& task : taskFile) {
        count++;
        cout << count << ". " << task << endl;
    }
    cout << "------------------------------------" << endl << endl;
}

void openList() {
    cout << "Enter file number: ";
    int i; cin >> i;
    FileList fileList = getFileList();
    if (i <= fileList.size()) {
        ifstream file("ToDoLists/" + fileList[i - 1]);
        if (!file.is_open()) {
            cout << "Problem with opening file " << fileList[i - 1] << '.' << endl;
            return;
        }
        TaskFile taskFile;
        string line;
        while (getline(file, line)) {
            taskFile.push_back(line);
        }
        file.close();
        displayList(taskFile);
        listActions();

        while (true) {
            int cs; cin >> cs;
            if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Enter a number, not a letter!" << endl << endl;
                    continue;
                }
            switch (cs) {
                case 1:
                    taskFile = createTask(taskFile);
                    saveTasksInFile(fileList[i - 1], taskFile);
                    displayList(taskFile);
                    listActions();
                    break;
                case 2:
                    taskFile = editTask(taskFile);
                    saveTasksInFile(fileList[i - 1], taskFile);
                    displayList(taskFile);
                    listActions();
                    break;

                case 5: displayLists(); listsActions(); return;
                default: cout << "Unknown comand" << endl << endl; int cs;
            }
        }
    }
    cout << "Enter correct file number.";


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
                case 2: openList(); break;
                case 3: deleteList(); break;

                case 0: cout << "Good luck!" << endl; again = false; break;
                default: cout << endl << "Unknown comand." << endl << endl; displayLists(); listsActions(); break;
            }
        }
    }

    return 0;
}
