#include "input.h"
#include <iostream>
#include <vector>

std::vector<int> inputNumbers() {
    std::vector<int> numbers;
    int n, num;
    
    std::cout << "Введите количество чисел: ";
    std::cin >> n;
    
    std::cout << "Введите " << n << " чисел:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cin >> num;
        numbers.push_back(num);
    }
    
    return numbers;
}
