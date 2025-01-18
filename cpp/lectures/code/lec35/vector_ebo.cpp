#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <iterator>
#include <iostream>

template <typename T, typename Alloc = std::allocator<T>>
class vector : private Alloc {
    T* arr_ = nullptr;
    size_t sz_ = 0;
    size_t cap_ = 0;
    // Alloc alloc_;

    template <bool IsConst>
    class base_iterator {
    public:
        using pointer_type = std::conditional_t<IsConst, const T*, T*>;
        using reference_type = std::conditional_t<IsConst, const T&, T&>;
        using value_type = T;

    private:
        pointer_type ptr;
        base_iterator(T* ptr) : ptr(ptr) {}

        friend class vector<T>;
    public:
        base_iterator(const base_iterator&) = default;
        base_iterator& operator=(const base_iterator&) = default;

        reference_type operator*() const { return *ptr; }
        pointer_type operator->() const { return ptr; }

        base_iterator& operator++() {
            ++ptr;
            return *this;
        }
        base_iterator& operator++(int) {
            base_iterator copy = *this;
            ++ptr;
            return copy;
        }

        operator base_iterator<true>() const {
            return {ptr};
        }

        bool operator!=(const base_iterator& other) const {
            return ptr != other.ptr;
        }
    };

public:
    using iterator = base_iterator<false>;
    using const_iterator = base_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() { return {arr_}; }
    iterator end() { return {arr_ + sz_}; }

    const_iterator begin() const { return {arr_}; }
    const_iterator end() const { return {arr_ + sz_}; }

    const_iterator cbegin() { return {arr_}; }
    const_iterator cend() { return {arr_ + sz_}; }

    void reserve(size_t newcap) {
        if (newcap < cap_) {
            return;
        }

        T* newarr = /*alloc_.*/this->allocate(newcap);
        size_t index = 0;
        try {
            for (; index < sz_; ++index) {
                this->construct(newarr + index, arr_[index]);
            }
        } catch (...) {
            for (size_t oldindex = 0; oldindex < index; ++oldindex) {
                this->destroy(newarr + oldindex);
            }
            this->deallocate(newarr, newcap);
            throw;
        }

        for (size_t index = 0; index < sz_; ++index) {
            this->destroy(arr_ + index);
        }
        this->deallocate(arr_, cap_);
        
        arr_ = newarr;
        cap_ = newcap;
    }

    void push_back(const T& value) {
        if (sz_ == cap_) {
            reserve(cap_ > 0 ? cap_ * 2 : 1);
        }
        this->construct(arr_ + sz_, value);
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
    v.push_back(4);
    v.push_back(5);

    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    vector<int>::iterator it = v.begin();
    vector<int>::const_iterator cit = it;
    // vector<int>::iterator it3 = cit;  // CE

    std::cout << "sizeof(vector<int>) = " << sizeof(vector<int>) << "\n";
}
