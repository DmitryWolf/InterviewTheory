## Курс лекций по программированию на C++ МФТИ ФПМИ 2023-24 учебный год, 1 курс, продвинутый поток.
[Плейлист](https://www.youtube.com/playlist?list=PLmSYEYYGhnBviRYhIDty-CSTDS16a3whl)

# Лекция 1
- общие слова о C и C++
- статическая и динамическая типизация
- основные типы данных и операции над ними


# Лекция 2
## Неявные преобразования типов
- integer promotions (повышают диапозон значений)
```cpp
char + int = int
int + long long = long long
// !но
int + unsigned int = unsigned int
```
- floating point promotions (так же)
- bool <- int
- int <-> float (-> с отбрасываеним дробной части)


# Лекция 3
## Области видимости
- scope - фигурные скобки, также есть один глобальный scope
```cpp
namespace N {
    int x;
}

int main() {
    std::cout << N::x;
    using N::x; // притащили x в наш scope
    std::cout << x;
}

using namespace N; // притащили глобально весь namespace
```
- ODR (one definition rule)\
Каждая использумая сущность в программе должна быть 1 раз определена. Но объявлять можно сколько угодно раз. Класс можно определить несколько раз, если все определения дословно идентичны.
```cpp
// объявление - сколько угодно
void f(int x);
void f(int x);

// определение - только 1
void f(int x) {
    //...
}
```

- перегрузка функций (function overloading)
```cpp
int f();
int f(int x);
int f(double x);
// должны отличаться принимаемые типы!
```

- statements
![statements](img/0.png)

- логические операторы
    - ленивые вычисления
    - branch prediction
    ![про if else](img/1.png)

- lvalue, rvalue\
в самом простом приближении (на самом деле всё сложнее):\
lvalue - ему можно что-то присвоить\
rvalue - нельзя


# Лекция 4
## Выражения и операторы
- лево и право ассоциативные
- [приоритет операторов](https://en.cppreference.com/w/cpp/language/operator_precedence)
- [order of evaluation](https://en.cppreference.com/w/cpp/language/eval_order)
- sequenced before rules


# Лекция 5
## Ошибки
- compilation errors
- лексические ошибки
- синтаксические ошибки
- семантические ошибки
- runtime error
- undefined behaviour (большая беда языка)
- the as-if rule (компилятор имеет право как угодно менять действия нашей программы, только чтобы наблюдаемое поведение корректной программы оставалось таким же)
- warinings


# Лекция 6
## Указатели
- `*` и `&`
```cpp
int x = 10;
int* p = &x;
std::cout << *p;

void* vp = &x;
// (для приведения типов)
```

- `nullptr`
```cpp
NULL // in C
nullptr // in C++
```
![nullptr](img/2.png)


# Лекция 7
## Память
- статическая
- динамическая
- автоматическая
- стек: примерно 8мб

![memory](img/4.png)

на самом деле вот так:
![memory](img/3.png)

- static\
проинициализируется, когда в 1 раз зайдем в функцию
```cpp
void f() {
    static int x = 0; // тоже в статической памяти
    //...
}
```

- динамическая память\
операторы `new`, `delete`
```cpp
int* p = new int;
delete p;

int* pa = new int[1000];
delete[] pa;
```


# Лекция 8
## Массивы
```cpp
int a[10];
int b[3] = {1, 2, 3};
int c[] = {1, 2, 3}; // можно не указывать размер
int d[5] = {}; // заполнятся нулями
int* e[10]; //  массив из 10 указателей на int
int (*f)[10]; // указатель на массив из 10 int
```
- указатели на функцию\
function to pointer conversion
```cpp
bool cmp(int x, int y) {
    return x < y;
}

int main() {
    // можно не писать &, можно просто указать название функции
    bool (*p) (int, int) = &cmp; 
    std::cout << (void*)p;
}
```

как читать указатели? (начало лекции 9)\
идем вправо, потом влево. затем поднимаемся на уровень выше. и повторяем
```cpp
void (*pf)(int); // указатель на функцию, которая принимает int, а возвращает void
// 0) начинаем с названия, pf это:
// 1) -> пусто; <- указатель
// 2) ^
// 3) -> на фукнцию, которая принимает int; <- а возвращает void
```
если справа видим круглые скобки, то это функция\
например, в шаге 3) `(int)`

```cpp
void (*pfa[10])(int); // массив из 10 указателей на функцию, которая принимает int, а возвращает void 
// 0) начинаем с названия, pfa это:
// 1) -> массив из 10; <- указателей
// 2) ^
// 3) -> на фукнцию, которая принимает int; <- а возвращает void
```

```cpp
void (*(*pff[10])(int))(int); // массив из  10 указателей на функцию, которая принимает int, а возвращает указатель на другую функцию, которая принимает int, а возвращает void 

// 0) начинаем с названия, pff это:
// 1) -> массив из 10; <- указателей
// 2) ^
// 3) -> на фукнцию, которая принимает int; <- а возвращает указатель
// 4) ^
// 5) -> на фукнцию, которая принимает int; <- а возвращает void
```
[полезная ссылка](http://www.unixwiz.net/techtips/reading-cdecl.html)

- аргументы по умолчанию\
только последние

```cpp
void point(int x = 3, int y = 4) {}
```

- variadic functions (легаси)\
любое число аргументов
```cpp
void simple_printf(const char* fmt...) {}
```


# Лекция 9
## Ссылки (references)
- амперсанд после типа
- можно считать, что это новое имя старого объекта
- на низком уровне реализуется через указатель, но не всегда
```cpp
int x = 5;
int& y = x; // ссылка на x
int& t = y; // тоже ссылка на x, нет ссылки на ссылку
sizeof(y); // = sizeof(x)
&y; // = &x
int& r; // нельзя без инициализации
```

- `error: call of overloaded ‘f(int&)’ is ambiguous`. не можем различить, вызываться от оригинального объекта, или от ссылки
```cpp
void f(int& y) {
    ++y;
}
void f(int x) {
    ++x;
}
int main() {
    int x = 5;
    int& y = x;
    f(y);
    f(x);
}
```

- ссылку нельзя проинициализировать через rvalue
- формально говоря `++x` возвращает `int&`, также и с разыменованием указателя `*p` (в си это не так, там нет такого понятия. но в плюсах можно думать об этом так)
- lvalue to rvalue conversion
```cpp
int& g(int& x) {/*...*/}
int main() {
    int x = 10;
    int t = g(x); // g(x) - lvalue, но используем как rvalue 
}
```
- нельзя путать (ну это skill issue)\
![прикол](img/5.png)\
и нельзя путать с `int&&` (rvalue-ссылка)

- dangling reference\
возвращаем ссылку, но такого объекта уже нет
```cpp
int& f(int& x) {
    int y = ++x;
    return y;
}
int main() {
    int x = 0;
    int& y = f(x); // UB
}
```

- static (аналогично глобальные переменные)
```cpp
int& f() {
    static int y = 0;
    return y; // y существует всё время работы программы
}
int main() {
    int& y = f(); // OK
}
```

- указатель (шок контент)
```cpp
int& f() {
    int* p = new int(1);
    return *p;
}
int main() {
    int& x = f();
    delete &x; // OK
}
```

- можно:
    - ссылка на указатель
    - ссылка на массив
    ```cpp
    int a[10];
    int (&b)[10] = a;
    // читается так же, как в прошлой лекции
    ```
    - ссылка на функцию
    ```cpp
    void f(int);
    void (&g)(int) = f;
    ```
- нельзя:
    - указатель на ссылку (нет такого типа. но можно создать указатель на исходный объект)
    - массив ссылок
    - вектор ссылок (т.к. там указатель на `T`)


# Лекция 10
## Константы
- интуиция: другой тип, у которого отсутствует часть операций исходного типа (модифицирующих)
```cpp
const int c = 2;
с = 2; // CE
int const a = 10; // можно писать const справа
```
- указатели
```cpp
int x = 5;
int* p = &x;

const int* pc = p; // указатель на const int
int* const cp = p; // const указатель на int 
const int* const cpc = p; // const указатель на const int
```
- неявное преобразование константности `int* -> const int*`. в обратную сторону нельзя.\
но это всего лишь запрет на некоторые операции, а не запрет, что объект не поменяется.
```cpp
int x = 5;
const int* p = &x;
++x;
std::cout << *p; // увидим новое значение
```

- аналогично, ссылки
```cpp
int x = 5;
const int& r = x; // новое название для x, с ограниченными правами
int& const rc = x; // CE. ссылка сама по себе не может быть переставлена на что-то другое, по этому не бывает константных ссылок
```

- константные ссылки (в смысле ссылка на const объект), (используются для передачи аргументов, если не собираемся менять объект).\
!!! Lifetime expansion: обычные ссылки инициализировать через rvalue нельзя, а константные можно. Создаётся временный объект. Компилятор считает это имя оригинальным именем объекта. Когда эта ссылка выйдет из области видимости, тогда объект уничтожится.
```cpp
void f(const std::string& s) {/*...*/}

int main() {
    f("asdf"); // OK
    const int& x = 5; // тоже OK
}
```
[советы про const](https://isocpp.org/wiki/faq/const-correctness)


# Лекция 11
## [Приведения типов](../casting/theory.md)

## Стадии сборки
1) препроцессинг
    - директивы `#include`, `#define`, `#if`, `#pragma`, ...
    - `.cpp -> .cpp`, но без директив
2) комплиция
    - получаем ассемблер код
    - `.cpp -> .s`
3) ассемблирование
    - получаем объектный файл с бинарным кодом
    - `.s -> .o` (или динамические `.so`, `.dll`)
4) линковка
    - собирает объектные файлы в исполняемый файл
    - резолвим вызовы всех функций
    - `.o -> executable`


# Лекция 12
## ООП
- выравнивание полей\
1-байтные переменные кладутся по адресам, кратным 1\
2-байтные переменные кладутся по адресам, кратным 2\
4-байтные переменные кладутся по адресам, кратным 4\
8-байтные переменные кладутся по адресам, кратным 8\
![alignment](img/6.png)

The alignment of the struct is the alignment of the most-aligned field in it
```cpp
struct S {
    int x = 1;
    double d = 3.14;
};
// 16 байт (4 int, 4 padding, 8 double)
```
- гарантируется порядок полей в структуре
- аггрегатная инициализация
```cpp
struct S {
    int x = 1;
    double d = 3.14;
};
int main() {
    S s{2, 4.5};
}
```
можно с именами, но в таком же порядке!
```cpp
struct xyz {
    int a;
    int b;
    int c;
};
int main() {
    xyz klm = { .a = 99, .c = 100 };
}
```

- методы
```cpp
struct S {
    void f(int x);
    void g(int y) {
        //...
    }
};
void S::f(int x) {/*...*/} // qualified id
```
- this - указатель на объект\
`->` обращение к полю/методу по указателю

- inner class
```cpp
struct A {
    int x = 1;
    double d = 3.14;
    struct AA {
        char c;
    };
};
int main() {
    A::AA a;
}
```

```cpp
struct A {
    int x = 1;
    double d = 3.14;
    struct AA { // можно даже без названия - анонимная структура
        char c;
    } a;
};
int main() {
    A::AA a;
}
```

- local class
```cpp
int main() {
    struct S {
        int x = 1;
        int y = 2;
    };
    S s;
}
```

## Модификаторы доступа
- классы и структуры\
в плюсах почти нет разницы. в структуре по умолчанию всё публичное, в классе приватное
```cpp
class C {
    int x;
};
int main() {
    C c;
    c.x; // CE
}
```
- public, private\
к private можем обращаться только внутри методов
- protected обсудим позже
- !! приватность проверяется после перегрузки


# Лекция 13
## Друзья
- функции или классы, которые не являются членами нашего класса, но им разрешен доступ к приватной части\
объявляем через friend в любом месте внутри класса (определить можно и внутри, и снаружи)
```cpp
class C {
private:
    int x{5};
    friend void g(C, int);
    friend class CC; // все методы этого класса будут друзьями
};

void g(C c, int y) {
    std::cout << c.x + y << "\n";
}
```

## Конструкторы
```cpp
class Complex {
    double re = 0.0;
    double im = 0.0;
public:
    Complex (double real) {
        re = real;
    }
};
int main() {
    // здесь 4 раза вызывается одинаковый конструктор
    Complex c(5.0);
    Complex c2 = 6.0;
    // если есть хоть 1 конструктор, то аггрегатная инициализация перестаёт работать
    Complex c3{7.0};
    Complex c4 = {8.0};
}
```

- uniform initialization
```cpp
struct Coord { int x, y; };
struct BadGrid { BadGrid(int width, int height); };
struct GoodGrid { explicit GoodGrid(int width, int height); };
```
![uniform initialization](img/7.png)
[статья 1](https://quuxplusone.github.io/blog/2019/02/18/knightmare-of-initialization/)
[статья 2](https://quuxplusone.github.io/blog/2022/06/03/aggregate-parens-init-considered-kinda-bad/)

- member initializer list
```cpp
class Complex {
    double re = 0.0;
    double im = 0.0;
public:
    // re не проинициализируется 0 по умолчанию
    // это выполнится перед конструктором
    Complex (double re) : re(re) {}
    Complex (double re, double im) : re(re), im(im) {} // желательно писать в таком же порядке
};
```

- std::initializer_list (C++ 11)
```cpp
class String {
    char* arr = nullptr;
    size_t sz = 0;
    size_t cap = 0;
public:
    String() {} // default constructor
    
    String(size_t n, char c) : arr(new char[n+1]), sz(n), cap(n+1) {
        std::fill(arr, arr+n, c);
        arr[sz] = '\0';
    }

    String(std::initializer_list<char> list)
        : arr(new char[list.size()])
        , sz(list.size())
        , cap(sz+1)
    {
        std::copy(list.begin(), list.end(), arr);
        arr[sz] = '\0';
    }
};

int main() {
    String s; // default initialization
    String s2 = {'a', 'b', 'c'};
    String s3 = {2, 'b'}; // тоже вызовется от std::initializer_list
    // !! но если бы его не было, вызвался бы String(size_t n, char c)
}
```

- если не объявили конструкторы, то компилятор сгенерирует сам конструктор по умолчанию (он будет просто инциализировать поля по умолчанию). Если объявили хоть один конструктор, то комплиятор не будет этого делать. Но можно попросить (C++ 11)
```cpp
class String {
    char* arr = nullptr;
    size_t sz = 0;
    size_t cap = 0;
public:
    String() = default; // explicitly declared, implicitly defined
    
    String(size_t n, char c) : arr(new char[n+1]), sz(n), cap(n+1) {
        memset(arr, c, n);
        // std::fill(arr, arr+n, c);
        arr[sz] = '\0';
    }
};
```

- или можно запретить генерировать (C++ 11)
```cpp
class C {
//...
    C() = delete;
};
```

- но не всегда может сгенерироваться конструктор по умолчанию
```cpp
class C {
    // не знаем как проинициализировать по умолчанию
    int& r;
    const int c;
};
```

## Деструкторы
- вызываются в обратном порядке констуркторам (как стек). Сначала выполняется код деструктора нашего класса, а потом его полей.
```cpp
~String() {
    delete[] arr;
}
```

## Конструктор копирования
- генерируется по умолчанию (просто копирует поля). Причем даже если есть и другие конструкторы
- константная ссылка, хотя можно и без const
- так же лучше делать с member initializer list
```cpp
String(const String& other)
    : arr(new char[other.cap])
    , sz(other.sz)
    , cap(other.cap)
{
    memcpy(arr, other.arr, sz+1);
}

int main() {
    String s(2, 'a');
    String s2 = s; // конструктор копирования

    String s3 = s3; // UB
}
```
## Делегирующие конструкторы (C++ 11)
- вызываем только 1 конструктор и нельзя использовать member initializer list
```cpp
class String {
    char* arr = nullptr;
    size_t sz = 0;
    size_t cap = 0;

    String(size_t n, char c) : arr(new char[n+1]), sz(n), cap(n+1) {
        arr[sz] = '\0';
    }
public:
    String() = default;

    String(size_t n, char c) : String(n) {
        memset(arr, c, n);
    }
    String(std::initializer_list<char> list) : String(list.size()) {
        std::copy(list.begin(), list.end(), arr);
    }
    String(const String& other) : String(other.sz) {
        memcpy(arr, other.arr, sz+1);
    }
};
```

## Оператор присваивания
```cpp
String& operator=(const String& other) {
    if (this == other) { // в таком случае обязательно надо проверять!
        return *this;
    }
    delete[] arr;
    sz = other.sz;
    cap = other.cap;
    arr = new char[other.cap];
    memcpy(arr, other.arr, sz+1);
    return *this;
}
```

- тоже можно попросить сгенерировать (но тоже не всегда получится)
- идиома copy and swap
```cpp
void swap(String &other) {
    std::swap(arr, other.arr);
    std::swap(sz, other.sz);
    std::swap(cap, other.cap);
}
String& operator=(const String& other) {
    String copy = other; // конструктор копирования
    swap(copy);
    return *this;
}
```
еще проще можно сделать так
```cpp
String& operator=(String other) { // конструктор копирования при вызове
    swap(other);
    return *this;
}
```
в таких случаях можно не делать проверку на присваивание самому себе

## Правило трех
Реккомендация по написанию классов: Если в классе есть нетривиальный конструктор копирования, или нетривиальный оператор присваивания, или нетривиальный деструктор, то нужно чтобы все 3 они были нами написаны.


# Лекция 15
## Const, mutable
- отсутствуют методы, которые явно не помечены, что должны присустствовать у константных объектов
```cpp
struct S {
    void f() {
        std::cout << "Hi!";
    }
};
int main() {
    const S s;
    s.f(); // CE
}
```

```cpp
struct S {
    void f() const {
        std::cout << "Hi!";
    }
};
int main() {
    const S s;
    s.f(); // OK
}
```
вывод: все методы, которые, предполагается, что будут вызваны у константных объектов, надо помечать const
- не имеет никакого отношения к конструкторам и деструкторам

- в const методах нельзя вызывать неконстантные операции у полей
```cpp
struct S {
    int x = 0;
    void f() const {
        ++x; // CE
        std::cout << "Hi!";
    }
};
```

- перегрузка
```cpp
struct S {
    void f() const {
        std::cout << 1;
    }
    void f() {
        std::cout << 2;
    }
};
```

- компилятор неявно навешивает const на все поля. Если это указатель, то он неявно превращается в const указатель (но не указатель на const!), то есть как будто появляется const справа. Однако на ссылку нельзя навесить const справа (подразумевается, что неизменными должны быть сами байты, которые кодируют ссылку, но не объект, на который она указывает). Поэтому для ссылок нет разницы между константным методом и обычным
```cpp
int x = 0;
struct S {
    int& r = x;
    void f(int y) const {
        r = y; // OK
    }
};
```
поэтому вот такой прикол
```cpp
struct S {
    int x = 1;
    int& r = x;
    void f(int y) const {
        r = y; // поменяет x в const методе
    }
};
```

- mutable (противоядие от const)
```cpp
struct S {
    mutable int x = 1;
    void f(int y) const {
       x = y;
    }
};
```

## Static
```cpp
struct S {
    static void f() {
        std::cout << "Hi!";
    }
};
int main() {
    S::f();
}
```

- нельзя внутри класса инициализировать неконстантные статические члены, но const можно
```cpp
struct S {
    static int x = 10; // CE
    const static int y = 5; // OK
};
int S::x = 1; // OK
```

- singleton 
```cpp
struct Singleton {
private:
    Singleton() {}
    static Singleton* ptr;

    Singleton(const Singleton&) = delete;
    Signleton& operator=(const Singleton&) = delete;
public:
    static Singleton& getObject() {
        if (ptr == nullptr) {
            ptr = new Singleton();
        }
        return *ptr;
    }
    // добавить деструктор
};
Singleton* Singleton::ptr = nullptr;

int main() {
    Singleton& s = Singleton::getObject();
}
```

## Приведение типов
- explicit запрещает неявную конверсию\
по кодстайлу рекомендуется делать так для конструкторов одного аргумента
```cpp
struct Latitude {
    double value;
    explicit Latitude(double value) : value(value) {}
};
```
также с aggregate initialization
```cpp
struct BadGrid {
    BadGrid(int width, int height);
};
struct GoodGrid {
    explicit GoodGrid(int width, int height);
};
int main() {
    BadGrid g1 = {10, 20}; // OK
    GoodGrid g2 = {10, 20}; // CE
}

```

- приведение типа к чему-то
```cpp
struct Latitude {
    double value;
    explicit Latitude(double value) : value(value) {}
    operator double() const { // к double
        return value;
    }
};
```

- но можно запретить неявную. Остается только явная (static_cast)
```cpp
struct Latitude {
    double value;
    explicit Latitude(double value) : value(value) {}
    explicit operator double() const { // запретили неявную
        return value;
    }
};
```

## Литеральные суффиксы
- их можно определять)
- `"sdfsadf"s // это std::string`


# Лекция 16
## Перегрузка операторов
```cpp
struct Complex {
    double re = 0.0;
    double im = 0.0;
    Complex(double re) : re(re) {}
    Complex(double re, double im) : re(re), im(im) {}

    Complex operator+(const Complex& other) const { // чтобы могли вызываться от const объектов
        return Complex(re + other.re, im + other.im);
    }
    // если определить внтури класса, то оператор не симметричный. запрещаем левому агрументу быть не объектом класса
};

int main() {
    Complex c(1.0);
    c + 3.14; // OK
    3.14 + c; // CE
}
```

```cpp
struct Complex {
    double re = 0.0;
    double im = 0.0;
    Complex(double re) : re(re) {}
    Complex(double re, double im) : re(re), im(im) {}
};

// здесь уже без const
Complex operator+(const Complex& a, const Complex& b) {
    return Complex(a.re + b.re, a.im + b.im);
}

int main() {
    Complex c(1.0);
    c + 3.14; // OK
    3.14 + c; // OK
}
```

- плохая реализация
```cpp
struct Complex {
    double re = 0.0;
    double im = 0.0;
    Complex(double re) : re(re) {}
    Complex(double re, double im) : re(re), im(im) {}

    // очень медленно на больших объектах
    Complex& operator+=(const Complex &other) {
        // 1) новый объект *this + other
        // 2) operator=
        *this = *this + other;
        return *this;
    }
};
Complex operator+(const Complex& a, const Complex& b) {
    return Complex(a.re + b.re, a.im + b.im);
}
```
надо `+` выражать через `+=`
```cpp
struct Complex {
    double re = 0.0;
    double im = 0.0;
    Complex(double re) : re(re) {}
    Complex(double re, double im) : re(re), im(im) {}

    Complex& operator+=(const Complex &other) {
        re += other.re;
        im += other.im;
        return *this;
    }
};

Complex operator+(const Complex& a, const Complex& b) {
    Complex result = a;
    result += b;
    return result;
}

int main() {
    Complex a(1.0);
    Complex b(2.0);
    Complex c(3.0);
    // так как мы явно не запретили присваиваивание для rvalue
    a + b = c; // OK
}
```
надо фиксить так (C++ 11)
```cpp
// применим только к lvalue
Complex& operator=(const Complex &other) &  {/**/}
// применим только к rvalue
Complex& operator=(const Complex &other) &&  {/**/}
```
- опять про копирования\
если возвращаем локальную переменную по значению наружу - RVO
```cpp
Complex operator+(const Complex& a, const Complex& b) {
    Complex result = a; // 1) копирование
    result += b;        // 2) в зависимости от объекта
    return result;      // 3) RVO - нет копирования
}

// так не сработает, не локальная переменная
Complex operator+(Complex a, const Complex& b) {
    return a += b;
}
```
- потоки вывода
```cpp
std::ostream& operator<<(std::ostream& out, const std::string& str) {/**/}
std::istream& operator>>(std::istream& in, std::string& str) {/**/}
```
- операторы сравнения
```cpp
bool operator<(const Complex& a, const Complex& b) {
    return a.re < b.re || a.re == b.re && a.im < b.im;
}
```
```
a > b  == b < a
a <= b == !(a > b)
a >= b == !(a < b)

a == b == !(a < b) && !(b < a)
a != b == a < b || b < a
// но лучше равенство определить руками
```
- operator spaceship (Three-way comparison) (C++ 20)\
partial_ordering (<, >, =, не сравним)\
weak_ordering (<, >, =)\
strong_ordering (<, >, =) и еще `a == b => f(a) == f(b)`
```cpp
struct Complex {
    double re = 0.0;
    double im = 0.0;
    Complex(double re) : re(re) {}
    Complex(double re, double im) : re(re), im(im) {}

    // по умолчанию - лексикографически
    std::weak_ordering operator<=>(const Complex& other) const = default;
};
```

```cpp
struct Complex {
    double re = 0.0;
    double im = 0.0;
    Complex(double re) : re(re) {}
    Complex(double re, double im) : re(re), im(im) {}

    std::weak_ordering operator<=>(const Complex& other) const {
        return std::weak_ordering::equivalent;
    }
    // operator== не доопределяется
};
```
- инкремент, декремент
```cpp
struct UserID {
    int value = 0;

    UserID& operator++() {
        ++value;
        return *this;
    }
    UserID operator++(int) {
        UserID copy = *this;
        ++value;
        return copy;
    }
}
```
- operator()
```cpp
// функтор
struct Greater {
    bool operator() (int x, int y) {
        return x > y;
    }
}
int main() {
    std::vector<int> v(10);
    std::sort(v.begin(), v.end(), Greater());
}
```

# Лекция 17
## Указатель на члены класса
```cpp
struct S {
    int x;
    double y;

    void f(int z) {
        std::cout << x + z;
    }
};

int main() {
    int S::* p = &S::x; // указатель на поле класса

    S s{1, 3.14};
    S* ps = &s;
    std::cout << s.*p; // достанет int из объекта
    std::cout << ps->*p; // то же самое, но по указателю

    void (S::* pf)(int) = &S::f; // указатель на метод класса
    (s.*pf)(3);
    (s->*pf)(5);
}
```

## Enums and enum classes
- enum
```cpp
enum E {
    White,
    Gray,
    Black
};

int main() {
    E e = White;
    int e1 = White; // OK
    std::cout << e; // OK
}
```
- enum class (C++ 11)
```cpp
enum class E {
    White,
    Gray,
    Black
};

int main() {
    E e = E::White;
    int e1 = E::White; // CE
    std::cout << e;    // CE
    // можно делать static_cast
}
```
- можно задать значения
```cpp
enum class E {
    White = 2,
    Gray = 9,
    Black= 6
};
```
- наследования
```cpp
enum class E : int8_t {
    White = 2,
    Gray = 9,
    Black= 6
};
```

## Наследования
- public, private, protected\
protected член класса доступен всем другим членам класса, друзьям и всем наследникам 
```cpp
struct Base {
protected:
    int x;
public:
    void f() {}
};

struct Derived : Base {
    int y;
    void g() {
        std::cout << x;
    }
}

int main() {
    Derived d;
    std::cout << d.x; // CE
}
```
- можно добавить модицикатор доступа при самом наследовании\
у классов по умолчанию родитель private, у структур public
```cpp
struct Derived : public Base {
    int y;
    void g() {
        std::cout << x;
    }
}

int main() {
    Derived d;
    std::cout << d.x; // CE
}
```
чтобы обратиться к полю, надо пройти через 2 двери. Надо и чтобы Derived не запрещал доступ к Base, и чтобы Base не запрещал доступ к своим полям.

- protected
```cpp
struct Granny {
    int x;
    void f() {}
};
struct Mom : protected Granny {
    int y;
    void g() {}
};
struct Son : Mom {
    int z;
    void h() {
        std::cout << x; // OK
    }
};
int main() {
    Son s;
    s.z; // OK
    s.y(); // OK
    s.x; // CE
}
```
- видимость и доступность полей и методов\
частное предпочтительнее общего
```cpp
struct Base {
    void f() {
        std::cout << 1;
    }
};
struct Derived : Base {
    void f() {
        std::cout << 2;
    }
};
int main() {
    Derived d;
    d.f(); // 2
}
```
Это не перегрузка функций. f() затмевает родительскую функцию. Функция из Base даже не рассматривается
```cpp
struct Base {
    void f(int) {
        std::cout << 1;
    }
};
struct Derived : Base {
    void f(double) {
        std::cout << 2;
    }
};
int main() {
    Derived d;
    d.f(0); // 2

    // но можно вызвать явно
    d.Base::f(0); // 1
}
```
тут было еще много разных душных кейсов, которые я не захотел записывать


# Лекция 18
## Размещение объектов в памяти
```cpp
struct Base {
    int x;
};
struct Derived : Base {
    double y;
};
int main() {
    std::cout << sizeof(Derived); // 16
    // 4 (int) + 4 (padding) + 8 (double)
}
```
- EBO (Empty Base Optimization)
```cpp
struct Base {
    void f() {}
};
struct Derived : Base {
    double y;
    void g() {}
};
int main() {
    std::cout << sizeof(Base);    // 1
    std::cout << sizeof(Derived); // 8
}
```
## Конструкторы, деструкторы при наследовании
Сначала создаётся объект родителя (инциализация полей + конструтор), только потом объект наследника (инициализация полей + конструктор)
```cpp
struct Base {
    int x;
    Base(int x) : x(x) {}
};
struct Derived : Base {
    int y;
    Derived(int y) : y(y) {}
};
int main() {
    Derived d = 3.14; // CE
    // нет нужного конструктора для Base
}
```
- явный вызов родительского конструктора
```cpp
struct Base {
    int x;
    Base(int x) : x(x) {}
};
struct Derived : Base {
    int y;
    Derived(int y) : Base(0), y(y) {}
};
int main() {
    Derived d = 3.14; // OK
}
```
- наследование конструкторов (C++ 11)
```cpp
struct Base {
    int x;
    Base(int x) : x(x) {}
};
struct Derived : Base {
    int y = 0;
    using Base::Base; // унаследовали конструктор из Base
};
```
конструкторы копирования и перемещения не наследуется

## Приведение типов при наследовании
основная идея наследования
```cpp
struct Base {
    int x;
};
struct Derived : Base {
    int y;
}
void f(Base& b) { // смотрим на Derived, как будто он Base
    cout << b.x;
}
int main() {
    Derived d;
    f(d);
}
```

аналогично для указателей
```cpp
void f(Base* b) {
    cout << b->x;
}
int main() {
    Derived d;
    f(&d);
}
```
можно и по значению
```cpp
// slicing (срезка при копировании)
// в новый объект явно скопируется та часть Base, которая внутри Derived
// вызовется нетривиальный конструктор копирования, если он есть
void f(Base b) {
    cout << b.x;
}
int main() {
    Derived d;
    f(d);
}
```

# Лекция 19
## [Приведение типов при наследовании](../oop/inheritance/theory.md#приведение-типов-при-наследовании)
- публичное наследование
![alt text](img/8.png)
possibly UB - при попытке обращения к полям и методам Derived будет UB
- приватное наследование\
если мы не член класса и не друг, нам недоступен тот факт, что Derived это на самом деле Base
![alt text](img/9.png)

## [Множественное наследование](../oop/inheritance/theory.md#multiple-inheritance)
![alt text](img/10.png)
- как располагается в памяти
![alt text](img/11.png)
- как должен работать каст\
сын - частный случай папы. Но адрес папы не такой, как адрес сына.
Подобъект папы начинается с адреса на 4 большего, чем сам исходный объект.
![alt text](img/12.png)
по этому неявный каст должен сдвинуть адрес.
Пример неявного каста одного указателя к другому, при котором численное значение адреса меняется
![alt text](img/13.png)
- если поля или методы называются одинаково
![alt text](img/14.png)\
Если в сыне объявили метод f, то дальше поиск не пойдет, он затмевает родительские - уже разбирали.
Но если в сыне не будет метода f, то CE.
Причем если просто унаследовали 2 одинаковых метода, то ничего не будет.
Но если собираемся вызвать - уже CE.\
![alt text](img/15.png)
Тут Мещерин сказал, что будет перегрузка. Но её нет! Но можно явно вызвать нужный метод
```cpp
s.M::f(5);
s.D::f(3.14);
```
## [Diamond problem](../oop/inheritance/theory.md#diamond-problem-ромбовидное-наследование)
![alt text](img/16.png)
![alt text](img/17.png)

```cpp
Son s;
s.g; // CE - неоднозначность
s.Granny::g; // CE - неоднозначность
s.Mom::g; // OK
```
```cpp
Granny& g = s; // CE - неоднозначный каст
// нужно скастится к маме/папе, потом уже к бабушке
```
```cpp
// если в бабушке метод void f()
s.f(); // CE
```
- касты между родителями одного уровня
![alt text](img/18.png)\
так как не понятно, как сдвинуть адрес. В зависимости от того, как перечислены
наследуемые классы, надо делать либо сдвиг вправо, либо сдвиг влево

- inaccessible base class\
к полям правой бабушки никак нельзя обратиться. Так не надо никогда делать\
![alt text](img/19.png)

- указатели на методы
![alt text](img/20.png)\
но в чем тут проблема?
![alt text](img/21.png)
указатель на метод хранит не только указатель на функцию, но и сдвиг объекта (на сколько байт начало того объекта, чей это метод, сдвинуто относительно начала того объекта, от которого мы вызвались)

## [Виртуальное наследование](../oop/inheritance/theory.md#virtual-inheritance)
Хотим, чтобы если мы 2 раза унаследуемся от одного и того же прородитель с разными промежуточными родителями, то этот прородитель не дублировался

- virtual
![alt text](img/22.png)

- как это работает
![alt text](img/23.png)
ptr создается для тех классов, у которых есть виртуальный предок. ptr на самом деле указывает не сюда.
для каждого из типов в статической памяти будет создана некоторая структура данных, в которой будет для каждого виртуального предка будет статически указан сдвиг относительно начала объекта это предка - vtable
![alt text](img/24.png)

- при виртуальном наследовании static_cast вниз не работает\
не понятно, насколько сдвигаться - CE
![alt text](img/25.png)

- на самом деле таблица не одна на класс\
чтобы уметь кастоваться от папы к бабушке, надо знать не только оффсет бабушки относительно сына, но и оффсет нашего текущего объекта от реального начала объекта - top offset (так как папа лежит не первый, ему надо прыгнуть на 32 - его оффсет)
![alt text](img/26.png)
в маме top offset 0, в папе 16

- можно унаследовать класс один раз виртуально, другой раз не виртуально\
![alt text](img/27.png)
дальше опять куча примеров про какой-то бред


# Лекция 20
## [Виртуальные функции](../oop/polymorphism/theory.md#идея-виртуальных-функций)
```cpp
struct Base {
    void f() {
        std::cout << 1;
    }
};

struct Derived : Base {
    void f() {
        std::cout << 2;
    }
};

int main() {
    Derived d;
    Base& b = d;
    b.f(); // 1
}
```
но хотим, чтобы выполнялся частный случай
```cpp
struct Base {
    virtual void f() {
        std::cout << 1;
    }
};

struct Derived : Base {
    // можно так, но не обязательно
    // virtual void f() {
    void f() {
        std::cout << 2;
    }
};

int main() {
    Derived d;
    Base& b = d;
    b.f(); // 2
}
```
решение, какую функцию вызвать принимается в runtime\
но вот так будет настоящая копия
```cpp
int main() {
    Derived d;
    Base b = d;
    b.f(); // 1
}
```

- полиморфный тип - у которого хотя бы 1 виртуальная функция (или хотя бы одна виртуальная функция унаследована)

- деструктор
```cpp
struct Base {
    virtual void f() {
        std::cout << 1;
    }
};

struct Derived : Base {
    int* p = new int(0);
    void f() {
        std::cout << 2;
    }
    ~Derived(){
        delete p;
    }
};

int main() {
    Base* b = new Derived();
    delete b; // деструктор Base -> утечка памяти
}
```
вывод: деструктор нужно делать виртуальным
```cpp
struct Base {
    virtual void f() {
        std::cout << 1;
    }
    virtual ~Base() = default;
};
```
или без default, но тогда не сможем работать с объектом Base

- сигнатура должна совпадать полностью
```cpp
struct Base {
    void f() const {
        std::cout << 1;
    }
};

struct Derived : Base {
    void f() {
        std::cout << 3;
    }
};

int main() {
    Derived d;
    Base& b = d;
    b.f(); // 1
}
```
так же с возвращаемым типом

- [override](../oop/polymorphism/theory.md#override) может нас спасти (CE, если такой виртуальной функции нет у родителя) (C++ 11)\
!никак не влияет на поведение работы с виртуальными фукнциями, просто добавляет CE, если такой функции нет - подсказка для нас самих
```cpp
struct Derived : Base {
    void f() override {
        std::cout << 3;
    }
};
```

- [final](../oop/polymorphism/theory.md#final) - запрещает всем дальнейшим наследникам переопределять функцию (с такой же сигнатурой) (C++ 11)
```cpp
struct Derived : Base {
    void f() final {
        std::cout << 3;
    }
};
```
`final` автоматически означает `override`. поэтому из слов `virtual`, `override`, `final` всегда требуется только одно

- другой смысл для классов
```cpp
struct Derived final : Base {};
```
от Derived больше нельзя наследоваться

- private
```cpp
struct Granny {
    virtual void f() const {
        std::cout << 1;
    }
};

struct Mom : Granny {
private:
    void f() const override {
        std::cout << 2;
    }
};

struct Son : Mom {
    void f() const final {
        std::cout << 3;
    }
};

int main() {
    Mom m;
    Granny& g = m;
    g.f(); // 2
}
```
виртуальные функции это runtime явления, а приватность - compile time. в compile time никак нельзя определить, какая функция будет выбрала в runtime.

но!
```cpp
struct Granny {
private:
    virtual void f() const {
        std::cout << 1;
    }
};

struct Mom : Granny {
    void f() const override {
        std::cout << 2;
    }
};

struct Son : Mom {
    void f() const final {
        std::cout << 3;
    }
};

int main() {
    Mom m;
    Granny& g = m;
    g.f(); // CE
}
```
берем имя f() из бабушки, но оно приватно => CE

- [абстрактный класс](../oop/polymorphism/theory.md#abstract-classes-and-pure-virtual-function) - есть хотя бы 1 pure virtual функция\
аналог интерфейса
```cpp
struct Shape {
    virtual double area() const = 0; // pure virtual
    virtual ~Shape() = default;
};
```
нельзя создать объект абстрактного типа, но можно создавать ссылки и указатели такого типа на потомков

- но можно всё таки определить pure virtual функцию
```cpp
struct Shape {
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

double Shape::area() {
    return 0.0;
}
```

- [полиморфизм](../oop/polymorphism/theory.md#что-такое-полиморфизм)
```cpp
struct Shape {
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

struct Square : Shape {
    double a;
    Square(double a) : a(a) {}
    double area() const override {
        return a * a;
    }
};

struct Circle : Shape {
    double r;
    Circle(double r): r(r) {}
    double area() const override {
        return 3.14159265358 * r * r;
    }
};

int main() {
    std::vector<Shape*> v;
    v.push_back(new Square(1.0));
    v.push_back(new Circle(1.0));
    for (Shape* s : v) {
        std::cout << s->area() << "\n";
    }
    for (Shape* s : v) {
        delete s;
    }
}
```

- можно явно попросить вызвать родиельскую
```cpp
int main() {
    Circle c(1.0);
    c.Shape::area();
}
```

# Лекция 21
## [dynamic_cast](../casting/theory.md#5-rtti-and-dynamic_cast)
- dynamic_cast (runtime cast) - только для для полиморфных типов (исходный тип должен быть полиморфным)
```cpp
struct Base {
    virtual void f() {}
    virtual ~Base() = default;
};

struct Derived : Base {
    void f() override {}
};

int main() {
    Derived d;
    Base& b = d;
    dynamic_cast<Derived&>(b);
}
```
проверит, на самом ли деле под b лежит Derived&. иначе кинет std::bad_cast
```cpp
dynamic_cast<Derived*>(&b);
```
аналогично проверит, но вернет `nullptr`\
довольная дорогая операция

- можно делать каст от любого полиморфного типа к void*

- умеет кастовать вбок (от мамы к папе) (исходный тип должен быть полиморфным)

- можно делать dynamic_cast вверх, даже если тип не полиморфный (как и static_cast)

- можно делать dynamic_cast вниз при виртуальном наследовании от полиморфного типа

## RTTI
- в каждом объекте полиморфного типа хранится runtime type information

- оператор typeid\
возвращает std::type_info
```cpp
std::cout << typeid(b).name(); // название типа, который реально был под ссылкой
// const char*
```
работает и не для полиморфных типов\
type_info можно сравнивать

## [Расположение в памяти полиморфных объектов](../casting/theory.md#6-memory-layout-of-polymorphic-objects)
формально не является стандартом C++

- vtable
![alt text](img/28.png)

- наследования
![alt text](img/29.png)
каст от сына к бабушке должен сместить указатель (причем это умеет даже static_cast)\
обратно сделать dynamic_cast уже не получится, т.к. бабушка не полиморфна (но static_cast можно в обе стороны!)

# Лекция 22
## Множественное наследование

- обычное наследование
![alt text](img/30.png)
первый ptr общий для мамы и сына, второй для папы (они ведут в разные таблицы)
![alt text](img/31.png)
если вызываем f() из папы, но сам объект - сын, надо вычесть top offset, чтобы могли работать с полями сына

- виртуальное наследование
![alt text](img/32.png)
virtual offset - оффсет виртуального родителя от начала\
top offset - оффсет нашего объекта от реального начала

## [Проблемы с виртуальными функциями](../casting/theory.md#8-non-obvious-problems-with-virtual-functions)

- нельзя не определить виртуальную функцию (кроме pure virtual), так как компилятор должен сгенерировать vtable c указателем на функцию, а линкер не сможет ее найти

- из конструктора вызывается не виртуальная функция, а обычная

мне лень писать все примеры, потому что это полный бред. можно почитать по ссылке выше

## Некоторые статьи
[Multiple Inheritance Considered Useful](https://www.drdobbs.com/cpp/multiple-inheritance-considered-useful/184402074) - статья про множественное наследование и виртуальные функции (в 7 страницах ☠️)

[Shahar Mike's Web Spot](https://shaharmike.com/cpp/) - блог какого-то крутого чувака, где есть подробные статьи про vtables с разбором, как они работают и располагаются в памяти


# Лекция 23
## Шаблоны

- шаблоны функций
```cpp
template <typename T>
void swap(T& x, T& y) {
    T t = x;
    x = y;
    y = t;
}

template <typename T>
T max(T x, T y) {
    return x > y ? x : y;
}
// это, конечно, плохие реализации
```
можно писать
```cpp
template <class T> // old style
```
разницы нет, кроме очень редких случаев (не будем разбирать)

- шаблоны классов
```cpp
template <typename T>
class vector {
    T* arr;
    size_t sz;
    size_t cap;
};
```

- шаблоны using (C++ 11)
```cpp
template <typename T>
using mymap = std::map<T, T, std::greater<T>>;
```

- шаблоны переменных (constexpr) (C++ 14)\
разберём позже

- выведение шаблонного параметра (Template argument deduction)
```cpp
int a = 0;
int b = 1;
swap(a, b); // можно не указывать явно
```
но вот так CE
```cpp
int a = 0;
long long b = 1;
swap(a, b); // не понятно, какой T имеется в виду
```

- кодогенерация\
глядя на то, от каких T мы вызываемся, компилятор должен понять, какой код сгенерировать
```cpp
int a1 = 0, b1 = 1;
swap(a1, b1);

long long a2 = 0, b2 = 1;
swap(a2, b2);

swap(a1, a2); // CE
```
стадия генерации должна пройти целиком до того, как начнется стадия выбора версий !!! (дальше будет поправка)

но можно указать явно, какую версию использовать (без deduction)
```cpp
swap<long long>(a1, b1); // CE
```
теперь будет CE, из-за того, что нельзя проинициализировать ссылку на long long через int, но если принимать по значению - OK (сделается каст от int к long long)

- с классами аналогично
```cpp
std::vector<int> v;
std::vector<double> v; // сгенерирует 2 абсолютно разных класса

v2 = v; // CE, нет конверсии от одного к другому
// т.к. 2 абсолютно разных класса
```

## Перегрузка шаблонных функций

- частное лучше общего
```cpp
template <typename T>
void f(T x) {}

void f(int x) {} // идеальное соответствие типа

int main() {
    int x = 0;
    f(x); // OK, выберется 2 версия
}
```
(поправка) на самом деле если есть выбор между шаблонной версией и не шаблонной, то это решение принимается раньше, до того, как компилятор начинет генерировать шаблонную версию

!но, если частная версия не так хорошо подходит, как общая, то выберется общая
```cpp
template <typename T>
void f(T x) {}

void f(long long x) {}

int main() {
    int x = 0;
    f(x); // сгенерируется шаблонная для int
}
```

но, опять же, можно указать явно
```cpp
template <typename T>
void f(T x) {
    std::cout << 1;
}

void f(int x) {
    std::cout << 2;
}

int main() {
    int x = 0;
    f<long long>(x); // 1
    f<int>(x); // 1, обязываем компилятор выбрать шаблонную
}
```

- шаблонные аргументы по умолчанию
```cpp
template <typename T = int>
void f(T x) {}
```

```cpp
template <typename T, typename U = int>
U f(T x) {
    return 0;
}
```
тогда при вызове можно указывать не все типы

- ссылки\
смысл ссылок в том, чтобы они были неотличимы от исходной переменной
```cpp
template <typename T = int>
void f(T& x) {
    std::cout << 1;
}

template <typename T = int>
void f(T x) {
    std::cout << 2;
}

int main() {
    int x = 0;
    f(x); // CE
}
```
но
```cpp
f(1); // 2
```

## Cпециализации шаблонов
- классы
```cpp
template <typename T>
class vector {
    T* arr;
    size_t sz;
    size_t cap;
};

// full specialization
template <>
class vector<bool> {
    char* arr;
    size_t sz;
    size_t cap;
};
```

```cpp
template <typename T, typename U>
class S {};

// partial specialization
template <typename T>
class S<T, T> {};
```

```cpp
template <typename T>
class S {};

template <typename T>
class S<T&> {};

template <typename T>
class S<const T> {};
```

# Лекция 24
## Cпециализации шаблонов
- функции
```cpp
template <typename T, typename U>
void f(T, U) {
    std::cout << 1;
}

template <typename T>
void f(T, T) {
    std::cout << 2;
}

int main() {
    f(0, 0); // 2
}
```
вторая версия более частная

- окунаемся
```cpp
template <typename T, typename U>
void f(T, U) {
    std::cout << 1;
}
template <>
void f(int, int) {
    std::cout << 3;
}
template <typename T>
void f(T, T) {
    std::cout << 2;
}

int main() {
    f(0, 0); // 2
}
```

```cpp
template <typename T, typename U>
void f(T, U) {
    std::cout << 1;
}
template <typename T>
void f(T, T) {
    std::cout << 2;
}
template <>
void f(int, int) {
    std::cout << 3;
}

int main() {
    f(0, 0); // 3
}
```

```cpp
template <typename T, typename U>
void f(T, U) {
    std::cout << 1;
}
template <typename T>
void f(T, T) {
    std::cout << 2;
}
template <>
void f(int, int) {
    std::cout << 3;
}
void f(int, int) {
    std::cout << 4;
}

int main() {
    f(0, 0); // 4
}
```
на самом деле: сначала из шаблонов выбирается наиболее подходящий или принимается решение, что шаблон не нужен вообще. потом в подходящий шаблон подставляются аргументы и генерируются нужные версии. затем делается перегрузка между тем, что получилось

- для функций запрещена частичная специализация

## Non-type template parameters (nttp)
не только типы могут быть параметрами шаблона

- целочисленные типы, char, bool
```cpp
template <typename T, size_t N>
class array {
    T arr[N];
};

array<int, 100> a;
```

```cpp
template <size_t M, size_t N, typename Field = Rational>
class Matrix {};

template <size_t N, typename Field = Rational>
using SquareMatrix = Marix<N, N, Field>;

int main() {
    Matrix<5, 5> m;
    SquareMatrix<5> sm;
}
```
```cpp
template <size_t M, size_t K, size_t N, typename Field = Rational>
Matrix<M, N, Field> operator*(const Matrix<M, K, Field>& a, const Matrix<K, N, Field>& b);
```
можно перемножать только матрицы нужных размеров над одним полем, иначе CE

- можно подставлять только константы, известные в compile time
```cpp
int x = 5;
Matrix<x, x> m; // CE
```
```cpp
const int x = 5;
Matrix<x, x> m; // OK
Matrix<5, 5> m2; // OK
```

- constexpr (C++ 11)\
гарантирует, что значение переменной известно, в момент компиляции

## Шаблонные аргументы, которые сами являются шаблонами

```cpp
template <typename T, template <typename> class Container>
class Stack {
    Container<T> container;
};
```
с C++ 11 можно
```cpp
template <typename T, template <typename> typename Container>
class Stack {
    Container<T> container;
};
```

```cpp
template <typename T, template <typename, typename> typename Container = std::vector>
class Stack {
    Container<T, std::allocator<T>> container;
};
```

- адаптеры над контейнерами (`std::stack`, `std::queue`, `std::priotiry_queue`) принимают вторым шаблонным аргументом конкретный тип контейнера (`std::vector<T>`) из-за аллокатора

## Compile time вычисления

- Фиббоначи
```cpp
template <size_t N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};
// template instantiation depth exceeds maximum of 900 (use ‘-ftemplate-depth=’ to increase the maximum)

int main() {
    std::cout << Fibonacci<20>::value;
}
```

```cpp
template <size_t N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template <>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

template <>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

int main() {
    std::cout << Fibonacci<20>::value; // O(n) по памяти и времени
}
```

- проверка числа на простоту
```cpp
template<int N, int D>
struct IsPrimeHelper {
    static constexpr bool value = N%D == 0 ? false : IsPrimeHelper<N, D-1>::value;
};

template<int N>
struct IsPrimeHelper<N, 1> {
    static constexpr bool value = true;
};

template<int N>
struct IsPrime {
    static constexpr bool value = IsPrimeHelper<N, N-1>::value;
};

template<>
struct IsPrime<1> {
    static constexpr bool value = false;
};

int main() {
    std::cout << IsPrime<257>::value; // O(n) по памяти и времени
}
```

- шаблонная переменная
```cpp
template <int N>
const bool is_prime = IsPrime<N>::value; // метафункция

int main() {
    std::cout << is_prime<257>;
}
```

- static_assert\
только от константных выражений
```cpp
int main() {
    static_assert(is_prime<257>);
}
```
кидает CE, если false

# Лекция 25
## Dependent names
когда мы работаем с шаблонами, компилятор не всегда знает, является ли имя функцией, типом или чем-то другим, что приводит к потенциальным ошибкам при компиляции

- перед зависимым именем надо писать `typename`, чтобы компилятор парсил его как названия типа, а не как expression

```cpp
template <typename T>
void printValue() {
    T::value_type x; // CE
}
```
```cpp
template <typename T>
void printValue() {
    typename T::value_type x; // OK, typename указывает компилятору, что это тип
}
```
- иногда надо будет еще написать `template`

у мещерина тут опять бредовые примеры. сейчас мне не кажется это сильно полезным, поэтому не буду это писать

но почитать можно тут [cppreference](https://en.cppreference.com/w/cpp/language/dependent_name)

- two phase translation

## type_traits (С++ 11)

- метафункции - функции от типов
- is_same\
хотим проверить, равны ли типы
```cpp
template <typename T, typename U>
struct is_same {
    static constexpr bool value = false;
};

template <typename T>
struct is_same<T, T> {
    static constexpr bool value = true;
};

template <typename T, typename U>
void f(const T& x, const U& y) {
    // ...
    if constexpr (is_same<T, U>::value) { // C++ 17

    }
    // ...
}
```
можно через typeid, но в runtime. Но если никакого полиморфизма нет, то можно просто для разных пар T, U в compile time сгенерировать разный код

под `if constexpr` проверится compile time условие. Если оно ложно, код под ифом даже не сгенерируется для данной пары

`std::is_same`

- remove_reference
```cpp
template <typename T>
struct remove_reference {
    using type = T;
};

template <>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
void f() {
    typename remove_reference<T>::type x;
}
```
`std::remove_reference`

- remove_const
```cpp
template <typename T>
struct remove_const {
    using type = T;
};

template <>
struct remove_const<const T> {
    using type = T;
};
```
`std::remove_const`

- conditional\
тернарый мета оператор
```cpp
template <bool B, typename T, typename F>
struct conditional {
    using type = F;
};

template <typename T, typename F>
struct conditional<true, T, F> {
    using type = T;
};
```
`std::conditional`

- [все функции](https://en.cppreference.com/w/cpp/header/type_traits)

- лучше не использовать все эти структуры в чистом виде, а использовать шаблонные using (C++ 14)
```cpp
template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;
```
`_t` в конце

- а еще шаблонные переменные (C++ 17)
```cpp
template <typename T, typename U>
const bool is_same_v = is_same<T, U>::value;
```
`_v` в конце

## Variadic templates
- шаблоны с переменным количеством аргументов (C++ 11)
```cpp
template <typename... Types> // объявили пачку типов
void f(Types... tx) { // распаковали пачку типов
    g(tx...);
}
```

```cpp
void print() {}

template <typename Head, typename... Tail>
void print(const Head& head, const Tail&... tail) {
    std::cout << head << ' ';
    print(tail...);
}

int main() {
    print(1, 2.0, "abc");
}
```

- is_homogeneous\
проверяет, что все типы в пакете одинаковые
```cpp
template <typename First, typename Second, typename... Types>
struct is_homogeneous {
    static constexpr bool value = std::is_same<First, Second> && is_homogeneous<Second, Types...>::value;
};

template <typename First, typename Second>
struct is_homogeneous<First, Second> {
    static constexpr bool value = std::is_same_v<First, Second>;
};
```

- sizeof...()
```cpp
template <typename Head, typename... Tail>
void print(const Head& head, const Tail&... tail) {
    std::cout << sizeof...(tail) << " "; // размер
    print(tail...);
}
```
