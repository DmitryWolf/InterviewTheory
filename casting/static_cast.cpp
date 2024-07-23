#include <iostream>
#include <vector>

void f(int) {std::cout << 1 << std::endl;}
void f(double) {std::cout << 2 << std::endl;}

int main(){
    {
        int x = 0;
        f(static_cast<double>(x));
    }
    {
        int n = 5;
        static_cast<std::vector<int>>(n);
    }
}