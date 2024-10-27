#include <iostream>

/*
                    =========================
                    | `constexpr` functions |
                    =========================
*/

/*
          <---- check constexpr_functions_timeline.png ---->
*/

/*
  int main()
  {
    constexpr int x = 5;
    // "x" data type is const int
  }
*/

/*
  int global_x{};
  int global_y{};

  int main()
  {
    // --------------------------------------------------

    int local_x{};

    constexpr int* p = &local_x;  // syntax error
    // error: '& local_x' is not a constant expression
    // local variable's addresses are not constant expressions.

    // --------------------------------------------------

    constexpr int* p2 = &global_x; // VALID
    // global variable's address is a constant expression.

    // --------------------------------------------------

    p2 = &global_y; // syntax error
    // error: assignment of read-only variable 'p2'
    // "p2"'s data type is "int* const" (top level const)



    *p2 = 11; // VALID
    // "p2"'s data type is "int* const" (top level const)
    // not "const int*"

    // --------------------------------------------------
  }
*/

/*
  class Myclass {
  public:
    constexpr int foo(int);
    constexpr int foo(int) const;
    // Those 2 functions are const overloads.
    // constexpr member function is not a const member function.
  };
*/

/*
  int foo(int x) { return x * 5; }
  constexpr int bar(int x) { return x * 10; }

  int main()
  {
    // --------------------------------------------------

    auto fn = [](int x){ return x * 5; };
    // lambda expression's member operator() call function 
    // is constexpr function

    constexpr int val = fn(5);
    // "fn(5)" is a constant expression

    // --------------------------------------------------

    constexpr int val2 = foo(5);  // syntax error
    // error: call to non-'constexpr' function 'int foo(int)'

    constexpr int val3 = bar(5);  // VALID

    // --------------------------------------------------
  }
*/

/*
  constexpr int foo(int x) { return x * 5; }
  // "foo" function can be called at compile time
  // and can be called at runtime.

  // There is no guarantee that "foo" function 
  // will be called at compile time.

  int main()
  {
    // --------------------------------------------------

    foo(5); 
    // foo(5) function is not guaranteed to be called at compile time

    // for "foo(5)" expression guaranteed to be called at compile time,
    // function call expression should be in a context that 
    // requires a constant expression.

    // --------------------------------------------------

    int x = foo(5); 

    // if "x" value is calculated at compile time,
    // it is a compiler optimization,
    // because there is no guarantee "x"'s value
    // is calculated at compile time.

    // --------------------------------------------------
  }
*/

/*
  #include <type_traits>  // is_constant_evaluated

  constexpr double Sqrt(double) {}

  // --------------------------------------------------

  // "Sqrt" function can be called at compile time
  // "Sqrt" function can be called at runtime.

  // --------------------------------------------------

  // if "Sqrt" function is called with a variable(as an argument),
  // that is not a constant expression, 
  // "Sqrt" function will be called at runtime.

  // if "Sqrt" function is called with constant expression;
  // depends on the context, 
  // "Sqrt" function will be called at compile time or runtime.

  // --------------------------------------------------

  constexpr double Sqrt2(double)
  {
    if (std::is_constant_evaluated()) {
      // compile time context
    }
    else {
      // runtime context
    }
  }

  // since C++20 
  // we can use "Sqrt2" function in both contexts(compile and run-time) 
  // seperately.

  // --------------------------------------------------
*/

/*
  // contexts that require constant expressions 

  #include <type_traits>  // is_constant_evaluated
  #include <array>
  #include <bitset>

  constexpr int foo(int x)
  {
    if (std::is_constant_evaluated())
      return x * 5;
    else
      return x * 10;
  }

  int main()
  {
    // --------------------------------------------------

    int x = foo(5);
    std::cout << "x = " << x << '\n';
    // output -> x = 50

    constexpr int y = foo(5);
    std::cout << "y = " << y << '\n';
    // output -> y = 25

    // --------------------------------------------------

    int arr[foo(5)]{};
    std::cout << "arr size = " << std::size(arr) << '\n';
    // output -> arr size = 25

    // --------------------------------------------------

    std::array<int, foo(5)> arr2{};
    std::cout << "arr2 size = " << arr2.size() << '\n';
    // output -> arr2 size = 25

    // --------------------------------------------------

    if constexpr (foo(5) == 25)
      std::cout << "foo(5) is called in compile time" << '\n';
    else
      std::cout << "foo(5) is called in runtime" << '\n';

    // output -> foo(5) is called in compile time

    // --------------------------------------------------

    if (foo(5) == 25)
      std::cout << "foo(5) is called in compile time" << '\n';
    else
      std::cout << "foo(5) is called in runtime" << '\n';
    // output -> foo(5) is called in runtime

    // --------------------------------------------------

    static_assert(foo(5) == 25);  // VALID

    // --------------------------------------------------

    std::bitset<foo(5)> bs; // "bs"' type is "std::bitset<25u>"

    // --------------------------------------------------
  }
*/

/*
  int foo(int idx)
  {
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    return arr[idx];
  }

  int  main()
  {
    // --------------------------------------------------

    int x = foo(20);  // undefined behavior(UB)

    // --------------------------------------------------

    constexpr int y = foo(20);  // syntax error
    // error: call to non-'constexpr' function 'int foo(int)'

    // --------------------------------------------------
  }
*/

/*
  constexpr int foo(int idx)
  {
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    return arr[idx];
  }

  int main()
  {
    constexpr int x = foo(99);  // syntax error
    // error: array subscript value '99' is outside 
    // the bounds of array 'arr' of type 'int [10]'

    // constexpr fonksiyon compile-time bağlamında(context) 
    // çağrılmaya zorlandığında, derleyici tanımsız davranışı
    // tespit edip, sentaks hatası verir.
  }
*/

/*
  // 4 byte int 13! will overflow
  // 8 byte int 20! will overflow
  // signed integral type overflow is UB

  constexpr int factorial(int N)
  {
    return N < 2 ? 1 : N * factorial(N - 1);
  }

  int main()
  {
    constexpr int x = factorial(14);  // syntax error
    // error: overflow in constant expression 

    // constexpr fonksiyon compile-time bağlamında(context) 
    // çağrılmaya zorlandığında, derleyici tanımsız davranışı
    // tespit edip, sentaks hatası verir.
  }
*/

/*
  // bitsel sağa veya sola kaydırma işlemlerinde,
  // tamsayının bit sayısına eşit ya da büyük bir değer kullanarak
  // bitsel operatörün sağ operandı ile kaydırma işlemi yapmak
  // tanımsız davranıştır(UB).

  constexpr int shift_left(int elem, int N)
  {
    return elem << N;
  }

  int main()
  {
    constexpr int val = shift_left(20, 32); // syntex error
    // error: right operand of shift expression '(20 << 32)' 
    // is greater than or equal to the precision 32 of the left operand
  }
*/

/*
  // constexpr fonksiyon içerisinde "throw" statement olması, 
  // constexpr olma koşulunu çiğnemiyor. (since C++14)

  constexpr int factorial(int N)
  {
    if (N < 0)
      throw std::runtime_error("negative argument!");
    
    int result{ 1 };

    for (int i = 1; i <= N; ++i)
      result *= i;

    return result;
  }

  int main()
  {
    // --------------------------------------------------

    int ival = -5;
    try {
      auto x = factorial(ival);
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
    // output -> exception caught: negative argument!

    // runtime context will throw an exception.

    // --------------------------------------------------

    constexpr int y = factorial(-5); // syntax error
    // error: expression '<throw-expression>' is not a constant expression

    // when "factorial(-5)" expression used in compile time context
    // because compiler saw "throw" statement, will be syntax error.
  
    // --------------------------------------------------
  }
*/

/*
  -> almost every algorithm is constexpr
*/

/*
  #include <algorithm>  // std::sort
  #include <functional> // std::greater

  constexpr int foo(int N)
  {
    int arr[10] = { 10, 4, 2, 6, 23, 65, 29, 5, 1, 3 };

    std::sort(std::begin(arr), std::end(arr), std::greater{});
    return arr[N];
  }

  int main()
  {
    constexpr auto val = foo(2);
    // val -> 23 (calculated at compile time)

    // std::sort is a constexpr function
  }
*/

/*
  #include <numeric>    // std::accumulate
  #include <iterator>   // std::next

  constexpr int foo(int x, int y)
  {
    using namespace std;
    int arr[10] = { 10, 4, 2, 6, 23, 65, 29, 5, 1, 3 };

    return accumulate(next(begin(arr), x),
                      next(begin(arr), y), 
                      0);

  }

  int main()
  {
    constexpr auto val = foo(2, 5);
    // val -> 31(2 + 6 + 23) (calculated at compile time)

    // std::accumulate is a constexpr function
  }
*/

/*
  #include <numeric>    // std::accumulate

  constexpr int factorial(int N)
  {
    return N < 2 ? 1 : N * factorial(N - 1);
  }

  constexpr double get_e_number(int N)
  {
    double* p = new double[N];

    for (int i{}; i < N; ++i)
      p[i] = 1.0 / factorial(i);

    auto sum = std::accumulate(p, p + N, 0.0);

    delete[] p;
    return sum;
  }

  int main()
  {
    constexpr auto e_number = get_e_number(10);
    // e_number -> 2.7182815255731922 (calculated at compile time)

    // std::accumulate is a constexpr function
    // dynamic memory allocation is allowed in constexpr functions
  }
*/

/*
  #include <numeric>    // std::accumulate

  constexpr int factorial(int N)
  {
    return N < 2 ? 1 : N * factorial(N - 1);
  }

  constexpr double get_e_number(int N)
  {
    double* p = new double[N];

    for (int i{}; i < N; ++i)
      p[i] = 1.0 / factorial(i);

    auto sum = std::accumulate(p, p + N, 0.0);

    delete p;   // --------------------->
    return sum;
  }

  int main()
  {
    constexpr auto e_number = get_e_number(10); // syntax error
    // error: non-array deallocation of object  
    // allocated with array allocation

    // "array new" should be deleted with "array delete"
    // if not it will be undefined behavior(UB)
    // when compiler sees undefined behaviour in compile time context
    // it will be syntax error.
  }
*/

/*
  #include <numeric>    // std::accumulate

  constexpr int factorial(int N)
  {
    return N < 2 ? 1 : N * factorial(N - 1);
  }

  constexpr void array_delete(double* p) { delete[] p; }

  constexpr double get_e_number(int N)
  {
    double* p = new double[N];

    for (int i{}; i < N; ++i)
      p[i] = 1.0 / factorial(i);

    auto sum = std::accumulate(p, p + N, 0.0);

    array_delete(p);
    return sum;
  }

  constexpr void regular_delete(double* p) { delete p; }

  constexpr double get_e_number2(int N)
  {
    double* p = new double[N];

    for (int i{}; i < N; ++i)
      p[i] = 1.0 / factorial(i);

    auto sum = std::accumulate(p, p + N, 0.0);

    regular_delete(p);
    return sum;
  }

  int main()
  {
    // --------------------------------------------------

    constexpr auto e_number = get_e_number(10);
    // e_number -> 2.7182815255731922 (calculated at compile time)

    // --------------------------------------------------

    constexpr auto e_number2 = get_e_number2(10); // syntax error
    // error: non-array deallocation of object 
    // allocated with array allocation

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <numeric>    // std::accumulate
  #include <iterator>   // std::next

  constexpr int foo(int N)
  {
    std::vector ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    return std::accumulate( ivec.begin(), 
                            std::next(ivec.begin(), N), 
                            0);
  }

  int main()
  {
    // std::vector is using dynamic memory

    constexpr int x = foo(5);
    // x -> 15(1 + 2 + 3 + 4 + 5) (calculated at compile time)
  }
*/

/*
  #include <vector>
  #include <algorithm>  // std::sort

  constexpr int get_median(std::vector<int> ivec)
  {
    std::sort(ivec.begin(), ivec.end());
    return ivec[ivec.size() / 2];
  }

  int main()
  {
    constexpr auto median = get_median(
        { 2, 4, 6, 1, 9, 3, 12, 67, 923, 76, 34, 98 });
    // median -> 12 (calculated at compile time)

    // std::sort is a constexpr function
    // std::vector using dynamic memory and 
    // can be used in constexpr functions
  }
*/

/*
  #include <vector>
  #include <string>
  #include <string_view>
  #include <cstddef>  // std::size_t

  constexpr std::vector<std::string> 
  split( std::string_view strv, std::string_view delims = " ")
  {
    std::vector<std::string> output;
    size_t first = 0;

    while (first < strv.size()) {
      const size_t second = strv.find_first_of(delims, first);

      if (first != second)
        output.emplace_back(strv.substr(first, second - first));

      if (second == std::string_view::npos)
        break;

      first = second + 1;
    }

    return output;
  }

  constexpr size_t number_of_words(std::string_view sw)
  {
    const auto words = split(sw);
    return words.size();
  }

  int main()
  {
    static_assert(number_of_words("hello world abc xyz") == 4);

    constexpr auto num_count = number_of_words("hello world abc xyz");
    // num_count -> 4u (calculated at compile time)
  }
*/

/*
  #include <vector>

  struct Point {
  public:
    constexpr Point& operator+=(const Point& other) noexcept
    {
      m_x += other.m_x;
      m_y += other.m_y;
      return *this;
    }
    double m_x{}, m_y{};
  };

  constexpr bool test(size_t N)
  {
    std::vector<Point*> vec(N); // size_t parameter constructor

    for (auto& pt : vec)
      pt = new Point{ 0., 1 };

    Point sum{};

    for (auto& pt : vec)
      sum += *pt;

    for (auto& pt : vec)
      delete pt;

    return static_cast<size_t>(sum.m_y) == N;
  }

  int main()
  {
    static_assert(test(10));  // VALID
  }
*/

/*
    ---------------------------------------------------------------
    | `consteval` keyword | consteval(immediate) function (C++20) |
    ---------------------------------------------------------------
*/

/*
  constexpr fonksiyonlardan farklı olarak,
  (constexpr fonksiyonlar compile-time ve runtime contextlerinde
  çağrılabilirler)
  consteval fonksiyonların compile time contextinde çağrılmaları 
  garanti altındadır.

  Eğer consteval fonksiyon compile time contextinde çağrılmaz ise
  sentaks hatası oluşur.
*/

/*
  consteval int consteval_square(int x) { return x * x; }
  constexpr int constexpr_square(int x) { return x * x; }

  int main()
  {
    int ival = 5;

    // --------------------------------------------------
    
    constexpr_square(ival); // VALID
    // function will be called in runtime context

    // --------------------------------------------------

    consteval_square(ival); // syntax error
    // error: call to consteval function 'consteval_square(ival)' 
    // is not a constant expression
    // error: the value of 'ival' is not usable in a constant expression

    // --------------------------------------------------
  }
*/

/*
  int foo(int x) { return x * x; }
  constexpr int bar(int x) { return x * x; }
  consteval int baz(int x) { return x * x; }

  int main()
  {
    int ival{ 135 };

    // --------------------------------------------------

    auto elem1 = foo(ival); 
    // run-time, compile can optimize it to compile-time

    // --------------------------------------------------

    auto elem2 = bar(ival);
    // ival is not a constant expression
    // "bar(ival)" expression is NOT used in compile-time context
    // run-time, compile can optimize it to compile-time

    // --------------------------------------------------

    auto elem3 = bar(10);
    // "10" is a constant expression
    // "bar(10)" expression is NOT used in compile-time context
    // run-time, compile can optimize it to compile-time

    // --------------------------------------------------

    constexpr auto elem4 = bar(10);
    // "10" is a constant expression
    // "bar(10)" expression is used in compile-time context
    // guaranteed to be int compile-time

    // --------------------------------------------------

    auto elem5 = baz(ival); // syntax error
    // "ival" is not a constant expression
    // "baz(ival)" expression is NOT used in compile-time context

    // error: call to consteval function 'baz(ival)' 
    // is not a constant expression
    // error: the value of 'ival' is not usable in a constant expression

    // --------------------------------------------------

    auto elem6 = baz(10);
    // "10" is a constant expression
    // "baz(10)" expression is NOT used in compile-time context
    // guaranteed to be in compile-time `consteval`

    // --------------------------------------------------
  }
*/

/*
                      ----------------------
                      | `consinit` keyword |
                      ----------------------
*/

/*
  `constinit` anahtar sözcüğü ile tanımlanmış bir değişken
  static olarak compile time'da initialize edilir.
*/

/*
  Program birden fazla kaynak dosyadan oluştuğunda,
  kaynak dosyadaki sınıfların static veri elemanları ve 
  global değişkenler, diğer kaynak dosyalardaki global değişkenler
  tarafından(onların hayata gelme süreci sırasında) kullanılabilir.

  Problem: bir nesne, bir sınıfın constructor'ı içinde kullanıldığında,
  kullanılan nesne henüz hayata gelmemiş(initialize edilmemiş) olabilir.
  Bu durum tanımsız davranışa(UB) sebep olur.
*/

/*
  //  statik ömürlü değişkenlerin aynı kaynak dosyadaki 
  //  hayata gelme sıraları, bildirildikleri sıra ile aynıdır.

  // source.cpp
  // ------------

  AClass global_ax;  
  BClass global_bx;
  CClass global_cx;

  // g_ax will be initialized first, then g_bx, then g_cx

  // guaranteed that AClass's constructor will be called before 
  // BClass's constructor and BClass's constructor will be called
  // before CClass's constructor.
*/

/*
  // source1.cpp
  // ------------
  AClass global_ax;

  // source2.cpp
  // ------------
  BClass global_bx;

  // there is no guarantee that global_ax will be initialized
  // before global_bx or vice versa.

  // no guarantee that AClass's constructor will be called before
  // BClass's constructor or vice versa.
*/

/*
  // source1.cpp
  // ------------
  AClass global_ax;

  // source2.cpp
  // ------------
  BClass global_bx;

  // if BClass uses global_ax object in its constructor
  // and global_bx object is initialized first(not guaranteed)
  // that will cause problem(static initialization order fiasco)
*/

/*
  ---------------------------------------------------------------

              <--- check storage_duration.png --->

  automatic storage duration : 
    programın akışı o bloğa(scope'a) girdiğinde nesneni YERİ
    AYRILACAK(hayata gelmekle aynı anlama gelmiyor.), bloktan
    çıktığında ayrılan yer, nesneye ait olmaktan çıkacak.

  dynamic storage duration : 
    allocation fonksiyonlarıyla runtime'da storage'ın elde edilmesi.

  thread local storage duration :
    `thread_local` anahtar sözcüğü ile tanımlanan nesnelerin
    thread'in çalışmaya başlamasıyla yeri ayrılır ve thread'in
    çalışmasının sona ermesiyle yeri ona ait olmaktan çıkar.

  static storage duration :
    nesnenin yerinin main fonksiyonu çağrılmadan önce ayrılması
    ve o yerin programın çalışması süresince o nesneye ait olması.

  <--- check static_storage_duration.png --->

    - global değişkenler
    - sınıfların static veri elemanları
    - fonksiyonların static yerel değişkenleri

  ---------------------------------------------------------------

                  <--- check lifetime.png --->

  Lifetime(Lifespan) : 
    nesne ne zaman hayata gelir ve ne zaman hayatı biter.

    - vector nesnesinin hayata gelmesi -> constructor'ının çağrılması
    - vector nesnesinin hayatının bitmesi -> destructor'ının çağrılması

    - primitive türler için hayatın başlaması initialize edilme anı
    hayatının bitmesi ise bloktan(scope) çıkma anıdır(automatic)

  ---------------------------------------------------------------

  static yerel değişkenlerin yeri main fonksiyonu çağrılmadan önce
  ayrılır fakat hayata gelmesi yerel değişkeni olduğu fonksiyonun 
  çağrılmasıyla olur. 
  Fonksiyon çağrılmazsa, değişkenin constructor'ı çağrılmaz 
  ve değişken hayata gelmez.

  ---------------------------------------------------------------

        <--- check global_variable_initialization.png --->

  Global variables are initialized in two phases: 
    1. Static init: memory is set to zero -> Compile Time
    2. Dynamic init: initializing expression is executed -> Run Time

  öyle nesneler var ki, o nesneler için dynamic initialization hiç
  uygulanmaz.

        <--- check ordering_of_dynamic_initialization.png --->

  ---------------------------------------------------------------

  Static initialization aşamasının defaultu "zero initialization"'dır.

  Statik initialization aşamasında, global bir değişken bir 
  sabit ifadesi ile initialize edilirse "constant initialization" denir.

  Static initialization aşamasında, "constant initialization" 
  yapılırsa dynamic initialization aşaması yapılmaz.
    
  ---------------------------------------------------------------

*/

/*
  ---------------------------------------------------------------

  int global_x = expr;

  - if "expr" is a constant expression, constant initialization 
    will be applied and no dynamic initialization will be applied.

  - if "expr" is not a constant expression, zero initialization
    will be applied and dynamic initialization will be applied.

  ---------------------------------------------------------------

  constinit int global_y = expr;

  - if "expr" is a constant expression, constant initialization 
    will be applied and no dynamic initialization will be applied.

  - if "expr" is not a constant expression, because of there is 
    a need for dynamic initialization, and `constinit` keyword 
    has been used, it will become a syntax error.

  - object that have `constinit` keyword guaranteed to 
    constant initialized. 

  ---------------------------------------------------------------
*/

/*
  // constinit = constexpr - const

  constexpr int foo(int x) { return x * x; }

  constexpr int g_x = foo(5);
  // "g_x" data type is const int

  constinit int g_y = foo(5);
  // "g_y" data type is const int

  int main()
  {
    g_x++;  // syntax error
    // error: increment of read-only variable 'g_x'

    g_y++;  // VALID
    std::cout << "g_y = " << g_y << '\n';   // output -> g_y = 26
  }
*/

/*
  constinit auto g_x = 5;

  int foo()
  {
    static constinit int s_count = 0;
    return ++s_count;
  }

  class Myclas {
  public:
    inline static constinit int ms_x = 12;
  };
  
  // global variable initialization (constant initialization)
  // static local variable initialization (constant initialization)
  // static data member initialization (constant initialization)

  // static storage duration variables will 
  // initialized at compile time.
*/

/*
  #include <array>
  #include <cstdio> // std::putchar

  constexpr std::array<int, 4> get_array_4()
  {
    return { 1, 2, 3, 4 };
  }

  constinit auto g_arr4 = get_array_4();

  int main()
  {
    for (auto i : g_arr4)
      std::cout << i << ' ';
    // output -> 1 2 3 4

    g_arr4[1]++;        
    g_arr4[2] += 100;
    // g_arr4 is not const array

    std::putchar('\n');

    for (auto i : g_arr4)
      std::cout << i << ' ';
    // output -> 1 3 103 4
  }
*/

/*
  #include <algorithm>  // std::for_each
  #include <array>      // std::begin, std::end
  #include <cstdio>     // std::putchar

  template <std::size_t N>
  constexpr std::array<int, N> get_array()
  {
    return std::array<int, N>{ 0 };
  }

  constinit auto g_arr = get_array<10>();

  int main()
  {
    for (auto i : g_arr)
      std::cout << i << ' ';
    // output -> 0 0 0 0 0 0 0 0 0 0

    for_each(begin(g_arr), end(g_arr), [](int& elem) { ++elem; });
    // ADL (Argument Dependent Lookup)

    std::putchar('\n');

    for (auto i : g_arr)
      std::cout << i << ' ';
    // 1 1 1 1 1 1 1 1 1 1
  }
*/