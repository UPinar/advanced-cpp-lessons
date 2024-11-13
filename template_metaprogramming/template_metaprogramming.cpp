#include <iostream>

/*
                  ============================
                  | Template Metaprogramming |
                  ============================
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
    // error: the value of 'arr1' is not usable in a constant expression

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
    friend std::ostream& operator<<(std::ostream& os, const Mystruct& ms)
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