#include <cstddef>
#include <utility>

template <typename T>
struct allocator {
    T* allocate(size_t count) {
        return operator new(count * sizeof(T));
    }

    void deallocate(T* ptr, size_t) {
        operator delete(ptr);
    }

    template <typename U, typename... Args>
    void construct(U* ptr, Args&&... args) {
        new (ptr) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* ptr) {
        ptr->~U();
    }

    template <typename U>
    allocator(allocator<U>) {}

    template <typename U>
    struct rebind {
        using other = allocator<U>;
    };
};

int main() {
}
