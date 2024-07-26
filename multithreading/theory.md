# Взаимное исключение, мьютексы и спинлоки
*Писать конкурентный код сложно, и от порядка там зависит все*

![alt text](images/1.png)
## Race condition
Аллокаторы памяти: у нас потоки могут аллоцировать свои структуры данных, но на уровне аллокаторов они будут сталкиваться, но аллокатор - это сервис, который написан в user-space. Он ходит в ОС, чтобы аллоцировать себе страницы, какие-то арены, но дальше аллокатор работает в user-space
![alt text](images/2.png)
К чему же это может привести? Запись *1* в память пока никому не видна, потому что узел в список не попал, он только в нашем потоке есть. А вот *head* мы читаем общий. Получается **Race condition**.
Вдруг запись будет не атомарной? Мы считаем, что чтение и запись машинных слов в ядрах - атомарная. Т.е. не будет такого, что мы из двух ядер в одну переменную пишем, и в итоге в ячейке памяти окажется половина первой записи и половина второй.
Если считать, что ячейки памяти сами атомарные, то какая-то запись случится первой, какая-то второй. В итоге head у нас укажет на зеленый блок.
В итоге имеем:

    head указывает на зеленый блок
    зеленый блок указывает на фиолетовый блок
    синий блок мы потеряли
Получили утечку памяти.
Если у нас все по-отдельности атомарно, а все вместе может приводить к каким-то спецэффектам - это **race condition**
Несинхронизированная запись в одну и ту же ячейку - это **data race**

---
## Mutex
*Самый древний mutex - **PetersonMutex***
![alt text](images/3.png)
Хотелось бы, чтобы операции выполнялись по-очереди, в каком-то порядке (смиримся с недетерминизмом).
### Что такое mutex
![alt text](images/4.png)
Какая семантика всего этого? Для одного конкретного mutex между вызывами lock и unlock может находится только один поток.
![alt text](images/5.png)
![alt text](images/6.png)
![alt text](images/7.png)
В любом разумном промышленном коде, где используются *lock_guard*, они используются для того, чтобы использовать RAII для контроля ресурсов + код становится чище, компактнее, понятнее
![alt text](images/8.png)
![alt text](images/9.png)
### Свойства mutex
![alt text](images/10.png)
Первое свойство - **Safety**
Второе свойство - **Liveness**
![alt text](images/11.png)
### Нарушения Liveness
Первое - **Deadlock**
![alt text](images/12.png)
Из дедлока никак не выйти
Второе - **Livelock**
![alt text](images/13.png)
В отличие от deadlock, из этой взаимной блокировки livelock можно выйти (например, при удачном планировании потоков, переключения контекстов)
**Starvation**
![alt text](images/14.png)
![alt text](images/15.png)
### Модель исполнения
![alt text](images/16.png)
Грубо говоря, есть одно ядро
![alt text](images/17.png)
![alt text](images/18.png)
### PetersonMutex
Самый древний и примитивный mutex (для двух потоков)

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

![alt text](images/19.png)
![alt text](images/20.png)

### Замечания
![alt text](images/21.png)
![alt text](images/22.png)
Мы пишем в свои переменные и читаем чужие, это в точности алгоритм взаимного исключения. Manual intel говорит, что мы можем в такой программе получить в r1 и в r2 регистрах по нулю. (потому что процессор хочет работать быстро, другого объяснения не будет, там уже чето сложное)

---
## Атомарные RMW (Read-Modify-Write) операции и спинлоки
У нас PetersonMutex неэфективен. Во-первых, он крутится в цикле и ждет процессор, это уже не круто, во-вторых, он для двух потоков. Мы хотим придумать алгоритм для произвольного числа потоков
![alt text](images/23.png)
Сам по себе int или любой 64-битный тип будет атомарным в процессоре: если мы пишем в него из двух потоков, то записи случатся сначала одна, потом другая.
[Why is integer assignment on a naturally aligned variable atomic on x86?](https://stackoverflow.com/questions/36624881/why-is-integer-assignment-on-a-naturally-aligned-variable-atomic-on-x86/36685056#36685056)
С атомиком мы получаем дополнительные вещи: атомарные операции. Например, exchange делает read, затем write. Эти два шага R+W происходят атомарно, не может процессор между двумя этими шагами прервать исполнение этой операции
![alt text](images/24.png)

---
## Spinlock

    #include <iostream>
    #include <atomic>

    # Test-and-Set (TAS) SpinLock
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
Ура, работает для произвольного числа потоков.
[В xv6 +- такая же реализация спилока](https://github.com/ctdk/xv6/blob/master/spinlock.c#L25)
Но этот спинлок все еще не совсем хорош, как минимум по причине того, что в цикле ничего не написано, а еще по причине когерентности кэшей, но его можно доработать.
![alt text](images/25.png)
Этот протокол взаимного исключения не гарантирует прогресса для каждого потока. Чтобы гарантировать прогресс для каждого потока, нужно использовать другой алгоритм, который использует другую операцию

---
## Fetch-and-Add
![alt text](images/26.png)
Сам по себе инкремент не атомарный. Если у нас есть какой-нибудь shared_ptr, там есть счетчик ссылок
![alt text](images/27.png)
Если мы просто напишем ref_count++, то он скомпилируется в такой код
![alt text](images/28.png)
Он, конечно, не атомарный. Без fetch_add многопоточный shared_ptr не написать
## Ticket Lock
![alt text](images/29.png)

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
            // Причем здесь можно делать не атомарно
            owner_ticket_.fetch_add(1);
        }
        
    private:
        std::atomic<Ticket> next_free_ticket_{0};
        std::atomic<Ticket> owner_ticket_{0};
    };
Вообще, у нас есть очередь в планировщике, и мы здесь сами сделали очередь, и они могут друг с другом плохо дружить. Но это сложное уже что-то
![alt text](images/30.png)
Но лучше использовать методы, т.к. код легче читается и становится сразу понятно, что операции атомарные

---
## Contention
Contention (контеншн) - это ситуация, когда за один ресурс борятся разные потоки
![alt text](images/31.png)
![alt text](images/32.png)
Во-первых, механизм на уровне процессора
![alt text](images/33.png)
![alt text](images/34.png)
[Пример SpinLockPause catboost](https://github.com/catboost/catboost/blob/master/util/system/spinlock.h)
Во-вторых, механизм на уровне ОС
![alt text](images/35.png)
[Yield](https://en.cppreference.com/w/cpp/thread/yield) - вызывает системный вызов sched_yield
[sched_yield](https://man7.org/linux/man-pages/man2/sched_yield.2.html) - указине планировщику ОС, что наш поток можно снять с ядра и поставить в конец очереди планировщика
![alt text](images/36.png)
![alt text](images/37.png)
SpinLock - это механизм, который не блокируется, который крутится
SleepLock и Mutex - это механизм, который блокируется, ждет захват
## Futex
[futex](https://man7.org/linux/man-pages/man2/futex.2.html) - чудовищно сложен, куча аргументов и куча режимов работы
![alt text](images/38.png)
![alt text](images/39.png)
[Код futex](https://github.com/spotify/linux/blob/master/kernel/futex.c)

    struct futex_q {
        struct plist_node list;

        struct task_struct *task;
        spinlock_t *lock_ptr;
        union futex_key key;
        struct futex_pi_state *pi_state;
        struct rt_mutex_waiter *rt_waiter;
        union futex_key *requeue_pi_key;
        u32 bitset;
    };
[Можно почитать про futex](https://eli.thegreenplace.net/2018/basics-of-futexes)
Для каждого фьютекса своя очередь. Очередь адресуется 32-битным адресом. Адрес - это такой ключ, по которому ядро в своей хеш-таблице ищет нужную очередь.
![alt text](images/40.png)
В C++, начиная с 20 стандарта, в API появился метод у atomica - [wait](https://en.cppreference.com/w/cpp/atomic/atomic/wait). Семантика такая: мы передаем туда значение old, и если значение атомика равно old, то мы засыпаем, иначе выходим сразу
Также есть операции [notify_one](https://en.cppreference.com/w/cpp/atomic/atomic/notify_one) и [notify_all](https://en.cppreference.com/w/cpp/atomic/atomic/notify_all). Они позволяют разбудить поток, который ждет в вызове wait. При этом, операции wait и notify друг относительно друга атомарные
Важно, что atomic - это просто ячейка памяти. А вызов wait под Linux транслируется во futex. И вот ключем для этого фьютекса, вот этим самым адресом, по которому адресуется очередь, будет адрес самой ячейки памяти самого атомика

---
## Дополнение
![alt text](images/41.png)
Спинлоки нужно писать немного аккуратнее, даже если мы хотим крутиться, все равно нужно крутиться бережнее к процессору, а для того, чтобы это делать, нужно знать, как процессор устроен, знать про когерентность кэшей ядер
![alt text](images/42.png)

---
# Пул потоков, условные переменные
## Что это такое?
Хотим примитив, такой базовый объект для работы с параллельностью. Он называется **ThreadPool**
Мы хотим создавать ThreadPool, который состоит из фиксированного числа потоков, и уже в этих потоках запускать некоторые задачи.

    #include <iostream>
    #include <atomic>
    #include <functional>
    #include <thread>
    #include <chrono>
    using namespace std::chrono_literals;
    

    class StaticThreadPool{
        using Task = std::function<void()>;
    public:
        StaticThreadPool(size_t workers);
        ~StaticThreadPool();
    
        void Submit(Task task);

        void Join();

    private:
        // TODO
    };

    int main(){
        // Make thread pool with 4 worker threads
        StaticThreadPool pool{/*threads==*/4};

        // Submit task to thread pool
        pool.Submit([](){
            // Unacceptable!
            std::this_thread::sleep_for(3s);
            std::cout << "Do not block worker threads!" << std::endl;
        });

        pool.Submit([](){
            std::cout << "Hello from pool!" << std::endl;
        });

        // Await until all submitted task have completed
        // and join worker threads
        pool.Join();

        return 0;
    }

## Зачем может понадобиться ThreadPool?
Придумаем какое-нибудь алгоритмическое приминение. Представим алгоритмы сортировки QuickSort и MergeSort. Оба алгоритма делят задачу пополам.
Рассмотрим QuickSort: делаем какой-то партишн корневой, а затем у нас две задачи. Мы можем корневую задачу запустить в тредпуле, а потом из тредпула прямо туда же, прямо в него же бросить две новые подзадачи.
Рассмотрим MergeSort: он сначала запускает две подзадачи, а потом он должен дождаться, когда они завершатся. Он устроен принципиально по другому. И мы могли бы положить в тредпул две подзадачи, но как же мы будем дожидаться, пока они завершатся?
Блокироваться и засыпать в задачах ThreadPool - очень плохо (!!!)
## Как реализовать
![alt text](images/43.png)
[Coroutines](https://en.cppreference.com/w/cpp/language/coroutines)
[cppcoro](https://github.com/lewissbaker/cppcoro#static_thread_pool)

---
## [condition_variable](https://en.cppreference.com/w/cpp/thread/condition_variable)
В чем смысл этого объекта? У него две (вообще три, но две по сути разные) основные операции: **wait**, **notify_one** и **notify_all**

[wait](https://en.cppreference.com/w/cpp/thread/condition_variable/wait) - мы вызываем метод wait в cv, только имея залоченный мьютекс. Зачем? Затем, что мы хотим его отпустить, и вместе с этим атомарно заблокироваться на ожидание какого-то события. Тот, кто ждет изменения состояния, вызывает wait и отпускает блокировку внутри автоматически
[notify_one](https://en.cppreference.com/w/cpp/thread/condition_variable/notify_one) и [notify_all](https://en.cppreference.com/w/cpp/thread/condition_variable/notify_all) - тот, кто потом блокировку отпущенную подбирает и кладет задачу, он вызывает notify и будит того, кто ждал
![alt text](images/44.png)
conditional_variable - это как бы обобщение фьютекса. Во фьютексе мы ждем на ячейке памяти, а в кондваре мы ждем на произвольном состоянии.
## Spurious wakeup
Если мы хотим ждать на каком-то предикате с помощью кондвара, то нам нужно писать while а не if
Нас могут выкинуть из wake просто так, даже если никто не позвал notify
Но в данном случае это не единственная причина, по которой мы должны писать while а не if. Чтобы после notify нам проснуться, нам нужно пройти через эту границу, по рисунку: войти в комнату и взять лок. Но мы не единственные претендуем на лок. У нас есть другие потоки, которые берут лок. Вдобавок, у нас есть потоки, которые только заходят в комнату, а не выходят из кладовки
## Почему conditional_variable связан с mutex?
mutex в cv нужны, потому что если мы читаем состояние, то невозможно его будет изменить, пока мы не заснули, а если мы изменяем состояние, то ...
## Реализация ThreadPool

    #include <iostream>
    #include <atomic>
    #include <functional>
    #include <thread>
    #include <chrono>
    #include <vector>
    #include <deque>
    #include <mutex>
    #include <assert.h>
    #include <condition_variable>
    using namespace std::chrono_literals;
    

    // Unbounded Blocking Multi-Producer/Multi-Consumer (MPMC) Queue

    // 1) shared state
    // 2) mutex
    // 3) predicate(state)

    template <typename T>
    class UnboundedBlockingMPMCQueue{
    public:
        // Thread role: producer
        void Put(T value){
            std::lock_guard<std::mutex> guard(mutex_);
            buffer_.push_back(std::move(value));
            not_empty_.notify_one();
        }
        
        // Thread role: consumer
        T Take(){
            std::unique_lock<std::mutex> lock(mutex_);
            while (buffer_.empty()){ // 
                // 1) Release mutex, 2) Wait 3) Reacquire mutex
                // Spurious wakeup
                not_empty_.wait(lock);
            }
            return TakeLocked();
        }
    private:
        T TakeLocked(){
            assert(!buffer_.empty());
            T front = std::move(buffer_.front());
            buffer_.pop_front();
            return front;
        }

        std::deque<T> buffer_; // Guarded by mutex_
        std::mutex mutex_;
        std::condition_variable not_empty_;
    };

    class StaticThreadPool{
        using Task = std::function<void()>;
    public:
        StaticThreadPool(size_t workers){
            StartWorkerThreads(workers);
        }
        ~StaticThreadPool() {
            assert(workers_.empty());
        }
    
        void Submit(Task task){
            tasks_.Put(std::move(task));
        }

        void Join(){
            for (auto& worker : workers_){
                tasks_.Put({}); // Poison pill
            }
            for (auto& worker : workers_){
                worker.join();
            }
            workers_.clear();
        }

    private:
        void StartWorkerThreads(size_t count){
            for (size_t i = 0; i < count; ++i){
                workers_.emplace_back([this](){
                    WorkerRoutine();
                });
            }
        }
        
        void WorkerRoutine(){
            while (true){
                auto task = tasks_.Take();
                if (!task){
                    break;
                }
                task();
            }
        }

        std::vector<std::thread> workers_;
        UnboundedBlockingMPMCQueue<Task> tasks_;
    };

    int main(){
        StaticThreadPool pool(4);
        int shared_counter = 0;
        for (size_t i = 0; i < 100500; ++i){
            pool.Submit([&]{
                ++shared_counter;
            });
        }
        pool.Join();
        std::cout << shared_counter << std::endl;
    }

# Futures & Promises, Executors
![alt text](images/45.png)
![alt text](images/46.png)
Мы сделали тредпул: сделали пул потоков, пришли к нему, сделали сабмит и в него улетела задача, эта задача ушла стала в какую-то очередь, и дальше какой-то из воркеров ее подхватил и асинхронно исполнил. Мы в этот момент связь с этой задачей потеряли: мы не знаем, когда именно она завершится, и, более того, мы не можем извлечь из нее никакой результат. Все, что у нас есть, это метод Join, который позволяет дождаться сразу всех задач. На уровне отдельных задач мы уже работать не можем.
Начнем с того, что доработаем механизм тредпула для того, чтобы он смогт возвращать нам результат. Для этого нам потребуется новый примитив синхронизации std::future.
[Future](https://en.cppreference.com/w/cpp/thread/future) - механизм, который в грубом первом приближении дает нам возможность из асинхронной операции вернуть значение, т.е. передать значение из одного потока в другой. std::future не самостоятельный примитив, он работает в паре с std::promise. Это конец канала, из которого consumer извлекает значение
[Promise](https://en.cppreference.com/w/cpp/thread/promise) - это конец канала, в который producer пишет значение
