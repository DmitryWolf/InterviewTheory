#include <iostream>

struct Mom {
    virtual void f() {
        std::cout << 1 << std::endl;
    }
};

struct Dad {
    void f(){
        std::cout << 2 << std::endl;
    }
};

struct Son: Mom, Dad {
    void f() override {
        std::cout << 3 << std::endl;
    }
};

int main(){
    Son s;
    Mom& m = s;
    Dad& d = s;
    m.f();
    d.f();
}