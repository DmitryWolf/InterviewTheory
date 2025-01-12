#include <cstddef>
#include <cstdint>
#include <new>

template <typename T>
class vector {
    T* arr_ = nullptr;
    size_t sz_ = 0;
    size_t cap_ = 0;

public:
    void reserve(size_t newcap) {
        if (newcap < cap_) {
            return;
        }

        T* newarr = reinterpret_cast<T*>(new char[newcap * sizeof(T)]);
        size_t index = 0;
        try {
            for (; index < sz_; ++index) {
                new(newarr + index) T(arr_[index]);
            }
        } catch (...) {
            for (size_t oldindex = 0; oldindex < index; ++oldindex) {
                (newarr + oldindex)->~T();
            }
            delete[] reinterpret_cast<char*>(newarr);
            throw;
        }

        for (size_t index = 0; index < sz_; ++index) {
            (arr_ + index)->~T();
        }
        delete[] reinterpret_cast<char*>(arr_);
        
        arr_ = newarr;
        cap_ = newcap;
    }

    void push_back(const T& value) {
        if (sz_ == cap_) {
            reserve(cap_ > 0 ? cap_ * 2 : 1);
        }
        new(arr_ + sz_) T(value);
        ++sz_;
    }
};

template <>
class vector<bool> {
    char* arr_ = nullptr;
    size_t sz_ = 0;
    size_t cap_ = 0;

    struct BitReference {
        char* cell;
        uint8_t index;

        BitReference(char* cell, uint8_t index)
            : cell(cell), index(index) {}

        BitReference operator=(bool b) {
            if (b) {
                *cell |= (1 << index);
            } else {
                *cell &= ~(1 << index);
            }
            return *this;
        }

        operator bool() const {
            return *cell & (1 << index);
        }
    };
public:
    BitReference operator[](size_t index) {
        return BitReference(arr_ + index / 8, index % 8);
    }
};

int main() {
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
}
