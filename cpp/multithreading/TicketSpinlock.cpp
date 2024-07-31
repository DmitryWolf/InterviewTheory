#include <iostream>
#include <atomic>

class TicketLock{
    using Ticket = size_t;

public:
    void Lock(){
        Ticket my_ticket = next_free_ticket_.fetch_add(1);
        while (owner_ticket_.load() != my_ticket){
            // Backoff
        }
    }

    void Unlock(){
        owner_ticket_.fetch_add(1);
    }
    
private:
    std::atomic<Ticket> next_free_ticket_{0};
    std::atomic<Ticket> owner_ticket_{0};
};