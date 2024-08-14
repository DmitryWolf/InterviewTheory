#include <iostream>

// polymorphic type
struct Granny{
    virtual void f(){
        std::cout << 1 << std::endl;
    }
    virtual ~Granny() = default;
};

struct Mom: Granny{
    int* p = new int(0);
    void f(){
        std::cout << 2 << std::endl;
    }
    ~Mom(){
        delete p;
    }
};

int main(){
    Granny* b = new Mom();
    delete b;
    return 0;
}