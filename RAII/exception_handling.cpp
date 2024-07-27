#include <iostream>

struct A{
    A() { std::cout << "A\n"; }
    A(const A&) { std::cout << "copy\n"; }
    ~A() { std::cout << "~A\n"; }
}; 

void f(int x){
    A a;
    std::cout << &a << '\n';
    if (x == 0){
        throw a;
    }
}

int main(){
    f(0);
}