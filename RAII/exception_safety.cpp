#include <iostream>

template <typename T>
void g() {}

template <typename T>
void f() noexcept(noexcept(g<T>())){
    
}

int main(){
    std::cout << noexcept(g<int>());
}