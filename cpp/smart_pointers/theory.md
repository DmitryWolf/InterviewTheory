# Smart pointers.
Очень неправильно делать несколько указателей на один и тот же объект.
Если создали сишный указатель, а затем два умных поинтера на этот указатель, то оба поинтера сделают delete.
Умные указатели думают, что несут ответственность за удаление того, от чего они созданы.

## 1. unique_ptr.
Уникальный доступ. Нельзя копировать (просто написано в конструкторе копирования = delete). Есть move конструктор.
Хранится указатель, можно создать указатель, положить указатель, в деструкторе нужно удалить указатель, копирование удалено, перемещение работает стандартно


## 2. shared_ptr.
Он позволяет себя копировать, соответственно нужно хранить счетчик, сколько shared_ptr указывает на то же, что и он
* Конструктор копирования: копируем данные и увеличиваем счетчик
* Конструктор перемещения: забираем данные и у старого делаем nullptr, счетчик не меняем
* Деструктор: смотрим на счетчик, если nullptr то выходим, если не nullptr, то если у нас счетчик=1, то делаем delete, иначе просто уменьшаем счетчик

## 3. make_shared, make_unique.
**make_shared** (since c++11) - функция, позволяющая создать shared_ptr на объект сразу из объекта, не делая new вручную

    auto p1 = std::shared_ptr<int>(new int(5));
    auto p2 = std::make_shared<int>(5);
Мотивация создания make_shared: мы вызывали в конструкторе дважды new и память выделялась в разных местах.
Мы можем написать функцию, которая сразу выделяет и объект, и счетчик, кладет их рядом, создает объект на месте, которое она выделила и рядом счетчик кладет

**make_unique** (since c++14) - функция, позволяющая создать unique_ptr на объект сразу из объекта, не делая new вручную
    
    // может быть ситуация, когда мы сделаем new int(5), затем g() и там 
    // выскочит исключение, то есть unique_ptr не сделается  
    f(unique_ptr<int>(new int(5)), g()); 

    // здесь g() не может нигде вклиниться, но в c++17 запретили так вклиниваться
    f(make_unique<int>(5), g()); 
    
## 4. weak_ptr.
Создан для решения проблемы циклических зависимостей.
Например в дереве у нас родитель указывает на детей, а дети на родителей.
Это поинтер, который смотреть на объект может, и достать объект может, но он не подсчитывается в strongPtrCount.
Решение для дерева: ссылки сверху вниз shared_ptr, а снизу вверх weak_ptr.
Нельзя разыменовать, но у weak_ptr можно попросить shared_ptr 

Недостатком умного указателя std::weak_ptr является то, что его нельзя использовать напрямую (нет оператора ->). Чтобы использовать std::weak_ptr, вы сначала должны конвертировать его в std::shared_ptr (с помощью метода lock()), а затем уже использовать std::shared_ptr.


    #include <iostream>
    #include <memory>
    #include <string>
    
    class Human
    {
        std::string m_name;
        // Используем std::weak_ptr
        // Если бы использовали std::shared_ptr, то была бы циклическая зависимость
        std::weak_ptr<Human> m_partner;
    
    public:
    
        Human(const std::string &name) : m_name(name)
        {
            std::cout << m_name << " created\n";
        }
        ~Human()
        {
            std::cout << m_name << " destroyed\n";
        }
    
        friend bool partnerUp(std::shared_ptr<Human> &h1, std::shared_ptr<Human> &h2)
        {
            if (!h1 || !h2)
                return false;
    
            h1->m_partner = h2;
            h2->m_partner = h1;
    
            std::cout << h1->m_name << " is now partnered with " << h2->m_name << "\n";
    
            return true;
        }
    
        const std::shared_ptr<Human> getPartner() const { return m_partner.lock(); } // используем метод lock() для конвертации std::weak_ptr в std::shared_ptr
        const std::string& getName() const { return m_name; }
    };
    
    int main()
    {
        auto anton = std::make_shared<Human>("Anton");
        auto ivan = std::make_shared<Human>("Ivan");
    
        partnerUp(anton, ivan);
    
        auto partner = ivan->getPartner(); // передаем partner-у содержимое умного указателя, которым владеет ivan
        std::cout << ivan->getName() << "'s partner is: " << partner->getName() << '\n';
    
        return 0;
    }

## Реализация shared_ptr и weak_ptr

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