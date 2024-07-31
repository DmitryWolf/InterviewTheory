#include <iostream>


// идейно так, на самом деле сложнее
template <typename T>
class shared_ptr{
private:
    T* ptr = nullptr;
    size_t* count = nullptr;
    template <typename U>
    struct ControlBlock{
        size_t count;
        T object;
        template<typename... Args>
        ControlBlock(size_t count, Args&&... args); // Тоже std::forward и создаем объекты
    };
    template <typename U, typename... Args>
    friend shared_ptr<U> make_shared(Args&&... args);

    shared_ptr(ControlBlock<T>* ptr) : ptr(&ptr->object), count(&ptr->count) {}

public:
    shared_ptr() {}
    explicit shared_ptr(T* ptr) : ptr(ptr), count(new size_t(1)) {}

    size_t use_count() const noexcept; // значение count

    T* get() const noexcept;

    ~shared_ptr(){
        if (!count) return;
        --*count;
        if (!*count){
            delete ptr;
            delete count;
        }
    }
};








void f2(){
    int* p = new int(5);
    // это корректно, но это плохой код. Зачем нам промежуточная сишная переменная?
    // По кодстайлу должны юзать либо сишный поинтер, либо shared_ptr на одно и то же
    shared_ptr<int> sp(p);
    // Даже так не очень хорошо, в современных плюсах тенденция полностью отказываться от new, это как-то слишком низкоуровнево
    // искл.: если пишем аллокаторы и прочее
    shared_ptr<int> sp2(new int(5));
}



void g(const shared_ptr<int>& p, int x){
    // ...
}

int f(int x){
    // ...
    if (x == 0){
        throw std::runtime_error("abcde");
    }
    return x + 1;
}

int h(){
    // Компилятор при вычислении параметров функции не обязан вычислять их последовательно
    // Хуже того, до некоторых версий компилятор не обязан был чередовать вычисление одного параметра с вычислением другого порядка (until c++17)
    // Он может в рандомном порядке это оптимизировать и чередовать эти вычисления
    // Т.е. мы можем вторгнуться после явного вызова new и что-нибудь напротит нам (например выбросит исключение)
    // нам нужно все это оборачивать все таким образом, чтобы мы сами явно не взаимодействовали с new
    g(shared_ptr<int>(new int(5)), f(0));
    return 1;
}



// чета бага где-то
template <typename T, typename ... Args>
shared_ptr<T> make_shared(Args&&... args){
    // она экономит вызов new, он вызывается 1 раз
    // + ptr и count лежат рядом в памяти, т.е. обращение быстрее
    auto* p = new typename shared_ptr<T>::ControlBlock<T>(1, std::forward<Args>(args)...);
    // auto p = new ControlBlock<T>(1, std::forward<Args>(args)...);
    return shared_ptr<T>(p);
}




int main(){
}













