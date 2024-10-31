#include <iostream>

/*
                          ============
                          | Concepts |
                          ============
*/

/*
  - template kodların sınırlandırılması(kısıtlanması).
  - to constraint template codes.

  - kısıtlamaların sağlanmadığı durumlarda derleyicinin 
    verdiği hata mesajlarının daha anlaşılır olmasını sağlamak.
  - to make the error messages more understandable 
    when the constraints are not satisfied(fulfilled).

  - bazı durumlarda derleme zamanında, derleyici üstündeki
    yükü azaltır. (compile time süresini azaltır)
*/

/*
  #include <concepts>

  template <std::integral T>
  void func(T x)
  {
    std::cout << "for integral types\n";
  }

  template <std::floating_point T>
  void func(T x)
  {
    std::cout << "for floating point types\n";
  }

  int main()
  {
    func(5);    // output -> for integral types
    func(6LL);  // output -> for integral types
    func(3.14); // output -> for floating point types
  }
*/

/*
  #include <concepts>

  // abbreviated template syntax
  void func(std::integral auto) 
  {
    std::cout << "for integral types\n";
  }

  void func(std::floating_point auto) 
  {
    std::cout << "for floating point types\n";
  }

  int main()
  {
    func(5U);     // output -> for integral types
    func(6L);     // output -> for integral types
    func(3.14F);  // output -> for floating point types
  }
*/

/*
  - std::integral, std::floating_point gibi isimlendirilmiş 
    conceptleri var.
  - kendi constraintlerimizi oluşturup, o constraintleri 
    isimlendirip diğer conceptlerle birlikte kullanabiliriz.

  - `concept` is a keyword.
  - `requires` is a keyword.
*/

/*
  #include <type_traits>

  // only constraint integral types before concepts 

  using namespace std;

  template <typename T, enable_if_t<is_integral_v<T>>* = nullptr>
  void foo(T)
  {
    cout << "for integral types\n";
  }

  template <typename T>
  std::enable_if_t<std::is_integral_v<T>> bar(T)
  {
    cout << "for integral types\n";
  }

  template <typename T>
  void baz(T, std::enable_if_t<std::is_integral_v<T>>* = nullptr)
  {
    cout << "for integral types\n";
  }

  int main()
  {
    // ----------------------------------------------
    
    foo(340);   // output -> for integral types
    bar(340);   // output -> for integral types
    baz(340);   // output -> for integral types

    // ----------------------------------------------

    foo(3.14);  // syntax error
    // error: no matching function for call to 'foo(double)'
    // error: no type named 'type' in 
    // 'struct std::enable_if<false, void>'

    bar(3.14);  // syntax error
    // error: no matching function for call to 'bar(double)'

    baz(3.14);  // syntax error
    // error: no matching function for call to 'baz(double)'

    // ----------------------------------------------
  }
*/

/*
  how to constraint a template 
    1. requires clause 
      - prefix
      - trailing
    2. requires expression
    3. named constraints ===> concept
*/

/*
                      -------------------
                      | requires clause |
                      -------------------
*/

/*
  // prefix reqeuire clause
  template <typename T>
  requires (sizeof(T) > 2) 
  void foo (T x);

  // "foo" function's parameter variable identifier "x" 
  // is NOT visible in prefix requires clause.

  // trailing require clause
  template <typename T>
  void bar (T x) 
  requires (sizeof(T) > 2);

  // "bar" function's parameter variable identifier "x" 
  // is visible in trailing requires clause.

  int main()
  {
    // ----------------------------------------------

    foo('A'); // syntax error
    // error: no matching function for call to 'foo(char)'
    // note: the expression 'sizeof (T) > 2 [with T = char]' 
    // evaluated to 'false'

    foo(45);  // VALID (sizeof(int) > 2)

    // ----------------------------------------------

    bar('B'); // syntax error
    // error: no matching function for call to 'bar(char)'
    // note: the expression 'sizeof (T) > 2 [with T = char]' 
    // evaluated to 'false'

    bar(23.1);  // VALID (sizeof(double) > 2)

    // ----------------------------------------------
  }
*/

/*
  #include <type_traits>  // std::is_integral, std::is_signed

  // both prefix and trailing requires clause
  template <typename T>
  requires std::is_integral_v<T> 
  void foo(T x)
  requires std::is_signed_v<T>;


  template <typename T>
  requires std::is_integral_v<T> && std::is_signed_v<T>
  void bar(T x);

  int main()
  {
    // ----------------------------------------------

    foo(5);   // VALID
    foo(5U);  // syntax error
    // error: no matching function for call to 'foo(unsigned int)'
    // note: constraints not satisfied

    // ----------------------------------------------

    bar(5);   // VALID
    bar(5U);  // syntax error
    // error: no matching function for call to 'bar(unsigned int)'
    // note: constraints not satisfied

    // ----------------------------------------------

    // foo and bar function's constraints are same.
  }
*/

/*
  template <typename T>
  requires (!std::is_integral_v<T>)
  void foo(T x);

  int main()
  {
    foo(6);   // syntax error
    // error: no matching function for call to 'foo(int)'
    // note: the expression '!(is_integral_v<T>) [with T = int]' 
    // evaluated to 'false'

    foo(3.14); // VALID
  }
*/

/*
  template <typename T>
  requires true
  void foo(T x);

  template <typename T>
  requires false
  void bar(T x);

  int main()
  {
    // ----------------------------------------------

    foo(6);     // VALID
    foo(3.14);  // VALID
    foo(8U);    // VALID
    foo(12LL);  // VALID

    // ----------------------------------------------

    bar(6);     // syntax error
    bar(3.14);  // syntax error
    bar(8U);    // syntax error
    bar(12LL);  // syntax error

    // ----------------------------------------------
  }
*/

/*
  #include <concepts>
  #include <iterator>   // std::input_iterator

  template <typename T>
  requires (sizeof(T) > 2) &&               // requires clause
  requires { typename T::size_type; }  &&   // requires expression
  std::input_iterator<T>                    // concept
  class Myclass {

  };
*/

/*
  #include <type_traits>  // std::is_pointer

  template <typename T>
  requires std::is_pointer_v<T> || std::is_reference_v<T>
  class Myclass;

  int main()
  {
    Myclass<int> obj1;    // syntax error
    // error: template constraint failure for 'template<class T>  
    // requires (is_pointer_v<T>) || (is_reference_v<T>) 

    // T is `int` not a pointer or a reference type
  }
*/

/*
  #include <type_traits>  // std::is_integral

  template <typename T>
  concept Integral = std::is_integral_v<T>;
  // named a concept with `Integral`
  // `Integral` is now a template and can be used as a constraint
*/

/*
  #include <type_traits>  // std::is_integral

  template <typename T>
  concept Integral = std::is_integral_v<T>;

  template <typename T>
  requires Integral<T>
  void foo(T x);
  // named concept(`Integral`) used in requires clause

  int main()
  {
    foo(5);             // VALID
    foo<int>(5);        // VALID

    foo(3.14);          // syntax error
    // error: no matching function for call to 'foo(double)'
    foo<double>(3.14);  // syntax error
    // error: no matching function for call to 'foo<double>(double)'
  }
*/

/*
  #include <type_traits>  // std::is_integral

  template <typename T>
  concept Integral = std::is_integral_v<T>;

  // using named concept(`Integral`) as template parameter
  // It is called constrained template parameter.

  template <Integral T>
  void foo(T x);
  
  template <Integral T, Integral U>
  class Myclass {};

  int main()
  {
    // ----------------------------------------------

    foo(10);            // VALID
    foo<long>(10L);     // VALID

    foo(6.28);          // syntax error
    // error: no matching function for call to 'foo(double)'
    foo<float>(6.28F);  // syntax error
    // error: no matching function for call to 'foo<float>(float)'

    // ----------------------------------------------

    Myclass<int, long> m1;      // VALID

    Myclass<double, long> m2;   // syntax error
    //  error: template constraint failure for 
    // 'template<class T, class U>  
    // requires (Integral<T>) && (Integral<U>) class Myclass'

    Myclass<int, double> m3;    // syntax error
    //  error: template constraint failure for 
    // 'template<class T, class U>  
    // requires (Integral<T>) && (Integral<U>) class Myclass'

    // ----------------------------------------------
  }
*/

/*
  template <std::size_t N>
  requires (N > 10)
  class Myclass{};

  int main()
  {
    Myclass<20U> m1;   // VALID

    Myclass<5> m2;    // syntax error
    // In substitution of 'template<long long unsigned int N>  
    // requires  N > 10 class Myclass 
    // [with long long unsigned int N = 5]':
  }
*/

// --------------------- REMINDER ---------------------

/*
  // constraint'in template parametresi ile
  // ilgili olması gerekmiyor.

  template <typename T>
  requires (sizeof(int) != sizeof(long))
  class Myclass{};

  int main()
  {
    Myclass<int> m1;
  }
*/

/*
  #include <random>  // std::mt19937

  template <typename T>
  requires (sizeof(T) > 64)
  class Myclass {};

  int main()
  {
    // ----------------------------------------------

    Myclass<int> m1;  // syntax error
    // note: the expression 'sizeof (T) > 64 [with T = int]' 
    // evaluated to 'false'

    // ----------------------------------------------

    std::cout << "sizeof(mt19937) = " 
              << sizeof(std::mt19937) << '\n'; 
    // output -> sizeof(mt19937) = 2504

    Myclass<std::mt19937> m2;  // VALID

    // ----------------------------------------------
  }
*/

/*
  #include <concepts>     // std::same_as
  #include <type_traits>  // std::is_pointer, std::is_same

  template <typename T>
  requires std::is_pointer_v<T> || std::same_as<T, std::nullptr_t>
  void foo(T);

  template <typename T>
  requires std::is_pointer_v<T> || std::is_same_v<T, std::nullptr_t>
  void bar(T);

  // Those 2 template functions are equivalent.
  // but better using std::same_as concept instead of 
  // std::is_same type trait.

  int main()
  {
    int ival{};

    foo(&ival);   // VALID
    bar(&ival);   // VALID

    foo(nullptr); // VALID
    bar(nullptr); // VALID
  }
*/

/*
  #include <concepts>     // std::convertible_to
  #include <string>

  template <typename T>
  requires std::convertible_to<T, std::string>
  void foo(T);

  template <typename T>
  requires (!std::convertible_to<T, std::string>)
  void bar(T);

  int main()
  {
    foo("hello world"); // VALID

    bar(12.5);          // VALID
    bar("hello world"); // syntax error
  }
*/

/*
  #include <concepts>     // std::convertible_to
  #include <type_traits>  // std::is_convertible

  template <typename T, typename U>
  requires std::convertible_to<T, U>
  void foo(T, U);
  // std::convertible_to is a concept

  template <typename T, typename U>
  requires std::is_convertible_v<T, U>
  void bar(T, U);
  // std::is_convertible is a type trait

  int main()
  {
    foo(2, 5.6);  // VALID
    bar(2, 5.6);  // VALID

    int x{};
    foo(x, &x); // syntax error
    bar(x, &x); // syntax error
    // note: the expression 'is_convertible_v<_From, _To> 
    // [with _From = int; _To = int*]' evaluated to 'false'
    // (int ===> int*) conversion is NOT VALID
  }
*/

/* 
                  ---------------------------
                  | `std::declval` REMINDER |
                  ---------------------------
*/

/*
  #include <utility>  // std::declval

  class AClass {
  public:
    int foo();
    AClass();
  };
  // AClass have a default ctor

  class BClass {
  public:
    int foo();
    BClass(int);
  };
  // BClass does not have a default ctor

  int main()
  {
    // ----------------------------------------------

    decltype(AClass{}.foo());   // VALID

    decltype(BClass{}.foo());   // syntax error
    // error: no matching function for call to 
    // 'BClass::BClass(<brace-enclosed initializer list>)'

    // ----------------------------------------------

    // `std::declval` must be called in unevaluated context

    decltype(std::declval<AClass>().foo());   // VALID

    decltype(std::declval<BClass>().foo());   // VALID

    // ----------------------------------------------
  }
*/

/*
  #include <concepts>     // std::integral
  #include <type_traits>  // std::remove_reference
  #include <utility>      // std::declval
  #include <optional>

  template <typename T>
  requires 
  std::integral<std::remove_reference_t<decltype(*std::declval<T>())>>
  void foo(T);

  // T türünden bir nesne dereference edildiğinde std::integral
  // conceptini sağlıyor ise foo fonksiyonu çağrılabilir.

  int main()
  {
    int ival{};
    double dval{};
    std::optional op{ 45 };

    foo(&ival);   // VALID
    // T type is int*, "*T" expressions type is int 

    foo(&dval);   // syntax error
    // T type is double*, "*T" expressions type is double

    foo(op);      // VALID
    // T type is std::optional<int>, "*T" expressions type is int&
    // remove_reference_t<int&> is int, int is an integral type
  }
*/

/*
                --------------------------------
                | named constraints (concepts) |
                --------------------------------
*/

/*
  #include <concepts> 
  #include <type_traits>  // std::is_integral, std::is_signed

  template <typename T>
  concept Integral = std::is_integral_v<T>;

  template <typename T>
  concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

  template <typename T>
  concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;
*/

/*
  // int olarak kullanılabilecek türleri hedefleyen bir concept
  //  - türün kendisi tam sayı türü olabilir.
  //  - tam sayi türüne implicit veya explicit dönüşüm sağlayabilir.

  #include <concepts>     // std::integral
  #include <type_traits>  // std::is_convertible

  template <typename T>
  concept as_int = std::integral<T> || std::is_convertible_v<T, int>;

  template <typename T>
  requires as_int<T>
  void foo();

  struct AClass {};

  struct BClass{
    operator int() const;
  };

  int main()
  {
    foo<long>();    // VALID

    // ----------------------------------------------

    foo<AClass>();  // syntax error
    // error: no matching function for call to 'foo<AClass>()'
    // In substitution of 'template<class T>  requires  as_int<T> 
    // void foo() [with T = AClass ':]

    foo<BClass>();  // VALID
  }
*/

/*
  #include <concepts>     // std::integral
  #include <type_traits>  // std::is_convertible

  template <typename T>
  concept as_int = std::integral<T> || std::is_convertible_v<T, int>;

  template <as_int T>
  class Myclass {};

  struct AClass {};

  int main()
  {
    Myclass<AClass> m1;  // syntax error
    // error: template constraint failure for 'template<class T>  
    // requires  as_int<T> class Myclass'
    // In substitution of 'template<class T>  requires  as_int<T> 
    // class Myclass [with T = AClass]':
  }
*/

/*
  #include <concepts>     // std::integral
  #include <type_traits>  // std::is_convertible

  template <typename T>
  concept as_int = std::integral<T> || std::is_convertible_v<T, int>;

  // abbreviated template syntax
  void foo_1(as_int auto x);

  template <as_int T>
  void foo_2(T x);
  
  // foo_1 and foo_2 are equivalent template functions.
*/

/*
  #include <concepts>     // std::convertible_to
  #include <string>

  // abbreviated template syntax
  void foo(std::convertible_to<std::string> auto);

  int main()
  {
    foo("hello world"); // VALID

    foo(34);  // syntax error
    // error: no matching function for call to 'foo(int)'
  }
*/

/*
  #include <concepts>   // std::integral

  // abbreviated template syntax
  void foo_1(std::integral auto x);

  template <typename T>
  requires std::integral<T> // requires clause
  void foo_2(T x);

  template <std::integral T>
  void foo_3(T x);

  // foo_1, foo_2 and foo_3 are equivalent template functions.
*/

/*
  #include <concepts>   // std::integral

  class Myclass {
  public:
    void foo(std::integral auto);   // member template function
  };
*/

/*
  #include <concepts>   // std::integral

  int foo();
  double bar();

  int main()
  {
    std::integral auto x = foo();   // VALID

    std::integral auto y = bar();  // syntax error
    // error: deduced initializer does not satisfy 
    // placeholder constraints
  }
*/

/*
  #include <concepts>   // std::integral

  std::integral auto foo(int x) { return x; } // VALID

  std::integral auto bar(int x) { return x * 3.14; }  // syntax error
  // error: deduced return type does not satisfy placeholder constraints
*/

/*
  #include <vector>
  #include <concepts>   // std::integral

  int main()
  {
    using namespace std;

    vector<int> ivec(100);

    for (std::integral auto x : ivec)
      ; // null statement
  }
*/

/*
  // concept is a constant expression

  #include <concepts>   // std::integral, std::floating_point

  template <typename T>
  void func(T x)
  {
    if constexpr (std::integral<T>)
      std::cout << "for integral types\n";
    else if constexpr (std::floating_point<T>)
      std::cout << "for floating point types\n";
    else
      std::cout << "for other types\n";
  }

  int main()
  {
    func('A');            // output -> for integral types
    func(5.);             // output -> for floating point types
    func("hello world");  // output -> for other types
  }
*/

/*
  template <typename T>
  concept add_sub_concept = requires (T x, T y) {
    x + y;  // x + x must be valid
    x - y;  // x - x must be valid
  };

  template <typename T>
  void func(T x)
  {
    if constexpr (add_sub_concept<T>)
      std::cout << "addition and subtraction are VALID\n";
    else
      std::cout << "addition and subtraction are NOT VALID\n";
  }

  int main()
  {
    // ----------------------------------------------

    func(12);   
    // output -> addition and subtraction are VALID
    // int + int and int - int are valid

    // ----------------------------------------------

    int x = 10;
    int* p = &x;

    func(p);    
    // output -> addition and subtraction are NOT VALID
    // two pointers cannot be added

    // ----------------------------------------------
  }
*/

/*
  #include <bit>      // std::has_single_bit
  #include <concepts>

  // std::has_single_bit is checking value has only one bit set.

  template <std::size_t N>
  requires (std::has_single_bit(N)) && (N > 32)
  class Myclass {};

  int main()
  {
    Myclass<5> m1;    // syntax error
    //  In substitution of 'template<long long unsigned int N>  
    // requires  has_single_bit(long long unsigned int)(N)
    // && N > 32 class Myclass [with long long unsigned int N = 5]':

    Myclass<64> m2;   // VALID
    Myclass<128> m3;  // VALID
  }
*/

/*
  #include <concepts>

  constexpr bool is_prime(int x)
  {
    if (x < 2)
      return false;

    if (x % 2 == 0) { return x == 2; };
    if (x % 3 == 0) { return x == 3; };
    if (x % 5 == 0) { return x == 5; };

    for (int i = 7; i * i <= x; i += 2)
      if (x % i == 0)
        return false;

    return true;
  }

  template <int N>
  requires (is_prime(N))
  class Myclass {};

  template <int N>
  concept Prime = is_prime(N);

  template <int N>
  requires Prime<N>
  void func();

  int main()
  {
    // ----------------------------------------------

    Myclass<17> x;  // VALID  

    Myclass<18> y;  // syntax error
    //  error: template constraint failure for 'template<int N>  
    // requires is_prime(int)(N) class Myclass'
    // In substitution of 'template<int N> requires is_prime(int)(N) 
    // class Myclass [with int N = 18]

    // ----------------------------------------------

    func<19>();   // VALID

    func<234>();  // syntax error
    // error: no matching function for call to 'func<234>()'
    // In substitution of 'template<int N> requires Prime<N> 
    // void func() [with int N = 234]

    // ----------------------------------------------
  }
*/

/*
                    -----------------------
                    | requires expression |
                    -----------------------
*/

/*
  - requires expression is a constant expression.
  - requires expression can be used in requires clause.
  - requires expressions are unevaluated context.
*/

/*
  #include <concepts>
  #include <vector>

  template <typename T>
  concept Myconcept = requires (T x) {

    // simple requirements 
    // ------------------------

    x++;  
    // "x++" expression must be valid for type T
  };

  struct AStruct {};

  struct BStruct {
    BStruct operator++(int);
  };

  int main()
  {
    // ----------------------------------------------

    static_assert(Myconcept<int>);
    static_assert(Myconcept<int*>);
    static_assert(Myconcept<std::vector<int>::iterator>);

    // ----------------------------------------------

    static_assert(Myconcept<AStruct>);  // syntax error
    // in requirements with 'T x' [with T = AStruct]
    // the required expression '(x ++)' is invalid

    static_assert(Myconcept<BStruct>);  // VALID

    // ----------------------------------------------
  }
*/

/*
          ----------------------------------------------
          | simple requirements in requires expression |
          ----------------------------------------------
*/

/*
  #include <concepts>

  template <typename T>
  concept Myconcept = requires (T x) {
    *x;
    x[0];
  };

  int main()
  {
    static_assert(Myconcept<int>);  // syntax error
    // in requirements with 'T x' [with T = int]
    // note: the required expression '* x' is invalid
    // note: the required expression 'x[0]' is invalid
  }
*/

/*
  #include <concepts>
  #include <memory>  // std::unique_ptr

  template <typename T>
  concept Myconcept = requires (T p) {
    p == nullptr; 
    // "p == nullptr" expression must be valid for type T
    // "p" must be a type that can be compared with nullptr
  };

  int main()
  {
    constexpr auto elem = Myconcept<int*>;
    // elem -> true

    constexpr auto elem2 = Myconcept<int>;
    // elem2 -> false

    constexpr auto elem3 = Myconcept<std::unique_ptr<int>>;
    // elem3 -> true
  }
*/

/*
  #include <concepts>

  template <typename T>
  concept Myconcept = requires (T x) {
    x < x;
    // "x < x" expression must be valid for type T
  };

  struct AStruct {};

  struct BStruct {
    bool operator<(const BStruct&) const;
  };

  int main()
  {
    // ----------------------------------------------

    constexpr auto elem1 = Myconcept<int>;
    // elem -> true
    // two objects of type `int` can be compared with operator<

    // ----------------------------------------------

    constexpr auto elem2 = Myconcept<AStruct>;
    // elem2 -> false

    constexpr auto elem3 = Myconcept<BStruct>;
    // elem3 -> true

    // ----------------------------------------------
  }
*/

/*
  #include <concepts>

  template <typename T>
  concept Myconcept_1 = requires (T x) {
    x < x;
  };

  template <typename T>
  concept Myconcept_2 = requires (T x, T y) {
    x < y;
  };

  // Myconcept_1 and Myconcept_2's constraints are same.
*/

/*
  #include <concepts>
  #include <type_traits>  // std::is_integral

  template <typename T>
  concept Myconcept = requires (T x) {
    std::is_integral_v<T>;
    // "std::is_integral_v<T>" expression must be valid for type T
  };

  int main()
  {
    constexpr auto elem1 = Myconcept<double>;
    // elem1 -> true

    // "std::is_integral_v<double>" expression's value is false
    // but it is a VALID expression.
  }
*/

/*
  #include <concepts>

  template <typename T>
  concept Myconcept = requires (T p) {
    *p;
    **p; 
  };

  int main()
  {
    constexpr auto elem1 = Myconcept<int*>;
    // elem1 -> false

    constexpr auto elem2 = Myconcept<int**>;
    // elem2 -> true
  }
*/

/*
  #include <concepts>
  #include <string>

  template <typename T>
  concept Myconcept = requires (T p) {
    ++**p; 
  };

  int main()
  {
    constexpr auto elem1 = Myconcept<int**>;
    // elem2 -> true

    constexpr auto elem2 = Myconcept<std::string**>;
    // elem2 -> false
  }
*/

/*
  #include <concepts>

  template <typename T>
  concept Printable = requires (T p) {
    p.print();
    // "p.print()" expression must be valid for type T
    // "p"'s type must have a print member function
  };

  struct AStruct {};

  struct BStruct {
    void print();
  };

  int main()
  {
    constexpr auto elem1 = Printable<int>;
    // elem1 -> false

    constexpr auto elem2 = Printable<std::string>;
    // elem2 -> false

    constexpr auto elem3 = Printable<AStruct>;
    // elem3 -> false

    constexpr auto elem4 = Printable<BStruct>;
    // elem4 -> true
  }
*/

/*
  #include <concepts>  // std::integral

  template <typename T>
  requires ( requires (T x) {  x + x; x - x; } && std::integral<T> )
  class Myclass{};
  // both requires clause and requires expression are used.

  int main()
  {
    Myclass<int> m1;      // VALID

    Myclass<double> m2;   // syntax error
    // required for the satisfaction of 'integral<T>' 
    // [with T = double] evaluated to 'false'
  }
*/

/*
  template <typename T>
  requires ( requires(T x) { 
    std::cout << x; 
    *x;
    sizeof(x) > 100; }) && (sizeof(T) > 4)
  class Myclass {};

  int main()
  {
    Myclass<int*> m1;    // VALID

    Myclass<long long int> m2;  // syntax error
    // in requirements with 'T x' [with T = long long int]
    // note: the required expression '*x' is invalid
  }
*/

/*
  #include <concepts> 
  #include <type_traits>  // std::is_integral

  // most common mistakes

  template <typename T>
  concept Myconcept = requires { 
    sizeof(int) > 100; 
    std::is_integral_v<T>;
  };

  // "sizeof(int) > 100" expression's value is false,
  // "std::is_integral_v<T>" expression's value can be true or false
  // but both are valid expressions
  // so there is not any constraint in requires expression.
  
  int main()
  {
    constexpr bool elem1 = Myconcept<int>;
    // elem1 -> true

    constexpr bool elem2 = Myconcept<float>;
    // elem2 -> true
  }
*/

/*
  #include <concepts>
  #include <type_traits>  // std::is_integral

  template <typename T>
  concept Myconcept = requires {
    requires (sizeof(T) > 100);
    requires (!std::is_integral_v<T>);
  };
  // nested requires clause is used in requires expression.

  struct AStruct {
    char m_buffer[2000] {};
  };

  int main()
  {
    constexpr bool elem1 = Myconcept<AStruct>;
    // elem1 -> true
  }
*/

/*
  template <typename T, typename U>
  concept Myconcept = requires(T x, U y){
    x.foo() || y.bar();
  };

  struct AStruct {
    bool foo();
  };

  struct BStruct {};

  struct CStruct {
    bool bar();
  };

  int main()
  {
    // ----------------------------------------------

    constexpr auto elem1 = Myconcept<AStruct, BStruct>;
    // elem1 -> false

    // Astruct has a member function named "foo"
    // x.foo() simple requirement is satisfied in require expression.
    // but elem1's value is false.

    // "x.foo() || y.bar()" expression is a simple requirement.
    // for "x.foo() || y.bar()" expression to be written,
    // "y.bar()" expression should be valid expression
    // (can be an operand of "||" operator)
    // but because of BStruct does not have a member function named "bar"
    // "y.bar()" expression is invalid.
    // so "x.foo() || y.bar()" is also becomes invalid

    // ----------------------------------------------

    constexpr auto elem2 = Myconcept<AStruct, CStruct>;
    // elem1 -> true

    // ----------------------------------------------
  }
*/

/*
  template <typename T, typename U>
  concept Myconcept = 
  requires(T x) { x.foo(); } || requires(U y) { y.bar(); };

  // This concept does have 2 requires expression

  struct AStruct {
    bool foo();
  };

  struct BStruct {
    bool bar();
  };

  struct CStruct {};

  int main()
  {
    // ----------------------------------------------

    constexpr auto elem1 = Myconcept<AStruct, BStruct>;
    // elem1 -> true

    // ----------------------------------------------
    
    constexpr auto elem2 = Myconcept<CStruct, BStruct>;
    // elem2 -> true

    // ----------------------------------------------

    constexpr auto elem3 = Myconcept<CStruct, CStruct>;
    // elem3 -> false

    // ----------------------------------------------
  }
*/

/*
  template <typename T>
  concept concept_has_foo = requires(T x) { x.foo(); };

  template <typename T>
  concept concept_has_bar = requires(T x) { x.bar(); };

  template <typename T, typename U>
  requires concept_has_foo<T> || concept_has_bar<U>
  class Myclass {};

  struct AStruct {
    void foo();
  };

  struct BStruct {
    bool bar();
  };

  struct CStruct {};

  int main()
  {
    // ----------------------------------------------

    Myclass<AStruct, BStruct> m1;   // VALID

    // ----------------------------------------------

    Myclass<CStruct, BStruct> m2;   // VALID

    // ----------------------------------------------

    Myclass<CStruct, CStruct> m3;   // syntax error
    // note: no operand of the disjunction is satisfied
    // requires concept_has_foo<T> || concept_has_bar<U>

    // ----------------------------------------------
  }
*/

/*
  template <typename T>
  concept Myconcept = requires (T x) { x.foo(); };

  // "x.foo()" expression must be a VALID expression for type T

  class AClass {
  public:
    void foo();
  };

  class BClass {
  private:
    void foo();
  };

  class CClass {
  public:
    void foo() = delete;
  };

  class DClass {
  public:
    void foo(int);
  };

  int main()
  {
    // ----------------------------------------------

    constexpr auto elem1 = Myconcept<AClass>;
    // elem1 -> true

    // ----------------------------------------------

    constexpr auto elem2 = Myconcept<BClass>;
    // elem2 -> false

    // BClass::foo() is a private member function
    // call to foo() is not valid

    // ----------------------------------------------

    constexpr auto elem3 = Myconcept<CClass>;
    // elem3 -> false

    // CClass::foo() is a deleted member function
    // call to foo() is not valid

    // ----------------------------------------------

    constexpr auto elem4 = Myconcept<DClass>;
    // elem4 -> false

    // DClass::foo(int) is a member function with int parameter
    // call to foo() is not valid

    // ----------------------------------------------
  }
*/

/*
  #include <utility>  // std::declval

  template <typename T>
  concept Myconcept_1 = requires (T x) { x.foo(); };

  template <typename T>
  concept Myconcept_2 = requires { std::declval<T>().foo(); };

  // Myconcept_1 and Myconcept_2 are equivalent concepts
  // with equivalent requires expressions.
*/

/*
          --------------------------------------------
          | type requirements in requires expression |
          --------------------------------------------
*/

/*
  - derleyici bir türün var olduğunu ve o türün kullanımının 
  geçerli olduğunu sınamak zorunda.
*/

/*
  #include <concepts>
  #include <vector>

  template <typename T>
  concept Myconcept = requires {
    typename T::value_type;
  };

  struct AStruct {};

  struct BStruct {
    using value_type = int;
  };

  int main()
  {
    constexpr auto elem1 = Myconcept<int>;
    // elem1 -> false

    constexpr auto elem2 = Myconcept<std::vector<int>>;
    // elem2 -> true

    constexpr auto elem3 = Myconcept<std::vector<int>::iterator>;
    // elem3 -> true

    constexpr auto elem4 = Myconcept<AStruct>;
    // elem4 -> false

    constexpr auto elem5 = Myconcept<BStruct>;
    // elem5 -> true
  }
*/

/*
  #include <concepts> // std::integral

  template <std::integral T>
  class Myclass {};

  template <typename T>
  concept Myconcept = requires {
    typename Myclass<T>;
  };

  int main()
  {
    // ----------------------------------------------

    constexpr auto elem1 = Myconcept<int>;
    // elem1 -> true
    // Myclass<int> is a VALID type

    // ----------------------------------------------

    constexpr auto elem2 = Myconcept<double>;
    // elem2 -> false
    // Myclass<double> is NOT a VALID type
    // Myclass is a constrainted template class
    // and its constraint is not satisfied for double type

    // ----------------------------------------------
  }
*/

/*
  #include <concepts>

  template <typename C>
  requires      // requires clause
  requires {    // requires expression
    typename C::value_type::first_type;
    typename C::value_type::second_type;
  }
  class Myclass {};


  template <typename C>
  concept Myconcept = requires {
    typename C::value_type::first_type;
    typename C::value_type::second_type;
  };
*/

/*
  #include <concepts>
  #include <vector>

  template <typename T>
  concept Myconcept = requires (typename T::value_type x) {
    std::cout << x;
  };

  // T türünün nested type olan 
  // "value_type" türüne sahip olması gerekiyor.

  // T türünün "value_type" nested type'ı türünden bir nesnenin
  // ostream nesnesine insert edilebilir olması gerekiyor.

  struct AStruct {};

  int main()
  {
    // ----------------------------------------------

    static_assert(Myconcept<std::vector<int>>); // VALID
    // std::vector<int>::value_type is `int`
    // `int` can be inserted into ostream object

    // ----------------------------------------------

    static_assert(Myconcept<std::vector<AStruct>>);
    // error: static assertion failed
    // note: the required expression '(std::cout << x)' is invalid

    // ----------------------------------------------
  }
*/

/*
          ------------------------------------------------
          | compound requirements in requires expression |
          ------------------------------------------------
*/

/*
  #include <concepts>

  template <typename T>
  concept Myconcept_1 = requires (T x) {
    { x.foo() };  // compound requirement
  };

  template <typename T>
  concept Myconcept_2 = requires (T x) {
    x.foo();      // simple requirement
  };

  // Myconcept_1 and Myconcept_2 are equivalent concepts.
*/

/*
  #include <concepts>

  template <typename T>
  concept Myconcept = requires (T x) {
    { x.foo() } noexcept;   // compound requirement   
  };

  struct AStruct {};

  struct BStruct {
    void foo();
  };

  struct CStruct {
    void foo() noexcept;
  };

  int main()
  {
    // ----------------------------------------------

    constexpr auto elem1 = Myconcept<AStruct>;
    // elem1 -> false

    // ----------------------------------------------

    constexpr auto elem2 = Myconcept<BStruct>;
    // elem2 -> false
    
    // ----------------------------------------------

    constexpr auto elem3 = Myconcept<CStruct>;
    // elem3 -> true

    // ----------------------------------------------
  }
*/

/*
  #include <concepts> // std::same_as

  // compound requirement used in requires expression
  template <typename T>
  concept Myconcept = requires (T x) {
    { x.foo() } noexcept -> std::same_as<int>;  
  };

  // std::same_as<int> ===> std::same_as<decltype(x.foo()), int>
  // concept'e gizli parametere olarak compound requirementta
  // kullanılan expression'ın(x.foo) türü gönderilir.

  // T türünden bir nesnenin foo fonksiyonunun noexcept olması
  // ve foo fonksiyonunun geri dönüş değerinin int olması gerekiyor.

  struct AStruct {
    void foo() noexcept;
  };

  struct BStruct {
    int foo() noexcept;
  };

  int main()
  {
    // ----------------------------------------------

    constexpr auto elem1 = Myconcept<AStruct>;
    // elem1 -> false

    // ----------------------------------------------

    constexpr auto elem2 = Myconcept<BStruct>;
    // elem2 -> true

    // ----------------------------------------------
  }
*/

/*
            --------------------------------------------
            | `std::common_type` metafunction REMINDER |
            --------------------------------------------
*/

/*
  #include <type_traits>  // std::common_type
  #include <string>

  struct AStruct {
    AStruct();
    AStruct(int);
  };

  int main()
  {
    // ----------------------------------------------

    std::common_type<int>::type elem1{};    
    // "elem1"'s type is `int`

    std::common_type_t<int, long> elem2{};
    // "elem2"s type is `long`

    std::common_type_t<int, long, double> elem3{};
    // "elem3"s type is `double`

    // ----------------------------------------------

    std::common_type_t<int, std::string>elem4{};   // syntax error

    //  error: no type named 'type' in 
    // 'struct std::common_type<int, std::string>

    // ----------------------------------------------

    std::common_type_t<const char*, std::string> elem5{};
    // "elem5"s type is `std::string`

    // ----------------------------------------------

    std::common_type_t<AStruct, int> elem6{};
    // "elem6"s type is `AStruct`

    // ----------------------------------------------
  }
*/

/*
  #include <type_traits>  // std::common_type
  #include <concepts>     // std::common_with

  struct AStruct {};

  template <typename T, typename U>
  concept Myconcept_1 = requires {
    typename std::common_type_t<T, U>;  // type requirement
  };

  template <typename T, typename U>
  concept Myconcept_2 = requires (T x, U y) {
    { x + y } -> std::common_with<U>;  // compound requirement
  };

  // "x + y" expression's type should be the same type as U type

  int main()
  {
    // ----------------------------------------------

    static_assert(Myconcept_1<int, double>);  // VALID
    static_assert(Myconcept_1<const char*, std::string>); // VALID

    static_assert(Myconcept_1<AStruct, int>); // syntax error
    // error: static assertion failed
    // note: the required type 
    // 'std::common_type_t<_Tp1, _Tp2>' is invalid

    // ----------------------------------------------

    static_assert(Myconcept_2<int, double>);  // VALID
    // int and double type's common type is double 
    //  x + y -> double and U's type is double

    // ----------------------------------------------
  }
*/

/*
            ----------------------------------------
            | nested requirement (requires clause) |
            ----------------------------------------
*/

/*
  #include <concepts>  // std::floating_point

  template <typename T>
  concept Myconcept = requires (T x) {
    *x;                       
    // simple requirement

    typename T::value_type;   
    // type requirement

    { x.foo() } noexcept -> std::floating_point;
    // compound requirement

    requires (std::is_integral_v<T>);
    // nested requirement (requires clause)
  };
*/

/*
  // we want create a concept with these requirements:
  //  - T type must be a reference type
  //  - T type can be dereferenced

  #include <concepts>
  #include <type_traits>  // std::is_reference

  template <typename T>
  concept Myconcept_1 = 
    std::is_reference_v<T> && requires(T x) { *x; };


  template <typename T>
  concept Myconcept_2 = requires (T x) {
    *x;                                   // simple requirement
    requires (std::is_reference_v<T>);    // nested requirement
  };

  // Myconcept_1 and Myconcept_2 are equivalent concepts.
*/

/*
  #include <type_traits>  // std::is_pointer, std::is_reference

  template <typename T>
  concept Pointer = std::is_pointer_v<T>;

  template <typename T>
  concept Reference = std::is_reference_v<T>;

  template <typename T>
  concept PointerOrReference = Pointer<T> || Reference<T>;

  template <typename T>
  concept Myconcept = requires {
    requires (Pointer<T> || Reference<T>);  
    // nested requirement (reqires clause)
  };

  // Myconcept and PointerOrReference are equivalent concepts.
*/


// ---------------------------------------------------------------
// ---------------------------------------------------------------
// ---------------------------------------------------------------
// ---------------------------------------------------------------
// ---------------------------------------------------------------


/*
  // constraint : 
  //  - template argument must be an integral type

  #include <concepts>  // std::integral

  // ----------------------------------------------

  template <typename T>
  concept Concept_1 = std::integral<T>;

  // ----------------------------------------------

  template <typename T>
  concept Concept_2 = requires {
    requires std::integral<T>;  
  };
  // nested requirement in requires expression

  // ----------------------------------------------

  template <std::integral T>
  class Myclass;

  template <typename T>
  concept Concept_3 = requires {
    typename Myclass<T>;
  };
  // type requirement in requires expression

  // ----------------------------------------------

  // ------------ !!!!! ------------
  template <typename T>
  concept Concept_4 = requires {
    std::integral<T>;
  };
  // "std::integral<T>" expression must be a valid expression
  // "std::integral<double>" and "std::integral<int>" 
  // are both valid expressions
  // this concept does not have any constraint
  // ------------ !!!!! ------------

  // ----------------------------------------------

  int main()
  {
    // ----------------------------------------------

    constexpr auto elem1 = Concept_1<double>;
    // elem1 -> false

    constexpr auto elem2 = Concept_1<int>;
    // elem2 -> true

    // ----------------------------------------------

    constexpr auto elem3 = Concept_2<double>;
    // elem3 -> false

    constexpr auto elem4 = Concept_2<int>;
    // elem4 -> true

    // ----------------------------------------------

    constexpr auto elem5 = Concept_3<double>;
    // elem5 -> false

    constexpr auto elem6 = Concept_3<int>;
    // elem6 -> true

    // ----------------------------------------------

    constexpr auto elem7 = Concept_4<double>;
    // elem7 -> true

    constexpr auto elem8 = Concept_4<int>;
    // elem8 -> true

    // ----------------------------------------------
  }
*/

/*
  // concept1 constraints :
  //  - must be an operand of dereference operator
  //  (AND)
  //  - must be an operand of subscript operator

  // concept2 constraints :
  //  - must be an operand of dereference operator 
  //  (OR)
  //  - must be an operand of subscript operator

  template <typename T>
  concept Dereferencable = requires (T x) { *x; };

  template <typename T>
  concept Subscriptable = requires (T x) { x[0]; };


  // -------------------------------------------
  // ---------------- concept1  ----------------
  // -------------------------------------------

  template <typename T>
  concept Concept_1_1 = requires (T x) {
    *x;
    x[0];
  };

  template <typename T>
  concept Concept_1_2 = Dereferencable<T> && Subscriptable<T>;

  // -------------------------------------------
  // ---------------- concept2  ----------------
  // -------------------------------------------

  template <typename T>
  concept Concept_2_1 = Dereferencable<T> || Subscriptable<T>;


  template <typename T>
  concept Concept_2_2 = 
    requires (T x) { *x; } || requires (T x) { x[0]; };
*/

/*
  #include <functional>  // std::hash

  template <typename T>
  void foo(T elem)
  {
    std::hash<T> x;
  }

  struct AStruct {};

  struct BStruct {};

  template <>
  struct std::hash<BStruct> {
    std::size_t operator()(const BStruct&) const;
  };

  int main()
  {
    foo(34);  // VALID
    // std::hash class template has a specialization for `int` type

    foo(AStruct{}); // syntax error
    // error: use of deleted function 'std::hash<AStruct>::hash()'

    foo(BStruct{}); // VALID
  }
*/

/*
  // scenario :
  // will use an unordered_set or unordered_map in a generic code,
  // and wanted to find out if a type has a std::hash specialization.
  // want to create a constraint for this.

  #include <concepts>
  #include <functional>  // std::hash

  // ----------------------------------------------

  template <typename T>
  concept wrong_has_hash = requires {
    typename std::hash<T>;
  };
  // Wrong concept because constraint is checking if
  // "std::hash<T>" expression is a valid type or not.
  // It did not check if T type has a std::hash specialization

  void foo(wrong_has_hash auto){}

  // ----------------------------------------------

  template <typename T>
  concept true_has_hash = requires {
    std::hash<T>{};
  };
  // True concept because constraint is checking if
  // an object can be created from "std::hash<T>" type
  // If T type has a std::hash specialization, object can be created

  void bar(true_has_hash auto){}  

  // ----------------------------------------------

  struct AStruct {};

  template <>
  struct std::hash<AStruct> {
    std::size_t operator()(const AStruct&) const;
  };

  struct BStruct {};


  int main()
  {
    // ----------------------------------------------

    foo(AStruct{});  // VALID
    foo(BStruct{});  // VALID

    // ----------------------------------------------

    bar(AStruct{});  // VALID
    bar(BStruct{});  // syntax error
    // error: no matching function for call to 'bar(BStruct)'
    // note: the required expression 'std::hash<_Tp>{}' is invalid

    // ----------------------------------------------
  }
*/

/*
  #include <concepts>   
  // std::convertible_to, std::same_as, std::integral

  template <typename T>
  concept HasValidFooBar = requires (T x) {
    { x.foo() } noexcept -> std::convertible_to<bool>;
    { x.bar() } noexcept -> std::same_as<bool>;
  };

  // ----------------------------------------------
  //  Question : where can we use concept itself ?
  // ----------------------------------------------

  // 1. In a class template we want this template parameter must
  //    satisfies this HasValidFooBar concepts constraints.

  template <typename T>
  requires HasValidFooBar<T>
  class Myclass_1 {};

  // ----------------------------------------------

  // 2. Can be used as constraint template parameter

  template <HasValidFooBar T>
  class Myclass_2 {};

  // ----------------------------------------------

  // 3. In a function template we want this template parameter must
  //    satisfies this HasValidFooBar concepts constraints.

  template <typename T>
  requires HasValidFooBar<T>    // prefix requires clause
  T f1(T x) {}

  template <typename T>
  T f2(T x) 
  requires HasValidFooBar<T> {}  // trailing requires clause

  // ----------------------------------------------

  // 4. Can be used as constraint template parameter 
  //    in a function template

  template <HasValidFooBar T>
  T f3(T x);

  // ----------------------------------------------

  // 5. Can be used as an abbreviated template syntax 
  //    in a function template
  
  HasValidFooBar auto f4(HasValidFooBar auto x);

  // function's return type is a type that satisfies
  // HasValidFooBar concept constraints
  // and 
  // function's parameter type is a type that satisfies
  // HasValidFooBar concept constraints

  // ----------------------------------------------

  // 6. When creating another concept, this concept can be
  //    be used as a boolean expression.

  template <typename T>
  concept Concept_1 = std::integral<T> || HasValidFooBar<T>;

  // ----------------------------------------------

  // 7. Can be used in familiar template syntax in a 
  //    lambda expression

  auto fn = []<HasValidFooBar T>(T x) -> HasValidFooBar auto {};

  auto fn2 = [](HasValidFooBar auto x) -> HasValidFooBar auto {};

  // ----------------------------------------------

  // 8. Can be used in inside requires expression's 
  //    compound requirement 
  //    to constraint the return type of an expression

  template <typename T>
  concept Concept_2 = requires (T x) {
    { ++x } noexcept -> HasValidFooBar; 
  };

  // ----------------------------------------------

  // 9. Can be used in inside requires expression
  //    as a requires clause

  template <typename T>
  concept Concept_3 = requires {
    requires HasValidFooBar<T>;
  };

  // ----------------------------------------------
*/

/*
  template <int N>
  concept Less50 = (N < 50);

  template <auto val>
  requires Less50<val>
  class Myclass {};

  int main()
  {
    Myclass<34> m1;  // VALID

    Myclass<56> m2;  // syntax error
    // In substitution of 'template<auto val> requires Less50<int()> 
    // class Myclass [with auto val = 56]
  }
*/

/*
  // before C++20 how to calling a function with a 
  // specific type of argument

  // ------------- WAY 1 -------------

  template <typename T>
  void func(T) = delete;

  void func(int);

  int main()
  {
    func(5);  // VALID

    func(12.1); // syntax error
    // error: use of deleted function 'void func(T) [with T = double]'
  }
*/

/*
  // before C++20 how to calling a function with a 
  // specific type of argument

  // ------------- WAY 2 ------------- (using SFINAE)

  #include <type_traits>  // std::enable_if, std::is_same

  using namespace std;

  template <typename T, enable_if_t<is_same_v<T, int>>* = nullptr>
  void func(T);

  int main()
  {
    func(5);  // VALID

    func(12.1); // syntax error
    // error: no matching function for call to 'func(double)'
  }
*/

/*
  // after C++20 how to calling a function with a 
  // specific type of argument

  #include <concepts> // std::same_as

  // ------------- WAY 1 -------------

  template <typename T>
  requires std::same_as<T, int>
  void foo(T);

  // ------------- WAY 2 -------------

  template <std::same_as<int> T>
  void bar(T);

  // ------------- WAY 3 -------------

  void baz(std::same_as<int> auto);

  int main()
  {
    // ----------------------------------------------

    foo(5);   // VALID
    bar(6);   // VALID
    baz(7);   // VALID

    // ----------------------------------------------

    foo(12.1); // syntax error
    bar(13.1); // syntax error
    baz(14.1); // syntax error
    // In substitution of 'template<class T> requires 
    // same_as<T, int> void func(T) [with T = double]

    // ----------------------------------------------
  }
*/

/*
  #include <concepts>     // std::invocable
  #include <type_traits>  // std::is_invocable

  // constraint : 
  //  - "fn" callable must be called with an integer argument

  // abbreviated function template syntax
  void func_1(std::invocable<int> auto fn)
  {
    fn(10);
  }

  // prefix requires clause
  template <typename F>
  requires std::invocable<F, int>
  void func_2(F fn)
  {
    fn(10);
  }

  // constrained template parameter
  template <std::invocable<int> F>
  void func_3(F fn)
  {
    fn(10);
  }

  int main()
  {
    func_1([](int x){ return x * x; });  // VALID

    func_2([](std::string x){ return x + x; }); // syntax error
    // In substitution of 'template<class F> requires 
    // invocable<F, int> void func_2(F) 
    // [with F = main()::<lambda(std::string)>]':
  }
*/

/*
  #include <type_traits>  // std::is_same
  #include <concepts>     // std::invocable

  auto func(std::invocable<int, int, int> auto fn)
  {
    return fn(10, 20, 30);
  }

  int main()
  {
    using namespace std;
    std::cout << std::boolalpha;

    auto f1 = [](int a, int b, double c){ return a + b * c; };
    // even if lambda expression's 3rd parameter is double
    // it will satistify the constraint 
    // because when "fn(10, 20, 30)" expression is evaluated 
    // 30 will be implicitly converted to double
    // "a + b * c" expression will return a double value

    std::cout << is_same_v<double, decltype(func(f1))> << '\n';
    // output -> true
  }
*/

/*
                          ----------------
                          | subsumption  |
                          ----------------
*/

/*
  #include <concepts> // std::integral

  void foo(std::integral auto)
  {
    std::cout << "foo(std::integral auto)\n";
  }

  void bar(std::integral auto)
  {
    std::cout << "bar(std::integral auto)\n";
  }

  void bar(std::unsigned_integral auto)
  {
    std::cout << "bar(std::unsigned_integral auto)\n";
  }

  int main()
  {
    // ----------------------------------------------

    foo(5);     // output -> foo(std::integral auto)
    foo(12UL);  // output -> foo(std::integral auto)
    
    // ----------------------------------------------

    bar(5);     // output -> bar(std::integral auto)
    bar(12UL);  // output -> bar(std::unsigned_integral auto)

    // std::unsigned_integral is subsume std::integral

    // daha fazla kısıtlayıcı olan concept
    // daha az kısıtlayıcı olanı kapsıyor.

    // ----------------------------------------------
  }
*/

/*
  template <typename T>
  concept Concept_1 = requires (T x) {
    x.foo();  // simple requirement
  };

  template <typename T>
  concept Concept_2 = Concept_1<T> && requires (T x) {
    x.bar();  // simple requirement
  };

  void func(Concept_1 auto)
  {
    std::cout << "foo(Concept_1 auto)\n";
  }

  void func(Concept_2 auto)
  {
    std::cout << "foo(Concept_2 auto)\n";
  }

  struct AStruct {
    void foo();
  };

  struct BStruct {
    void foo();
    void bar();
  };

  int main()
  {
    func(AStruct{});  // output -> foo(Concept_1 auto)
    // AStruct object is satisfy "Concept_1" concept's constraints
    // AStruct object is NOT satisfy "Concept_2" concept's constraints

    func(BStruct{});  // output -> foo(Concept_2 auto)
    // BStruct object is satisfy "Concept_1" concept's constraints
    // BStruct object is satisfy "Concept_2" concept's constraints

    // Concept_2 is subsume Concept_1
    // Concept_2, Concept_1'den daha çok kısıtlamaya(constraint) sahip.
    // bu sebeple Concept_2, Concept_1'i kapsıyor.
  }
*/

/*
  template <typename T>
  concept Concept_1 = requires (T x) {
    x.foo();  // simple requirement
  };

  template <typename T>
  concept Concept_2 = Concept_1<T> || requires (T x) {
    x.bar();  // simple requirement
  };

  void func(Concept_1 auto)
  {
    std::cout << "foo(Concept_1 auto)\n";
  }

  void func(Concept_2 auto)
  {
    std::cout << "foo(Concept_2 auto)\n";
  }

  struct AStruct {
    void foo();
  };

  struct BStruct {
    void foo();
    void bar();
  };

  int main()
  {
    func(AStruct{});  // output -> foo(Concept_1 auto)
    func(BStruct{});  // output -> foo(Concept_1 auto)

    // Concept_1 is subsume Concept_2
  }
*/

/*
  // concepts needed for subsumption mechanism to work

  template <typename T>
  requires ( requires (T x) {
    x.foo();
  })
  void func(T x)
  {
    std::cout << "func that have 1 simple requirement\n";
  }

  template <typename T>
  requires ( requires (T x) {
    x.foo();
    x.bar();
  })
  void func(T x)
  {
    std::cout << "func that have 2 simple requirement\n";
  }

  struct AStruct {
    void foo();
  };

  struct BStruct {
    void foo();
    void bar();
  };

  int main()
  {
    func(AStruct{});  
    // output -> func that have 1 simple requirement
    // only satisfy the first function overload

    func(BStruct{});  // syntax error
    // error: call of overloaded 'func(BStruct)' is ambiguous
    // satisfies both function overloads
  }
*/

/*
  #include <concepts>  
  // std::integral, std::floating_point, std::same_as

  template <typename T>
  concept integral_or_floating = 
    std::integral<T> || std::floating_point<T>;

  template <typename T>
  concept integral_and_char =
    std::integral<T> && std::same_as<T, char>;

  // overload(1) 
  void func(std::integral auto)
  {
    std::cout << "func(std::integral auto)\n";
  }

  // overload(2)
  void func(integral_or_floating auto)
  {
    std::cout << "func(integral_or_floating auto)\n";
  }

  // overload(3)
  void func(std::same_as<char> auto)
  {
    std::cout << "func(std::same_as<char> auto)\n";
  }

  // overload(4)
  void func(integral_and_char auto)
  {
    std::cout << "func(integral_and_char auto)\n";
  }

  int main()
  {
    // ----------------------------------------------

    func(5);  // output -> func(std::integral auto)
    // overload(1)'s constraint is satisfied
    // overload(2)'s constraint is satisfied
    // overload(3)'s constraint is NOT satisfied
    // overload(4)'s constraint is NOT satisfied

    // std::integral is integral_or_floating concept 

    // ----------------------------------------------

    func(12.5); // output -> func(integral_or_floating auto)
    // overload(1)'s constraint is NOT satisfied
    // overload(2)'s constraint is satisfied
    // overload(3)'s constraint is NOT satisfied
    // overload(4)'s constraint is NOT satisfied

    // ----------------------------------------------

    func('A');
    // overload(1)'s constraint is satisfied
    // overload(2)'s constraint is satisfied
    // overload(3)'s constraint is satisfied
    // overload(4)'s constraint is satisfied

    // integral_and_char subsume 
    // std::integral and std::same_as<char> and integral_or_floating

    // ----------------------------------------------

    // if we remove overload(4) no subsume relation is found

    func('A');  // syntax error
    // error: call of overloaded 'func(char)' is ambiguous

    // ----------------------------------------------
  }
*/

/*
                  -----------------------------
                  | standart library concepts |
                  -----------------------------
*/

/*
              <--- check standart_concepts.png --->
*/

/*
  // std::regular -> 
  // default init, copies, moves, swaps, and equality comparisons

  #include <concepts>  // std::regular
  #include <string>

  void func(std::regular auto){}

  struct AClass {};

  struct BClass {
    bool operator==(const BClass&) const = default;
  };

  int main()
  {
    func(5);  // VALID

    func(std::string{ "hello world" });   // VALID
    
    func(AClass{});  // syntax error
    // note: the required expression '(__t == __u)' is invalid
    // note: the required expression '(__t != __u)' is invalid
    // note: the required expression '(__u == __t)' is invalid
    // note: the required expression '(__u != __t)' is invalid
    // equality comparisons constraint is not satisfied

    func(BClass{});  // VALID
    // equality comparisons constraint is satisfied
  }
*/

/*
  // std::totally_ordered_with 
  // '<', '>', '<=', '>=', '==', '!=' operations should be valid

  #include <concepts>  // std::totally_ordered_with

  void func(std::totally_ordered_with<int> auto){}

  struct AStruct {};

  int main()
  {
    func(34);   // VALID
    func(2.4);  // VALID
    func(66U);  // VALID

    func(AStruct{});  // syntax error
    // note: the required expression '(__t == __u)' is invalid
    // note: the required expression '(__t != __u)' is invalid
    // note: the required expression '(__u == __t)' is invalid
    // note: the required expression '(__u != __t)' is invalid
    // equality comparisons constraint is not satisfied
  }
*/

/*
  #include <concepts>  // std::copyable

  void func(std::copyable auto){}

  struct AStruct {
    AStruct(const AStruct&) = delete;
  };

  struct BStruct {
  private:
    BStruct(const BStruct&);
  };

  struct CStruct {};

  int main()
  {
    func(AStruct{});  // syntax error
    // error: no matching function for call to 
    // 'AStruct::AStruct(<brace-enclosed initializer list>)'

    func(BStruct{});  // syntax error
    // error: no matching function for call to 
    // 'BStruct::BStruct(<brace-enclosed initializer list>)'

    func(CStruct{});  // VALID
  }
*/

/*
  #include <concepts> // std::convertible_to
  #include <string> 

  void foo(const std::convertible_to<std::string> auto& x)
  {
    std::string str = x;
  }

  void bar(const auto& x)
  requires std::convertible_to<decltype(x), std::string>
  {
    std::string str = x;
  }

  template <std::convertible_to<std::string> T>
  void baz(const T& x)
  {
    std::string str = x;
  }

  template <typename T>
  requires std::convertible_to<T, std::string>
  void func(const T& x)
  {
    std::string str = x;
  }

  // foo, bar, baz, func functions are equivalent.
*/

/*
  #include <concepts> 
  #include <string>
  #include <memory>

  template <std::convertible_to<bool> T>
  void foo(T x)
  {
    std::cout << static_cast<bool>(x) << '\n';
  }

  int main()
  {
    std::cout << std::boolalpha;

    int ival{ 245 };

    // ----------------------------------------------

    foo(ival);      // output -> true

    // ----------------------------------------------

    foo(&ival);     // output -> true

    // conversion from pointer types to bool is VALID

    // ----------------------------------------------

    foo(nullptr);   // syntax error 
    // In substitution of 'template<class T> requires 
    // convertible_to<T, bool> void foo(T) [with T = std::nullptr_t]'

    // NO conversion from nullptr to bool.

    // ----------------------------------------------

    foo("hello world");  // output -> true

    // ----------------------------------------------

    foo(std::string{ "hello galaxy "});  // syntax error
    //  In substitution of 'template<class T> requires 
    // convertible_to<T, bool> void foo(T) 
    // [with T = std::__cxx11::basic_string<char>]':

    // NO implicit conversion from std::string to bool.

    // ----------------------------------------------

    foo(std::make_unique<int>(10));  // syntax error

    // conversion from std::unique_ptr to bool is VALID
    // but because of the type conversion operator function 
    // is explicit, it will be a syntax error

    // ----------------------------------------------
  }
*/

/*
  #include <concepts>   // std::copyable

  template <std::copyable T>
  class Myclass {};

  struct AStruct {};

  struct BStruct {
    BStruct() = default;
    BStruct(const BStruct&) = default;
    BStruct& operator=(const BStruct&) = default; 
  };

  struct CStruct {
    CStruct() = default;
    CStruct(const CStruct&) = delete;
    CStruct& operator=(const CStruct&) = delete;
  };

  struct DStruct {
    DStruct() = default;
    DStruct(const DStruct&) = default;
    DStruct& operator=(const DStruct&) = default;
    DStruct(DStruct&&) = delete;
    DStruct& operator=(DStruct&&) = delete;
  };

  int main()
  {
    // ----------------------------------------------

    Myclass<int> m1;      // VALID

    // ----------------------------------------------

    Myclass<AStruct> m2;  // VALID
    // copy members are implicitly declared defaulted by the compiler

    // ----------------------------------------------

    Myclass<BStruct> m3;  // VALID
    // copy members are user declared defaulted 

    // ----------------------------------------------

    Myclass<CStruct> m4;  // syntax error
    // copy members are user declared deleted
    // In substitution of 'template<class T> requires copyable<T> 
    // class Myclass [with T = CStruct]'

    // ----------------------------------------------

    Myclass<DStruct> m5;  // syntax error
    // copy members are user declared defaulted
    // move members are user declared deleted

    // std::copyable concept includes std::movable concepts constraints
    // so it will be a sytax error 

    // when move operatios wanted to be applied, because of 
    // move members are deleted, fallback mechanism CAN NOT 
    // be applied.

    // from Microsoft's STL implementation
    //  _EXPORT_STD template <class _Ty>
    //  concept copyable = copy_constructible<_Ty>
    //  && movable<_Ty>
    //  && assignable_from<_Ty&, _Ty&>
    //  && assignable_from<_Ty&, const _Ty&>
    //  && assignable_from<_Ty&, const _Ty>;

    // ----------------------------------------------
  }
*/

/*
  #include <iterator>
  #include <vector>
  #include <concepts> // std::indirect_unary_predicate

  template <typename F, typename Iter>
  requires std::indirect_unary_predicate<F, Iter>
  void bar(F fn, Iter iter)
  {
    std::cout << *iter << " : " << fn(*iter) << '\n';
  }
  // indirect_unary_predicate : 
  //  predicate can be called with *iter(int)

  int main()
  {
    std::cout << std::boolalpha;

    const auto pred = [](int x) { return x % 5 == 0; };
    std::vector ivec{ 10, 11, 12, 13, 14, 15 };

    for (auto iter = ivec.begin(); iter != ivec.end(); ++iter)
      bar(pred, iter);

    // output ->
    //  10 : true
    //  11 : false
    //  12 : false
    //  13 : false
    //  14 : false
    //  15 : true
  }
*/

/*
  #include <iterator>
  #include <vector>
  #include <concepts> // std::indirect_unary_predicate

  template <typename F, typename Iter>
  requires std::indirect_unary_predicate<F, Iter>
  void bar(F fn, Iter iter)
  {
    std::cout << *iter << " : " << fn(*iter) << '\n';
  }
  // indirect_unary_predicate : 
  //  predicate can be called with *iter(int)

  int main()
  {
    std::cout << std::boolalpha;

    const auto pred = [](int x) { return x % 5 == 0; };
    std::vector<const char*> cstr_vec{ 
      "hello", "world", "we", "are", "live", "from", "Istanbul"  };

    for (auto iter = cstr_vec.begin(); iter != cstr_vec.end(); ++iter)
      bar(pred, iter);  // syntax error
    
    // we can not pass const char* argument to pred callable
    // because pred callable's argument type is int
    // so sending *iter(const char*) type is syntax error
  }
*/

/*
  #include <concepts>  // std::invocable

  void foo(std::invocable<int> auto fn, int x)
  {
    fn(x);
  }

  void bar(std::invocable<int, double> auto fn, int x)
  {
    fn(x, 1);
  }

  int main()
  {
    // ----------------------------------------------

    foo([](int x){ return x * x; }, 10);

    // [](int x){ return x * x; } closure object can be 
    // called with int argument 

    // ----------------------------------------------

    bar([](int x){ return x * x; }, 10);  // syntax error

    // [](int x){ return x * x; } closure object can be 
    // called with int argument 
    // but CAN NOT be called with (int, double) arguments

    // ----------------------------------------------
  }

  // std::invocable is variadic parameter concept 
  // (1st parameter is callable, 2nd parameter is variadic arguments)

  // std::invocable<int> means callable must be called 
  // with int argument.
  // std::invocable<int, double> means callable must be called 
  // with int as first and double as second argument.
*/

/*
  #include <concepts>   // std::totally_ordered
  #include <string>

  // std::totally_ordered concept's constraints are 
  // '<', '>', '<=', '>=', '==', '!=' operations should be VALID

  template <typename T>
  concept TotallyOrdered = requires(T x, T y) {
    x == y;
    x != y;
    x < y;
    x <= y;
    x > y;
    x >= y;
  };

  void foo(std::totally_ordered auto x){}

  struct AStruct {};

  struct BStruct {
    auto operator<=>(const BStruct&) const = default;
  };

  struct CStruct {
    bool operator==(const CStruct&) const;
  };

  int main()
  {
    // ----------------------------------------------

    foo(1);   // VALID

    // ----------------------------------------------
    foo(std::string{ "hello world" });    // VALID

    // ----------------------------------------------

    foo(AStruct{});  // syntax error
    // note: the required expression '(__t == __u)' is invalid
    // note: the required expression '(__t != __u)' is invalid
    // note: the required expression '(__u == __t)' is invalid
    // note: the required expression '(__u != __t)' is invalid

    // ----------------------------------------------

    foo(BStruct{});  // VALID

    // ----------------------------------------------

    foo(CStruct{});  // syntax error
    // note: the required expression '(__t < __u)' is invalid
    // note: the required expression '(__t > __u)' is invalid
    // note: the required expression '(__t <= __u)' is invalid
    // note: the required expression '(__t >= __u)' is invalid
    // note: the required expression '(__u < __t)' is invalid
    // note: the required expression '(__u > __t)' is invalid
    // note: the required expression '(__u <= __t)' is invalid
    // note: the required expression '(__u >= __t)' is invalid

    // ----------------------------------------------
  }
*/

/*
  #include <ranges>   // std::ranges::range 
  #include <vector>

  void func(const std::ranges::range auto&){}

  // std::ranges::range concept's constraints are
  // argument type must be sent ranges::begin and ranges::end functions

  // https://en.cppreference.com/w/cpp/ranges/range
  // template< class T >
  // concept range = requires( T& t ) {
  //     ranges::begin(t);
  //     ranges::end (t);
  // };

  struct AStruct {};

  struct BStruct {
    int* begin();
    int* end();
  };

  int main()
  {
    std::vector<int> ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; 

    // ----------------------------------------------

    auto beg = std::ranges::begin(ivec);  // VALID
    auto end = std::ranges::end(ivec);    // VALID

    // because of "beg" and "end" variables are valid
    // vector<int> type is a valid type for std::ranges::range concept

    func(ivec);  // VALID

    // ----------------------------------------------

    func(AStruct{});  // syntax error
    // the required expression 'std::ranges::_Cpo::begin(__t)' 
    // is invalid
    // the required expression 'std::ranges::_Cpo::end(__t)' 
    // is invalid

    // ----------------------------------------------

    func(BStruct{}); // VALID

    // ----------------------------------------------
  }
*/