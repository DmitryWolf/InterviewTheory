#include <iostream>

struct A{
    A() { std::cout << "A\n"; }
    A(const A&) { std::cout << "copy" << std::endl; }
    ~A() { std::cout << "~A" << std::endl; }
}; 

void f(int x){
    A a;
    std::cout << &a << '\n';
    if (x == 0){
        throw a;
    }
}

int main(){
    try{
        try{
            f(0);
        } catch (A& a){
            std::cout << "caught!" << &a << '\n';
            throw a;
        }
    } catch (A& a){
        std::cout << "caught again!" << &a << '\n';
    }
}