
#include <cmath>
#include <iostream>
#include <iterator>

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

int main()
{
    int i;
    int a[10], b[10], c[10];
    int l = 10;
    
    output(a,  "Первая последовательность", "Среднее первой последовательности = ", l);
    output(b, "Вторая последовательность", "Среднее второй последовательности = ", l);
    output(c, "Третья последовательность", "Среднее третьей последовательности = ", l);
    
    return 0;
}
