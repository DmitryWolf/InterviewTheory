#include <iostream>
#include <atomic>
#include <array>

class PetersonMutex{
public:
    PetersonMutex(){
        want_[0].store(false);
        want_[1].store(false);
    }

    void Lock(size_t index){
        want_[index].store(true);
        victim_.store(index);

        while(want_[1 - index].load() && victim_.load() == index){
            // Backoff
        }
        // Acquired
        owner_ = index;
    }

    void Unlock(){
        want_[owner_].store(false);
    }

private:
    std::array<std::atomic<bool>, 2> want_;
    std::atomic<size_t> victim_;
    size_t owner_;
};


int main(){

}