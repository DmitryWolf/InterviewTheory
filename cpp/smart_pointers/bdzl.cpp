#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class TSensor
{
public:
    explicit TSensor(std::string name)
        : Name_(std::move(name))
    {
        std::cout << "Constructing sensor " << Name_ << std::endl;
    }

    TSensor(const TSensor&) = delete;
    TSensor& operator=(const TSensor&) = delete;

    ~TSensor()
    {
        std::cout << "Destructing sensor " << Name_ << std::endl;
    }

    void Increment(int count)
    {
        Count_ += count;
    }

    int GetValue() const
    {
        return Count_;
    }

    const std::string& GetName() const
    {
        return Name_;
    }

private:
    const std::string Name_;

    int Count_ = 0;
};

class TProfiler
{
public:
    void Register(std::weak_ptr<TSensor> sensor)
    {
        Sensors_.push_back(std::move(sensor));
    }

    void Send()
    {
        std::cout << "Sending sensors (count: " << Sensors_.size() << ")" << std::endl;
        std::vector<std::weak_ptr<TSensor>> sensors;
        for (const std::weak_ptr<TSensor>& weakSensor : Sensors_) {
            if (std::shared_ptr<TSensor> sensor = weakSensor.lock()) {
                std::cout << sensor->GetName() << " = " << sensor->GetValue() << std::endl;
                sensors.push_back(weakSensor);
            }
        }
        std::swap(Sensors_, sensors);
    }

private:
    std::vector<std::weak_ptr<TSensor>> Sensors_;
};

void HandleRequestFromUser(
    std::shared_ptr<TSensor> requestCount,
    std::shared_ptr<TSensor> downloadCount)
{
    requestCount->Increment(1);
    std::cout << "User request handled" << std::endl;

    if (rand() % 2 == 0) {
        downloadCount->Increment(1);
        std::cout << "Download handled" << std::endl;
    }
}

/////////////////////////////////////////////////////

template <class T>
struct TSharedState
{
    T* Object = nullptr;
    int StrongCount = 0;
    int WeakCount = 0;

    ~TSharedState()
    {
        std::cout << "Destructing shared state" << std::endl;
    }
};

template <class T>
class TWeakPtr;

template <class T>
class TSharedPtr
{
public:
    explicit TSharedPtr(T* object)
    {
        if (object) {
            State_ = new TSharedState<T>{object, /*StrongCount*/ 1, /*WeakCount*/ 0};
        } else {
            State_ = nullptr;
        }
    }

    TSharedPtr(const TSharedPtr& other)
        : State_(other.State_)
    {
        if (State_) {
            ++State_->StrongCount;
        }
    }

    ~TSharedPtr()
    {
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

    void Reset()
    {
        if (State_) {
            --State_->StrongCount;
            if (State_->StrongCount == 0) {
                delete State_->Object;
                State_->Object = nullptr;
            }
            if (State_->StrongCount + State_->WeakCount == 0) {
                delete State_;
            }
            State_ = nullptr;
        }
    }

private:
    TSharedState<T>* State_;

    friend class TWeakPtr<T>;

    explicit TSharedPtr(TSharedState<T>* state)
        : State_(state)
    {
        if (State_) {
            assert(State_->Object);
            ++State_->StrongCount;
        }
    }
};

template <class T>
class TWeakPtr
{
public:
    explicit TWeakPtr(const TSharedPtr<T>& other)
        : State_(other.State_)
    {
        if (other.State_) {
            ++State_->WeakCount;
        }
    }

    TWeakPtr(const TWeakPtr& other)
        : State_(other.State_)
    {
        if (other.State_) {
            ++State_->WeakCount;
        }
    }

    TSharedPtr<T> Lock()
    {
        if (!State_ || State_->StrongCount == 0) {
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
        if (State_) {
            --State_->WeakCount;
            if (State_->StrongCount + State_->WeakCount == 0) {
                delete State_;
            }
            State_ = nullptr;
        }
    }

private:
    TSharedState<T>* State_;
};

int main()
{
    // Part 1.
    TProfiler profiler;

    std::shared_ptr<TSensor> requestCount = std::make_shared<TSensor>("request_count");
    profiler.Register(requestCount);

    std::shared_ptr<TSensor> downloadCount = std::make_shared<TSensor>("download_count");
    profiler.Register(downloadCount);

    HandleRequestFromUser(requestCount, downloadCount);

    profiler.Send();

    HandleRequestFromUser(requestCount, downloadCount);
    HandleRequestFromUser(requestCount, downloadCount);
    HandleRequestFromUser(requestCount, downloadCount);
    HandleRequestFromUser(requestCount, downloadCount);
    HandleRequestFromUser(requestCount, downloadCount);

    profiler.Send();

    std::cout << "Resetting sensors" << std::endl;
    requestCount.reset();
    downloadCount.reset();

    profiler.Send();
    profiler.Send();
    profiler.Send();
    profiler.Send();

    // Part 2.
    TSharedPtr<TSensor> sensor(new TSensor("upload_count"));

    std::cout << "Value of " << sensor->GetName() << " is " << sensor->GetValue() << std::endl;
    std::cout << "Value of " << (*sensor).GetName() << " is " << (*sensor).GetValue() << std::endl;

    TSharedPtr<TSensor> sensor2(sensor);

    std::cout << "Value of " << sensor2->GetName() << " is " << sensor2->GetValue() << std::endl;
    std::cout << "Value of " << (*sensor2).GetName() << " is " << (*sensor2).GetValue() << std::endl;

    sensor.Reset();

    sensor2->Increment(5);

    std::cout << "Value of " << sensor2->GetName() << " is " << sensor2->GetValue() << std::endl;
    std::cout << "Value of " << (*sensor2).GetName() << " is " << (*sensor2).GetValue() << std::endl;

    TWeakPtr<TSensor> weakSensor(sensor2);

    std::cout << "Value of " << weakSensor.Lock()->GetName() << " is " << weakSensor.Lock()->GetValue() << std::endl;

    // Runtime error!
    // sensor->GetName();

    sensor2.Reset();

    // Runtime error!
    // std::cout << "Value of " << weakSensor.Lock()->GetName() << " is " << weakSensor.Lock()->GetValue() << std::endl;

    weakSensor.Reset();

    // Runtime error!
    // sensor2->GetName();

    TSharedPtr<const TSensor> sensor3(new TSensor("user_count"));
    // sensor3->Increment();
    std::cout << "Value of " << (*sensor3).GetName() << " is " << (*sensor3).GetValue() << std::endl;

    return 0;
}
