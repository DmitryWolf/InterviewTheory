#include <iostream>

// RTTI (RunTime Type Information) and dynamic cast

struct Base{
    int x = 0;
    virtual void f() {}
    virtual ~Base() = default;
};

struct Derived : Base{
    int y = 0;
    void f() override {}
};




int main(){
    Derived d;
    Base& b = d;
    // При неправильном касте будет std::bad_cast, нужно ловить исключение
    std::cout << dynamic_cast<Derived&>(b).y << "\n";
    // При неправильном касте будет nullptr
    if (Derived* pd = dynamic_cast<Derived*>(&b); pd){
        // ...
        std::cout << "OK\n";
    }

    typeid(b); // вернет std::type_info

    std::cout << typeid(b).name() << "\n"; // Выведется 7Derived
    /*
    есть оператор сравнение, равенство будет т.ит.д., когда это одинаковые типы
    есть метод name, возвращающий const char, в котором написано название типа
    */
}















