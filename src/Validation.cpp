#include "../include/Validation.h"
#include <cstddef>

bool validIndex(int num, std::size_t size) {
    return num > 0 && static_cast<std::size_t>(num) <= size;
}
