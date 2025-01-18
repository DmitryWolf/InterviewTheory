#include <type_traits>

template <typename T>
std::remove_reference_t<T>&& move(T&& value) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(value);
}

template <typename T>
typename std::conditional_t<
    !std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>,
    const T&,
    T&&
> move_if_noexcept(T& value) noexcept {
    return move(value);
}

template <typename T>
T&& forward(std::remove_reference_t<T>& value) noexcept {
    return static_cast<T&&>(value);
}

template <typename T>
T&& forward(std::remove_reference_t<T>&& value) noexcept {
    static_assert(!std::is_lvalue_reference_v<T>);
    return static_cast<T&&>(value);
}

int main() {
}
