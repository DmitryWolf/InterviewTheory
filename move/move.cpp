#include <iostream>

class string{
public:
    string(string&& other) 
    : arr(other.arr), sz(other.sz), cap(other.cap) {
        other.arr = nullptr;
        other.sz = other.cap = 0;
    }
    
    string& operator=(string&& other) {
        if (this == &other){
            return *this;
        }
        delete[] arr;
        
        arr = other.arr; other.arr = nullptr;
        sz = other.sz; other.sz = 0;
        cap = other.cap; other.cap = 0;
        return *this;
    }

private:
    char* arr;
    size_t sz;
    size_t cap;
};

int main(){
}