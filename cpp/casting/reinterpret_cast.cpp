#include <iostream>

void f(int x) {std::cout << x << std::endl;}
void f(double x) {std::cout << x << std::endl;}

int main(){
    {
        int x = 0;
        double d = 3.14;

        std::cout << std::hex << reinterpret_cast<int&>(d) << std::endl;
        
        std::cout << reinterpret_cast<int*&>(d) << std::endl;

        int* p = reinterpret_cast<int*&>(d);
        std::cout << d << std::endl;

        int q = reinterpret_cast<int&>(d);
        std::cout << q << std::endl;
    }
    {
        int x = 0;
        int* p = &x;
        double* pd = reinterpret_cast<double*>(p);
        double d = 3.14;
        int* pp = reinterpret_cast<int*&>(d);
        *pp = 1; // segfault   
    }

}