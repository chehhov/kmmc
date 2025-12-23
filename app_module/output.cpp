#include "output.h"
#include <iostream>
#include <vector>

void printArray(const std::vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << ' ';      
    }
    std::cout << "Вектор отсротирован";
}
