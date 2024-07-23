#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


template <class T>
struct TSharedState{
    T* Object = nullptr;
    int StrongCount = 0;
    int WeakCount = 0;
    ~TSharedState(){
        std::cout << "Destructing shared state" << std::endl;
    }
};


template <class T>
class TWeakPtr;

template <class T>
class TSharedPtr{
public:
    explicit TSharedPtr(T* object){
        if (object){
            State_ = new TSharedState<T>{object, /*StrongCount*/ 1, /*WeakCount*/ 0};
        }
        else{
            State_ = nullptr;
        }
    }

    TSharedPtr(const TSharedPtr& other) 
    : State_(other.State_) 
    {
        if (State_){
            ++State_->StrongCount;
        }
    }

    ~TSharedPtr(){
        Reset();
    }

    T& operator*() const
    {
        assert(State_);
        return *State_->Object;
    }

    T* operator->() const
    {
        assert(State_);
        return State_->Object;
    }

    void Reset(){
        if (State_){
            --State_->StrongCount;
            if (State_->StrongCount == 0){
                delete State_->Object;
                State_->Object = nullptr;
            }
            if (State_->StrongCount + State_->WeakCount == 0){
                delete State_;
            }
            State_ = nullptr;
        }
    }

private:
    TSharedState<T>* State_;
    friend class TWeakPtr<T>;
    explicit TSharedPtr(TSharedState<T>* state) : State_(state)
    {
        if (State_){
            assert(State_->Object);
            ++State_->StrongCount;
        }
    }
};

template <class T>
class TWeakPtr{
public:
    explicit TWeakPtr(const TSharedPtr<T>& other)
    : State_(other.State_)
    {
        if (other.State_){
            ++State_->WeakCount;
        }
    }
    
    TWeakPtr(const TWeakPtr& other)
    : State_(other.State_)
    {
        if (other.State_){
            ++State_->WeakCount;
        }
    }

    TSharedPtr<T> Lock()
    {
        if (!State_ || State_->StrongCount == 0){
            return TSharedPtr<T>(static_cast<T*>(nullptr));
        }
        return TSharedPtr<T>(State_);
    }

    ~TWeakPtr()
    {
        Reset();
    }

    void Reset()
    {
        if (State_){
            --State_->WeakCount;
            if (State_->StrongCount + State_->WeakCount == 0){
                delete State_;
            }
            State_ = nullptr;
        }
    }

private:
    TSharedState<T>* State_;
};





int main(){
    TSharedPtr<int> p(new int(5));

    return 0;

}