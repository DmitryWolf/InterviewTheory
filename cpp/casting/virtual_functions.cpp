#include <iostream>

struct Mother{
    int x = 0;
    virtual void f(){
        std::cout << x;
    }
};

struct Father{
    int y = 1;
    virtual void g(){
        std::cout << y;
    }
};

struct Son : Mother, Father{
    int z = 2;
    void f() override{
        std::cout << z;
    }
    void g() override{
        std::cout << z;
    }
};

struct S{
    long long a;
    long long b;
};

int main(){
    void (Son::* p)() = &Mother::f;

    Son son;
    Mother& m = son;
    (son.*p)();

    void (Son::*p2)() = &Son::g;
    Father& f = son;
    std::cout << '\n';
    (son.*p2)();


    std::cout << '\n';

    std::cout << sizeof(p) << ' ';
    S s = reinterpret_cast<S&>(p2);
    std::cout << s.a << ' ' << s.b << '\n';
}