#include <iostream>
#include <vector>

void f(int&) {std::cout << 1 << std::endl;}
void f(const int&) {std::cout << 2 << std::endl;}
void f2(const int&) {std::cout << 2 << std::endl;}

int main(){
    { // soft
        std::cout << "soft" << std::endl;
        int n = 5;
        f(n); 
        f(const_cast<const int&>(n));
        
        // тоже кастует
        f2(n);
        f(static_cast<const int&>(n)); 
        f(reinterpret_cast<const int&>(n));
    }
    { // hard
        std::cout << "hard" << std::endl;
        int n = 5;
        const int& cn = n;
        f(const_cast<int&>(cn));
    }
}