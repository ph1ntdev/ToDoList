#pragma once

#include <vector>
#include <string>

enum class LReturnValues { Ok, InvalidInput, AlreadyExist, Error };

class ListStorage {
private:
    std::vector<std::string> fileList;
public:
    std::vector<std::string> getFileList();
    LReturnValues create(const std::string& filename);
    LReturnValues remove(int num);
};
