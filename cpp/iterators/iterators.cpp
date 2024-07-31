#include <iostream>
#include <vector>

template <typename InputIterator>
void find_most_often_number(InputIterator begin, InputIterator end){
    typename std::iterator_traits<InputIterator>::value_type x = *begin;
}
template <typename T>
void f(T) = delete;

int main(){
    std::vector<bool> vb(10);
    // f(*vb.begin());
    find_most_often_number(vb.begin(), vb.end());
}