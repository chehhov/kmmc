#include <iostream>
#include <vector>
#include "input.h"
#include "sort.h"
#include "output.h"

int main() {
    std::vector<int> numbers = inputNumbers();
    bubbleSort(numbers);
    printArray(numbers);
    return 0;
}
