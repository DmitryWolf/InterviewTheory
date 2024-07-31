#include <iostream>
#include <atomic>

class TASSpinLock{
public:
    TASSpinLock(){
        locked_.store(false);
    }

    void Lock(){
        while (locked_.exchange(true)){
            // Backoff
        }
    }

    void Unlock(){
        locked_.store(false);
    }

private:
    // twist::stdlike::atomic<bool> locked_(false);
    std::atomic<bool> locked_;
};

int main(){
    
}