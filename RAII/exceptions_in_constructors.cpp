#include <iostream>

struct A{
    A(int) {
        std::cout << "A";
    }
    ~A() {
        std::cout << "~A";
    }
};

struct S{
    S(int x) {
        std::cout << "S";
    }
    ~S() noexcept(false) {
        std::cout << "~S";
        if (!std::uncaught_exceptions()){
            throw 1;
        }
    }
};


int main(){
    try{
        S s(0);
        throw 1;
    } catch(...){

    }
}