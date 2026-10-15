#include "../include/ListStorage.h"
#include "../include/Paths.h"
#include "../include/Validation.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

std::vector<std::string> ListStorage::getFileList() {
    fileList.clear();
    for (const auto& entry : fs::directory_iterator(kListsDir)){
        fileList.push_back(entry.path().filename().string());
    }
    sort(fileList.begin(), fileList.end());

    return fileList;
}

LReturnValues ListStorage::create(const std::string& filename) {
    if (filename.empty() || filename.find_first_of("/\\*?:;\"\'><|") != std::string::npos) return LReturnValues::InvalidInput;
    if (fs::exists(kListsDir + filename)) return LReturnValues::AlreadyExist;

    std::ofstream file(kListsDir + filename);
    if (file.is_open()) {
        file.close();
        return LReturnValues::Ok;
    }
    return LReturnValues::Error;
}

LReturnValues ListStorage::remove(int num) {
    if (!validIndex(num, fileList.size())) return LReturnValues::InvalidInput;
    if (fs::remove(kListsDir + fileList[num - 1])) return LReturnValues::Ok;
    return LReturnValues::Error;
}
