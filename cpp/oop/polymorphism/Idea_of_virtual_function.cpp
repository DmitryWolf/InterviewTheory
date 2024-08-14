#include <iostream>

// polymorphic type
struct Base{
    virtual void f(){
        std::cout << 1 << std::endl;
    }
    virtual ~Base() = default;
};

struct Derived: Base{
    int* p = new int(0);
    void f(){
        std::cout << 2 << std::endl;
    }
    ~Derived(){
        delete p;
    }
};

int main(){
    Base* b = new Derived();
    delete b;
    return 0;
}