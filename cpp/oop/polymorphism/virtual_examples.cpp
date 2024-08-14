#include <iostream>

// polymorphic type
struct Base{
    virtual void f() const{
        std::cout << 1 << std::endl;
    }
};

struct Derived: Base{
    void f() const override {
        std::cout << 2 << std::endl;
    }
    void f(){
        std::cout << 3 << std::endl;
    }
};

int main(){
    Derived d;
    Base& b = d;
    b.f();
}