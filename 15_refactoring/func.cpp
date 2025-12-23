#include <cmath>
#include <iostream>
#include <iterator>
#include "func.h"


void output(int arr[], std::string a, std::string a_1, int length) {
    
    int s_size = length;
    float s = 0;

    std::cout << a << std::endl;

    for (int i = 0; i < s_size; i++)
    {
        arr[i] = rand() % 10;
        std::cout << arr[i];
        std::cout << ' ';
        s += arr[i];
    }
    
    std::cout << ' ' << std::endl;
    std::cout <<  a_1 << s / s_size << std::endl;
    std::cout << ' ' << std::endl;
}