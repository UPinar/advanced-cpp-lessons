#include <iostream>

/*
                      =======================
                      | Generic Programming |
                      =======================
*/

/*
                  <--- check templates.png --->
*/

/*
                  -------------------------------
                  | template argument deduction |
                  -------------------------------
*/

/*
  // ------------------------------------------------

  template <typename T>
  void foo(T){}

  // - dizi ismi veya string literali ile çağrı yapıldığında,
  //  array decay olur.

  // - fonksiyon ismi ile çağrı yapıldığında
  //  function to function pointer decay olur.

  // - const nesne ile çağrı yapıldığında const'luk düşer.
  // - referans nesne ile çağrı yapıldığında referans düşer.

  // ------------------------------------------------

  int func(int, int){ return 1; }

  int main()
  {
    // ------------------------------------------------

    int x = 10;
    const int& r{ x };

    foo(r);       // T = int (const ve referans düşer)

    // ------------------------------------------------

    foo(func);    // T = int(*)(int, int) 
    // (function to pointer conversion) 

    // ------------------------------------------------
  }
*/

/*
  // ------------------------------------------------

  template <typename T>
  void bar(T&){}
  // - const nesne ile çağrı yapıldığında const'luk düşmez.

  // - dizi ismi veya string literali ile çağrı yapıldığında,
  //  array decay olmaz.

  // - function ismi ile çağrı yapıldığında
  //   function to function pointer decay olmaz.
  //   tür çıkarımı fonksiyon türü olarak yapılır.

  template <typename T>
  void bar_2(const T&, const T&){}

  // ------------------------------------------------

  int func(int, int){ return 1; }

  int main()
  {
    // ------------------------------------------------

    const int y = 20;
    bar(y);       // T = const int (const düşmez)

    // ------------------------------------------------

    int a_arr[4]{};
    int b_arr[4]{};
    int c_arr[5]{};

    bar_2(a_arr, b_arr);  // T = int[4]

    bar_2(a_arr, c_arr);  // syntax error
    // error: no matching function for call to 
    // 'bar_2(int [4], int [5])'

    // ------------------------------------------------

    bar(func);    // T = int(int, int)
    // compiler written specialization for bar(func);
    // template <>
    // void bar<int(int)>(int(&)(int)){}  

    // ------------------------------------------------
  }
*/

/*
  template <typename T, std::size_t N>
  void foo(const T(&)[N]);

  int main()
  {
    int arr[45]{};
    foo(arr);   // T = int, N = 45
    // hem T için hem de N için tür çıkarımı yapılır.
  }
*/

/*
  template <typename T, typename U>
  void foo(T(*)(U));

  int func(double);

  int main()
  {
    foo(func);  // T = int, U = double
    // hem T için hem de U için tür çıkarımı yapılır.
  }
*/

/*
  // bazı durumlarda, tür çıkarımının yapılması 
  // fonksiyonun çağrılmasına bağlı değildir.

  template <typename T>
  void foo(T, T);

  int main()
  {
    void (*fp)(int, int) = &foo;
    // tür çıkarımı yapılır ve T = int olur.
  }
*/

/*
  template <typename T>
  class TypeTeller;     // incomplete type

  template <typename T>
  void func(T&&)
  {
    TypeTeller<T> t;
  }

  int main()
  {
    // ------------------------------------------------

    func(4);  // T = int
    // In instantiation of 'void func(T&&) [with T = int]':
    // error: 'TypeTeller<int> t' has incomplete type

    // ------------------------------------------------
    
    int x = 10;
    func(x);  // T = int&
    // In instantiation of 'void func(T&&) [with T = int&]':
    // error: 'TypeTeller<int&> t' has incomplete type

    // ------------------------------------------------
  }
*/

/*
  #include <array> 

  template <typename T, typename U>
  void foo(std::array<T, sizeof(U)>, std::array<U, sizeof(T)>);

  int main()
  {
    std::array<int, sizeof(double)> a1;
    std::array<double, sizeof(int)> a2;
    std::array<double, 5> a3;

    foo(a1, a2);  // T = int, U = double

    foo(a1, a3);  // syntax error
    // error: could not convert 'a3' from 
    // 'array<[...],5>' to 'array<[...],4>'
  }
*/

/*
  #include <vector>

  template <typename T, 
            typename A, 
            template <typename, typename> typename Con>
  void func(const Con<T, A>& con);

  int main()
  {
    std::vector<int> ivec;
    func(ivec);
    // T    = int, 
    // A    = class std::allocator<int>
    // Con  = class std::vector
  }
*/

/*
  template <typename T>
  void func_1(T*);

  template <typename E, int N>
  void func_2(E(&)[N]);

  template <typename T1, typename T2, typename T3>
  void func_3(T1(T2::*)(T3*));

  class AClass {
  public:
    void m_foo(double*);
  };

  void g_func(int*** ppp)
  {
    func_1(ppp);        
    // T = int**

    func_1(*ppp);
    // T = int*

    func_1(**ppp);
    // T = int

    bool b_arr[42];
    func_2(b_arr);          
    // E = bool, N = 42

    func_3(&AClass::m_foo); 
    // T1 = void, T2 = AClass, T3 = double
  }
*/

/*
  template <typename T, typename U>
  void func(T(*)(U));

  int foo(double);

  int main()
  {
    func(foo);  // T = int, U = double

    func([](int x){ return x * 5; }); // syntax error
    // error: no matching function for call to 
    // 'func(main()::<lambda(int)>)'

    // no implicit conversion from closure type 
    // to function pointer type

    func(+[](int x){ return x * 5; });  // positive lambda idiom
    // T = int, U = int
  }
*/

/*
  #include <vector>

  template <typename T>
  void func(T&&, const std::vector<T>& vec);

  int main()
  {
    std::vector<int> ivec(10);

    // ------------------------------------------------

    func(ivec[0], ivec);  // syntax error

    // "ivec[0]" is an LValue expression, its data type is `int`
    // for first argument  T will be deduce to int&
    // for second argument T will be deduce to int
    // so syntax error

    // ------------------------------------------------

    func((int)ivec[0], ivec);  // VALID
    // "(int)ivec[0]" is a PRValue expression, its data type is `int`
    // for first argument  T will be deduce to int
    // for second argument T will be deduce to int
    // VALID

    // ------------------------------------------------
  }
*/

/*
  template <typename T>
  T func(T*);

  int main()
  {
    void* vp{};

    func(vp);  // T = void
  }
*/

/*
  template <typename T>
  void func(T = 0);

  class Myclass {};

  int main()
  {
    int x = 20;

    func(20);     // T = int
    func(2.0);    // T = double
    func(&x);     // T = int*

    func();       // syntax error
    // error: no matching function for call to 'func()'
    // varsayılan argümandan hareketle tür çıkarımı yapılamaz.

    func(Myclass{});  // T = Myclass
    // varsayılan argümen kullanılmıyor.
  }
*/

/*
                    ----------------------------
                    | template parameter types |
                    ----------------------------
*/

/*
  - template type parameter
  - non-type template parameter
  - template template parameter
*/

/*
                    ---------------------------
                    | template type parameter |
                    ---------------------------
*/

/*
  // ------------------------------------------------

  template <typename T>  // template type parameter
  void foo(T);

  void foo(auto);

  // Those 2 functions are equivalent.

  // ------------------------------------------------

  template <typename T>
  void bar(T&);

  void bar(auto&);

  // Those 2 functions are equivalent.

  // ------------------------------------------------

  template <typename T>
  void baz(T&&);

  void baz(auto&&);

  // Those 2 functions are equivalent.

  // ------------------------------------------------
*/

/*
  #include <concepts> // std::integral

  template <std::integral T>
  void func(T);

  void func(std::integral auto);

  // Those 2 functions are equivalent.
*/

/*
                -------------------------------
                | non-type template parameter |
                -------------------------------
*/

/*
  template <int N>    // can be an integral type
  class Myclass {};

  template <int* p>   // can be a object pointer type
  class Myclass_2 {};

  template <int& r>   // can be a reference type
  class Myclass_3 {};

  // can be a function pointer type
  // can be a function reference type
  // can be a member function pointer type
  // can be a member function reference type
*/

/*
  class Myclass {
  public:
    double foo(double);
    double bar(double);
  };

  template <int x>    // template <auto x>  ->  auto is also NTTP
  class AClass {};

  template <int* p>
  class BClass {};

  template <int& r>
  class CClass {};

  template <int(*fp)(int)>
  class DClass {};

  template <double(Myclass::*mfp)(double)>
  class EClass {};

  int g_x{};  // static storage duration variable
  int foo(int);

  int main()
  {
    int x{};  // automatic storage duration variable
    static int y = 12;  // static storage duration variable

    // ------------------------------------------------

    AClass<10> a; // x = 10

    // ------------------------------------------------

    BClass<&g_x> b1;  // p = &g_x

    BClass<&x> b2;  // syntax error
    // error: the address of 'x' is not a valid template argument 
    // because it does not have static storage duration

    BClass<&y> b2;  // p = &y

    // ------------------------------------------------

    CClass<g_x> c1;  // r = g_x

    CClass<x> c2;    // syntax error
    // error: the address of 'x' is not a valid template argument 
    // because it does not have static storage duration

    CClass<y> c3;    // r = y

    // ------------------------------------------------

    DClass<foo>  d1;  // fp = foo
    DClass<&foo> d2;  // fp = foo
    // Those 2 lines are equivalent.

    // ------------------------------------------------

    EClass<&Myclass::foo> e1; // mfp = &Myclass::foo
    EClass<&Myclass::bar> e2; // mfp = &Myclass::bar

    // ------------------------------------------------
  }
*/

/*
  template <const char* str>
  class Myclass {};

  char g_str[] = "hello";             // static storage duration
  const char g_cstr[] = "world";      // static storage duration

  int main()
  {
    char str[] = "galaxy";            // automatic storage duration
    static char s_str[] = "universe"; // static storage duration

    // ------------------------------------------------

    Myclass<"Istanbul"> m1;  // syntax error
    // error: '"Istanbul"' is not a valid template argument 
    // for type 'const char*' because string literals 
    // can never be used in this context

    // string literals have internal linkage

    // ------------------------------------------------

    Myclass<str> m2;  // syntax error
    // error: the address of 'str' is not a valid template argument 
    // because it does not have static storage duration

    // ------------------------------------------------

    Myclass<s_str> m3;    // VALID
    Myclass<g_str> m4;    // VALID
    Myclass<g_cstr> m5;   // VALID

    // ------------------------------------------------
  }
*/

/*
  // both template parameters are NTTP
  template <int, bool>
  class Myclass {};

  int main()
  {
    // ------------------------------------------------

    Myclass<10, true> m1;

    // ------------------------------------------------

    Myclass<sizeof(int), sizeof(int) == 4> m2;
    // "sizeof(int)" is a constant expression
    // "sizeof(int) == 4" is a constant expression

    // ------------------------------------------------

    Myclass<10, sizeof(int) > 2> m3;  // syntax error (parsing error)
    // error: narrowing conversion of '4' 
    // from 'long long unsigned int' to 'bool'
    
    // ------------------------------------------------

    Myclass<10, (sizeof(int) > 2)> m4;

    // ------------------------------------------------
  }
*/

/*
  template <typename T, int N, int M>
  constexpr bool Less(const T(&arr_1)[N], const T(&arr_2)[M])
  {
    for (int i = 0; i < N && i < M; ++i)
    {
      if (arr_1[i] < arr_2[i])
        return true;

      if (arr_2[i] < arr_1[i])
        return false;
    }

    return N < M;
  }

  int main()
  {
    std::cout << std::boolalpha;

    int arr1[] = { 3, 7, 9 };
    constexpr int arr2[] = { 3, 7, 9, 2, 6 }; 
    constexpr int arr3[] = { 3, 7, 9, 2, 6 }; 

    std::cout << Less(arr1, arr2) << '\n';
    // output -> true

    constexpr bool b1 = Less(arr1, arr2); // syntax error
    // error: the value of 'arr1' is not usable 
    // in a constant expression

    constexpr bool b2 = Less(arr2, arr3);
    // b2 -> false
  }
*/

/*
  template <auto Val, typename T = decltype(Val)>
  T foo();

  int main()
  {
    auto x = foo<10>();     
    // foo<12, int> specialization
    // T = int, x's type is int

    auto y = foo<5.5>();
    // foo<5.5, double> specialization
    // T = double, y's type is double

    auto z = foo<1.2, int>();  
    // foo<1.2, int> specialization
    // T = int, z's type is int
  }
*/

/*
  struct AStruct {
    int foo(int);
    int bar(int);
    int m_x;
    int m_y;
  };

  template <int AStruct::* mp, int (AStruct::* fp)(int)>
  class Myclass {};

  int main()
  {
    Myclass<&AStruct::m_x, &AStruct::foo> m1;
    Myclass<&AStruct::m_y, &AStruct::bar> m2;
    Myclass<&AStruct::m_x, &AStruct::bar> m3;
    Myclass<&AStruct::m_y, &AStruct::foo> m4;
  }
*/

/*
  #include <vector>
  #include <algorithm>  // std::transform

  template <typename T, int val>
  T add_value(T x)
  {
    return x + val;
  }

  int main()
  {
    using namespace std;
    std::vector<int> source_vec(100);
    std::vector<int> dest_vec(100);

    transform(begin(source_vec), end(source_vec), 
              begin(dest_vec), add_value<int, 10>);

    transform(begin(source_vec), end(source_vec), 
              begin(dest_vec), [](int x){ return x + 10; });
  }
*/

/*
  // NTTP can also be parameter pack

  #include <type_traits>  // std::is_same

  template <int... Vals>
  class Myclass {};

  int main()
  {
    Myclass<> m0;
    Myclass<1> m1;
    Myclass<2> m2;
    Myclass<1, 2, 3, 4> m3;

    static_assert(std::is_same_v<Myclass<1>, Myclass<2>>);
    // output -> error: static assertion failed

    static_assert(not std::is_same_v<Myclass<1>, Myclass<2>>);
    // VALID
  }
*/

/*
  enum class Pos { OFF, ON, HOLD, STANDBY, ERROR };

  template <Pos...>
  class Myclass {};

  int main()
  {
    using enum Pos;

    Myclass<OFF> m1;
    Myclass<OFF, ON, HOLD> m2;
  }
*/

/*
  template <decltype(auto) x>
  class Myclass {
  public:
    Myclass()
    {
      ++x;
    }
  };

  int g_ival = 0;

  int main()
  {
    std::cout << "g_ival = " << g_ival << '\n';
    // output -> g_ival = 0

    Myclass<(g_ival)> m1;   // reference type
    std::cout << "g_ival = " << g_ival << '\n';
    // output -> g_ival = 1
  }
*/

/*
  template <int N, int& r>
  void bar()
  {
    // ------------------------------------------------

    ++N;  // syntax error
    // error: increment of read-only location 'N'
    // error: lvalue required as increment operand

    ++r;  // VALID

    // ------------------------------------------------

    auto p1 = &N;  // syntax error
    // error: lvalue required as unary '&' operand

    auto p2 = &r;  // VALID

    // ------------------------------------------------

    int& r1 = N;  // syntax error
    // error: cannot bind non-const lvalue reference of type 'int&' 
    // to an rvalue of type 'int'

    int& r2 = r;  // VALID

    // ------------------------------------------------
  }

  int g_ival = 15;

  int main()
  {
    bar<10, g_ival>();
  }
*/

/*
  template <auto x, auto y>
  struct Sum {
    static constexpr auto value = x + y;
  };

  int main()
  {
    Sum<10, 24L>::value;
    // "Sum<10, 24L>::value" is a constant expression
    // its data type is "long"
  }
*/

/*
  // NTTP can be a floating point number  (C++20)

  template <double dval>
  class Myclass {};

  constexpr auto foo(double d)
  {
    return d * d;
  }

  int main()
  {
    constexpr auto dval = foo(234.123);

    Myclass<dval> m1;
  }
*/

/*
  #include <type_traits>  // std::is_same_v

  template <double>
  class Myclass {};

  int main()
  {
    static_assert(std::is_same_v<Myclass<0.3>, Myclass<0.7 - 0.4>>);
    // error : static assertion failed

    static_assert(std::is_same_v<Myclass<0.2>, Myclass<0.1 + 0.1>>);
    // VALID (related with floating point format)
    
    static_assert(std::is_same_v<Myclass<+0.>, Myclass<-0.>>);
    // error : static assertion failed
  }
*/

/*
  template <auto x>
  class Myclass {};

  int main()
  {
    Myclass<1.2f> m1;
    Myclass<1.2> m2;
    Myclass<1.2L> m3;
  }
*/

/*
  template <double ...Args>
  class Myclass {};

  int main()
  {
    Myclass<1.2, 3.4, 2.3, 9.8> m1;
  }
*/

/*
  template <auto ...Args>
  class Myclass {};

  int main()
  {
    Myclass<11, 'B', 3.4, 8LL, 5.4f> m1;
  }
*/

/*
            <--- check structural_type.png --->

  - structural types can be used as a NTTP
*/

/*
  class AClass{};
  AClass g_ax;

  template <AClass a>
  class Myclass {};

  int main()
  {
    Myclass<g_ax> a1;  // VALID
  }
*/

/*
  class AClass{
  public:
    AClass(int) {};
  };

  template <AClass> // syntax error
  class Myclass{};
  // error: 'AClass' is not a valid type for a 
  // template non-type parameter because it is not structural

  // note: 'AClass' is not an aggregate, 
  // does not have a trivial default constructor, 
  // and has no 'constexpr' constructor 
  // that is not a copy or move constructor
*/

/*
  class AClass {
  public:
    constexpr AClass(int x) : m_x{ x } {}
    int m_x;
  };

  template <AClass a>
  class Myclass {};

  constexpr AClass g_ax{ 4 };

  int main()
  {
    Myclass<g_ax> m1;         // VALID
    Myclass<AClass{ 5 }> m2;  // VALID
  }
*/

/*
  #include <algorithm>  // std::copy

  template <int N>
  struct MyLiteral {
    char m_str[N];

    constexpr MyLiteral(const char(&arr)[N])
    {
      std::copy(arr, arr + N, m_str);
    }
  };

  template <MyLiteral str>
  struct AStruct {
    AStruct()
    {
      std::cout << str.m_str << '\n';
    }
  };

  int main()
  {
    // ------------------------------------------------

    AStruct<MyLiteral<12>("hello world")> a1;
    // output -> hello world

    // ------------------------------------------------

    AStruct<"hello galaxy"> a2;   // CTAD
    // output -> hello galaxy
    // "a2"'s type is AStruct<MyLiteral<13>("hello galaxy")>

    // ------------------------------------------------
  }
*/

/*
  struct NullOptT {} g_NullOpt;

  struct OptionalInt {
    constexpr OptionalInt(NullOptT) {}
    constexpr OptionalInt(unsigned int val) : 
      m_has_value{ true }, m_value{ val } {}

    const bool m_has_value = false;
    const uint32_t m_value = 0;
  };

  // OptionalInt is a structural type

  template <OptionalInt maybe>
  void Print()
  {
    if constexpr (maybe.m_has_value)
      std::cout << "value = " << maybe.m_value << '\n';
    else
      std::cout << "No value\n";
  }

  int main()
  {
    constexpr OptionalInt Opt1(444);
    Print<Opt1>();        // output -> value = 444

    Print<123>();         // output -> value = 123
    Print<g_NullOpt>();   // output -> No value
  }
*/

/*
  // closure type is a structural type

  template <auto fn>
  struct MyStruct {
    auto foo(int val)
    {
      return fn(val);
    }
  };

  int main()
  {
    MyStruct<[](int x){ return x * x; }> m1;
    std::cout << m1.foo(111) << '\n'; // output -> 12321
  }

  // we are holding callable as a structure member 
  // we are passing a callable as a template argument
  // which can be used in compile time.
*/

/*
  // std::pair and std::array classes can be used as a NTTP

  #include <array>

  template <auto x>
  struct Mystruct {};

  int main()
  {
    // ------------------------------------------------

    Mystruct<std::pair{ 10, 5.43 }> m1; // VALID

    constexpr std::pair p1{ 11, 6.34 };
    Mystruct<p1> m2; // VALID

    // ------------------------------------------------
    Mystruct<std::array{ 1, 2, 3, 4, 5 }> m4; // VALID

    constexpr std::array arr{ 1, 2, 3, 4, 5 };
    Mystruct<arr> m3; // VALID

    // ------------------------------------------------
  }
*/

/*
  #include <array>

  constexpr int foo()
  {
    return 42;
  }

  struct Lit {
    int m_x = foo();
    int m_y;
    constexpr Lit(int val) : m_y{ val } {}
  };

  struct Data {
    int m_i;
    std::array<double, 5> m_arr;
    Lit m_Lit;
  };

  template <auto Obj>
  void func(){}


  int main()
  {
    func<Data{ 42, { 1, 2, 3, 4, 5 }, 44 }>();  // VALID

    constexpr Data d2{ 1, { 2 }, 33 };
    func<d2>(); // VALID
  }
*/

/*
  #include <cmath>  // std::round

  struct Mystruct {
    double m_val;
    constexpr Mystruct(double param) : m_val{ param } {}

    // hidden friend function
    friend std::ostream& operator<<(std::ostream& os, 
                                    const Mystruct& ms)
    {
      return os << ms.m_val;
    }
  };

  template <Mystruct ms>
  int add_vat(int val)
  {
    return static_cast<int>(std::round(val * (1 + ms.m_val)));
  }

  int main()
  {
    using namespace std;

    constexpr Mystruct ms1{ 0.18 };
    cout << ms1 << '\n';  // output -> 0.18

    cout << add_vat<ms1>(100) << '\n';  // output -> 118
    cout << add_vat<ms1>(377) << '\n';  // output -> 445
  }
*/

/*
                  -------------------------------
                  | template template parameter |
                  -------------------------------
*/

/*
  #include <vector>
  #include <list>

  template <typename T, 
            typename A, 
            template <typename, typename> typename Con>
  void func(const Con<T, A>& con){}


  int main()
  {
    // -------------------------------------------------

    std::vector<int> ivec;

    func(ivec);
    // function's parameter is std::vector<int, std::allocator<int>>
    // T    -> int, 
    // A    -> std::allocator<int>, 
    // Con  -> std::vector

    // -------------------------------------------------

    std::list<double> dlist;

    func(dlist);
    // function's parameter is std::list<double, allocator<double>>
    // T    -> double, 
    // A    -> std::allocator<double>,
    // Con  -> std::list

    // -------------------------------------------------
  }
*/

/*
  #include <type_traits>  // std::is_same

  template <typename T>
  class AClass {};

  template <template<typename> typename Temp>
  class Myclass {
  public:
    Myclass()
    {
      Temp<int> t1;
      Temp<double> t2;
    }
  };

  int main()
  {
    Myclass<AClass> m1;
    // Temp -> `AClass` class template
    // t1   -> `AClass<int>`
    // t2   -> `AClass<double>`
  }
*/

/*
  template <int>    // NTTP
  class AClass {};

  template <template<int> typename Temp>
  class Myclass {
  public:
    Myclass()
    {
      Temp<5> t1;
    }
  };

  int main()
  {
    Myclass<AClass> m1;
    // Temp -> `AClass<int>` class template
    // t1   -> `AClass<5>`
  }
*/

/*
  template <auto>    // NTTP
  class AClass {};

  template <template <auto> typename Temp>
  class Myclass {
  public:
    Myclass()
    {
      Temp<5> t1;
      Temp<61u> t2;
      Temp<3.14> t3;
    }
  };

  int main()
  {
    Myclass<AClass> m1;
  }
*/

/*
  #include <tuple>

  template <typename T>
  class A {};

  template <typename T>
  class B {};

  template <typename T>
  class C {};

  template <template <typename> typename ...Ts>
  class Myclass {
  public:
    Myclass()
    {
      std::cout << typeid(Myclass).name() << '\n';
      std::cout << typeid(std::tuple<Ts<int>...>).name() << '\n';
    }
  };

  // Myclass sınıf template'inin, template parametresi
  // template template parametre türünden.

  // Myclass sınıf template'inin template parametresi,
  // template parametre paketi.

  int main()
  {
    Myclass<A, B, C> m1;
    // output ->
    //  class Myclass<class A, class B, class C>
    //  class std::tuple<class A<int>, class B<int>, class C<int>>
  }
*/

/*
  #include <vector>
  #include <list>


  template <typename T, typename A,
            template <typename, typename> typename Con>
  std::ostream& operator<<(std::ostream& os, const Con<T,A>& con)
  {
    for (const auto& elem : con)
      os << elem << ' ';

    return os;
  }

  int main()
  {
    // -------------------------------------------------

    std::vector ivec{ 10, 20, 30, 40, 50 };

    std::cout << ivec <<'\n';
    // output -> 10 20 30 40 50

    // -------------------------------------------------

    std::list ilist{ 2, 3, 5, 7, 11, 13, 15 };

    std::cout << ilist << '\n';
    // output -> 2 3 5 7 11 13 15

    // -------------------------------------------------

  }
*/

/*
  template <template <typename ...> typename Con>
  void foo()
  {
    std::cout << "foo called\n";
  }

  template <typename T> class T1 {};
  template <typename T, typename U> class T2 {};
  template <typename T, typename U, typename V> class T3 {};

  int main()
  {
    foo<T1>();    // output -> foo called
    foo<T2>();    // output -> foo called
    foo<T3>();    // output -> foo called
  }
*/

/*
  // how to write an inserter functin template 
  // for both std::map and std::vector classes can use.

  #include <map>
  #include <vector>
  #include <string>

  template <typename T, typename U>
  std::ostream& operator<<(std::ostream& os, const std::pair<T,U>& p)
  {
    return os << '[' << p.first << ',' << p.second << ']';
  }

  template <template <typename ...> typename Con, typename ...Ts>
  std::ostream& operator<<(std::ostream& os, const Con<Ts...>& con)
  {
    for (const auto& elem : con)
      os << elem << ' ';
    return os;
  }

  // operator<< function template does have 2 template parameter
  // 1st template parameter is a template template parameter
  // 2nd template parameter is a type parameter pack

  // 1'inci parametreye herhangi bir sayıda tür parametresine 
  // sahip bir sınıf şablonu geçilebilir.

  int main()
  {
    // -------------------------------------------------

    std::vector ivec{ 10, 20, 30, 40, 50 };

    std::cout << ivec <<'\n';
    // output -> 10 20 30 40 50

    // -------------------------------------------------

    std::map<int, int> ii_map{ { 1, 2 }, { 3, 4 }, { 5, 6 }};

    std::cout << ii_map << '\n';
    // output -> [1,2] [3,4] [5,6]

    // -------------------------------------------------
  }
*/

/*
                      ---------------------
                      | variable template |
                      ---------------------
*/

/*
  template <typename T>
  constexpr T PI = (T)3.1415926535897932385L;

  template <class T>
  T circular_area(T r)
  {
    return PI<T> * r * r;
  }

  template <typename T>
  constexpr bool bx = sizeof(T) > 4;

  int main()
  {
    constexpr int x = PI<int>;        // x -> 3
    constexpr double y = PI<double>;  // y -> 3.141592653589793

    auto area = circular_area(5.6);  
    // area -> 78.5398 (double)

    auto area_2 = circular_area(5.2L);
    // area_2 -> 85.3273 (long double)

    constexpr bool b1 = bx<int>;      // b1 -> false
    constexpr bool b2 = bx<double>;   // b2 -> true
  }
*/

/*
  template <std::size_t N>
  constexpr std::size_t factorial = N * factorial<N - 1>;

  template <> // explicit specialization
  constexpr std::size_t factorial<0> = 1u;

  int main()
  {
    constexpr auto val = factorial<5>;
    // val -> 120u
  }
*/

/*
  template <std::size_t Base, std::size_t Exp>
  constexpr std::size_t power = Base * power<Base, Exp - 1>;

  template <std::size_t Base>
  constexpr std::size_t power<Base, 0> = 1u;


  int main()
  {
    static_assert(power<3, 4> == 81u);    // VALID
    static_assert(power<2, 10> == 1024u); // VALID

    constexpr auto val = power<5, 4>;
    // val -> 625u
  }
*/

/*
  #include <type_traits>  // std::is_pointer, std::is_integral  

  template <typename T>
  constexpr bool IsPointer_v = std::is_pointer<T>::value;

  template <typename T>
  constexpr bool IsIntegral_v = std::is_integral<T>::value;
*/

/*
  // fold expression can be used in variable template

  template <int ...Vals>  // template non-type parameter pack
  constexpr int i_arr[sizeof...(Vals)] = { Vals... };

  int main()
  {
    constexpr int val = i_arr<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>[5];
    // val -> 6

    constexpr int val_2 = i_arr<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>[8];
    // val_2 -> 9

    for (auto elem : i_arr<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>)
      std::cout << elem << ' ';
    // output -> 1 2 3 4 5 6 7 8 9 10
  }
*/

/*
  template <int ...Vals>
  constexpr int sum = (... + Vals); // unary left fold

  template <int ...Vals>
  constexpr int sum_square = (... + (Vals * Vals)); // unary left fold

  int main()
  {
    constexpr int i1 = sum<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>;
    // i1 -> 55

    constexpr int i2 = sum_square<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>;
    // i2 -> 385
  }
*/

/*
  template <int ...Vals>
  constexpr int div_left = (... / Vals); // unary left fold

  template <int ...Vals>
  constexpr int div_right = (Vals / ...); // unary right fold

  int main()
  {
    // ------------------------------------------------

    constexpr auto x1 = div_left<500, 10, 5, 2>;
    // x1 -> 5

    // (((500 / 10) / 5) / 2)
    // 500 / 10 = 50
    // 50 / 5 = 10
    // 10 / 2 = 5

    // ------------------------------------------------

    constexpr auto x2 = div_right<500, 10, 5, 2>;
    // x2 -> 100

    // (500 / (10 / (5 / 2)))
    // 5 / 2 = 2
    // 10 / 2 = 5
    // 500 / 5 = 100

    // ------------------------------------------------
  }
*/

/*
  template <int N>
  constexpr bool isPositive{ N > 0 };

  int main()
  {
    constexpr bool b1 = isPositive<10>;  // b1 -> true
    constexpr bool b2 = isPositive<-5>;  // b2 -> false
  }
*/

/*
  template <auto N>
  constexpr bool isPositive{ N > 0 };

  class MyInt {
  public:
    int m_x;
    constexpr MyInt(int val) : m_x{ val } {}
    constexpr bool operator>(const MyInt& other) const
    {
      return m_x > other.m_x;
    }
  };

  int main()
  {
    constexpr bool b1 = isPositive<MyInt{ 98 }>;  // b1 -> true
    constexpr bool b2 = isPositive<MyInt{ -5 }>;  // b2 -> false
  }
*/

/*
  #include <concepts> // std::integral

  template <std::integral T, typename U>
  constexpr std::size_t sz = static_cast<T>(sizeof(U));

  class Myclass {
    int m_arr[6]{};
  };

  int main()
  {
    constexpr auto s1 = sz<int, Myclass>;
    // s1 -> 24u
  }
*/

/*
            --------------------------------------------
            | default arguments in template parameters |
            --------------------------------------------
*/

/*
  // --------------------------------------------------------

  // for class templates, only the last template parameter 
  // can have a default argument

  template <typename T = int, typename U>   // syntax error
  class Myclass {};
  // error: no default argument for 'U'

  // --------------------------------------------------------

  // for function templates, all template parameters
  // can have default arguments

  template <typename T = int, typename U>   // VALID
  void func();

  // --------------------------------------------------------
*/

/*
  template <typename T = double, typename U = int>
  class Myclass {};

  Myclass<> x1;
  // "x1"'s type is  Myclass<double, int> specialization

  Myclass<int> x2;  
  // "x2"'s type is  Myclass<int, int> specialization

  Myclass<int, double> x3;
  // "x3"'s type is  Myclass<int, double> specialization
*/

/*
  #include <vector>
  #include <string>
  #include <set>
  #include <unordered_set>
  #include <memory>

  template <typename T>
  struct Default_Delete {
    operator() (T* p)
    {
      delete p;
    }
  };

  template <typename T, typename D = Default_Delete<T>>
  class Unique_Ptr {
  private:
    T* mp_obj;
  public:
    Unique_Ptr(T* p = nullptr) : mp_obj{ p } {}
    ~Unique_Ptr() 
    { 
      if (mp_obj) 
        D{}(mp_obj);
    }

  };

  int main()
  {
    using namespace std;

    // --------------------------------------------------------

    vector<int> ivec;
    // "ivec"'s type is vector<int, allocator<int>> specialization
    // there is a default argument for the 2nd template parameter

    // --------------------------------------------------------

    string str;
    // "str"'s type basic_string<char, char_traits<char>, 
    //                                              allocator<char>> 

    // --------------------------------------------------------

    set<int> iset;
    // "iset"'s type is set<int, std::less<int>, std::allocator<int>>

    // --------------------------------------------------------

    unordered_set<int> uo_iset;
    // "uo_iset"'s type is 
    // unordered_set<int, std::hash<int>, std::equal_to<int>,
    //                                          std::allocator<int>>

    // --------------------------------------------------------

    unique_ptr<string> uptr;
    // "uptr"'s type is unique_ptr<string, default_delete<string>>

    // --------------------------------------------------------
  }
*/

/*
  #include <string>
  #include <memory>   // std::unique_ptr

  template <typename T>
  struct Default_Delete {
    operator() (T* p)
    {
      delete p;
    }
  };

  template <typename T, typename D = Default_Delete<T>>
  class Unique_Ptr {
  private:
    T* mp_obj;
  public:
    Unique_Ptr(T* p = nullptr) : mp_obj{ p } {}
    ~Unique_Ptr() 
    { 
      if (mp_obj) 
        D{}(mp_obj);
    }

  };

  int main()
  {
    std::unique_ptr<std::string> up_str;
    // "up_str"'s type is unique_ptr<string, default_delete<string>>
  }
*/

/*
  #include <vector>
  #include <list>
  #include <stack>  // std::stack
  #include <queue>  // std::queue, std::priority_queue

  // general default argument themes that have been used. 

  template <typename T, typename U = T>
  class AClass {};

  // container adaptor
  template <typename T, typename U = std::vector<T>>
  class BClass {};


  int main()
  {
    // --------------------------------------------------------------

    BClass<int> b1;
    // "b1"'s type is BClass<int, std::vector<int>> specialization

    BClass<int, std::list<int>> b2;
    // "b2"'s type is BClass<int, std::list<int>> specialization

    // --------------------------------------------------------------

    std::stack<int> i_stack;  
    // std::stack is a container adaptor
    // "i_stack"'s type is stack<int, deque<int>>

    std::queue<int> i_queue;
    // std::queue is a container adaptor
    // "i_queue"'s type is queue<int, deque<int>>

    std::priority_queue<int> i_pqueue;
    // std::priority_queue is a container adaptor
    // "i_pqueue"'s type is 
    // priority_queue<int, vector<int>, less<int>>

    // --------------------------------------------------------------
  }
*/

/*
  // --------------------------------------------------------

  void foo(int, int, int = 0);
  void foo(int, int = 0, int);
  void foo(int = 0, int, int);
  // compiler merge those redeclarations into 
  // a single function declaration

  // --------------------------------------------------------

  void bar(int, int = 0, int);  // syntax error
  // error: default argument missing for parameter 3 
  // of 'void bar(int, int, int)'

  // --------------------------------------------------------

  void baz(int = 0, int, int);  // syntax error
  // error: default argument missing for parameter 2 
  // of 'void baz(int, int, int)'
  // error: default argument missing for parameter 3 
  // of 'void baz(int, int, int)'

  // --------------------------------------------------------
*/

/*
  #include <concepts> // std::same_as

  template <typename, typename U, typename V = char>
  class Myclass;    // forward declaration

  template <typename, typename U = int, typename V>
  class Myclass;    // forward declaration

  template <typename T = double, typename U, typename V>
  class Myclass {}; // definition

  int main()
  {
    using namespace std;

    Myclass<> m1;

    static_assert(same_as<decltype(m1), Myclass<double, int, char>>);
    // VALID
  }
*/

/*
  #include <concepts> // std::same_as

  template <typename, typename>
  struct Mystruct_1 {};

  template <typename, typename>
  struct Mystruct_2 {};

  template <typename T, 
            typename U, 
            template<typename, typename> typename Con = Mystruct_1>
  class Myclass {};

  // Myclass sınıf şablonunun 3. template parametresi
  // template template parametre ve 
  // default argüman olarak Mystruct_1 sınıf şablonunu alıyor.

  int main()
  {
    // ------------------------------------------------

    Myclass<int, double> m1;
    // "m1"'nin türü Myclass<int, double, Mystruct_1> olacak.

    Myclass<int, double, Mystruct_1> m2;
    // "m2"'nin türü Myclass<int, double, Mystruct_1> olacak.

    static_assert(std::same_as<decltype(m1), decltype(m2)>);  // VALID

    // ------------------------------------------------

    Myclass<int, double, Mystruct_2> m3;
    // "m3"'nin türü Myclass<int, double, Mystruct_2> olacak.

    static_assert(std::same_as<decltype(m1), decltype(m3)>);
    // error: static assertion failed

    // ------------------------------------------------
  }
*/

/*
  template <typename T = int>
  T func(T x = {}) 
  {
    std::cout << "x = " << x << '\n';
    return x;
  }

  int main()
  {
    // ------------------------------------------------

    func();     // output -> x = 0
    // for template parameter default argument is used
    // for function parameter default argument is used

    // ------------------------------------------------

    func(3.98); // output -> x = 3.98

    // ------------------------------------------------
    
    func<std::string>();  // output -> x = ""

    // for template parameter "std::string" is used
    // for function parameter default argument is used 
    // which is default constructed std::string -> empty string

    // ------------------------------------------------
  }
*/

/*
  #include <functional>  // std::less, std::greater

  template <typename T, typename F = std::less<T>>
  bool func(T x, F f = {})
  {
    return f(x, x + 1);
  }

  int main()
  {
    std::cout << std::boolalpha;

    // ------------------------------------------------

    std::cout << func(4) << '\n';   
    // output -> true

    // no argument sent to 2nd template parameter
    // default argument is used -> std::less<int>
    // function's 2nd parameter is std::less<int>{}

    // ------------------------------------------------

    std::cout << func(10, std::greater{}) << '\n';  
    // output -> false

    // ------------------------------------------------
  }
*/

/*
                ---------------------------------
                | explicit(full) specialization |
                ---------------------------------
*/

/*
  // primary template and explicit specialization's interface
  // can be different.

  template <typename T>
  struct Mystruct {
    Mystruct() 
    { 
      std::cout << "primary template\n"; 
    }

    void f1(){ std::cout << "Mystruct::f1\n"; }
    void f2(){ std::cout << "Mystruct::f2\n"; }
  };
  // primary(master) class template of Mystruct

  template <>
  struct Mystruct<int> {
    Mystruct() 
    { 
      std::cout << "Mystruct<int> explicit specialization\n"; 
    }

    void f3(){ std::cout << "Mystruct<int>::f3\n"; }
    void f4(){ std::cout << "Mystruct<int>::f4\n"; }
  };
  // explicit specialization of `Mystruct` class template 
  // for `int` type

  int main()
  {
    Mystruct<char> m1;
    m1.f1();
    // output ->
    //  primary template
    //  Mystruct::f1

    Mystruct<double> m2;
    m2.f2();
    // output ->
    //  primary template
    //  Mystruct::f2

    Mystruct<int> m3;
    m3.f3();
    // output ->
    //  Mystruct<int> explicit specialization
    //  Mystruct<int>::f3
  }
*/

/*
  // It can be more than one explicit specialization

  template <typename T, typename U>
  class Myclass {};
  // primary template of Myclass

  template <>
  class Myclass<int, double> {};
  // explicit specialization of Myclass for Myclass<int, double>

  template <>
  class Myclass<double, int> {};
  // explicit specialization of Myclass for Myclass<double, int>
*/

/*
  template <int N>
  class Myclass {
  public:
    Myclass()
    {
      std::cout << "primary template\n";
    }
  };
  // primary template of Myclass `class template`

  template <>
  class Myclass<10> {
  public:
    Myclass()
    {
      std::cout << "explicit specialization for Myclass<10>\n";
    }
  };
  // explicit specialization of Myclass for Myclass<10>

  template <>
  class Myclass<20> {
  public:
    Myclass()
    {
      std::cout << "explicit specialization for Myclass<20>\n";
    }
  };
  // explicit specialization of Myclass for Myclass<20>

  int main()
  {
    Myclass<5> m1;
    // output -> primary template

    Myclass<10> m2;
    // output -> explicit specialization for Myclass<10>

    Myclass<20> m3;
    // output -> explicit specialization for Myclass<20>
  }
*/

/*
  // to create an explicit specialization
  // primary template CAN BE an incomplete type,
  // if only the specializations will be used.

  template <int N>
  class Myclass;  // incomplete type
  // primary template of Myclass

  template <>
  class Myclass<3> {
  public:
    Myclass()
    {
      std::cout << "explicit specialization for Myclass<3>\n";
    }
  };
  // explicit specialization of Myclass for Myclass<10>

  template <>
  class Myclass<21> {
  public:
    Myclass()
    {
      std::cout << "explicit specialization for Myclass<21>\n";
    }
  };
  // explicit specialization of Myclass for Myclass<20>

  int main()
  {
    Myclass<3> m1;
    // output -> explicit specialization for Myclass<3>

    Myclass<21> m2;
    // output -> explicit specialization for Myclass<21>
  }
*/

/*
  // C++98
  // static const integral data members can be initialized
  // in the class definition

  template <std::size_t N>
  struct Factorial {
    static const std::size_t value = N * Factorial<N - 1>::value;
  };
  // primary template - Factorial class template

  template <> 
  struct Factorial<0> {
    static const std::size_t value = 1;
  };
  // base case - Factorial<0> explicit specialization
  // explicit specialization of Factorial class template 

  int main()
  {
    Factorial<6>::value;  // 720u
  }
*/

/*
  // printing from 1 to 100 without using loop.

  template <int N>
  struct Print : Print<N - 1> {
    Print()
    {
      std::cout << N << ' ';
    }
  };
  // primary template - Print class template

  template <>
  struct Print<0> {};
  // base case - Print<0> explicit specialization
  // explicit specialization of Print class template

  int main()
  {
    Print<100> p1;
    // output ->
    // 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 
    // 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 
    // 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 
    // 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 
    // 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 
    // 94 95 96 97 98 99 100
  }
*/

/*
  // classes member function can also be explicitly specialized

  template <typename T>
  struct Mystruct {
    void func(T)
    {
      std::cout << "primary template for Mystruct::func()\n";
    }
  };

  template <>
  void Mystruct<int>::func(int)
  {
    std::cout << 
      "explicit specialization for Mystruct<int>::func(int)\n";
  }

  int main()
  {
    Mystruct<double> m1;
    m1.func(3.14);
    // output -> primary template for Mystruct::func()

    Mystruct<int> m2;
    m2.func(42);
    // output -> explicit specialization for Mystruct<int>::func(int)

    Mystruct<char> m3;
    m3.func('A');
    // output -> primary template for Mystruct::func()
  }
*/

/*
  - fonksiyon şablonlarında(function template) 
    function overload resolution'a template'in kendisi giriyor.

  - eğer template'in kendisi seçilirse ve seçilen 
    fonksiyon şablonu bir explicit specialization ise
    bu durumda explicit specialization seçilir.
*/

/*
  template <typename T>
  void foo(T)
  {
    std::cout << "primary template\n";
  }
  // primary template of "foo" function template

  template <>
  void foo<int>(int)
  {
    std::cout << "explicit specialization for foo(int)\n";
  }
  // explicit specialization of "foo" function template for "int"

  int main()
  {
    foo(4.5);
    // output -> primary template

    foo(12);
    // output -> explicit specialization for foo(int)
  }
*/

/*
  template <typename T>
  void foo(T)
  {
    std::cout << "primary template\n";
  }
  // primary template of "foo" function template

  template <>
  void foo<int>(int)
  {
    std::cout << "explicit specialization for foo(int)\n";
  }
  // explicit specialization of "foo" function template for "int"

  void foo(int)
  {
    std::cout << "non-template function.\n";
  }
  // non-template function

  int main()
  {
    foo(4.5);
    // output -> primary template

    foo(12);
    // output -> non-template function.

    // functions which are in overload resolution
    // 1. primary function template
    // 2. non-template function
    // so non-template function will be selected(exact match)
  }
*/

/*
  template <typename T>
  void func(T)
  {
    std::cout << "primary template func(T)\n";
  }

  template <>
  void func<int*>(int*)
  {
    std::cout << "explicit specialization for func(int*)\n";
  }
  // explicit specialization of func(T) function template

  template <typename T>
  void func(T*)
  {
    std::cout << "primary template func(T*)\n";
  }

  int main()
  {
    int* p = nullptr;
    func(p);  // output -> primary template func(T*)

    // functions which are in overload resolution
    // 1. primary function template func(T)
    // 2. primary function template func(T*)  
    // -> 2(more specific) is selected with partial ordering rules.
  }
*/

/*
  template <typename T>
  void func(T)
  {
    std::cout << "primary template func(T)\n";
  }
  // primary function template of func(T) function template

  template <typename T>
  void func(T*)
  {
    std::cout << "primary template func(T*)\n";
  }
  // primary function template of func(T*) function template

  template <>
  void func(int*)
  {
    std::cout << "explicit specialization for func(int*)\n";
  }
  // explicit specialization of func(T*) function template


  int main()
  {
    int* p = nullptr;
    func(p);  // output -> explicit specialization for func(int*)

    // functions which are in overload resolution
    // 1. primary function template func(T)
    // 2. primary function template func(T*) --> selected
    
    // 2nd primary function template has a specialization
    // for int* type, so this specialization is selected.
  }
*/

/*
  template <typename T>
  void func(T)
  {
    std::cout << "primary template func(T)\n";
  }
  // primary function template of func(T) function template

  template <>
  void func(int*)
  {
    std::cout << "explicit spec of func(T) for func(int*)\n";
  }
  // explicit specialization of func(T) function template

  template <typename T>
  void func(T*)
  {
    std::cout << "primary template func(T*)\n";
  }
  // primary function template of func(T*) function template

  template <>
  void func(int*)
  {
    std::cout << "explicit spec of func(T*) for func(int*)\n";
  }
  // explicit specialization of func(T*) function template


  int main()
  {
    int* p = nullptr;
    func(p);  // output -> explicit spec of func(T*) for func(int*)

    // functions which are in overload resolution
    // 1. primary function template func(T)
    // 2. primary function template func(T*) --> selected
    
    // 2nd primary function template has a specialization
    // for int* type, so this specialization is selected.
  }
*/

/*
  // explicit specialization for static data members

  template <typename T>
  struct Mystruct {
    static int ms_x;
  };

  template <typename T>
  int Mystruct<T>::ms_x = 11;

  template <>
  int Mystruct<int>::ms_x = 22;

  int main()
  {
    std::cout << "Mystruct<double>::ms_x = " 
              << Mystruct<double>::ms_x <<'\n';
    // output -> Mystruct<double>::ms_x = 11
    
    std::cout << "Mystruct<int>::ms_x = " 
              << Mystruct<int>::ms_x << '\n';
    // output -> Mystruct<int>::ms_x = 22
  }
*/

/*
  template <typename T>
  constexpr std::size_t sz = sizeof(T);

  int main()
  {
    // ------------------------------------------------

    auto sum = sz<int> + sz<double> + sz<long long>;

    std::cout << "sum = " << sum << '\n';
    // output -> sum = 20

    // ------------------------------------------------

    auto sum_2 = sz<int> + sz<void>;  // syntax error

    // void is an incomplete type, 
    // so `void` can not be an operand of `sizeof` operator.
    
    // ------------------------------------------------
  }
*/

/*
  // explicit specialization for variable template 

  template <typename T>
  constexpr std::size_t sz = sizeof(T);
  
  template <>
  constexpr std::size_t sz<void> = 0;

  int main()
  {
    // ------------------------------------------------

    auto sum = sz<int> + sz<double> + sz<long long>;

    std::cout << "sum = " << sum << '\n';
    // output -> sum = 20

    // ------------------------------------------------

    auto sum_2 = sz<int> + sz<void>;

    std::cout << "sum_2 = " << sum_2 << '\n';
    // output -> sum_2 = 4
    
    // ------------------------------------------------
  }
*/

/*
                    --------------------------
                    | partial specialization |
                    --------------------------
*/

/*
  template <typename T>
  struct Mystruct {};
  // primary template of Mystruct

  template <typename T>
  struct Mystruct<T*> {};
  // partial specialization of Mystruct for (pointer types)
  // not for a specific pointer type(that is explicit specialization)

  template <typename T>
  struct Mystruct<T&> {};
  // partial specialization of Mystruct for (reference types)

  template <typename T>
  struct Mystruct<T**> {};
  // partial specialization of Mystruct for (pointer to pointer types)
*/

/*
  template <typename T>
  struct Mystruct {
    Mystruct()
    {
      std::cout << "primary template of Mystruct class template\n";
    }
  };
  // primary template of Mystruct class template

  template <typename T>
  struct Mystruct<T*> {
    Mystruct()
    {
      std::cout << 
        "Mystruct<T*> partial specialization for pointer types\n";
    }
  };
  // partial specialization for Mystruct class template
  // for pointer types

  int main()
  {
    Mystruct<int> m1;
    // output -> primary template of Mystruct class template

    Mystruct<double> m2;
    // output -> primary template of Mystruct class template

    Mystruct<int*> m3;
    // output -> Mystruct<T*> partial specialization for pointer types

    Mystruct<float*> m4;
    // output -> Mystruct<T*> partial specialization for pointer types

    Mystruct<int**> m5;
    // output -> Mystruct<T*> partial specialization for pointer types
  }
*/

/*
  template <typename T, typename U>
  struct Mystruct {
    Mystruct()
    {
      std::cout << "primary template of Mystruct class template\n";
    }
  };
  // primary template of Mystruct class template

  template <typename T>
  struct Mystruct<T, T> {
    Mystruct()
    {
      std::cout << "Mystruct<T, T> partial specialization\n";
    }
  };
  // partial specialization for Mystruct class template
  // have both template parameters are same type

  int main()
  {
    Mystruct<int, double> m1;
    // output -> primary template of Mystruct class template

    Mystruct<int, int> m2;
    // output -> Mystruct<T, T> partial specialization

    Mystruct<double, double*> m3;
    // output -> primary template of Mystruct class template

    Mystruct<double*, double*> m4;
    // output -> Mystruct<T, T> partial specialization
  }
*/

/*
  template <typename T, typename U>
  struct Mystruct {
    Mystruct()
    {
      std::cout << "primary template of Mystruct class template\n";
    }
  };
  // primary template of Mystruct class template

  template <typename T>
  struct Mystruct<T, T*> {
    Mystruct()
    {
      std::cout << "Mystruct<T, T*> partial specialization\n";
    }
  };
  // partial specialization for Mystruct class template
  // have 1st template parameter T and 2nd template parameter T* type

  int main()
  {
    Mystruct<int, double> m1;
    // output -> primary template of Mystruct class template

    Mystruct<int, int> m2;
    // output -> primary template of Mystruct class template

    Mystruct<double, double*> m3;
    // output -> Mystruct<T, T*> partial specialization

    Mystruct<double*, double*> m4;
    // output -> primary template of Mystruct class template
  }
*/

/*
  template <typename T, typename U>
  struct Mystruct {
    Mystruct()
    {
      std::cout << "primary template of Mystruct class template\n";
    }
  };
  // primary template of Mystruct class template

  template <typename T>
  struct Mystruct<T&, T&> {
    Mystruct()
    {
      std::cout << "Mystruct<T&, T&> partial specialization\n";
    }
  };
  // partial specialization for Mystruct class template
  // for both template parameters are l value reference types

  int main()
  {
    Mystruct<int, int> m1;
    // output -> primary template of Mystruct class template

    Mystruct<int&, int&> m2;
    // output -> Mystruct<T&, T&> partial specialization
  }
*/

/*
  #include <tuple>
  #include <string>

  template <typename T>
  struct Mystruct {
    Mystruct()
    {
      std::cout << "primary template of Mystruct class template\n";
    }
  };
  // primary template of Mystruct class template

  template <typename T, typename U, typename M, typename N>
  struct Mystruct<std::tuple<T, U, M, N>> {
    Mystruct()
    {
      std::cout << 
        "Mystruct<std::tuple<T, U, M, N>> partial specialization\n";
    }
  };
  // partial specialization for Mystruct class template
  // for std::tuple<T, U, M, N> type

  int main()
  {
    Mystruct<int> m1;
    // output -> primary template of Mystruct class template

    Mystruct<std::tuple<int, int, int, int>> m2;
    // output -> Mystruct<tuple<T, U, M, N>> partial specialization

    Mystruct<std::tuple<int, double, char, std::string>> m3;
    // output -> Mystruct<tuple<T, U, M, N>> partial specialization
  }
*/

/*
  #include <type_traits>  
  // std::integral_constant, std::true_type, std::false_type

  int main()
  {
    // ------------------------------------------------

    constexpr int i1 = std::integral_constant<int, 5>::value;
    // i1 -> 5

    constexpr bool b1 = std::integral_constant<bool, true>::value;
    // b1 -> true

    // ------------------------------------------------

    std::integral_constant<bool, true>::value_type b2 = false;
    // "b2"'s type is bool 

    std::integral_constant<int, 5>::value_type i2 = 10;
    // "i2"'s type is int

    // ------------------------------------------------

    std::integral_constant<int, 5>::type i2;
    // "i2"'s data type is std::integral_constant<int, 5>

    // ------------------------------------------------

    std::true_type;
    // "std::true_type" is a type alias 
    // for "std::integral_constant<bool, true>"

    std::false_type;
    // "std::false_type" is a type alias
    // for "std::integral_constant<bool, false>"

    // ------------------------------------------------
  }
*/

/*
  #include <type_traits>

  template <typename T>
  struct Mystruct : std::false_type {};

  int main()
  {
    // ------------------------------------------------

    constexpr bool b1 = Mystruct<int>::value; // b1 -> false

    // ------------------------------------------------

    Mystruct<int>::type;
    // "Mystruct<int>::type" is a type alias for "std::false_type"
    // which is a type alias for "integral_constant<bool, false>"

    // ------------------------------------------------

    Mystruct<int>::value_type b2 = true;
    // "b2"'s type is bool

    // ------------------------------------------------
  }
*/

/*
  // metafunctions are written 
  // using std::false_type and std::true_type

  #include <type_traits>

  template <typename T>
  struct IsPointer : std::false_type {};
  // primary template for IsPointer metafunction

  template <typename T>
  struct IsPointer<T*> : std::true_type {};
  // partial specialization for IsPointer metafunction
  // for pointer types

  template <typename T>
  constexpr bool IsPointer_v = IsPointer<T>::value;
  // variable template for IsPointer metafunction

  int main()
  {
    constexpr bool b1 = IsPointer<int>::value;  // b1 -> false
    constexpr bool b2 = IsPointer<int*>::value; // b2 -> true

    constexpr bool b3 = IsPointer_v<double>;  // b3 -> false
    constexpr bool b4 = IsPointer_v<double*>; // b4 -> true
  }
*/

/*
  #include <memory> // std::unique_ptr

  int main()
  {
    std::unique_ptr<int> iptr1(new int);
    std::unique_ptr<int[]> iptr2(new int[5]);

    auto x = *iptr1;  // VALID

    // ------------------------------------------------

    auto y = *iptr2;  // syntax error
    // error: no match for 'operator*' 
    // (operand type is 'std::unique_ptr<int []>')

    auto z = iptr2[3];  // VALID

    // operator* is not defined for  
    // std::unique_ptr<T[]> partial specialization 

    // operator[] is defined for 
    // std::unique_ptr<T[]> partial specialization

    // ------------------------------------------------

    // default_delete's partial specialization for T[] type
    // is using array delete expression

    // ------------------------------------------------
  }
*/

/*
  template<typename T>
  struct Mystruct {
    Mystruct()
    {
      std::cout << "primary template of Mystruct class template\n";
    }
  };
  // primary template of Mystruct class template

  template<typename T>
  struct Mystruct<T[5]> {
    Mystruct()
    {
      std::cout << "Mystruct<T[5]> partial specialization\n";
    }
  };
  // partial specialization for Mystruct class template
  // for 5 element array types

  template<typename T>
  struct Mystruct<T[12]> {
    Mystruct()
    {
      std::cout << "Mystruct<T[12]> partial specialization\n";
    }
  };
  // partial specialization for Mystruct class template
  // for 12 element array types

  template<typename T>
  struct Mystruct<T[]> {
    Mystruct()
    {
      std::cout << "Mystruct<T[]> partial specialization\n";
    }
  };
  // partial specialization for Mystruct class template
  // for incomplete array types

  int main()
  {
    Mystruct<int[7]> m1;
    // output -> primary template of Mystruct class template

    Mystruct<int[5]> m2;
    // output -> Mystruct<T[5]> partial specialization

    Mystruct<double[12]> m3;
    // output -> Mystruct<T[12]> partial specialization

    Mystruct<double> m4;
    // output -> primary template of Mystruct class template

    Mystruct<int[]> m5;
    // output -> Mystruct<T[]> partial specialization
  }
*/

/*
  template <typename T, int N>
  struct Mystruct {
    Mystruct()
    {
      std::cout << "primary template of Mystruct class template\n";
    }
  };
  // primary template of Mystruct class template

  template <typename T>
  struct Mystruct<T, 20> {
    Mystruct()
    {
      std::cout << "Mystruct<T, 20> partial specialization\n";
    }
  };
  // partial specialization for Mystruct class template
  // for 2nd template parameter is 20
*/

/*
  #include <concepts>   // std::integral, std::floating_point
  #include <string>

  template <typename T>
  struct X {
    X() { std::cout << "primary template\n"; }  
  };

  template <std::integral T>
  struct X<T> {
    X() { std::cout << "integral specialization\n"; }
  };
  // partial specialization for X class template
  // for integral types

  template <std::floating_point T>
  struct X<T> {
    X() { std::cout << "floating_point specialization\n"; }
  };
  // partial specialization for X class template
  // for floating point types

  int main()
  {
    X<int> x1;            // output -> integral specialization
    X<char> x2;           // output -> integral specialization
    X<double> x3;         // output -> floating_point specialization
    X<std::string> x4;    // output -> primary template
    X<int*> x5;           // output -> primary template
  }
*/

/*
  #include <ranges> // std::ranges::range(concept)
  #include <vector>
  #include <stack>

  template <typename T>
  struct Mystruct {
    Mystruct()
    {
      std::cout << "primary template of Mystruct class template\n";
    }
  };

  template <std::ranges::range R>
  struct Mystruct<R> {
    Mystruct()
    {
      std::cout << "partial specialization for range types\n";
    }
  };
  // partial specialization for Mystruct class template
  // for types that satisfy the std::ranges::range concept

  int main()
  {
    Mystruct<int> m1;
    // output -> primary template of Mystruct class template

    Mystruct<std::vector<int>> m2;
    // output -> partial specialization for range types

    Mystruct<std::stack<int>> m3;
    // output -> primary template of Mystruct class template
  }
*/

/*
                  ------------------------------
                  | variadic templates (C++11) |
                  ------------------------------
*/

/*
  template <typename ...Args>
  class AClass {};
  // template type parameter pack

  template <int ...Args>
  class BClass {};
  // non-type template parameter pack 

  template <template <typename> typename ...Args>
  class CClass {};
  // template template parameter pack
*/

/*
  template <typename ...Args>
  void func(Args... args){}

  // compiler will generate a function like this
  template <typename T1, typename T2, typename T3>
  void func(T1 arg1, T2 arg2, T3 arg3){}
*/

/*
  // parameter pack can be empty [0 - N]

  template <typename ...Args>
  void func(Args... args)
  {
    constexpr auto sz_1 = sizeof...(Args);
    constexpr auto sz_2 = sizeof...(args);

    std::cout << "sz_1 = " << sz_1 << '\n';
    std::cout << "sz_2 = " << sz_2 << '\n';

    // sz1 and sz2 are constants which are equal to the number of
    // template type parameter inside the parameter pack.
  }

  int main()
  {
    func(1, 2, 3.4);
    // output -> 
    //  sz_1 = 3
    //  sz_2 = 3

    func(1, 2, 3, 4);
    // output ->
    //  sz_1 = 4
    //  sz_2 = 4

    func();
    // output ->
    //  sz_1 = 0
    //  sz_2 = 0
  }
*/

/*
  template <typename T, typename ...Args>
  void func(int, Args... args) {}
*/

/*
  - compile time recursivity
  - compile time recursivity using static if(if constexpr)
  - initializer_list technique
  - fold expressions
*/

/*
  // compile time recursivity (eksiltme tekniği)

  template <typename T>
  void print(const T& ref)
  {
    std::cout << ref << ' ';
  }

  template <typename T, typename ...Ts>
  void print(const T& ref, const Ts&... args)
  {
    print(ref);
    print(args...);
  }

  int main()
  {
    int ival = 11;
    double dval = 22.44;

    print(ival, dval, 9L, "hello world");
    // output -> 11 22.44 9 hello world
*/

/*
  template <typename T>
  void print(const T& ref)
  {
    std::cout << ref << '\n';
  }

  template <typename T, typename ...Ts>
  void print(const T& ref, const Ts&... args)
  {
    std::cout << "[0] - print started\n";

    print(ref);

    std::cout << "[1] - print continues\n";

    print(args...);

    std::cout << "[2] - print finished\n";
  }

  int main()
  {
    int ival = 11;
    double dval = 22.44;

    print(ival, dval, 9L, "hello world");
    // output -> 
    //  [0] - print started   --> print(ival, dval, 9L, "hello world")
    //  11
    //  [1] - print continues
    //  [0] - print started   --> print(dval, 9L, "hello world")
    //  22.44
    //  [1] - print continues
    //  [0] - print started   --> print(9L, "hello world")
    //  9
    //  [1] - print continues 
    //  hello world           --> print("hello world") [base case]
    //  [2] - print finished
    //  [2] - print finished
    //  [2] - print finished
  }
*/

/*
  // initializer_list technique

  #include <initializer_list>

  template <typename ...Args>
  void Print(const Args&... args)
  {
    using namespace std;
    (void)initializer_list<int>{ (cout << args << ' ', 0)... };
  }

  // comma operator generates a sequence point.
  // left to right evaluation order.
  // comma operators generated value is 
  // the value of the right operand.

  int main()
  {
    Print(11, 22.44, 9L, "hello world");
    // output -> 11 22.44 9 hello world
  }
*/

/*
  template <typename ...Args>
  auto sum_1(const Args... args)
  {
    return (args + ...);
  }

  auto sum_2(const auto... args)
  {
    return (args + ...);
  }
*/

/*
  // poor mans fold expression 

  #include <initializer_list>

  template <typename ...Ts>
  void print_1(const Ts&... args)
  {
    int arr[] = { ((std::cout << args << ' '), 0)... };
    // problem is an array is defined but not used.
  }

  template <typename ...Ts>
  void print_2(const Ts&... args)
  {
    using namespace std;

    (void)initializer_list<int>{ ((cout << args << ' '), 0)... };
  }

  template <typename ...Ts>
  void print_3(const Ts&... args)
  {
    using extender = int[];
    (void)extender{ ((std::cout << args << ' '), 0)... };
  }

  int main()
  {
    print_1(11, 22.44, 9L, "hello world");
    // output -> 11 22.44 9 hello world

    std::cout << '\n';

    print_2(11, 22.44, 9L, "hello world");
    // output -> 11 22.44 9 hello world
    
    std::cout << '\n';

    print_3(11, 22.44, 9L, "hello world");
    // output -> 11 22.44 9 hello world
  }
*/

/*
                        ------------------
                        | pack expansion |
                        ------------------
*/

/*
  template <typename ...Args>
  void func(Args... args)
  {
    foo(args...);
    // foo(p1, p2, p3);
  }
*/

/*
  #include <tuple>

  template <typename ...Args>
  class Myclass{
    std::tuple<Args...> m_tx;
  };

  int main()
  {
    Myclass<int, int, int> m1;
    // m_tx data members type is std::tuple<int, int, int>
  }
*/

/*
  template <typename ...Args>
  class Myclass {
  public:
    static constexpr std::size_t size = sizeof...(Args);
  };

  int main()
  {
    constexpr size_t sz1 = Myclass<int, long, double>::size;  
    // sz1 -> 3u

    constexpr size_t sz2 = Myclass<int, int, int, int>::size;
    // sz2 -> 4u
  }
*/

/*
  #include <array>

  template <typename ...Args>
  auto create_array(Args... args)
  {
    using namespace std;

    return array<size_t, sizeof...(Args)>{ sizeof(Args)... };

    // sizeof...(Args) -> sizeof operator
    // sizeof(Args)... -> pack expansion
  }

  struct Mystruct{
    int buffer[10];
  };

  int main()
  {
    auto arr = create_array(3, 4.5, Mystruct{});
    // "arr"'s type is std::array<std::size_t, 3u>

    for (const auto& val : arr)
      std::cout << val << ' ';
    // output -> 4 8 40
    // 4(sizeof(int)), 8(sizeof(double)), 40(sizeof(Mystuct))
  }
*/

/*
  template <typename ...Ts>
  void func(Ts... args)
  {
    foo(args...);         // foo(p1, p2, p3)
    foo(++args...);       // foo(++p1, ++p2, ++p3)
    foo(&args...);        // foo(&p1, &p2, &p3)
    foo(sizeof(args)...); // foo(sizeof(p1), sizeof(p2), sizeof(p3))
  }
*/

/*
  template <typename ...Ts>
  void foo(Ts... args)
  {
  }

  template <typename ...Ts>
  void func(Ts... args)
  {
    foo(&args...);  // foo(&p1, &p2, &p3)
  }

  int main()
  {
    // ------------------------------------------------

    func(12, 45, 67);
    // foo<int*, int*, int*> specialization will be called

    // ------------------------------------------------
    
    int x{}, int y{};
    func(&x, &y);
    // foo<int**, int**> specialization will be called

    // ------------------------------------------------
  }
*/

/*
  int bar(int x) 
  { 
    std::cout << "bar(" << x << ")\n";
    return x;
  }

  template <typename ...Ts>
  void foo(Ts... args){}

  template <typename ...Ts>
  void func(Ts... args)
  {
    foo(bar(args)...);  // foo(bar(p1), bar(p2), bar(p3))
  }


  int main()
  {
    func(12, 45, 67, 33);
    // output ->
    //  bar(33)
    //  bar(67)
    //  bar(45)
    //  bar(12)
  }
*/

/*
  int bar(int x) 
  { 
    std::cout << "bar(" << x << ")\n";
    return x;
  }

  template <typename ...Ts>
  void foo(Ts... args){}

  template <typename ...Ts>
  void func(Ts... args)
  {
    foo(bar(args * args)...);  
    // foo(bar(p1 * p1), bar(p2 * p2), bar(p3 * p3))
  }

  int main()
  {
    func(2, 5, 7, 9);
    // output ->
    //  bar(81)
    //  bar(49)
    //  bar(25)
    //  bar(4)
  }
*/

/*
  template <typename ...Ts>
  class Base {
  public:
    Base()
    {
      std::cout << typeid(Base).name() << '\n';
    }
  };

  template <typename ...Ts>
  class Der : public Base<Ts...> {
  public:
    Der()
    {
      std::cout << typeid(Der).name() << '\n';
    }
  };

  template <typename ...Ts>
  class Der_2 : public Base<Ts*...> {
  public:
    Der_2()
    {
      std::cout << typeid(Der_2).name() << '\n';
    }
  };

  int main()
  {
    Der<int, double, long> d1;
    // output ->
    // class Base<int, double, long>
    // class Der<int, double, long>

    Der_2<long, double, int> d2;
    // output ->
    // class Base<long*, double*, int*>
    // class Der_2<long, double, int>
  }
*/

/*
  struct AStruct {
    AStruct(){}
    void func_A(){}
  };

  struct BStruct {
    BStruct(){}
    void func_B(){}
  };

  struct CStruct {
    CStruct(){}
    void func_C(){}
  };

  template <typename ...Ts>
  class Myclass : public Ts... {};
  // class Myclass : public AStruct, public BStruct, public CStruct

  int main()
  {
    Myclass<AStruct, BStruct, CStruct> m1;

    m1.func_A();    // VALID
    m1.func_B();    // VALID
    m1.func_C();    // VALID
  }
*/

/*
  struct AStruct {
    AStruct(int val)
    {
      std::cout << "AStruct(int val), val = " << val << '\n';
    }
  };

  struct BStruct {
    BStruct(int val)
    {
      std::cout << "BStruct(int val), val = " << val << '\n';
    }
  };

  struct CStruct {
    CStruct(int val)
    {
      std::cout << "CStruct(int val), val = " << val << '\n';
    }
  };

  template <typename ...Ts>
  class Myclass : public Ts... {
  public:
    Myclass() : Ts{ 44 }... {}
  };

  int main()
  {
    Myclass<AStruct, BStruct, CStruct> m1;
    // output ->
    //  AStruct(int val), val = 44
    //  BStruct(int val), val = 44
    //  CStruct(int val), val = 44
  }
*/

/*
  #include <tuple>

  // 2 template type parameter, 1 template parameter pack
  template <typename A, typename B, typename ...Args>
  void func_1(A p1, B p2, Args... pack)
  {
    std::tuple<A, B, Args...> tx;
    std::cout << typeid(tx).name() << '\n';
  }

  // 2 template type parameter, 1 template parameter pack
  template <typename A, typename B, typename ...Args>
  void func_2(A p1, B p2, Args... pack)
  {
    std::tuple<Args..., A, B> tx;
    std::cout << typeid(tx).name() << '\n';
  }

  // 2 template type parameter, 1 template parameter pack
  template <typename A, typename B, typename ...Args>
  void func_3(A p1, B p2, Args... pack)
  {
    std::tuple<A, Args..., B> tx;
    std::cout << typeid(tx).name() << '\n';
  }

  int main()
  {
    func_1(2, 3.4, 'A', 14, 15.5f);
    // output -> class std::tuple<int, double, char, int, float>

    func_2(2, 3.4, 'A', 14, 15.5f);
    // output -> class std::tuple<char, int, float, int, double>

    func_3(2, 3.4, 'A', 14, 15.5f);
    // output -> class std::tuple<int, char, int, float, double>
  }
*/

/*
  // pack expansion `...` elipsis tokenından önceki 
  // en uzun ifadeye uygulanır.

  void print(auto... args)
  {
    ((std::cout << args << ' '), ...);
    std::cout << '\n';
  }

  auto func_1(auto* x)
  {
    return *x * *x + 10;
  }

  void func_2(auto... args)
  {
    print(args ...);              
    // print(p1, p2, p3)

    print(&args ...);             
    // print(&p1, &p2, &p3)

    print(10 * args ...);         
    // print(10 * p1, 10 * p2, 10 * p3)

    print(args * args ...);       
    // print(p1 * p1, p2 * p2, p3 * p3)

    print(func_1(&args) ...);     
    // print(func_1(&p1), func_1(&p2), func_1(&p3))

    print(++args ...);
    // print(++p1, ++p2, ++p3)
  }

  int main()
  {
    int x { 1 }, y { 2 }, z { 3 };

    func_2(x, y, z);
    // output ->
    //  1 2 3
    //  0xe810bffe00 0xe810bffe08 0xe810bffe10
    //  10 20 30
    //  1 4 9
    //  11 14 19
    //  2 3 4
  }
*/

/*
  template <typename T>
  T square(T x)
  {
    return x * x;
  }

  template <typename ...Ts>
  auto sum(Ts... args)
  {
    return (... + args);
  }

  template <typename ...Ts>
  void call_sum(Ts... args)
  {
    // ------------------------------------------------

    auto x1 = sum(args...);
    // sum(1, 2, 3, 4, 5)
    std::cout << "x1 = " << x1 << '\n'; 
    // output -> x1 = 15

    // ------------------------------------------------

    auto x2 = sum(85, args...);
    // sum(85, 1, 2, 3, 4, 5)
    std::cout << "x2 = " << x2 << '\n'; 
    // output -> x2 = 100

    // ------------------------------------------------

    auto x3 = sum(square(args)...);
    // sum(square(1), square(2), square(3), square(4), square(5))
    std::cout << "x3 = " << x3 << '\n'; 
    // output -> x3 = 55

    // ------------------------------------------------
  }

  int main()
  {
    call_sum(1, 2, 3, 4, 5);
  }
*/

/*
  // cppreference parameter pack example 

  template <class... Us>
  void func_1(Us... args){}

  template <class... Ts>
  void func_2(Ts... args)
  {
    func_1(&args...);
    // func(&p1, &p2, &p3)
    // 1st parameter type is int*
    // 2nd parameter type is double*
    // 3rd parameter type is const char**
  }

  int main()
  {
    func_2(1, 0.2, "A");
  }
*/

/*
  #include <concepts> // std::integral

  // variadic parameter pack can be constrainted with concepts

  template <std::integral ...Ts>
  constexpr auto sum(Ts... args)
  {
    return (0 + ... + args);
  }

  template <typename ...Ts>
  constexpr auto mul(Ts... args)
  {
    return (1 * ... * args);
  }

  template <typename ...Ts>
  constexpr auto foo(Ts... args)
  {
    return mul(sum(args...) + args...);
    // mul(sum(1, 2, 4) + 1, sum(1, 2, 4) + 2, sum(1, 2, 4) + 4)
    // mul(8, 9, 11);
  }

  int main()
  {
    constexpr auto result = foo(1, 2, 4); // result -> 792
  }
*/

/*
  #include <utility>      // std::forward, std::move
  #include <type_traits>  // std::is_lvalue_reference

  template <typename ...Ts>
  void func_2(Ts&&... args)
  {
    int count{};

    ((std::cout << ++count << ". argument = " 
      << (std::is_lvalue_reference_v<Ts> ? "L value" : "R value")
      << '\n'), ...);
  }

  template <typename ...Ts>
  void func_1(Ts&&... args)
  {
    // perfect forwarding parameter pack

    func_2(std::forward<Ts>(args)...);
    // func_2(std::forward<T1>(p1), 
    //        std::forward<T2>(p2), std::forward<T3>(p3));
  }

  int main()
  {
    int x{};

    func_1(x, 35, std::move(x), "hello world");
    // output ->
    //  1. argument = L value
    //  2. argument = R value
    //  3. argument = R value
    //  4. argument = L value
  }
*/

/*
  #include <vector>
  #include <string>
  #include <utility>  // std::forward

  template <typename ...Args>
  class Myclass {
  public:
    Myclass(Args...) {}
  };

  template <typename ...Ts>
  void func_2(Ts... args)
  {
    Myclass<Ts...> m1;
    Myclass<Ts...> m2(args...);
  }

  template <typename ...Ts>
  void func_1(Ts&&... args)
  {
    Myclass<Ts...> m3(std::forward<Ts>(args)...);
  }

  int main()
  {
    std::string str{ "hello world" };
    std::vector ivec{ 2, 5, 7, 9 };

    func_1(str, ivec, 10);
  }
*/

/*
  // non-type template parameter pack
  template <size_t ...Ns>
  struct AStruct {
    AStruct()
    {
      std::cout << typeid(AStruct).name() << '\n';
    }
  };

  // non-type template parameter pack
  template <size_t ...Ns>
  void func()
  {
    AStruct<Ns...> a1;
  }

  int main()
  {
    func<1, 3, 5, 7>();
    // output -> struct AStruct<1, 3, 5, 7>

    func<1, 2>();
    // output -> struct AStruct<1, 2>

    func<>();
    // output -> struct AStruct<>
  }
*/

/*
  template <typename ...Types, int ...Ns>
  void func(Types(&... args)[Ns])
  {
    std::cout << __FUNCSIG__ << '\n'
    // std::cout << __PRETTY_FUNCTION__ << '\n';
  }

  int main()
  {
    int arr_1[2]{};
    double arr_2[3]{};

    func(arr_1);
    // func<int, 2u>(int(&)[2])

    func(arr_1, arr_2);
    // func<int, double, 2u, 3u>(int(&)[2], double(&)[3])
  }
*/

/*
  #include <type_traits>    // std::common_type
  #include <array> 

  template <typename ...Ts>
  struct Mystruct {
    std::common_type_t<Ts...> m_val;

    Mystruct(Ts... args) : m_val{ (... + args) } {}
  };

  template <typename ...Ts>
  void func(Ts... args)
  {
    using namespace std;

    array<common_type_t<Ts...>, sizeof...(Ts)> arr{ args... };
    cout << "type of arr = " << typeid(arr).name() << '\n';

    Mystruct m1(args...);
    cout << "m1.m_val = " << m1.m_val << '\n';
    cout << "type of m1 = " << typeid(m1).name() << '\n';

    Mystruct m2(--args...);
    cout << "m2.m_val = " << m2.m_val << '\n';
    cout << "type of m2 = " << typeid(m2).name() << '\n';
  }

  int main()
  {
    func(10, 20, 30, 40);
    // output ->
    // type of arr = std::array<int, 4>
    // m1.m_val = 100
    // type of m1 = Mystruct<int, int, int, int>
    // m2.m_val = 96
    // type of m2 = Mystruct<int, int, int, int>
  }
*/

/*
  #include <type_traits>  // std::integral_constant

  int main()
  {
    using namespace std;

    // ------------------------------------------------

    constexpr auto x = integral_constant<int, 5>{};
    // "x"'s type is std::integral_constant<int, 5>

    // ------------------------------------------------

    constexpr int i1 = integral_constant<int, 5>{};
    constexpr int i2 = integral_constant<int, 5>{}.operator int();
    // Those 2 lines are equivalent.

    // ------------------------------------------------

    constexpr int i3 = integral_constant<int, 5>{}();
    constexpr int i4 = integral_constant<int, 5>{}.operator()();
    // Those 2 lines are equivalent.

    // ------------------------------------------------

    constexpr auto elem = integral_constant<int, 5>{} +
                          integral_constant<int, 2>{};
    // elem -> 7
    // both expressions are casted to int 
    // with operator int() function
  }
*/

/*
  #include <type_traits> 

  template <typename T>
  struct Is_Pointer : std::false_type {};
  // primary template of Is_Pointer metafunction

  template <typename T>
  struct Is_Pointer<T*> : std::true_type {};
  // partial specialization for Is_Pointer metafunction
  // for pointer types

  template <typename T>
  constexpr bool Is_Pointer_v = Is_Pointer<T>::value;
  // variable template for Is_Pointer metafunction

  int main()
  {
    constexpr bool b1 = Is_Pointer<int>::value;   
    // b1 -> false

    constexpr bool b2 = Is_Pointer<int*>::value;
    // b2 -> true

    constexpr bool b3 = Is_Pointer_v<double>;
    // b3 -> false

    constexpr bool b4 = Is_Pointer_v<double*>;
    // b4 -> true
  }
*/

/*
  // template type parameter pack
  template <typename ...Ts>
  struct Encloser {

    // non-type template parameter pack
    template <Ts... args>
    struct Nested {};
  };

  using type_encloser = Encloser<double, long, char>;
  using type_nested = type_encloser::Nested<4.5, 55L, 'B'>;

  int main()
  {
    std::cout << typeid(type_encloser).name() << '\n';
    // output -> Encloser<double, long, char>

    std::cout << typeid(type_nested).name() << '\n';
    // output -> 
    //  Encloser<double, long, char>::Nested<4.500000, 55, 65>
  }
*/

/*
  #include <concepts>  // std::same_as

  constexpr int square(int x) { return x * x; }

  template <int N>
  void print_int_array(int(&i_arr)[N])
  {
    for (int i : i_arr)
      std::cout << i << ' ';
    std::cout << '\n';
  }

  // concept constrained abbreviated template syntax
  void func(std::same_as<int> auto... args)
  {
    int arr_1[] = { args... };
    print_int_array(arr_1);

    int arr_2[] = { args... , 0 };
    print_int_array(arr_2);

    int arr_3[sizeof...(args) + 2] = { 0, args..., 9 };
    print_int_array(arr_3);

    int arr_4[] = { square(args)... };
    print_int_array(arr_4);
  }

  int main()
  {
    func(1, 3, 5, 7);
    // output -> 
    //  1 3 5 7
    //  1 3 5 7 0
    //  0 1 3 5 7 9
    //  1 9 25 49
  }
*/

/*
  #include <type_traits>  // std::is_base_of

  template <typename ...Ts>
  class Base {};

  template <typename ...Ts>
  class Der : public Base<Ts...> {
  public:
    constexpr static size_t size = sizeof...(Ts);
  };

  int main()
  {
    using namespace std;

    constexpr auto N = Der<int, double>::size;
    // N -> 2u

    static_assert(is_base_of_v<Base<int, char>, Der<int, char>>); 
    // VALID
  }
*/

/*
  #include <type_traits>  // std::is_base_of

  template <typename ...Ts>
  class Base {};

  template <typename ...Ts>
  class Der : public Base<Ts*...> {
  public:
    constexpr static size_t size = sizeof...(Ts);
  };

  int main()
  {
    using namespace std;

    static_assert(is_base_of_v<Base<int, char>, Der<int, char>>); 
    // error : static assertion failed

    static_assert(is_base_of_v<Base<int*, long*>, Der<int, long>>);
    // VALID
  }
*/

/*
  #include <type_traits>  // std::is_base_of

  struct X {
    X(int i) { std::cout << "X::X(int i), i = " << i << '\n'; }
  };

  struct Y {
    Y(int i) { std::cout << "Y::Y(int i), i = " << i << '\n'; }
  };

  struct Z {
    Z(int i) { std::cout << "Z::Z(int i), i = " << i << '\n'; }
  };

  template <typename ...Ts>
  class A : public Ts... {
  public:
    A() : Ts{ 22 }... {}
  };

  int main()
  {
    A<X, Y, Z> a1;
    // output ->
    //  X::X(int i), i = 22
    //  Y::Y(int i), i = 22
    //  Z::Z(int i), i = 22

    using a1_type = decltype(a1);

    static_assert(std::is_base_of_v<X, a1_type> &&
                  std::is_base_of_v<Y, a1_type> &&
                  std::is_base_of_v<Z, a1_type>);
    // VALID
  }
*/

/*
  struct A {
    void a_func(){}
  };

  struct B {
    void b_func(){}
  };

  struct C {
    void c_func(){}
  };

  template <typename ...Ts>
  struct Mystruct : Ts... {};

  int main()
  {
    Mystruct<A, B, C> m1;

    m1.a_func();  // VALID
    m1.b_func();  // VALID
    m1.c_func();  // VALID
  }
*/

/*
  struct A {
    void func(int x)
    {
      std::cout << "A::func(int x), x = " << x << '\n';
    }
  };

  struct B {
    void func(double x)
    {
      std::cout << "B::func(double x), x = " << x << '\n';
    }
  };

  struct C {
    void func(char x)
    {
      std::cout << "C::func(char x), x = " << x << '\n';
    }
  };

  template <typename ...Ts>
  struct XStruct : Ts... {};

  template <typename ...Ts>
  struct YStruct : Ts... {

    using Ts::func...;  
    // using declaration with pack expansion 
  };

  int main()
  {
    // ------------------------------------------------

    XStruct<A, B, C> x1;
    x1.func(10);    // syntax error
    x1.func(3.4);   // syntax error
    x1.func('A');   // syntax error
    // error: request for member 'func' is ambiguous

    // ------------------------------------------------

    YStruct<A, B, C> y1;
    y1.func(10);  // output -> A::func(int x), x = 10
    y1.func(3.4); // output -> B::func(double x), x = 3.4
    y1.func('A'); // output -> C::func(char x), x = A

    // ------------------------------------------------
  }
*/

/*
  #include <functional>  // std::hash
  #include <string>

  int main()
  {
    std::cout << std::hash<int>{}(56) << '\n';
    // output -> 56

    std::cout << std::hash<double>{}(3.14) << '\n';
    // output -> 5464867211497793177

    std::string str{ "hello world" };
    std::cout << std::hash<std::string>{}(str) << '\n';
    // output -> 5577293430985752569
  }
*/

/*
  #include <string>
  #include <functional>  // std::hash

  struct Mystruct {
    int m_ival;
    double m_dval;
    std::string m_str;
  };

  // ---- First Way ----
  // explicit specialization
  template <>
  struct std::hash<Mystruct> {
    std::size_t operator()(const Mystruct& m)
    {
      std::hash<int>{}(m.m_ival)        +  
      std::hash<double>{}(m.m_dval)     +
      std::hash<std::string>{}(m.m_str);
    }
  };

  // ---- Second Way ----
  struct Mystruct_hasher {
    std::size_t operator()(const Mystruct& m)
    {
      std::hash<int>{}(m.m_ival)        +  
      std::hash<double>{}(m.m_dval)     +
      std::hash<std::string>{}(m.m_str);
    }
  };
*/

/*
  template <typename T>
  void hash_combine(std::size_t& seed, const T& val)
  {
    seed ^= std::hash<T>{}(val) + 
            0x9e3779b9          + 
            (seed << 6)         + 
            (seed >> 2);
  }

  template <typename T>
  std::size_t hash_val(std::size_t& seed, const T& val)
  {
    hash_combine(seed, val);
  }

  template <typename T, typename ...Ts>
  inline void hash_val( std::size_t& seed, 
                        const T& val, 
                        const Ts&... args)
  {
    hashCombine(seed, val);
    hash_val(seed, args...);
  }

  template <typename ...Ts>
  inline std::size_t hash_val(const Ts&... args)
  {
    std::size_t seed = 0;
    hash_val(seed, args...);
    return seed;
  }
*/

/*
                  ----------------------------
                  | fold expressions (C++17) |
                  ----------------------------
*/

/*
  --------------
  | unary fold | 
  --------------

  binary operator is being used. 
    - one operand is `...` elipsis token
    - other operand is parameter pack

  when parameter pack is empty it will cause syntax error.

  ------------------------------------------------

  - when elipsis token is on the left of parameter pack
    it is called unary left fold.

  (... + args)  -> unary left fold
    (((arg1 + arg2) + arg3) + arg4)

  ------------------------------------------------

  - when elipsis token is on the right of parameter pack
    it is called unary right fold.

  (args + ...)  -> unary right fold
    (arg1 + (arg2 + (arg3 + arg4)))

  ------------------------------------------------
  ------------------------------------------------

  ---------------
  | binary fold | 
  ---------------
  
  - when elipsis token is on the left of parameter pack
    it is called binary left fold.

  (init expression + ... + args ) -> binary left fold
    (((0 + arg1) + arg2) + arg3)

  ------------------------------------------------

  - when elipsis token is on the right of parameter pack
    it is called binary right fold.

  (args + ... + init expression ) -> binary right fold
    (arg1 + (arg2 + (arg3 + 0)))

  ------------------------------------------------
*/

/*
                        --------------
                        | unary fold |
                        --------------
*/

/*
  #include <string>

  template <typename ...Ts>
  constexpr auto sum_left(Ts... args)
  {
    return (... + args);    // unary left fold
  }

  template <typename ...Ts>
  constexpr auto sum_right(Ts... args)
  {
    return (args + ...);    // unary right fold
  }

  int main()
  {
    // ------------------------------------------------

    constexpr auto ival_1 = sum_left(1, 2, 3, 4, 5);  
    // ival_1 -> 15
    // ((((1 + 2) + 3) + 4) + 5)

    // ------------------------------------------------

    constexpr auto ival_2 = sum_right(1, 2, 3, 4, 5);
    // ival_2 -> 15
    // (1 + (2 + (3 + (4 + 5))))

    // ------------------------------------------------

    using namespace std::literals;

    auto str_1 = sum_left("hello"s, "_world", "_galaxy");
    // (("hello"s + "world") + "galaxy")
    // ((std::string + const char*) + const char*)

    // std::string + const char* -> std::string

    std::cout << "str_1 = " << str_1 << '\n';
    // output -> str_1 = hello world galaxy

    // ------------------------------------------------

    auto str_2 = sum_right("hello"s, " world", " universe");
    // syntax error
    // error: invalid operands of types 'const char*' 
    // and 'const char*' to binary 'operator+'

    // (std::string + (const char* + const char*))
    // const char* + const char* -> syntax error 
    // pointer addition is syntax error

    // ------------------------------------------------
  }
*/

/*
  template <typename ...Args>
  constexpr auto fdiv_right(Args&&... args)
  {
    return (args / ...);  // unary right fold
  }

  template <typename ...Args>
  constexpr auto fdiv_left(Args&&... args)
  {
    return (... / args);  // unary left fold
  }

  int main()
  {
    constexpr auto i1 = fdiv_right(500, 50, 5, 2);  // i1 -> 20
    // (500 / (50 / (5 / 2)))

    constexpr auto i2 = fdiv_left(500, 50, 5, 2);   // i2 -> 1
    // (((500 / 50) / 5) / 2)
  }
*/

/*
  template <typename ...Args>
  constexpr auto fdiv_right(Args&&... args)
  {
    return (args / ...);  // unary right fold
  }

  template <typename ...Args>
  constexpr auto fdiv_left(Args&&... args)
  {
    return (... / args);  // unary left fold
  }

  int main()
  {
    // ------------------------------------------------
    
    // sending 1 argument is VALID

    constexpr auto i1 = fdiv_right(11);   // i1 -> 11
    constexpr auto i2 = fdiv_left(22);    // i2 -> 22

    // ------------------------------------------------

    // sending 0 argument is syntax error

    constexpr auto i3 = fdiv_right(); // syntax error
    // error: fold of empty expansion over operator/

    constexpr auto i4 = fdiv_left();  // syntax error
    // error: fold of empty expansion over operator/

    // ------------------------------------------------

    // when unary fold expression's binary operator
    // is one of those operators,

    // -  `&&` logical AND operator   --> true
    // -  `||` logical OR  operator   --> false
    // -  `,`  comma operator         --> void type

    // sending 0 argument is VALID

    // ------------------------------------------------
  }
*/

/*
  #include <utility>  // std::forward

  template <typename ...Args>
  constexpr auto sum_right(Args&&... args)
  {
    return (std::forward<Args>(args) + ...);
    // unary right fold
  }

  template <typename ...Args>
  constexpr auto sum_left(Args&&... args)
  {
    return (... + std::forward<Args>(args));
    // unary left fold
  }

  int main()
  {
    constexpr auto val1 = sum_right(12, 4.5, 55L); // val -> 71.5
    // (12 + (4.5 + 55))

    constexpr auto val2 = sum_left(12, 4.5, 55L);  // val -> 71.5
    // ((12 + 4.5) + 55)
  }
*/

/*
  #include <algorithm>  // std::count
  #include <iterator>   // std::begin, std::end
  #include <vector>
  #include <string>

  template <typename C, typename ...Ts>
  auto matches(const C& range, const Ts&... ts)
  {
    using namespace std;
    return (std::count(begin(range), end(range), ts) + ...); 
    // unary right fold
  }

  int main()
  {
    std::vector ivec{ 11, 22, 33, 44, 55, 66, 77, 88, 99 };
    std::cout << matches(ivec, 10, 11, 12, 20, 21, 22) << '\n';
    // output -> 2 
    // (1 - '11', 1 - '22')  --> (11, 22)

    std::string str{"hello world and universe"};
    std::cout << matches(str, 'e', 'f', 'i') << '\n';
    // output -> 4
    // (3 - 'e', 1 - 'i')   --> ('e', 'i', 'e', 'e')
  }
*/

/*
  template <int ...Vals>
  constexpr int Sum = (... + Vals); 
  // unary left fold  

  template <int ...Vals>
  constexpr int Sum_Square = (... + (Vals * Vals));
  // unary left fold

  int main()
  {
    std::cout << Sum<1> << '\n';                  // output -> 1
    // (1)

    std::cout << Sum<1, 2> << '\n';               // output -> 3
    // (1 + 2)

    std::cout << Sum<11, 33, 66> << '\n';         // output -> 110
    // ((11 + 33) + 66)

    std::cout << Sum_Square<1, 3, 5, 7> << '\n';  // output -> 84
    // (((1*1 + 3*3) + 5*5) + 7*7)
  }
*/

/*
  #include <type_traits>  // std::is_void

  auto fold_and_right(auto... args)
  {
    return (args && ...);
  }

  auto fold_or(auto... args)
  {
    return (args || ...);
  }

  auto fold_comma(auto... args)
  {
    return (args, ...);
  }

  int main()
  {
    std::cout << std::boolalpha;

    std::cout << fold_and() << '\n';  // output -> true
    std::cout << fold_or() << '\n';   // output -> false

    static_assert(std::is_void_v<decltype(fold_comma())>);  
    // VALID

    // Those results for empty parameter pack are same for,
    // unary left and unary right fold expressions.
  }
*/

/*
  #include <concepts> // std::convertible_to

  // concept constrained abbreviated template syntax
  constexpr auto func_and(std::convertible_to<bool> auto... args)
  {
    return (... && args); 
    // unary left fold
  }

  int main()
  {
    constexpr bool b1 = func_and(true, true, true, true);
    // b1 -> true

    constexpr bool b2 = func_and((10 > 5), (4 < 8), false, 5 == 5);
    // b2 -> false
  }
*/

/*
  #include <utility>  // std::forward

  template <typename T, typename ...Args>
  bool all_in_range(const T& min, const T& max, Args&&... args)
  {
    return (( min <= std::forward<Args>(args) 
              && max >= std::forward<Args>(args)) && ...);
    // unary right fold
  }

  int main()
  {
    std::boolalpha(std::cout);

    // ------------------------------------------------

    std::cout << all_in_range(1, 20, 2, 4, 6, 7, 9, 11) << '\n';
    // return (1 <= 2 && 20 >= 2) && 
              ((1 <= 4 && 20 >= 4) && 
              ((1 <= 6 && 20 >= 6) && 
              ((1 <= 7 && 20 >= 7) && 
              ((1 <= 9 && 20 >= 9) && 
              (1 <= 11 && 20 >= 11)))))
    // output -> true;

    // ------------------------------------------------
    std::cout << all_in_range(10, 20, 12, 15, 25, 17, 16) <<'\n';
    // return (10 <= 12 && 20 >= 12) && 
              ((10 <= 15 && 20 >= 15) && 
              ((10 <= 25 && 20 >= 25) && 
              ((10 <= 17 && 20 >= 17) && 
              (10 <= 16 && 20 >= 16))))
    // output -> false;

    // ------------------------------------------------
  }
*/

/*
  // "insert" member function of std::set is returning 
  // an <iterator, bool> pair.

  // when an element inserted to a set successfully,
  // iterator will point to the inserted element and
  // bool value will be true.

  // when an element is already in the set,
  // iterator will point to the element and
  // bool value will be false.

  #include <set>
  #include <utility>  // std::forward

  template <typename S, typename ...Args>
  bool func_insert(S& set_arg, Args&&... args)
  {
    return (set_arg.insert(std::forward<Args>(args)).second && ...);
    // unary right fold
  } 

  int main()
  {
    boolalpha(std::cout);

    std::set myset{ 33, 55, 99 };
    std::cout << "myset.size() = " << myset.size() << '\n';
    // output -> myset.size() = 3

    std::cout << func_insert(myset, 10, 20, 30) << '\n';
    // output -> true
    std::cout << "myset.size() = " << myset.size() << '\n';
    // output -> myset.size() = 6

    std::cout << func_insert(myset, 11, 30, 44) << '\n';
    // output -> false (30 is already in the set)
    std::cout << "myset.size() = " << myset.size() << '\n';
    // output -> myset.size() = 7
  }
*/

/*
  #include <string>
  #include <bitset>

  void print(const auto&... args)
  {
    ((std::cout << '|' << args << "| "), ...) << '\n';
    // unary right fold
  }

  int main()
  {
    std::string str{ "hello world" };
    std::bitset<16> bs{ 34525u };

    print(12, str, bs, "hello galaxy", 3.14);
    // output -> 
    //  |12| |hello world| |1000011011011101| |hello galaxy| |3.14|

    // print<int, std::string, std::bitset<16>, char[13], double>
    //                          (12, str, bs, "hello galaxy", 3.14);
  }
*/

/*
  #include <concepts> // std::same_as

  constexpr int sum(std::same_as<int> auto... args)
  {
    int sum{};

    ((sum += args), ...); // unary right fold
    return sum;
  }

  int main()
  {
    constexpr auto val = sum(1, 2, 3, 4);   // val -> 10
    // (sum += 1, ((sum += 2), ((sum += 3), (sum += 4))));
  }
*/

/*
  template <typename T>
  void func_2(T x)
  {
    std::cout << "bar(" << x << "), ";
  }

  template <typename ...Ts>
  void func_1(Ts... args)
  {
    (func_2(args), ...);
  }

  int main()
  {
    func_1(1, 2, 3, 4, 5);
    // output -> bar(1), bar(2), bar(3), bar(4), bar(5),
  }
*/

/*
  #include <array>
  #include <vector>
  #include <type_traits>  // std::is_pointer

  template <class T>
  concept Has_Iterator = requires (T a) {
    a.begin();
    a.end();
  };

  template <typename T>
  void func_impl(const T& x)
  {
    if constexpr(Has_Iterator<T>)
    {
      std::cout << "[";
      for (auto& elem : x)
        std::cout << elem << ", ";
      std::cout << "]";

      std::cout << '\n';
    }
    else if constexpr (std::is_pointer_v<T>)
      std::cout << *x << '\n';
    else
      std::cout << x << '\n';
  }

  void func_print(const auto&... args)
  {
    (func_impl(args), ...);   // unary right fold
    // (func_impl(p1), (func_impl(p2), (func_impl(p3)))
  }

  int main()
  {
    std::array arr{ 9, 8, 7, 6, 5 };
    std::vector dvec{ 3.4, 5.6, 7.8, 9.1 };
    int* p_ival{ new int{ 11 } };

    func_print(333, 4.444, arr, dvec, p_ival);
    // output ->
    //  333
    //  4.444
    //  [9, 8, 7, 6, 5, ]
    //  [3.4, 5.6, 7.8, 9.1, ]
    //  11

    delete p_ival;
  }
*/

/*
  #include <utility>  // std::forward

  template <typename First, typename ...Args>
  void fold_print(First&& f, Args&&... args)
  {
    std::cout << f;

    auto print_with_comma = [](const auto& v) {
      std::cout << ", " << v;
    };

    (... , print_with_comma(std::forward<Args>(args)));
    // unary left fold

    std::cout << '\n';
  }

  int main()
  {
    fold_print("hello", 11, 22, 33, "world");
    // output ->
    //  hello, 11, 22, 33, world
  }
*/

/*
  #include <vector>
  #include <utility>  // std::forward

  template <typename T, typename ...Args>
  void push_back_right(std::vector<T>& vec, Args&&... args)
  {
    (vec.push_back(std::forward<Args>(args)), ...);
    // unary right fold
  }

  template <typename T, typename ...Args>
  void push_back_left(std::vector<T>& vec, Args&&... args)
  {
    (... , vec.push_back(std::forward<Args>(args)));
    // unary left fold
  }

  int main()
  {
    std::vector<int> ivec{};

    push_back_left(ivec, 1, 2, 3, 4, 5);
    push_back_right(ivec, -1, -2, -3, -4, -5);

    for (auto elem : ivec)
      std::cout << elem << ' ';
    // output -> 1 2 3 4 5 -1 -2 -3 -4 -5
  }
*/

/*
  #include <vector>
  #include <utility>  // std::forward

  template <typename T, typename ...Ts>
  decltype(auto) func_push_back(std::vector<T>& vec, Ts&&... args)
  {
    (... , vec.push_back(std::forward<Ts>(args)));
    return (vec); // decltype(expression) -> std::vector<T>&
  }

  int main()
  {
    std::vector<int> ivec;

    func_push_back(ivec, 1, 2, 3, 4, 5).push_back(6);

    for (auto elem : ivec)
      std::cout << elem << ' ';
    // output -> 1 2 3 4 5 6
  }
*/

/*
  #include <concepts> // std::same_as

  template <typename T, std::same_as<T> ...Ts>
  constexpr auto func_min(const T& a, const T& b, const Ts&... ts)
  {
    auto m = a < b ? a : b;

    if constexpr (sizeof...(ts) > 0)
    {
      auto cmp = [&](const auto& value){
        if (value < m)
          m = value;
      };

      (... , cmp(ts));    // unary left fold
    }

    return m;
  }

  int main()
  {
    constexpr auto val = func_min(10, 5, 12, 9, 4, 5); // val -> 4
  }
*/

/*
                        ---------------
                        | binary fold |
                        ---------------
*/

/*
  #include <utility>  // std::forward
  #include <string>
  #include <bitset>

  template <typename ...Args>
  void func_print(Args&&... args)
  {
    (std::cout << ... << std::forward<Args>(args)) << '\n';
    // binary left fold
  }

  int main()
  {
    std::string str{"hello world"};

    func_print(121, str, std::bitset<8>(255u));
    // output -> 121hello world11111111
  }
*/

/*
  #include <concepts> // std::same_as

  template <std::same_as<int> ...Args>
  constexpr int func_subtract_left(int val, Args&&... args)
  {
    return (val - ... - args); // binary left fold
    // (((val - p1) - p2) - p3)
  }

  template <std::same_as<int> ...Args>
  constexpr int func_subtract_right(int val, Args&&... args)
  {
    return (args - ... - val); // binary right fold
    // (p1 - (p2 - (p3 - val)))
  }

  int main()
  {
    constexpr auto val = func_subtract_left(100, 10, 20, 30, 33);
    // val -> 7
    // (((100 - 10) - 20) - 30) - 33

    constexpr auto val2 = func_subtract_right(100, 10, 20, 30, 33);
    // val2 -> 87
    // 10 - (20 - (30 - (33 - 100)))
  }
*/

/*
  template <typename ...Args>
  constexpr auto sum(Args&&... args)
  {
    return (0 + ... + std::forward<Args>(args));  
    // binary left fold
  }

  int main()
  {
    constexpr auto x1 = sum(1, 2);  // x1 -> 3
    // ((0 + 1) + 2)

    constexpr auto x2 = sum(1);     // x2 -> 1
    // (0 + 1)

    constexpr auto x3 = sum();      // x3 -> 0
    // 0
  }
*/

/*
  template <typename T, typename ...Args>
  constexpr auto sum(const T& init, Args&&... args)
  {
    return (init + ... + std::forward<Args>(args));
    // binary left fold
  }

  int main()
  {
      constexpr auto x1 = sum(1, 2);  // x1 -> 3
      // ((0 + 1) + 2)

      constexpr auto x2 = sum(1);     // x2 -> 1
      // (0 + 1)

      constexpr auto x3 = sum();  // syntax error
      // error: no matching function for call to 'sum()'
      // note:   candidate expects at least 1 argument, 0 provided
  }
*/

/*
  #include <concepts> // std::same_as
  #include <utility>  // std::forward

  template <typename T>
  constexpr auto sum(const T& init, std::same_as<T>auto&&... args)
  {
    return (init + ... + std::forward<T>(args));
  }

  int main()
  {
    constexpr auto x1 = sum(1, 2);  // x1 -> 3
    // ((0 + 1) + 2)

    constexpr auto x2 = sum(1);     // x2 -> 1
    // (0 + 1)

    constexpr auto x3 = sum();  // syntax error
    // error: no matching function for call to 'sum()'
    // note:   candidate expects at least 1 argument, 0 provided
  }
*/

/*
  constexpr auto func_sum1(auto... args)
  {
    return (... + args);  
    // unary left fold
  }

  constexpr auto func_sum2(auto... args)
  {
    return (0 + ... + args);  
    // binary left fold
  }

  int main()
  {
    constexpr auto x1 = func_sum1();  // syntax error
    // error: fold of empty expansion over operator+

    constexpr auto x2 = func_sum2();  // VALID
  }
*/

/*
                ---------------------------
                | fold expressions idioms |
                ---------------------------
*/

/*
  // Jonathan Müller
  // parametre paketindeki ilk argümanı elde etmek.

  #include <type_traits>  // std::common_type

  template <typename ...Ts>
  constexpr auto get_first_element(Ts... args)
  {
    std::common_type_t<decltype(args)...> result;
    ((result = args, true) || ...); // unary right fold

    // ((result = arg1, true)) || ((result = arg2, true)) || ...  
    

    // "((result = arg1, true))", in this expression
    // "arg1" will be assign to "result", then because of comma operator 
    // is creating a sequence point and its value is the 
    // right operands value, expression's value will become true.
    // and because of the || logical OR operator,
    // short-circuit will end the evaluation
    // and result will be the first element in the parameter pack.

    return result;
  }

  int main()
  {
    using namespace std::literals;

    constexpr auto first_elem = get_first_element(11, 3.145);
    // first_elem -> 11.0
    
    constexpr auto first_elem2 = get_first_element(3.14, 55);
    // first_elem2 -> 3.14
  }
*/

/*
  // Jonathan Müller
  // parametre paketindeki son argümanı elde etmek.

  template <typename ...Ts>
  constexpr auto get_last_element(Ts... args)
  {
    return (args, ...);
  }

  int main()
  {
    using namespace std::literals;

    constexpr auto last_elem = get_last_element(11, 3.145, "hello");
    // last_elem -> "hello"(const char*)

    constexpr auto last_elem2 = get_last_element("world", 3.14, 55);
    // last_elem2 -> 55
  }
*/

/*
  // parametre paketindeki bütün argümanları bir fonksiyona göndermek.

  void func(int x)
  {
    std::cout << "func(int x), x = " << x << '\n';
  }

  void foo(auto... args)
  {
    (func(args), ...);
  }

  int main()
  {
    foo(1, 2, 3, 4, 5);
    // output -> 
    //  func(int x), x = 1
    //  func(int x), x = 2
    //  func(int x), x = 3
    //  func(int x), x = 4
    //  func(int x), x = 5
  }
*/

/*
  // Jonathan Müller
  // parametre paketindeki argümanları tersten sırayla 
  // bir fonksiyona göndermek.

  void func(int x)
  {
    std::cout << "func(int x), x = " << x << '\n';
  }

  void foo(auto... args)
  {
    int dummy;

    (dummy = ... = (func(args), 0)); 
    // binary left fold
    // (((dummy = (func(p1), 0)) = (func(p2), 0)) = (func(p3), 0))
    // assignment operator('=') is a right associative operator
  }

  int main()
  {
    foo(1, 2, 3, 4, 5);
    // output -> 
    //  func(int x), x = 5
    //  func(int x), x = 4
    //  func(int x), x = 3
    //  func(int x), x = 2
    //  func(int x), x = 1
  }
*/

/*
  // Jonathan Müller
  constexpr bool pred(int x)
  {
    return x % 3 == 0;
  }

  constexpr std::size_t pred_count(auto... args)
  {
    // return (std::size_t{} + ... + (pred(args) ? 1 : 0));
    return (std::size_t{} + ... + (static_cast<int>(pred(args)));
  }

  int main()
  {
    constexpr auto N = pred_count(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    // N -> 3
  }
*/

/*
  // Jonathan Müller
  constexpr auto fold_min(auto... args)
  {
    auto min = (args, ...);

    ((args < min ? min = args : 0), ...); 

    return min;
  }

  int main()
  {
    constexpr auto val = fold_min(3, 2, 7, 6, 11, 1, -2, 8);
    // val -> -2
  }
*/

/*
  // David Mazières (C++20 idioms for parameter packs)

  #include <tuple>    // std::apply
  #include <string>

  template <class ...Ts>
  struct multilambda : Ts... {
    using Ts::operator()...;
    // constexpr multilambda(Ts... ts) : Ts(std::move(ts))... {}
  };

  int main()
  {
    using namespace std;
    using namespace std::literals;

    constexpr multilambda action{
      [](int i){ cout << "int: " << i << '\n'; },
      [](double d){ cout << "double: " << d << '\n'; },
      [](bool b){ cout << (b ? "true" : "false") << '\n'; },
      [](string str){ cout << "string: " << str << '\n'; },
    };  // CTAD

    action(12);     // output -> int: 12
    action(3.14);   // output -> double: 3.14

    tuple tx(11, true, "hello"s, 3.14);

    std::apply([action](auto... args){ (action(args), ...); }, tx);

    // [action](auto... args){ (action(args), ...); } is callable

    // output ->
    //  int: 11
    //  true
    //  string: hello
    //  double: 3.14
  }
*/

/*
  // std::integer_sequence class template (C++14)

  #include <utility>  // std::integer_sequence

  template <typename T, T ...Vals>
  struct Integer_Sequence {
    static constexpr std::size_t size = sizeof...(Vals);
    using value_type = T;
  };

  int main()
  {
    // ------------------------------------------------

    std::integer_sequence<int, 1, 3, 5, 7> seq1;
    // std::integer_sequence<int, 1, 3, 5, 7> is a type.
    std::integer_sequence<long, 1, 2, 4> seq2;
    // std::integer_sequence<long, 1, 2, 4> is a type.

    // ------------------------------------------------

    std::integer_sequence<int, 1, 3, 5, 7>::size;
    std::integer_sequence<int, 1, 3, 5, 7>::value_type;

    // ------------------------------------------------
  }
*/

/*
  #include <utility>  // std::integer_sequence, std::index_sequence

  template <typename T, T ...Vals >
  void print_sequence(std::integer_sequence<T, Vals...> seq)
  {
    std::cout << "size of the sequence = " << seq.size() << '\n';
    ((std::cout << Vals << ' '), ...);  // unary left fold
    std::cout << '\n';
  }

  template <std::size_t ...Vals>
  using Index_Sequence = std::integer_sequence<std::size_t, Vals...>;

  int main()
  {
    // ------------------------------------------------

    print_sequence(std::integer_sequence<int, 1, 2, 3, 4, 5>());
    // output -> 
    //  size of the sequence = 5
    //  1 2 3 4 5

    print_sequence(std::integer_sequence<char, 'A', 'B', 'C'>());
    // output ->
    //  size of the sequence = 3
    //  A B C

    print_sequence(std::integer_sequence<std::size_t, 3, 5, 6>());
    // output -> 
    //  size of the sequence = 3
    //  3 5 6

    // ------------------------------------------------

    print_sequence(Index_Sequence<1, 2, 3, 4, 5>{});
    // output ->
    //  size of the sequence = 5
    //  1 2 3 4 5

    print_sequence(std::index_sequence<10, 20, 30, 40, 50>{});
    // output ->
    //  size of the sequence = 5
    //  10 20 30 40 50

    // ------------------------------------------------
  }
*/

/*
  // std::make_integer_sequence is a type alias 
  // NOT a class template

  #include <utility>      // std::make_integer_sequence
  #include <type_traits>  // std::is_same

  int main()
  {
    using namespace std;

    int N = 3;
    static_assert(is_same_v<make_integer_sequence<int, 3>, 
                            integer_sequence<int, 0, 1, 2>>);
  }
*/

/*
  // std::make_index_sequence is a type alias 
  // NOT a class template

  #include <utility>      // std::make_index_sequence
  #include <type_traits>  // std::is_same

  int main()
  {
    using namespace std;

    int N = 3;
    static_assert(is_same_v<make_index_sequence<3>, 
                            integer_sequence<size_t, 0, 1, 2>>);
    // first parameter variable is std::size_t     
  }
*/

/*
  #include <utility>  // std::make_integer_sequence

  template <typename T, T ...Vals >
  void print_sequence(std::integer_sequence<T, Vals...> seq)
  {
    std::cout << "size of the sequence = " << seq.size() << '\n';
    ((std::cout << Vals << ' '), ...);  // unary left fold
    std::cout << '\n';
  }

  constexpr int N = 10;

  int main()
  {
    // ------------------------------------------------

    print_sequence(std::make_integer_sequence<int, N>());
    // output ->
    //  size of the sequence = 10
    //  0 1 2 3 4 5 6 7 8 9

    // ------------------------------------------------

    print_sequence(std::make_integer_sequence<std::size_t, N>{});
    // output ->
    //  size of the sequence = 10
    //  0 1 2 3 4 5 6 7 8 9

    // ------------------------------------------------

    print_sequence(std::make_index_sequence<N>{});
    // output ->
    //  size of the sequence = 10
    //  0 1 2 3 4 5 6 7 8 9

    // ------------------------------------------------
  }
*/

/*
  #include <utility>  // std::index_sequence_for

  template <typename T, T ...Vals >
  void print_sequence(std::integer_sequence<T, Vals...> seq)
  {
    std::cout << "size of the sequence = " << seq.size() << '\n';
    ((std::cout << Vals << ' '), ...);  // unary left fold
    std::cout << '\n';
  }

  int main()
  {
    using namespace std;

    print_sequence(index_sequence_for<int, double, long>{});
    // std::make_index_sequence<3> 
    // output ->
    //  size of the sequence = 3
    //  0 1 2
  }
*/

/*
  // cppreference

  #include <tuple>  
  #include <utility>  
  // std::index_sequence, std::index_sequence_for

  template <class Tuple, std::size_t... Idxs>
  void print_tuple_impl(std::ostream& os, 
                        const Tuple& t, 
                        std::index_sequence<Idxs...>)
  {
    ((os << (Idxs == 0 ? "" : ", ") << std::get<Idxs>(t)), ...);
  }

  template <class... Args>
  auto& operator<<( std::ostream& os, 
                    const std::tuple<Args...>& t)
  {
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    // sending index's of the parameter pack
    return os << ")";
  }

  int main()
  {
    using namespace std;

    tuple<int, double, string> tp{ 11, 3.14, "hello"s };

    cout << tp << '\n';
    // output -> (11, 3.14, hello)

    cout << make_tuple(1, 2.14, 3.11, "world"s, 5) << '\n';
    // output -> (1, 2.14, 3.11, world, 5)
  }
*/

/*
  #include <array>
  #include <tuple>  
  #include <utility>  
  // std::index_sequence, std::index_sequence_for,
  // std::make_index_sequence
  #include <type_traits>  // std::is_same

  template <class Tuple, std::size_t... Idxs>
  void print_tuple_impl(std::ostream& os, 
                        const Tuple& t, 
                        std::index_sequence<Idxs...>)
  {
    ((os << (Idxs == 0 ? "" : ", ") << std::get<Idxs>(t)), ...);
  }

  template <class... Args>
  auto& operator<<( std::ostream& os, 
                    const std::tuple<Args...>& t)
  {
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
  }

  template <typename Array, std::size_t... Idxs>
  auto arr_to_tuple_impl(const Array& arr, 
                         std::index_sequence<Idxs...>)
  {
    return std::make_tuple(arr[Idxs]...);
  }

  template <typename T, 
            std::size_t N, 
            typename Indices = std::make_index_sequence<N>>
  auto arr_to_tuple(const std::array<T, N>& arr)
  {
    return arr_to_tuple_impl(arr, Indices{});
  }

  int main()
  {
    std::array<int, 4> iarr{ 11, 22, 33, 44 };

    auto tx = arr_to_tuple(iarr);
    static_assert(std::is_same_v<
                    decltype(tx), std::tuple<int, int, int, int>>);

    std::cout << tx << '\n';
    // output -> (11, 22, 33, 44)
  }
*/

/*
  #include <tuple>    // std::tuple_size
  #include <utility>  // std::make_index_sequence

  template <typename ...Ts>
  void func(Ts&&... args)
  {
    // code here...
  }

  template <typename Tuple, std::size_t... Idxs>
  void process_impl(const Tuple& tp, std::index_sequence<Idxs...>)
  {
    func(std::get<Idxs>(tp)...);
  }

  template <typename Tuple>
  void process(const Tuple& tp)
  {
    using namespace std;

    process_impl( tp, make_index_sequence<tuple_size_v<Tuple>>{});
  }
*/

/*
  #include <tuple>  // std::decay, std::tuple_size, std::get
  #include <utility>  
  // std::forward, std::make_index_sequence

  namespace detail {
    template <class Fn, class Tuple, std::size_t... Idxs>
    decltype(auto) Apply_impl(Fn&& fn, 
                              Tuple&& t, 
                              std::index_sequence<Idxs...>)
    {
      using namespace std;

      return forward<Fn>(fn)(get<Idxs>(forward<Tuple>(t))...);
    }
  }

  template <class Fn, class Tuple>
  decltype(auto) Apply(Fn&& fn, Tuple&& t)
  {
    using namespace std;

    return detail::Apply_impl( 
      std::forward<Fn>(fn), 
      std::forward<Tuple>(t), 
      make_index_sequence<tuple_size_v<decay_t<Tuple>>>{});
  }

  // for function's return value's value category to be  
  // remain same `decltype(auto)` is used.

  int func(int x, char c, double d)
  {
    std::cout << "x = " << x 
              << ", c = " << c 
              << ", d = " << d << '\n';
    return 1;            
  }

  int main()
  {
    auto tp = std::make_tuple(11, 'A', 3.14);

    int ret = Apply(func, tp);
    // output -> x = 11, c = A, d = 3.14
  }
*/

/*
  #include <utility>  // std::index_sequence
  #include <array>
  #include <string>

  namespace detail {
    template <typename T, std::size_t... Is>
    auto Make_Array_Impl(const T& val, std::index_sequence<Is...>)
    {
      return std::array<T, sizeof...(Is)>{ (Is, val)... };
      // {(0, "hello"), (1, "hello"), ... (N-1, "hello") }
    }
  }

  template <int N, typename T>
  std::array<T, N> Make_Array(const T& val)
  {
    return detail::Make_Array_Impl(val, std::make_index_sequence<N>());
  }


  int main()
  {
    // ------------------------------------------------

    int i_arr[5] = { (0, 3), (1, 3), (2, 3), (3, 3), (4, 3) };

    for (const auto& elem : i_arr)
      std::cout << elem << ' ';
    // output -> 3 3 3 3 3

    // ------------------------------------------------
  
    auto str_arr = Make_Array<5>(std::string("hello"));

    for (const auto& elem : str_arr)
      std::cout << elem << ' '; 
    // output -> hello hello hello hello hello

    std::cout << '\n';

    // ------------------------------------------------
  }
*/

/*
  #include <type_traits>  // std::is_same
  #include <tuple>        // std::make_tuple, std::get
  #include <utility>      // std::index_sequence

  template <typename Tuple, std::size_t... Idxs>
  decltype(auto) Tuple_Subset(const Tuple& tp, 
                              std::index_sequence<Idxs...>)
  {
    return std::make_tuple(std::get<Idxs>(tp)...);
  }

  int main()
  {
    std::tuple tp1(11, 3.14, "hello", true, 'A');
    auto tp2 = Tuple_Subset(tp1, std::index_sequence<1, 3, 4>{});

    static_assert(std::is_same_v<decltype(tp2), 
                                std::tuple<double, bool, char>>);
    // VALID

    std::cout << std::get<0>(tp2) << ' ' 
              << std::get<1>(tp2) << ' ' 
              << std::get<2>(tp2) << '\n';
    // output -> 3.14 1 A
  }
*/

/*
                    ---------------------
                    | static if (C++17) |
                    ---------------------
*/

/*
  if constexpr (`constant expression`) 
  {
    // code here...
  }
*/

/*
  #include <type_traits>  // std::is_integral

  template <typename T>
  void func(T tx)
  {
    if constexpr (std::is_integral_v<T>){
      if (tx != 0)
        func(tx--);
    }
    else{
      undeclared(); // syntax error
      // error: there are no arguments to 'undeclared' 
      // that depend on a template parameter, 
      // so a declaration of 'undeclared' must be available
    }

  }

  int main()
  {
    func(2);
    // compiler will not generate code for else block
    // but because of "undeclared" identifier is not dependent
    // on template parameter, there will be a syntax error.
  }
*/

/*
  #include <type_traits>  // std::is_integral

  template <typename T>
  void func(T tx)
  {
    if constexpr (std::is_integral_v<T>){
      if (tx != 0)
        func(tx--);
    }
    else{
      tx(4);
      undeclared(tx);
    }

  }

  int main()
  {
    func(2);  // VALID
    // compiler will not generate code for else block
    // so there won't be a syntax error for "undeclared"
    // identifier because of it is dependent on template parameter.
  }
*/

/*
  #include <type_traits>  // std::is_integral

  template <typename T>
  void func(T tx)
  {
    if constexpr (std::is_integral_v<T>){
      if (tx != 0)
        func(tx--);
    }
    else {
      tx(4);          // syntax error
      // error: 'tx' cannot be used as a function
      
      undeclared(tx); // syntax error
      // error: 'undeclared' was not declared in this scope
    }
  }

  int main()
  {
    func(2.5);
    // compiler will generate the else block's code 
    // so both statements will cause a syntax error.
  }
*/

/*
  #include <type_traits>  // std::is_integral

  template <typename T>
  void func(T tx)
  {
    if constexpr (std::is_integral_v<T>){
      if (tx != 0)
        func(tx--);
    }
    else {
      static_assert(false, "T must be an integral type");
    }
  }

  int main()
  {
    func(2);    // VALID

    func(2.5);  // syntax error
    // error: static assertion failed: T must be an integral type
  }
*/

/*
  struct AStruct {};

  int main()
  {
    AStruct as;

    if constexpr (sizeof(int) > 4){
      ++as; // syntax error
      // error: no match for 'operator++' (operand type is 'AStruct')
    }
    else{
      --as; // syntax error
      // error: no match for 'operator--' (operand type is 'AStruct')
    }

    // because of static if is not inside a template 
    // both branches will be compiled and syntax error will occur.
  }
*/

/*
  struct AStruct {
    AStruct& operator++() { return *this; }
  };

  int main()
  {
    AStruct as;

    if constexpr (sizeof(int) > 2){
      ++as;
    }
    else{
      --as; // syntax error
      // error: no match for 'operator--' (operand type is 'AStruct')
    }

    // because of static if is not inside a template 
    // both branches will be compiled and syntax error will occur.
  }
*/

/*
  struct AStruct {
    AStruct& operator++() { return *this; }
  };

  template <typename T>
  void func(T t)
  {
    if constexpr (sizeof(int) > 2){
      ++t;
    }
    else{
      --t;
    }
  }

  int main()
  {
    func(AStruct{});  // VALID

    // because of if constexpr block is compiled
    // and there is operator++ for AStruct
    // there won't be a syntax error.
  }
*/

/*
  struct AStruct {
    AStruct& operator++() { return *this; }
  };

  template <typename T>
  void func(T t)
  {
    if constexpr (sizeof(int) > 8){
      ++t;
    }
    else{
      --t; // ---->
    }
  }

  int main()
  {
    func(AStruct{});  // syntax error
    // error: no match for 'operator--' (operand type is 'AStruct')

    // because of else block is compiled
    // and there is no operator-- for AStruct
    // there will be a syntax error.
  }
*/

/*
  #include <type_traits>  // std::is_integral

  template <typename T>
  void func(T& tx)
  {
    if (tx > 0){  // run-time if
      if constexpr (std::is_integral_v<T>){ // compile-time if
        ++tx;
      }
      else{
        --tx;
      }
    }
  }

  int main()
  {
    int ival = 11;
    double dval = 3.14;

    func(ival);
    func(dval);

    std::cout << "ival = " << ival << '\n';
    // output -> ival = 12
    std::cout << "dval = " << dval << '\n';
    // output -> dval = 2.14
  }
*/

/*
  #include <type_traits>  // std::is_void

  constexpr auto func_1()
  {
    if constexpr (sizeof(int) > 4u)
      return 11;
    else
      return 3.14;
  }
  // compiler written "func_1" have a return type of int 
  // when sizeof(int) > 4u is true
  // otherwise return type is double

  auto func_2()
  {
    if constexpr (sizeof(int) > 4u)
      return 11;
  }
  // compiler written "func_2" have a return type of int
  // when sizeof(int) > 4u is true
  // otherwise return type is void

  int main()
  {
    constexpr auto val = func_1();  // val -> 3.14
    static_assert(std::is_void_v<decltype(func_2())>);  // VALID
  }
*/

/*
  #include <type_traits>  // std::is_pointer

  template <typename T>
  auto get_val(T t)
  {
    if constexpr (std::is_pointer_v<T>)
      return *t;
    else
      return t;
  }

  int main()
  {
    int ival{ 88 };
    double dval{ 3.14 };
    int* iptr = &ival;
    double* dptr = &dval;

    std::cout << get_val(ival) << '\n';
    // output -> 88
    std::cout << get_val(dval) << '\n';
    // output -> 3.14
    std::cout << get_val(iptr) << '\n';
    // output -> 88
    std::cout << get_val(dptr) << '\n';
    // output -> 3.14
  }
*/

/*
  #include <type_traits>  // std::is_same, std::is_arithmetic
  #include <string>       // std::to_string

  template <typename T>
  std::string as_string(T x)
  {
    if constexpr (std::is_same_v<T, std::string>)
      return x;
    else if constexpr (std::is_arithmetic_v<T>)
      return std::to_string(x);
    else
      return std::string(x);
  }

  class Myclass {};

  int main()
  {
    std::cout << as_string(11) << '\n';
    // output -> 11
    std::cout << as_string(3.14) << '\n';
    // output -> 3.14
    std::cout << as_string(std::string("hello")) << '\n';
    // output -> hello
    std::cout << as_string("world") << '\n';
    // output -> world

    std::cout << as_string(Myclass{}) << '\n';  // syntax error
    // error: no matching function for call to 
    // 'std::__cxx11::basic_string<char>::basic_string(Myclass&)'
  }
*/

/*
  // using tag-dispatch technique for std::advance

  #include <iterator>  
  // std::random_access_iterator_tag
  // std::bidirectional_iterator_tag
  // std::input_iterator_tag
  // std::iterator_traits
  #include <vector>
  #include <list>

  namespace details {

    // implementation for random access iterators
    template <typename Iter, typename Dist>
    void Advance_impl(Iter& pos, 
                      Dist N, 
                      std::random_access_iterator_tag)
    {
      std::cout << "random access iterator\n";
      pos += N;
    }

    // implementation for bidirectional iterators
    template <typename Iter, typename Dist>
    void Advance_impl(Iter& pos, 
                      Dist N, 
                      std::bidirectional_iterator_tag)
    {
      std::cout << "bidirectional iterator\n";
      if (N >= 0)
        while (N--)
          ++pos;
      else
        while (N++)
          --pos;
    }

    // implementation for input iterators
    template <typename Iter, typename Dist>
    void Advance_impl(Iter& pos, 
                      Dist N, 
                      std::input_iterator_tag)
    {
      std::cout << "input iterator\n";
      while (N--)
        ++pos;
    }
  }

  template <typename Iter, typename Dist>
  void Advance(Iter& pos, Dist N)
  {
    using namespace std;

    using Cat = typename iterator_traits<Iter>::iterator_category;
    details::Advance_impl(pos, N, Cat{});
  }

  int main()
  {
    // ------------------------------------------------

    std::vector<int> ivec{ 1, 2, 3, 4, 5 };
    auto vec_iter = ivec.begin();
    Advance(vec_iter, 3);
    // output -> random access iterator
    std::cout << *vec_iter << '\n';
    // output -> 4

    // ------------------------------------------------

    std::list<int> ilist{ 11, 22, 33, 44, 55 };
    auto list_iter = ilist.begin();
    Advance(list_iter, 3);
    // output -> bidirectional iterator
    std::cout << *list_iter << '\n';
    // output -> 44

    // ------------------------------------------------
  }
*/

/*
  // using static if for std::advance 

  #include <type_traits>  // std::is_same
  #include <iterator>     
  // std::random_access_iterator_tag
  // std::bidirectional_iterator_tag
  // std::input_iterator_tag
  #include <vector>
  #include <list>

  template <typename Iter, typename Dist>
  void Advance(Iter& pos, Dist N)
  {
    using namespace std;

    using Cat = typename iterator_traits<Iter>::iterator_category;

    if constexpr (is_same_v< Cat, random_access_iterator_tag>)
    {
      cout << "random access iterator\n";
      pos += N;
    }
    else if constexpr (is_same_v<Cat, bidirectional_iterator_tag>)
    {
      cout << "bidirectional iterator\n";

      if (N >= 0)
        while (N--)
          ++pos;
      else
        while (N++)
          --pos;
    }
    else // std::input_iterator_tag
    {
      cout << "input iterator\n";

      while (N--)
        ++pos;
    } 
  }

  int main()
  {
    // ------------------------------------------------

    std::vector<int> ivec{ 1, 2, 3, 4, 5 };
    auto vec_iter = ivec.begin();
    Advance(vec_iter, 3);
    // output -> random access iterator
    std::cout << *vec_iter << '\n';
    // output -> 4

    // ------------------------------------------------

    std::list<int> ilist{ 11, 22, 33, 44, 55 };
    auto list_iter = ilist.begin();
    Advance(list_iter, 4);
    // output -> bidirectional iterator
    std::cout << *list_iter << '\n';
    // output -> 55

    // ------------------------------------------------
  }
*/

/*
  template <int N>
  constexpr int fibonacci()
  {
    return fibonacci<N - 1>() + fibonacci<N - 2>();
  }

  template <>
  constexpr int fibonacci<1>() { return 1; }

  template <>
  constexpr int fibonacci<0>() { return 0; }

  int main()
  {
    constexpr auto x = fibonacci<10>();   // x -> 55
  }
*/

/*
  // fibonacci with static if

  template <int N>
  constexpr int fibonacci()
  {
    if constexpr (N >= 2)
      return fibonacci<N - 1>() + fibonacci<N - 2>();
    else
      return N;
  }

  int main()
  {
    constexpr auto x = fibonacci<10>();   // x -> 55
  }
*/

/*
  #include <string>

  template <typename T>
  std::string to_str(T t)
  {
    return std::to_string(t);
  }

  std::string to_str(const std::string& str)
  {
    return str;
  }

  std::string to_str(const char* c_str)
  {
    return c_str;
  }

  std::string to_str(bool b)
  {
    return b ? "true" : "false";
  }

  int main()
  {
    std::cout << to_str("hello") << '\n';
    std::cout << to_str(std::string{ "world" }) << '\n';
    std::cout << to_str(13) << '\n';
    std::cout << to_str(3.14) << '\n';
    std::cout << to_str(true) << '\n';

    // output ->
    //  hello
    //  world
    //  13
    //  3.140000
    //  true
  }
*/

/*
  // using static if for to_str

  #include <type_traits>  // std::is_same, std::is_convertible
  #include <string>       // std::to_string

  template <typename T>
  std::string to_str(T t)
  {
    if constexpr (std::is_convertible_v<T, std::string>)
      return t;
    else if constexpr (std::is_same_v<T, bool>)
      return t ? "true" : "false";
    else 
      return std::to_string(t);
  }

  int main()
  {
    std::cout << to_str("hello") << '\n';
    std::cout << to_str(std::string{ "world" }) << '\n';
    std::cout << to_str(13) << '\n';
    std::cout << to_str(3.14) << '\n';
    std::cout << to_str(true) << '\n';

    // output ->
    //  hello
    //  world
    //  13
    //  3.140000
    //  true
  }
*/

/*
  template <typename T, typename ...Ts>
  void print(const T& x, const Ts&... args)
  {
    if constexpr (sizeof...(args) == 0)
      std::cout << x << '\n';
    else
      std::cout << x << ", ";

    if constexpr (sizeof...(args) != 0)
      print(args...);
  }

  int main()
  {
    print(12, 3.14, "hello", true, 'A');
    // output -> 12, 3.14, hello, 1, A
  }
*/

/*
  // is_trivially_copyable_v is a compile-time check
  // that a type can be copied with std::memcpy(byte by byte)

  #include <cstring>      // std::memcpy
  #include <type_traits>  // std::is_trivially_copyable
  #include <algorithm>    // std::copy

  template <typename T, std::size_t N>
  void copy_array(T(&dest)[N], const T(&src)[N])
  {
    if constexpr (std::is_trivially_copyable_v<T>)
      std::memcpy(dest, src, N * sizeof(T));
    else
      std::copy(src, std::end(source), dest);
  }
*/

/*
  #include <string>
  #include <vector>

  // --------------------------------------------------

  class AClass {
  private:
    int m_i;
    double m_d;
    std::string m_str;
    std::vector<int> m_ivec;
  public:
    template <std::size_t N>
    auto get();
  };

  template <>
  auto AClass::get<0>(){ return m_i; }

  template <>
  auto AClass::get<1>(){ return m_d; }

  template <>
  auto AClass::get<2>(){ return m_str; }

  template <>
  auto AClass::get<3>(){ return m_ivec; }

  // --------------------------------------------------

  // static if alternative
  class BClass {
  private:
    int m_i;
    double m_d;
    std::string m_str;
    std::vector<int> m_ivec;
  public:
    template <std::size_t N>
    auto get() 
    {
      if constexpr (N == 0) return m_i;
      else if constexpr (N == 1)  return m_d;
      else if constexpr (N == 2)  return m_str;
      else if constexpr (N == 3)  return m_ivec;
    }
  };

  // --------------------------------------------------
*/

/*
  #include <vector>

  template <typename T>
  void func(std::vector<T>& vec, T&& elem);

  int main()
  {
    using namespace std;

    vector<int> ivec;
    int x{ 11 };

    // ------------------------------------------------

    func(ivec, 12); // VALID
    // T is deduced as int for both function parameter

    // ------------------------------------------------

    func(ivec, x);  // syntax error
    // error: no matching function for call to 
    // 'func(std::vector<int>&, int&)
    // note: deduced conflicting types for parameter 'T' 
    // ('int' and 'int&')

    // T will be deduce as int for the 1st function parameter
    // T will be deduce as int& for the 2nd function parameter

    // ------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <string>

  template <typename T>
  void func(std::vector<T>, T);

  int main()
  {
    // ------------------------------------------------

    std::vector<int> ivec{};
    func(ivec, 11); // VALID

    // ------------------------------------------------

    std::vector<std::string> svec{};
    func(svec, "hello world"); // syntax error

    // error: no matching function for call to 
    // 'func(std::vector<std::basic_string<char>>&, const char [12])'

    // T will be deduce to std::string for the 1st function parameter
    // T will be deduce to const char* for the 2nd function parameter

    // ------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <string>
  #include <type_traits>  // std::type_identity(metafunction)

  // ------------------------------------------------------

  template <typename T>
  struct Type_Identity {
    using type = T;
  };

  template <typename T>
  using Type_Identity_t = typename Type_Identity<T>::type;

  template <typename T>
  void func(std::vector<T>, Type_Identity_t<T>);

  // ------------------------------------------------------

  template <typename T>
  void func_2(std::vector<T>, std::type_identity_t<T>);

  int main()
  {
    std::vector<int> ivec{};
    func(ivec, 11); // VALID

    std::vector<std::string> svec{};
    func(svec, "hello world");  // VALID
    // template argument deduction is disabled for the 2nd parameter
  }
*/

/*
                  -----------------------------
                  | static_assert declaration |
                  -----------------------------
*/

/*
  #include <type_traits>  
  // std::is_integral, std::is_floating_point

  template <typename T>
  void foo(T x)
  {
    if constexpr (std::is_integral_v<T>)
      std::cout << "integral type\n";
    else if constexpr (std::is_floating_point_v<T>)
      std::cout << "floating point type\n";
    else
      static_assert(false);
  }

  class Myclass{};

  int main()
  {
    foo("hello");
    // error: static assertion failed: 
    // integral or floating point type needed

    foo(Myclass{});
    // error: static assertion failed: 
    // integral or floating point type needed
  }
*/

/*
  #include <type_traits>  
  // std::is_integral, std::is_floating_point

  template <typename T>
  void foo(T x)
  {
    if constexpr (std::is_integral_v<T>)
      std::cout << "integral type\n";
    else if constexpr (std::is_floating_point_v<T>)
      std::cout << "floating point type\n";
    else
      static_assert(sizeof(T) != sizeof(T));
  }

  class Myclass{};

  int main()
  {
    foo("hello");
    // error: static assertion failed: 

    foo(Myclass{});
    // error: static assertion failed: 
  }
*/

/*
  #include <type_traits>  // std::false_type

  template <typename T>
  struct always_false : std::false_type{};

  template <typename T>
  constexpr bool always_false_v = always_false<T>::value;

  template <typename T>
  void foo(T x)
  {
    if constexpr (std::is_integral_v<T>)
      std::cout << "integral type\n";
    else if constexpr (std::is_floating_point_v<T>)
      std::cout << "floating point type\n";
    else
      static_assert(always_false_v<T>);
  }

  class Myclass{};

  int main()
  {
    foo("hello");
    // error: static assertion failed

    foo(Myclass{});
    // error: static assertion failed
  }
*/

/*
  template <typename ...>
  constexpr bool always_false() { return false; }

  template <typename T>
  void foo(T x)
  {
    if constexpr (std::is_integral_v<T>)
      std::cout << "integral type\n";
    else if constexpr (std::is_floating_point_v<T>)
      std::cout << "floating point type\n";
    else
      static_assert(always_false<T>());
  }

  class Myclass{};

  int main()
  {
    foo("hello");
    // error: static assertion failed
    // note: 'always_false<const char*>()' evaluates to false

    foo(Myclass{});
    // error: static assertion failed
    // note: 'always_false<Myclass>()' evaluates to false
  }
*/

/*
                  --------------------------
                  | template instantiation |
                  --------------------------
*/

/*
  template <typename T>
  class Myclass {};

  template <typename T>
  void foo(T);

  int main()
  {
    Myclass<int> x; // Myclass<int> specialization
    foo(1.34);      // foo<double> specialization
  }
*/

/*
  template <typename T>
  void foo(T x)
  {
    // ------------------------------------------------

    func_1(x);    // VALID
    // "func_1" function's parameter is dependent on 
    // template parameter(T)

    // two phase name lookup
    // 1. unqualified lookup for "func_1" identifier
    // 2. ADL for "func_1" identifier

    // because of there is a chance that "func_1" identifier
    // can be found with ADL because "func_1" parameter 
    // variable is dependent on template parameter,
    // it will not be a syntax error.

    // ------------------------------------------------

    func_2(12);   // syntax error
    // error: there are no arguments to 'func_2' 
    // that depend on a template parameter, 
    // so a declaration of 'func_2' must be available 

    // one phase name lookup
    // 1. unqualified lookup for "func_2" identifier

    // because of "func_2" parameter is not dependent on
    // template parameter, there won't be a chance to find
    // "func_2" identifier with ADL. 
    // and because of there is no "func_2" visible identifier
    // it will be a syntax error.

    // ------------------------------------------------
  }
*/

/*
  template <typename T>
  void foo(T x)
  {
    func_1(x);
  }

  void func_1(int){}

  int main()
  {
    foo(11);  // syntax error
    // error: 'func_1' was not declared in this scope, 
    // and no declarations were found by argument-dependent lookup 
    // at the point of instantiation 

    // 2 phase name lookup
    // unqualified lookup for "func_1" identifier   : NOT FOUND
    // ADL for "func_1" identifier                  : NOT FOUND
    // so will be syntax error
  }
*/ 

/*
  class Myclass{};

  template <typename T>
  void foo(T x)
  {
    func_1(x);
  }

  void func_1(Myclass){}

  int main()
  {
    foo(Myclass{}); // VALID

    // 2 phase name lookup
    // unqualified lookup for "func_1" identifier   : NOT FOUND
    // ADL for "func_1" identifier                  : FOUND

    // because Myclass definition is in global namespace scope
    // whole global namespace scope will be searched and "func_1"
    // identifier will be found. NO syntax error
  }
*/

/*
  class Myclass {};

  template <typename T>
  void foo(T x)
  {
    func_1(x);
  }

  int main()
  {
    foo(Myclass{}); // VALID

    // 2 phase name lookup
    // unqualified lookup for "func_1" identifier   : NOT FOUND
    // ADL for "func_1" identifier                  : FOUND

    // because Myclass definition is in global namespace scope
    // whole global namespace scope will be searched and "func_1"
    // identifier will be found. NO syntax error
  }

  void func_1(Myclass){}
*/

/*
  ----------------------------------------------------

  - derleyici, ismin bir template ismi olduğunu anladığı zaman 
    belirli bir template argümanı setiye, bir template ID 
    oluşturmak zorunda.

  ----------------------------------------------------

  template ID : 
    template parametrelerine karşılık, template argümanlarının 
    türlerinin belirlenmesi.

  "foo" bir function template ismi, foo<int, int> bir template ID.
  "Myclass" bir class template ismi, Myclass<int> bir template ID.

  ----------------------------------------------------

  derleyicinin template parametrelerine karşılık gelecek,
  template argümanlarının türlerini bilmesi gerekiyor.

  1. template argümanları explicit olarak belirlenebilir.
  2. default template argümanı kullanıldığı durumda, 
     template argümanları belirlenmiş olur.
  3. fonksiyon çağrılarında, gönderilen argümanlardan 
    template argument deduction yapılır.

  ----------------------------------------------------

  - template ID'si ile template instantiation aynı şey değil.

  ----------------------------------------------------

  - template instantiation öncesinde template substitution gerçekleşir.

  - substitution : örneğin bir fonksiyon şablonu(function template) 
    ise fonksiyonun parametrik yapısı ve geri dönüş türünün 
    belirlenmesi. 
    (substitution : yerleştirme, yerine koyma)

  - template substitution sonucunda, oluşan fonksiyon  
    function overload resolution'a girecek.
  
  - substitution sonucunda geçersiz bir durum oluştursa
    (bir türün olmaması gibi) derleyici direkt sentak hatası 
    vermek yerine, fonksiyonu overload setten çıkartır.
    (SFINAE - Substitution Failure Is Not An Error)
    Eğer overload sette başka bir aday bulunmuyorsa 
    sentaks hatası olur.
  
  ----------------------------------------------------
*/

/*
  template <typename T>
  void func();

  int main()
  {
    func<double>();   // explicit template argument
    // the signature will join the overload resolution 
    // will be generated by that line
  }
*/

/*
  template <typename T = int>
  void func();

  int main()
  {
    func();
    // default template argument will be used when template ID 
    // is being generated
  }
*/

/*
  template <typename T>
  typename T::value_type func(T);

  int main()
  {
    func(12); // syntax error
    // error: no matching function for call to 'func(int)'
    // error: 'int' is not a class, struct, or union type

    // ---------------------------------------------------------

    // 1. name lookup for "func" identifier  : FOUND 

    // 2. T will be deduce to int with template argument deduction 

    // 3. in substitution phase, because of function's return 
    // type is int::value_type which is not a valid type
    // "func" function template will be removed from overload set
    // and because of there is no other viable function in overload set
    // there will be a syntax error.

    // ---------------------------------------------------------
  }
*/

/*
  // substitution failure is not an error (SFINAE)

  template <typename T>
  typename T::value_type func(T){}

  void func(...){}

  int main()
  {
    func(12); // VALID 

    // ---------------------------------------------------------

    // 1. name lookup for "func" identifier  : FOUND (2 viable)

    // 2. T will be deduce to int with template argument deduction
    //    in "func" function template 

    // 3. in substitution phase, because of function template's 
    // return type is int::value_type which is not a valid type
    // "func" function template will be removed from overload set
    // and there is another viable function in overload set
    // so there won't be a syntax error.

    // ---------------------------------------------------------
  }
*/

/*
  - belirli bir template specialization'ın instantiate edilmesini
    derleyiciye verilen talimatla
    (explicit template instantiation directive) sağlayabiliriz.


  - bir başlık dosyasına template'lerin tanımları yerleştirildi  
    ve bu başlık dosyası birden fazla kaynak dosyası tarafından
    include edildi. Link zamanında ODR(One Definition Rule) 
    sebebi ile bir hata alınmaz fakat her translation unit 
    derleyici tarafından derlendiğinden, aynı template
    tanımı her başlık dosyası için derlenecek, bu da derleme
    süresinde artışına sebep olacaktır.
    Header only library'lerde bu durum bir dezavantajdır.
*/

/*
  - Hangi durumlarda instantiation gerekiyor, hangi durumlarda
    gerekmiyor?
*/

/*
  template <typename T>
  struct AStruct {
    void func_1(){}
    void func_2(){}
    void func_3(){}
  };

  int main()
  {
    // ------------------------------------------------

    AStruct<int>* p_as{};   // no instantiation
    // compiler does not need to instantiate 
    // AStruct<int> specialization because 
    // only a pointer variable is created.

    // ------------------------------------------------

    AStruct<int> as;        // instantiation
    // derleyici sınıf tanımını oluşturmak zorunda,
    // fakat fonksiyonlar henüz çağrılmadıkları için 
    // fonksiyonlar için herhangi bir kod yazmayacak.

    // ------------------------------------------------

    as.func_1(); 
    // AStruct<int> specialization'ı için 
    // "func_1" fonksiyonu için kod oluşturulacak.

    // ------------------------------------------------

    // fonksiyonlar için geçerli olan bu durum sınıfın 
    // statik veri elemanları içinde geçerlidir.

    // ------------------------------------------------
  }
*/

/*
  // when a class template is instantiated, 
  // its static data members should be instantiated as well.
  // because it will become alive before "main" function is invoked.

  template <typename T>
  struct AStruct {
    static T ms_x;
  };

  template <typename T>
  T AStruct<T>::ms_x = 0;

  int main()
  {
    AStruct<int> a1;          
    // instantiation of AStruct<int> specialization
    AStruct<double> a2;
    // instantiation of AStruct<double> specialization
    AStruct<double> a3;       

    // ------------------------------------------------

    std::cout << a1.ms_x << '\n';   // output -> 0
    std::cout << a2.ms_x << '\n';   // output -> 0
    std::cout << a3.ms_x << '\n';   // output -> 0

    // ------------------------------------------------

    a2.ms_x = 22;

    std::cout << a1.ms_x << '\n';   // output -> 0
    std::cout << a2.ms_x << '\n';   // output -> 22
    std::cout << a3.ms_x << '\n';   // output -> 22

    // ------------------------------------------------
  }
*/

/*
  template <typename T> 
  class Myclass;  
  // declaration only no instantiation

  Myclass<int>* p = 0;
  // definition of Myclass<int> specialization is not needed

  template <typename T>
  class Myclass {
  public:
    void func();  // member function declaration
  };

  // class template definition is completed in this line


  // parameter variable mx, uses class template declaration only
  void global_func(Myclass<int>& mx)
  {
    mx.func();  
    // for calling mx variable's member function 
    // class template definition is needed
    // Myclass<int>::func() member function definition is needed
    // which is below this function("global_func").

    Myclass<void>* p_mx = new Myclass<void>;
    // for dynamic storage duration object 
    // Myclass<void> class template definition is needed
  }

  template <typename T>
  void Myclass<T>::func() 
  {
    // member function definition
  }
*/

/*
  template <typename T>
  struct AStruct {
    using type = typename T::value_type;
  };

  int main()
  {
    // ------------------------------------------------

    AStruct<int>* p_as{};   
    // no instantiation because no need for class definition
    // because of no instantiation, 
    // no syntax error for because int::value_type 

    // ------------------------------------------------

    AStruct<double> a1; // syntax error
    // error: 'double' is not a class, struct, or union type

    // for "a1" variable to be created,
    // AStruct<double> specialization should be instantiated
    // in AStruct<double> specialization's instantiation
    // because of T::value_type is not a valid type
    // there will be a syntax error.

    // ------------------------------------------------
  }
*/

/*
  template <typename T>
  T func(T x)
  {
    return x * x;
  }

  template <typename T>
  T func_2(T x)
  {
    using type = typename T::value_type;
    return x * x;
  }

  int main()
  {
    // ------------------------------------------------

    decltype(func(12)) x = 5; 
    // partial instantiation

    // for compiler to deduce x's type it needs to know 
    // "func(12)" expression's type
    // to know "func(12)" expression's type it needs to know 
    // T's type in function template "func"
    // because of it can understand T's type from function argument
    // and function's return type  
    // it will not need to instantiate whole "func" function template

    // ------------------------------------------------

    decltype(func_2(12)) y = 5;   // VALID

    // if "func_2" function template is completely instantiated
    // "using type = typename T::value_type;" statement inside 
    // must generate a syntax error.
    // but because of "func_2" function template is partially
    // instantiated, it will not be a syntax error.
    // (Only the signature should be generated)

    // ------------------------------------------------
  }
*/

/*
                --------------------------------
                | point of instantiation (POI) |
                --------------------------------
*/

/*
  template <typename T>
  void func(T x)
  {
    bar(x);
  }

  class AClass {};

  void bar(AClass)
  {
    std::cout << "bar(AClass)\n";
  }

  void bar(int)
  {
    std::cout << "bar(int)\n";
  }

  int main()
  {
    // ------------------------------------------------

    func(10); // syntax error
    // error: 'bar' was not declared in this scope, 
    // and no declarations were found by argument-dependent lookup 
    // at the point of instantiation

    // unqualified lookup for "bar" identifier  : NOT FOUND

    // ------------------------------------------------

    AClass ax;
    func(ax); // VALID
    // unqualified lookup for "bar" identifier  : NOT FOUND
    // (in POI) ADL for "bar" identifier        : FOUND
    // because of "AClass" definition is in global namespace scope
    // whole global namespace scope will be searched and "bar"
    // identifier will be found. NO syntax error

    // Two phase lookup

    // ------------------------------------------------
  }
*/

/*
                  --------------------------
                  | explicit instantiation |
                  --------------------------
*/

/*
  template <typename T>
  void func(T){}

  template void func<int>(int);  
  // explicit instantiation directive

  // compiler has already generate func<int> specialization 
  // in this line because of explicit instantiation directive.
*/

/*
  template <typename T>
  void func(T){}

  template void func<int>(int);  
  template void func<>(int);
  template void func(int);
  // Those 3 lines are equivalent.
*/

/*
  // explicit instantiation can also be done inside a namespace

  namespace ASpace {
    template <typename T>
    class Myclass {};

    template class Myclass<int>;
  }
*/

/*
  // "header_1.h"
  // --------------

  namespace ASpace {

    template <typename T>
    class Myclass {
      void func(){}
    };

    template class Myclass<int>;
  }

  // "source.cpp"
  // --------------
  // #include "header_1.h"

  template class ASpace::Myclass<double>;
  // explicit instantiation directive  
  // for Myclass<double> specialization
*/

/*
  template <typename T>
  class Myclass {
    void func(){}
  };

  template void Myclass<int>::func(); 
  // explicit instantiation directive 
  // for Myclass<int> specialization's func member function
*/

/*
  // --------------------------------------------

  // ==== file.hpp
  // --------------
  template <typename T>
  void foo(); 
  // only declaration no definition (prevents instantiation)

  // --------------------------------------------

  // ==== file.tpp
  // #include "file.hpp"

  template <typename T>
  void foo(){}
  // "foo" template function's definition

  // --------------------------------------------

  // ==== file.cpp
  // #include "file.tpp"

  template void foo<int>(); 
  // explicit instantiation directive
  // for foo<int> specialization

  // --------------------------------------------
*/

/*
              -------------------------------
              | extern template declaration |
              -------------------------------
*/

/*
  // -------------------------------------------

  // ==== tpl_func.h
  // ---------------

  template <typename T>
  void tpl_func(){};   
  // template function definition

  // -------------------------------------------

  // ==== file_1.cpp
  // ---------------
  // #include "tpl_func.h"

  void some_function_in_file1()
  {
    tpl_func<int>();  
    // "tpl_func" function template instantiation is needed
  }

  // -------------------------------------------

  // ==== file_2.cpp
  // ---------------
  // #include "tpl_func.h"

  void some_function_in_file2()
  {
    tpl_func<int>();  
    // "tpl_func" function template instantiation is needed
  }

  // -------------------------------------------

  // file_1.o
  // 000000000  W void tpl_func<int>()
  // 000000000  T some_function_in_file1()

  // file_2.o
  // 000000000  W void tpl_func<int>()
  // 000000000  T some_function_in_file2()


  // in both object file (file_1.o and file_2.o)
  // "tpl_func<int>" specialization is instantiated.

  // -------------------------------------------
*/

/*
  // -------------------------------------------

  // ==== tpl_func.h
  // ---------------

  template <typename T>
  void tpl_func(){};   
  // template function definition

  // -------------------------------------------

  // ==== file_1.cpp
  // ---------------
  // #include "tpl_func.h"

  void some_function_in_file1()
  {
    tpl_func<int>();  
    // "tpl_func" function template instantiation is needed
  }

  // -------------------------------------------

  // ==== file_2.cpp
  // ---------------
  // #include "tpl_func.h"

  extern template void tpl_func<int>();
  // extern template declaration
  // for tpl_func<int> specialization

  void some_function_in_file2()
  {
    tpl_func<int>();  
  }

  // extern template declaration is a way to tell compiler to
  // NOT INSTANTIATE "void tpl_func<int>()" function 
  // that have this template ID("tpl_func<int>" specialization)
  // for this module.

  // in some other module, compiler will compile 
  // "tpl_func<int>" specialization's code.

  // -------------------------------------------

  // file_1.o
  // 000000000  W void tpl_func<int>()
  // 000000000  T some_function_in_file1()

  // file_2.o
  // 000000000  T some_function_in_file2()

  // only in file_1.o "tpl_func<int>" 
  // specialization is instantiated.
  // becuase of extern template declaration in file_2.cpp
  // "tpl_func<int>" specialization is not instantiated.

  // -------------------------------------------
*/

/*
  -----------------------------------------------------------

  - bir başlık dosyasına template yerleştirilir.

  - eğer o template'ın belirli specializationlarının, 
    hangi kaynak dosyalarında kullanılacağı başlangıçta belli ise,
    kaynak dosyalarının hepsine(1 tanesi hariç) 
    "extern template declaration" yerleştirilir.
    (tabii ki bu durumda, extern template declaration'ın başlık 
    dosyasına yerleştirilmesi gerekir.)

  - 1 adet kaynak dosyasına ise, explicit instantiation directive 
    yerleştirilir.

  dolayısıyla...
  - başlık dosyasını include eden kaynak dosyaları 
    extern template declaration'ı da include etmiş olacaklar.
  - ayrı bir kaynak dosyasına ise, explicit instantiation 
    directive yerleştirilmiş olacak.

  Dezavantaj
  -----------
  - derleyicinin optimizasyon yapma kabiliyeti düştü.
  - 1 adet kaynak dosyasında fonksiyon inline expansion'a 
    uğrar(fonksiyona çağrı yapılmaz) ise derlenmiş kodda 
    fonksiyonun tanımı olmayacak ve başlık dosyasını include 
    etmiş olan diğer kaynak dosyalarında bu fonksiyonun
    kullanılması durumu Link hatasına sebep olacaktır.
    Dolayısıyla bu kaynak dosyasında fonksiyonun inline 
    expand edilmemesi gerekiyor ve standart değil!!

  -----------------------------------------------------------
*/

/*
  // -------------------------------------------

  // ==== tpl_func.h
  // ---------------

  template <typename T>
  void tpl_func(){};   
  // template function definition

  extern template void tpl_func<int>();
  // extern template declaration

  // -------------------------------------------

  // ==== tpl_func.cpp
  // -----------------
  // #include "tpl_func.h"

  template void tpl_func<int>();  
  // explicit instantiation directive

  // -------------------------------------------

  // ==== file_1.cpp
  // ---------------
  // #include "tpl_func.h"

  void some_function_in_file1()
  {
    tpl_func<int>();  
  }

  // -------------------------------------------

  // ==== file_2.cpp
  // ---------------
  // #include "tpl_func.h"

  void some_function_in_file2()
  {
    tpl_func<int>();  
  }

  // -------------------------------------------

  // instantiation of "tpl_func<int>" specialization
  // is generated in tpl_func.o object file.
  // and in file_1.o and file_2.o object files
  // "tpl_func<int>" specialization is not instantiated.

  // -------------------------------------------
*/

/*
  // -------------------------------------------

  // ==== third_party_lib.h
  // ---------------

  template <typename T>
  void tpl_func()
  {
    // function implementation
  };   
  // template function definition

  // -------------------------------------------

  // ==== tpl_func.h
  // ---------------
  // #include "third_party_lib.h"

  extern template void tpl_func<int>();
  // extern template declaration

  // -------------------------------------------

  // ==== tpl_func.cpp
  // -----------------
  // #include "tpl_func.h"

  template void tpl_func<int>();  
  // explicit instantiation directive

  // -------------------------------------------

  // ==== file_1.cpp
  // ---------------
  // #include "tpl_func.h"

  void some_function_in_file1()
  {
    tpl_func<int>();  
  }

  // -------------------------------------------

  // ==== file_2.cpp
  // ---------------
  // #include "tpl_func.h"

  void some_function_in_file2()
  {
    tpl_func<int>();  
  }

  // -------------------------------------------
*/

/*
  // extern template declaration can also be used 
  // for class templates 

  // -------------------------------------------

  // ==== myclass.h
  // ---------------

  template <typename T>
  class Myclass {};  
  // class template implementation

  // -------------------------------------------

  // ==== file_1.cpp
  // ---------------
  // #include "myclass.h"

  void some_function_in_file1()
  {
    Myclass<int> a1;  
  }

  // -------------------------------------------

  // ==== file_2.cpp
  // ---------------
  // #include "myclass.h"

  extern template class Myclass<int>;
  // extern template declaration

  void some_function_in_file2()
  {
    Myclass<int> a1;   
  }

  // -------------------------------------------
*/

/*
  namespace std {
    template <typename charT, 
              typename traits = char_traits<charT>,
              typename Allocator = allocator<charT>>
    class basic_string {};

    extern template class basic_string<char>;
    extern template class basic_string<wchar_t>;
  }
*/

/*
          -----------------------------------------------
          | CRTP (Curiously Recurring Template Pattern) |
          -----------------------------------------------
*/

/*
  template <typename T>
  class Base {};

  // sınıf şablonundan bir specialization oluştururken 
  // template argümanı olan tür, kalıtım yoluyla elde edilir.

  class Der : public Base<Der> {};

  // türemiş sınıflar taban sınıfın interface'ini kullanabilir.
*/

/*
  // regular inheritance

  class Counter {
  private:
    inline static std::size_t  ms_no_of_live_objects{};
    inline static std::size_t  ms_no_of_created_objects{};
  public:
    Counter()
    {
      ++ms_no_of_live_objects;
      ++ms_no_of_created_objects;
    }
    ~Counter()
    {
      --ms_no_of_live_objects;
    }

    static std::size_t get_live_object_count()
    {
      return ms_no_of_live_objects;
    }
    static std::size_t get_created_object_count()
    {
      return ms_no_of_created_objects;
    }
  };

  class AClass : public Counter {};
  class BClass : public Counter {};

  int main()
  {
    AClass ax1, ax2;

    {
      AClass ax3, ax4, ax5, ax6;
    }

    std::cout << "live object count = " 
              << AClass::get_live_object_count() << '\n';

    std::cout << "created object count = "
              << AClass::get_created_object_count() << '\n';

    // output -> 
    // live object count = 2
    // created object count = 6

    BClass bx1;

    {
      BClass bx3, bx4;
    }

    std::cout << "live object count = " 
              << BClass::get_live_object_count() << '\n';

    std::cout << "created object count = "
              << BClass::get_created_object_count() << '\n';
    
    // output ->
    //  live object count = 3
    //  created object count = 9
  }
*/

/*
  // CRTP technique

  #include <cstddef>

  // CRTP Base class
  template <typename Der>
  class Counter {
  private:
    inline static std::size_t  ms_no_of_live_objects{};
    inline static std::size_t  ms_no_of_created_objects{};
  public:
    Counter()
    {
      ++ms_no_of_live_objects;
      ++ms_no_of_created_objects;
    }
    ~Counter()
    {
      --ms_no_of_live_objects;
    }

    static std::size_t get_live_object_count()
    {
      return ms_no_of_live_objects;
    }
    static std::size_t get_created_object_count()
    {
      return ms_no_of_created_objects;
    }
  };

  class AClass : Counter<AClass> {
  public:
    using Counter<AClass>::get_live_object_count;
    using Counter<AClass>::get_created_object_count;
    // when private inheritance is used,
    // we can inject Base class's member functions
    // into the derived class's scope by using, using declaration.
  };

  class BClass : Counter<BClass> {
  public:
    using Counter<BClass>::get_live_object_count;
    using Counter<BClass>::get_created_object_count;
    // when private inheritance is used,
    // we can inject Base class's member functions
    // into the derived class's scope by using, using declaration.
  };

  int main()
  {
    // bu seneryodaki CRTP, her sınıf ayrı bir specialization
    // olduğu için, her sınıfın static veri elemanları farklıdır.

    AClass ax1, ax2;

    {
      AClass ax3, ax4, ax5, ax6;
    }

    std::cout << "live object count = " 
              << AClass::get_live_object_count() << '\n';

    std::cout << "created object count = "
              << AClass::get_created_object_count() << '\n';

    // output -> 
    // live object count = 2
    // created object count = 6

    BClass bx1;

    {
      BClass bx3, bx4;
    }

    std::cout << "live object count = " 
              << BClass::get_live_object_count() << '\n';

    std::cout << "created object count = "
              << BClass::get_created_object_count() << '\n';
    
    // output ->
    //  live object count = 1
    //  created object count = 3
  }
*/

/*
  // if purpose is using different static data members,
  // decltype([]{}) technique can also be used instead of CRTP

  #include <cstddef>

  template <typename T = decltype([]{})>
  class Counter {
  private:
    inline static std::size_t  ms_no_of_live_objects{};
    inline static std::size_t  ms_no_of_created_objects{};
  public:
    Counter()
    {
      ++ms_no_of_live_objects;
      ++ms_no_of_created_objects;
    }
    ~Counter()
    {
      --ms_no_of_live_objects;
    }

    static std::size_t get_live_object_count()
    {
      return ms_no_of_live_objects;
    }
    static std::size_t get_created_object_count()
    {
      return ms_no_of_created_objects;
    }
  };

  class AClass : public Counter<> {};
  class BClass : public Counter<> {};

  int main()
  {
    AClass ax1, ax2;

    {
      AClass ax3, ax4, ax5, ax6;
    }

    std::cout << "live object count = " 
              << AClass::get_live_object_count() << '\n';

    std::cout << "created object count = "
              << AClass::get_created_object_count() << '\n';

    // output -> 
    // live object count = 2
    // created object count = 6

    BClass bx1;

    {
      BClass bx3, bx4;
    }

    std::cout << "live object count = " 
              << BClass::get_live_object_count() << '\n';

    std::cout << "created object count = "
              << BClass::get_created_object_count() << '\n';
    
    // output ->
    //  live object count = 1
    //  created object count = 3
  }
*/

/*
  // if purpose is using different static data members,
  // non type template parameter can also be used instead of CRTP
  // C++20

  #include <cstddef>

  template <auto = []{}>
  class Counter {
  private:
    inline static std::size_t  ms_no_of_live_objects{};
    inline static std::size_t  ms_no_of_created_objects{};
  public:
    Counter()
    {
      ++ms_no_of_live_objects;
      ++ms_no_of_created_objects;
    }
    ~Counter()
    {
      --ms_no_of_live_objects;
    }

    static std::size_t get_live_object_count()
    {
      return ms_no_of_live_objects;
    }
    static std::size_t get_created_object_count()
    {
      return ms_no_of_created_objects;
    }
  };

  class AClass : public Counter<> {};
  class BClass : public Counter<> {};

  int main()
  {
    AClass ax1, ax2;

    {
      AClass ax3, ax4, ax5, ax6;
    }

    std::cout << "live object count = " 
              << AClass::get_live_object_count() << '\n';

    std::cout << "created object count = "
              << AClass::get_created_object_count() << '\n';

    // output -> 
    // live object count = 2
    // created object count = 6

    BClass bx1;

    {
      BClass bx3, bx4;
    }

    std::cout << "live object count = " 
              << BClass::get_live_object_count() << '\n';

    std::cout << "created object count = "
              << BClass::get_created_object_count() << '\n';
    
    // output ->
    //  live object count = 1
    //  created object count = 3
  }
*/

/*
  template <typename Der>
  class Base {};

  class AClass : public Base<AClass> {};
  class BClass : public Base<AClass> {};    // -----> Problem

  // problem using Base<AClass> instead of Base<BClass>

  int main()
  {
    AClass ax;  // VALID
    BClass bx;  // VALID
  }
*/

/*
  template <typename Der>
  class Base {
  private:
    friend Der;
    Base() = default;
  };

  class AClass : public Base<AClass> {};
  class BClass : public Base<AClass> {};    // -----> Problem

  int main()
  {
    // ------------------------------------------------

    AClass ax;  // VALID
    // because of AClass(Der) is friend declared inside 
    // Base class template, AClass(Der) can access Base 
    // classes default constructor in its private section.

    // ------------------------------------------------

    BClass bx;  // syntax error
    // error: use of deleted function 'BClass::BClass()'
    // error: 'constexpr Base<Der>::Base() [with Der = AClass]' 
    // is private within this context

    // because of <AClass> is used as template argument
    // BClass itself is not declared as friend in Base class
    // so BClass can not access Base classes default constructor
    // which is in its private section.

    // ------------------------------------------------
  }
*/

/*
  template <typename Der>
  class Base {
  public:
    void func_interface()
    {
      static_cast<Der*>(this)->func_impl();
    }

    static void func_static_interface()
    {
      Der::func_static_impl();
    }

    void func_impl()
    {
      std::cout << "Base::func_impl()\n";
    }

    static void func_static_impl()
    {
      std::cout << "Base::func_static_impl()\n";
    }
  };

  class Der_AClass : public Base<Der_AClass> {
  public:
    void func_impl()
    {
      std::cout << "Der_AClass::func_impl()\n";
    }

    static void func_static_impl()
    {
      std::cout << "Der_AClass::func_static_impl()\n";
    }
  };

  class Der_BClass : public Base<Der_BClass> {
  public:
    void func_impl()
    {
      std::cout << "Der_BClass::func_impl()\n";
    }
  };

  int main()
  {
    Der_AClass ax;

    ax.func_interface();  
    // output -> Der_AClass::func_impl()

    ax.func_static_interface();
    // output -> Der_AClass::func_static_impl()

    Der_BClass bx;
    bx.func_interface();
    // output -> Der_BClass::func_impl()

    bx.func_static_interface();
    // output -> Base::func_static_impl()
  }
*/

/*
  // türemiş sınıfların "print" fonksiyonun sahip olduklarını
  // varsayarak, türemiş sınıflara, print fonksiyonunu N kere
  // çağıracak bir interface enjeckte etmek istiyoruz.

  #include <cstddef>
  #include <string>

  template <typename Printable>
  struct PrintNTimes {
    void print_N(std::size_t N) const
    {
      while (N--)
        static_cast<const Printable*>(this)->print();
        // static_cast<const Printable&>(*this).print();
    }
  };

  class Person : public PrintNTimes<Person> {
  private:
    std::string m_name;
    std::string m_surname;

  public:
    Person() = default;
    Person(const char* p_name, const char* p_surname)
      : m_name{ p_name }, m_surname{ p_surname } {}

    void print() const
    {
      std::cout << m_name << ' ' << m_surname << '\n';
    }
  };

  int main()
  {
    Person p1{ "hello", "world" };
    p1.print_N(5);
    // output ->
    //  hello world
    //  hello world
    //  hello world
    //  hello world
    //  hello world
  }
*/

/*
  // outdated example but expressing CRTP well

  // Base class injected "operator==" and "operator!=" functions
  // to Derived classes interface.

  #include <string>

  template <typename Der>
  struct Equality {
    const Der& derived() const 
    {
      return static_cast<const Der&>(*this);
    }

    // hidden friend operator== function
    friend bool operator==(const Equality& lhs, const Equality& rhs)
    {
      return  not(lhs.derived() < rhs.derived()) and
              not(rhs.derived() < lhs.derived());     
    }
    // hidden friend operator!= function
    friend bool operator!=(const Equality& lhs, const Equality& rhs)
    {
      return not(lhs == rhs);
    }
  };

  class Person : public Equality<Person> {
  private:
    std::string m_name;

  public:
    Person(const std::string& str) : m_name{ str } {}

    bool operator<(const Person& other) const
    {
      return m_name < other.m_name;
    }
  };

  int main()
  {
    std::boolalpha(std::cout);

    Person p1{ "hello" };
    Person p2{ "world" };
    Person p3{ "hello" };

    std::cout << (p1 == p2) << '\n';  // output -> false
    std::cout << (p1 != p2) << '\n';  // output -> true
    std::cout << (p1 == p3) << '\n';  // output -> true
  }
*/

/*
  // derived class itself is also a class template

  #include <iterator>   // std::prev, std::next, std::distance
  #include <memory>     // std::make_unique, std::unique_ptr

  template <typename Der>
  class Container {
  private:
    Der& derived()
    { 
      return static_cast<Der&>(*this); 
    }
    const Der& derived() const 
    { 
      return static_cast<const Der&>(*this); 
    }
  public:
    decltype(auto) front()
    {
      return *derived().begin();
    }
    decltype(auto) back()
    {
      return *std::prev(derived().end());
    }
    decltype(auto) size() const
    {
      return std::distance(derived().begin(), derived().end());
    }
    decltype(auto) operator[](std::size_t idx)
    {
      return *std::next(derived().begin(), idx);
    }
  };

  template <typename T>
  class DynArray : public Container<DynArray<T>> {
  private:
    std::size_t m_size;
    std::unique_ptr<T[]> mp_data;

  public:
    DynArray(std::size_t N) 
        : m_size{ N }, mp_data{ std::make_unique<T[]>(m_size) } {} 
    
    T* begin() { return mp_data.get(); }
    const T* begin() const { return mp_data.get(); }
    T* end() { return mp_data.get() + m_size; }
    const T* end() const { return mp_data.get() + m_size; }
  };

  int main()
  {
    DynArray<int> d_arr(10);

    d_arr.front() = 11;
    d_arr[2] = 22;
    d_arr.back() = 33;

    for (auto elem : d_arr)
      std::cout << elem << ' ';
    // output -> 11 0 22 0 0 0 0 0 0 33

    std::cout << '\n';
    std::cout << "d_arr.size() = " << d_arr.size() << '\n';
    // output -> d_arr.size() = 10
  }
*/

/*
  // CRTP Base singleton class

  #include <memory>   // std::unique_ptr
  #include <mutex>    // std::once_flag, std::call_once

  template <typename TDerived>
  class Singleton {
  private:
    inline static std::unique_ptr<TDerived> msp_instance{};
    inline static std::once_flag            ms_once{};

  protected:
    Singleton() {}

  public:
    ~Singleton() {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static TDerived& get_instance()
    {
      std::call_once(
        Singleton::ms_once, 
        [](){ Singleton::msp_instance.reset(new TDerived());});

      return *msp_instance;
    }
  };

  class AClass : public Singleton<AClass> {
  public:
    AClass()
    {
      std::cout << "AClass::AClass()\n";
    }

    ~AClass()
    {
      std::cout << "AClass::~AClass()\n";
    }

    void func()
    {
      std::cout << "AClass::func()\n";
    }
  };

  int main()
  {
    std::cout << "[0] - main started\n";

    AClass::get_instance().func();
    AClass::get_instance().func();

    std::cout << "[1] - main finished\n";

    // output ->
    //  [0] - main started
    //  AClass::AClass()
    //  AClass::func()
    //  AClass::func()
    //  [1] - main finished
    //  AClass::~AClass()
  }
*/

/*
  #include <cstdio>   // std::fprintf, std::fopen, std::fclose
  #include <stdexcept> // std::runtime_error

  template <class Der>
  class Writer {
  public:
    void write(const char* str) const
    {
      static_cast<const Der*>(this)->write_impl(str);
    }
  };

  class FileWriter : public Writer<FileWriter> {
  private:
    FILE* mp_file;

    friend class Writer<FileWriter>;

    void write_impl(const char* str) const
    {
      std::fprintf(mp_file, "%s\n", str);
    }

  public:

    FileWriter(const char* p_filename) 
      : mp_file{ std::fopen(p_filename, "w") }
    {
      using namespace std::literals;

      if(!mp_file)
        throw std::runtime_error{ p_filename + "couldn't created"s };
    }

    ~FileWriter()
    {
      std::fclose(mp_file);
    }
  };

  class ConsoleWriter : public Writer<ConsoleWriter> {
  private:
    friend class Writer<ConsoleWriter>;

    void write_impl(const char* str) const
    {
      std::printf("%s\n", str);
    }
  };

  int main()
  {
    ConsoleWriter cw;
    FileWriter fw{ "test.txt" };

    cw.write("hello world");
    fw.write("hello world");
  }
*/

/*
  // Using more than one CRTP base class

  template <typename Der>
  class A {
  public:
    void func_A()
    {
      static_cast<Der*>(this)->func_1();
    }
  };

  template <typename Der>
  class B {
  public:
    void func_B()
    {
      static_cast<Der*>(this)->func_2();
    }
  };

  template <typename Der>
  class C {
  public:
    void func_C()
    {
      static_cast<Der*>(this)->func_3();
    }
  };

  class Myclass : public A<Myclass>, 
                  public B<Myclass>, 
                  public C<Myclass> {
  public:
    void func_1()
    {
      std::cout << "Myclass::func_1()\n";
    }             
    void func_2()
    {
      std::cout << "Myclass::func_2()\n";
    }
    void func_3()
    {
      std::cout << "Myclass::func_3()\n";
    }
  };

  int main()
  {
    Myclass m1;

    m1.func_A();  // output -> Myclass::func_1()
    m1.func_B();  // output -> Myclass::func_2()
    m1.func_C();  // output -> Myclass::func_3()
  }
*/

/*
  // making derived class itself as a template 

  #include <string>

  template <typename Der>
  struct MakeDouble {
    Der make_it_double() const
    {
      const auto& self = static_cast<const Der&>(*this);
      return self + self;
    }
  };

  template <typename Der>
  struct MakeTriple {
    Der make_it_triple() const
    {
      const auto& self = static_cast<const Der&>(*this);
      return self + self + self;
    }
  };

  template <typename T>
  class Val : public MakeDouble<Val<T>>, public MakeTriple<Val<T>> {
  private:
    T m_val;

  public:
    Val(const T& val) : m_val{ val } {}

    Val operator+(const Val& other) const
    {
      return m_val + other.m_val;
    }

    void print() const 
    {
      std::cout << '[' <<m_val << "]\n";
    }
  };

  int main()
  {
    using namespace std::literals;

    Val<int> v1 = 15, v2 = 22;
    Val<std::string> v3 = "hello "s;

    v1.make_it_double().print();  // output -> [30]
    v2.make_it_triple().print();  // output -> [66]
    v3.make_it_double().print();  // output -> [hello hello ]
  }
*/

/*
  template <typename Der>
  class AClass {
  public:
    void func_A()
    {
      std::cout << "AClass::func_A()\n";
      static_cast<Der*>(this)->foo_1();
    }
  };

  template <typename Der>
  class BClass {
  public:
    void func_B()
    {
      std::cout << "BClass::func_B()\n";
      static_cast<Der*>(this)->foo_2();
    }
  };

  template <typename Der>
  class CClass {
  public:
    void func_C()
    {
      std::cout << "CClass::func_C()\n";
      static_cast<Der*>(this)->foo_3();
    }
  };

  // template template parameter pack
  template <template <typename> typename... Bases>
  class Myclass : public Bases<Myclass<Bases...>>... {
  public:
    void foo_1()
    {
      std::cout << "Myclass::foo_1()\n";
    }

    void foo_2()
    {
      std::cout << "Myclass::foo_2()\n";
    }

    void foo_3()
    {
      std::cout << "Myclass::foo_3()\n";
    }
  };

  // class Myclass 
  //    :   public AClass<Myclass<AClass, BClass, CClass>>, 
  //        public BClass<Myclass<AClass, BClass, CClass>>,
  //        public CClass<Myclass<AClass, BClass, CClass>>

  int main()
  {
    Myclass<AClass, BClass, CClass> m1;

    m1.func_A();  
    // output ->
    //  AClass::func_A()
    //  Myclass::foo_1()

    m1.func_B();
    // output ->
    //  BClass::func_B()
    //  Myclass::foo_2()

    m1.func_C();
    // output -> 
    //  CClass::func_C()
    //  Myclass::foo_3()
  }
*/

/*
  #include <ostream>

  class Printer {
  private:
    std::ostream& m_ostream;

  public:
    Printer(std::ostream& param_os) : m_ostream{ param_os } {}

    template <typename T>
    Printer& print(const T& t)
    {
      m_ostream << t;
      return *this;
    }

    template <typename T>
    Printer& print_line(const T& t)
    {
      m_ostream << t << '\n';
      return *this;
    }
  };

  int main()
  {
    using namespace std;

    Printer(cout).print("hello ").print_line("world").print_line(10);
    // output ->
    //  hello world
    //  10
  }
*/

/*
  #include <ostream>

  class Printer {
  private:
    std::ostream& m_ostream;

  public:
    Printer(std::ostream& param_os) : m_ostream{ param_os } {}

    template <typename T>
    Printer& print(const T& t)
    {
      m_ostream << t;
      return *this;
    }

    template <typename T>
    Printer& print_line(const T& t)
    {
      m_ostream << t << '\n';
      return *this;
    }
  };

  class ColorPrinter : public Printer {
  public:
    ColorPrinter() : Printer{ std::cout } {}
  
    ColorPrinter& set_color(int color)
    {
      std::cout << "color set\n";
      return *this;
    }
  };

  int main()
  {
    using namespace std;

    ColorPrinter().print("hello ").set_color(12).print_line("world");
    // syntax error
    // error: 'class Printer' has no member named 'set_color'

    // "print" function is returning Printer& which 
    // does not have "set_color" member function.
  }
*/

/*
  // CRTP solution

  #include <ostream>

  template <typename DerPrinter>
  class Printer {
  private:
    std::ostream& m_ostream;

  public:
    Printer(std::ostream& param_os) : m_ostream{ param_os } {}

    template <typename T>
    DerPrinter& print(const T& t)
    {
      m_ostream << t;
      return static_cast<DerPrinter&>(*this);
    }

    template <typename T>
    DerPrinter& print_line(const T& t)
    {
      m_ostream << t << '\n';
      return static_cast<DerPrinter&>(*this);
    }
  };

  class ColorPrinter : public Printer<ColorPrinter> {
  public:
    ColorPrinter() : Printer{ std::cout } {}
  
    ColorPrinter& set_color(int color)
    {
      std::cout << "color set\n";
      return *this;
    }
  };

  int main()
  {
    using namespace std;

    ColorPrinter().print("hello ").set_color(12).print_line("world");
    // output ->
    //  hello color set
    //  world
  }
*/

/*
  - to convert virtual dispatch mechanism to static polymorphism
    CRTP base class will be used.
*/

/*
  // CRTP base class
  template <typename Der>
  class Animal {
  public:
    void sound()
    {
      static_cast<Der*>(this)->sound_impl();
    }
  };

  class Cat : public Animal<Cat> {
  public:
    void sound_impl()
    {
      std::cout << "meow\n";
    }
  };

  class Dog : public Animal<Dog> {
  public:
    void sound_impl()
    {
      std::cout << "woof\n";
    }
  };

  template <typename T>
  void pet_animal(Animal<T>& a)
  {
    a.sound();
  }
  
  int main()
  {
    Cat c1;
    Dog d1;

    // -------------------------------------------

    c1.sound();  // output -> meow
    d1.sound();  // output -> woof

    // -------------------------------------------

    pet_animal(c1);  // output -> meow
    pet_animal(d1);  // output -> woof

    // -------------------------------------------
  }
*/

/*
                      -----------------
                      | Mixin Pattern |
                      -----------------
*/

/*
  - farklı farklı interface'lere sahip olan sınıfların
    kullanıcı kod tarafından birleştirilmesi ile 
    istenilen özelliklerin bir araya getirilmesi.

  - CRTP'ye benziyor ama oluşturulma biçimi farklı.
*/

/*
  template <typename Base>
  class Skill_A : public Base {
  public:
    void func_A()
    {
      std::cout << "Skill_A::func_A\n";
      Base::foo_A();
    }
  };

  template <typename Base>
  class Skill_B : public Base {
  public:
    void func_B()
    {
      std::cout << "Skill_B::func_B\n";
      Base::foo_B();
    }
  };

  template <typename Base>
  class Skill_C : public Base {
  public:
    void func_C()
    {
      std::cout << "Skill_C::func_C\n";
      Base::foo_C();
    }
  };

  // Derived classes are invoking Base classes member functions.

  class Myclass {
  public:
    void foo_A()
    {
      std::cout << "Myclass::foo_A\n";
    }

    void foo_B()
    {
      std::cout << "Myclass::foo_B\n";
    }

    void foo_C()
    {
      std::cout << "Myclass::foo_C\n";
    }
  };

  int main()
  {
    // -------------------------------------------

    Skill_A<Myclass>{}.func_A();
    // output ->
    //  Skill_A::func_A
    //  Myclass::foo_A

    Skill_B<Myclass>{}.func_B();
    // output ->
    //  Skill_B::func_B
    //  Myclass::foo_B

    Skill_C<Myclass>{}.func_C();
    // output ->
    //  Skill_C::func_C
    //  Myclass::foo_C

    // -------------------------------------------

    using Myclass_A = Skill_A<Myclass>;
    using Myclass_B = Skill_B<Myclass>;
    using Myclass_C = Skill_C<Myclass>;

    using Myclass_AB = Skill_B<Myclass_A>;
    using Myclass_ABC = Skill_C<Myclass_AB>; 

    Myclass_ABC m1;
    m1.func_A();
    m1.func_B();
    m1.func_C();

    // output ->
    //  Skill_A::func_A
    //  Myclass::foo_A
    //  Skill_B::func_B
    //  Myclass::foo_B
    //  Skill_C::func_C
    //  Myclass::foo_C

    // -------------------------------------------
  }
*/

/*
  #include <type_traits>  // std::is_same

  template <typename Base>
  class Skill_A : public Base {};

  template <typename Base>
  class Skill_B : public Base {};

  template <typename Base>
  class Skill_C : public Base {};

  class Myclass {};

  int main()
  {
    using t1 = Skill_A<Skill_B<Skill_C<Myclass>>>;  // ABC
    using t6 = Skill_A<Skill_C<Skill_B<Myclass>>>;  // ACB
    using t2 = Skill_B<Skill_A<Skill_C<Myclass>>>;  // BAC
    using t5 = Skill_B<Skill_C<Skill_A<Myclass>>>;  // BCA
    using t3 = Skill_C<Skill_A<Skill_B<Myclass>>>;  // CAB
    using t4 = Skill_C<Skill_B<Skill_A<Myclass>>>;  // CBA

    static_assert(!std::is_same_v<t1, t2> && 
                  !std::is_same_v<t1, t3> &&
                  !std::is_same_v<t1, t4> &&
                  !std::is_same_v<t1, t5> &&
                  !std::is_same_v<t1, t6> &&);  // holds
  }
*/

/*
  #include <string>

  template <typename Printable>
  struct Print_Repeat : Printable
  {
    explicit Print_Repeat(const Printable& printable)
      : Printable{ printable } {}
    
    using Printable::Printable; // inherited constructor

    void repeat(unsigned int N) const
    {
      while (N-- > 0)
        this->print();
    }
  };

  class Person {
  private:
    std::string m_first_name;
    std::string m_last_name;

  public:
    Person(std::string first_name, std::string last_name)
      : m_first_name{ first_name }, m_last_name{ last_name } {}

    void print() const 
    {
      std::cout << m_last_name << ", " << m_first_name << '\n';
    }
  };

  using Repeatedly_Printable_Person = Print_Repeat<Person>;

  int main()
  {
    // --------------------------------------------------------

    Repeatedly_Printable_Person p1{ Person{ "hello", "world" } };
    p1.repeat(2);
    // output ->
    //  world, hello
    //  world, hello

    // --------------------------------------------------------
  }
*/
