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
    // error: no type named 'type' in 'struct std::enable_if<false, void>'

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
