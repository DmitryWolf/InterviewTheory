#include <iostream>
#include <memory>


int main(){
    auto p = std::make_shared<int>(5);
    auto p2 = p;
    auto p3 = p; // p, p2, p3 -> 5
    p2 = std::make_shared<int>(8); // p, p3 -> 5; p2 -> 8;
    auto p4 = std::move(p); // p -> nullptr; p2 -> 8; p3, p4 -> 5
    
    
    std::weak_ptr<int> wp = p3;
    p3.reset(); // p3 -> nullptr; p4 -> 5; wp - валиден
    auto p5 = wp.lock(); // вернет новый shared_ptr
    bool is = wp.expired(); // проверяет жив ли объект под weak_ptr
    
    // оставляем wp одного
    p4.reset();
    p5.reset();
    is = wp.expired();
    return 0;
}