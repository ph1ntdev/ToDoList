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

/* ------------------------------------ */

void listsActions();
void listActions();
FileList getFileList();
void displayLists();
void createList();
void deleteList();
void createTask(string filename, int i);
int displayList(ifstream& file);
int openList();

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

void createTask(const string filename, int i) {
    ofstream file("ToDoLists/" + filename, ios_base::app);
    cout << "Enter your task: ";
    cin.ignore(10000, '\n');
    string task;
    getline(cin, task);
    if (task.empty()){
        cout << "Task can't be empty." << endl;
        file.close();
        return;
    }
    file << i << ". [ ] " << task << endl;
    file.close();
}

int displayList(ifstream& file) {
    cout << "------------------------------------" << endl;
    string line;
    int count = 0;
    while (getline(file, line)){
        count++;
        cout << line << endl;
    }
    cout << "------------------------------------" << endl;
    return count;
}

int openList() {
    cout << "Enter file number: " << endl;
    int i; cin >> i;
    FileList fileList = getFileList();
    int curTasks;
    if (i <= fileList.size()) {
        ifstream file("ToDoLists/" + fileList[i - 1]);
        if (!file.is_open()) {
            cout << "Problem with opening file " << fileList[i - 1] << '.' << endl;
            return 1;
        }
        curTasks = displayList(file);
        listActions();
        file.close();

        while (true) {
            int i; cin >> i;
            switch (i) {
                case 1: createTask(fileList[i - 1], curTasks); curTasks++; displayList(file); break;

                case 5: displayLists(); listActions(); return 0;
                default: cout << "Unknown comand" << endl << endl;
            }
        }
    }
    else {
        cout << "Enter correct file number.";
        return 1;
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
