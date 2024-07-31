#include <iostream>
#include <memory>
#include <vector>
// Smart pointers



// идейно так
template<typename T>
struct default_delete{
    void operator()(T* ptr) const{
        delete ptr;
    }
};
template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr{
private:
    T* ptr;
    [[no_unique_address]] Deleter del;
public:
    unique_ptr(T* ptr, Deleter del) : ptr(ptr), del(del) {}
    unique_ptr(T* ptr) : ptr(ptr) {}
    ~unique_ptr() {
        // delete ptr;
        del(ptr);
    }
    unique_ptr(const  unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
    unique_ptr(unique_ptr&& other)
    : ptr(other.ptr), del(std::move(other.del))
    {
        other.ptr = nullptr;
    }
    unique_ptr& operator=(unique_ptr&& other){
        if (this != &other){
            // delete ptr;
            del(ptr);
            ptr = other.ptr;
            del = std::move(other.del);
            other.ptr = nullptr;
        }
        return *this;
    }
    T& operator*(this auto&& self) const{
        return *ptr;
    }
    T* operator->() const{
        return ptr;
    }
    T* get() const{
        return ptr;
    }
    // есть Reset
};





void f(int x){
    std::unique_ptr<int> p(new int(5)); // RAII

    // auto pp = p;

    std::vector<std::unique_ptr<int>> v;
    for (int i = 0; i < 10; ++i){
        // могло бы не работать, если бы мы при реалокации копировали бы
        // но мы делаем move вместо copy
        v.emplace_back(new int(i));
        // в push_back нужно явно указать, что мы кладем, иначе не компилится (пока хз почему так)
        // v.push_back(std::unique_ptr<int>(new int(i)));
    }
    
    for (size_t i = 0; i < 10; ++i){
        // разыменование ptr дает ссылку на объект, то есть l-value
        // к l-value мы можем применять инкремент, тем самым меняя, то что под unique_ptr лежит 
        std::cout << ++*v[i] << " ";

        // Вернет адреса
        std::cout << v[i].get() << "\n";
    }
    auto pp = std::move(p);
    if (x == 0){
        throw std::runtime_error("Oops!");
    }
}

void f2(){
    // В деструкторе вызываем delete, а не delete[]
    // Поэтому для корреткного удаления нам нужно записать так
    std::unique_ptr<int[]> p(new int[5]); // RAII
    
    // since c++17
    p[3] = 5;
    // появились [] для unique_ptr для массивов
    // ++ для unique_ptr нет (видимо т.к. можем попасть на чужой участок памяти)
    // вообще не нужно юзать сишные массивы с плюсовыми классами, странная конструкция
}


template <typename T, typename ... Args>
unique_ptr<T> make_unique(Args&&... args){
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
int h2(){
    auto p = make_unique<int>(5);
}


int main(){
    try{
        f(0);
    }
    catch (...){

    }
    f2();
}