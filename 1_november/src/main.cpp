#include <iostream>
#include "sort.h"

int main() {
    int arr[] = {64, 34, 25, 12, 22, 11, 90, 5};
    const int size = sizeof(arr) / sizeof(arr[0]);

    std::cout << "Исходный массив: ";
    lae::printArray(arr, size);

    lae::mergeSort(arr, size);

    std::cout << "Отсортированный массив: ";
    lae::printArray(arr, size);

    return 0;
}
