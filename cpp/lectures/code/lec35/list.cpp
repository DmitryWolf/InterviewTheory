#include <memory>

template <typename T, typename Alloc = std::allocator<T>>
class list {
    struct BaseNode {
        BaseNode* prev;
        BaseNode* next;
    };

    struct Node {
        T value;
    };

    BaseNode fakeNode;
    size_t count;
    typename Alloc::template rebind<Node>::other alloc;

    list(const Alloc& alloc) : fakeNode{}, count(), alloc(alloc) {}
};

int main() {
    
}
