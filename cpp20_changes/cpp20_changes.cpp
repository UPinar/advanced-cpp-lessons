#include <iostream>

/*
                      ======================
                      | Changes with C++20 |
                      ======================
*/

/*
            ----------------------------------------------
            | signed integral types right and left shift |
            ----------------------------------------------
*/

/*
  // signed integral types left shift(<<) is NOT UB since C++20

  #include <format>

  int main()
  {
    int ival = -10;

    std::cout << std::format("{0:032b}  {0}\n", ival);
    // output -> -0000000000000000000000000001010  -10

    ival <<= 1; 
    // when negative signed integral types are left shifted
    // undefined behavior(UB) before C++20
    // VALID after C++20

    std::cout << std::format("{0:032b}  {0}\n", ival);
    // output -> -0000000000000000000000000010100  -20
  }
*/

/*
  // signed integral types right shift(>>) 
  // will always be done by arithmetic feed since C++20

  int main()
  {
    int ival = -1;    
    // 0xFFFFFFFF -> 0b11111111'11111111'11111111'11111111
    
    // --------------------------------------------------

    // before C++20, right shift(>>) on signed integral types
    // is implementation defined
    // depends on the compiler 
    // can be arithmetic feed, or can be logical right shift

    // -> arithmetic feed         (most significant bit is copied)
    // if negative, 1 is copied, if positive, 0 is copied
    // -> logical right shift     (0 is copied)

    // since C++20, right shift(>>) on signed integral types
    // is always be arithmetic feed

    // --------------------------------------------------

    if ((ival >> 1) == -1)
      std::cout << "arithmetic feed\n";
    else
      std::cout << "logical right shift\n";

    // output -> arithmetic feed

    // --------------------------------------------------
  }
*/

/*
                ----------------------------------
                | ssize global function template |
                ----------------------------------
*/

/*
  // for solving comparison warnings 
  // with signed and unsigned integer types
  // C++20 added std::ssize global function template

  #include <vector>   // std::size, std::ssize

  int main()
  {
    std::vector<int> ivec{ 1, 2, 3, 4, 5 };

    // --------------------------------------------------

    for (int i{}; i < std::size(ivec); ++i)
      ; // null statement

    // warning: comparison of integer expressions of 
    // different signedness: 'int' and {aka 'long long unsigned int'} 

    // --------------------------------------------------

    for (int i{}; i < std::ssize(ivec); ++i)
      ; // null statement

    // --------------------------------------------------

    int arr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for (int i{}; i < std::ssize(arr); ++i)
      ; // null statement

    // --------------------------------------------------
  }
*/

/*
                  -----------------------------
                  | struct's bitfield members |
                  -----------------------------
*/

/*
  // since C++20, struct's bitfield members 
  // can have default member initializer and 
  // in-class member initializer

  struct MyStruct
  {
    int a : 4{ 7 }; // default member initializer
    int b : 3 = 5;  // in-class initializer
    int c : 1;
  };
*/

/*
              ----------------------------------------
              | `typename` keyword in dependent type |
              ----------------------------------------
*/

/*
  // for compiler to know that `T::value_type` is a dependent type
  // we need to use `typename` keyword before C++20
  // else it will be treated as a static data member of a class.

  template <typename T>
  void func(typename T::value_type);
*/

/*
  in some situations, there is no chance that `T::value_type` 
  can be a static data member of T class 
  but still we need to use `typename` keyword before C++20

  situations that we don't need to use `typename` keyword anymore
    - functions return type
    - type alias declarations inside class definitions
    - member function's return type and parameter variables
    - cast operators operands

    - requires expressions parameter (came with C++20)
*/

/*
  // functions return type

  template <typename T>
  T::value_type foo();

  // before C++20 syntax error
  // error: need 'typename' before 'T::value_type' 
  // because 'T' is a dependent scope

  // since C++20 VALID
*/

/*
  // in free function's parameter variables 
  // we still need to use `typename` keyword

  template <typename T>
  void foo(T::value_type){} // syntax error
  // error: variable or field 'foo' declared void

  template <typename T>
  void bar(typename T::value_type){}  // VALID  
*/

/*
  // type alias declarations inside class definitions

  template <typename T>
  struct PointerTrait {
    using Pointer = void*;
  };

  template <typename T>
  struct AStruct {
    using Ptr = PointerTrait<T>::Pointer;
  };

  // before C++20
  // error: need 'typename' before 'PointerTrait<T>::Pointer' 
  // because 'PointerTrait<T>' is a dependent scope

  // since C++20 VALID
*/

/*
  // member function's return type and parameter variables

  template <typename T>
  struct AStruct {
    T::value_type foo(T::value_type); 
  };

  // before C++20 syntax error
  //  error: need 'typename' before 'T::value_type' because 
  // 'T' is a dependent scope  ---> for return type
  // error: 'T::value_type' is not a type ---> for parameter variable

  // since C++20 VALID
*/

/*
  // cast operators operands

  template <typename T>
  struct AStruct {
    void foo()
    {
      static_cast<T::value_type>(0);
    }
  };

  // before C++20 syntax error
  //  error: need 'typename' before 'T::value_type' because 
  // 'T' is a dependent scope

  // since C++20 VALID
*/

/*
  // requires expressions parameter (came with C++20)

  template <typename T>
  concept Concept = requires (T::value_type x) {
    std::cout << x << '\n';
  };
*/

/*
                ----------------------------------
                | safe comparison in mixed types |
                ----------------------------------
*/

/*
  #include <cstdio>   // std::printf

  #define   asize(a)  (sizeof(a) / sizeof(a[0]))

  int main()
  {
    // --------------------------------------------------

    int arr[5] = { 1, 2, 3, 4, 5 };

    for(int i = -2; i < asize(arr) - 2; ++i)
      std::printf("%d ", arr[i + 2]);
    //  warning: comparison of integer expressions 
    // of different signedness: 'int' and 'long long unsigned int'

    // "i" will be converted to size_t type 
    // in "i < asize(arr) - 2" expression
    // because "asize(arr) - 2" expression's type is size_t type
    // which is unsigned long long int
    // so "i" will become INT_MAX - 1

    // --------------------------------------------------

    int x = -1;
    unsigned int y = 1;

    if (x > y)
      std::printf("x > y\n");
    // output -> x > y

    // --------------------------------------------------
  }
*/

/*
  #include <utility>      // std::cmp_greater (since C++20)
  #include <type_traits>  // std::make_unsigned

  template <typename T, typename U>
  constexpr bool Cmp_Less(T t, U u) noexcept
  {
    using UT = std::make_unsigned_t<T>;
    using UU = std::make_unsigned_t<U>;

    if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
      return t < u;
    else if constexpr (std::is_signed_v<T>)
      return t < 0 ? true : UT(t) < u;
    else
      return U < 0 ? false : t < UU(u);
  }

  int main()
  {
    int x = -1;
    unsigned int y = 1;

    if (std::cmp_greater(x, y))
      std::cout << "x > y\n";
    else 
      std::cout << "x <= y\n";
    // output -> x <= y
  }
*/

/*
                -------------------------------------
                | conversion from `void*` to `bool` |
                -------------------------------------
*/

/*
  // before C++20 (void* ===> bool) conversion 
  // is not a narrowing conversion
  // since C++20, it is a narrowing conversion

  struct AStruct {
    int m_x;
    bool m_b;
  };

  int main()
  {
    void* vp{};

    // ----------------------------------------------

    AStruct a1{ 10, vp };
    // warning: narrowing conversion of 'vp' from 'void*' to 'bool'

    // ----------------------------------------------

    bool flag{ vp };
    // warning: narrowing conversion of 'vp' from 'void*' to 'bool'

    // ----------------------------------------------
  }
*/

// ----------------------------------------------
// ----------------------------------------------
// ----------------------------------------------

/*
  int main()
  {
    char* ptr = new char[]{ "hello world" };
    // syntax error before C++20 valid since C++20
  }
*/

/*
                  ----------------------------
                  | alias template deduction |
                  ----------------------------
*/

/*
  #include <utility>  // std::pair

  // alias template
  template <typename T>
  using fint_pair = std::pair<int, T>;

  int main()
  {
    std::pair p{ 1, 7.8 };

    fint_pair f1 = { 2, 5.6 };
    // error: alias template deduction only available with '-std=c++20'
    // valid in C++20
  }
*/

// ----------------------------------------------
// ----------------------------------------------
// ----------------------------------------------

/*
  // inside a nested namespace definition 
  // using `inline` keyword is allowed (C++20)

  namespace ASpace::BSpace::inline CSpace {
    int g_c;
  }

  namespace ASpace::BSpace {
    int g_b;
  }

  namespace ASpace {
    int g_a;
  }

  int main()
  {
    ASpace::g_a = 10;
    ASpace::BSpace::g_b = 20;
    ASpace::BSpace::CSpace::g_c = 30;

    ASpace::BSpace::g_c = 40;
  }
*/

// ----------------------------------------------
// ----------------------------------------------
// ----------------------------------------------

/*
  #include <type_traits>  // std::is_nothrow_move_constructible

  struct AStruct {
    int m_x;
    int m_y;
  };

  struct BStruct {
    BStruct();
    BStruct(BStruct&&);
  };

  struct CStruct {
    CStruct();
    CStruct(CStruct&&) noexcept;
  };

  struct DStruct {
    int m_x;
    int m_y;
    BStruct m_bstruct;    // BStruct's move ctor is NOT noexcept
  };

  struct EStruct {
    int m_x;
    int m_y;
    CStruct m_cstruct;    // CStruct's move ctor is noexcept
  };

  int main()
  {
    static_assert(std::is_nothrow_move_constructible_v<AStruct>);
    // VALID

    static_assert(std::is_nothrow_move_constructible_v<BStruct>);
    // static assertion failed

    static_assert(std::is_nothrow_move_constructible_v<CStruct>);
    // VALID

    static_assert(std::is_nothrow_move_constructible_v<DStruct>);
    // static assertion failed

    static_assert(std::is_nothrow_move_constructible_v<EStruct>);
    // VALID
  }
*/

/*
  // we can set noexcept specifier to a move constructor
  // even if one of member type's move constructor is not noexcept

  #include <type_traits>  // std::is_nothrow_move_constructible

  struct AStruct {
    AStruct();
    AStruct(AStruct&&); // AStruct's move ctor is NOT noexcept
  };

  struct BStruct {
    BStruct();
    BStruct(BStruct&&) noexcept = default;
    int m_x;
    int m_y;
    AStruct m_astruct;    // AStruct's move ctor is NOT noexcept
  };

  int main()
  {
    static_assert(std::is_nothrow_move_constructible_v<AStruct>);
    // static assertion failed

    static_assert(std::is_nothrow_move_constructible_v<BStruct>);
    // VALID
  }
*/

/*
              ------------------------------------
              | `source_location` module (C++20) |
              ------------------------------------
*/

/*
  int main()
  {
    // predefined symbolic constants
    __FILE__; // integral
    __LINE__; // string literal
    __func__; // function name(char array)
  }
*/

/*
  #include <source_location>  // std::source_location

  int main()
  {
    constexpr auto sl = std::source_location::current();  
    // consteval(immediate) function

    constexpr auto p_filename = sl.file_name();
    constexpr auto p_function_name = sl.function_name();
    // p_function_name -> "main"
    constexpr auto line_number = sl.line();
    // line_number -> 527U
    constexpr auto column_number = sl.column();
    // column_number -> 53U
  }
*/

/*
  #include <source_location>  // std::source_location

  void func(std::source_location sl = std::source_location::current())
  {
    std::cout << "file name: " << sl.file_name() << '\n';
    std::cout << "function name: " << sl.function_name() << '\n';
    std::cout << "line number: " << sl.line() << '\n';
    std::cout << "column number: " << sl.column() << '\n';
  }

  std::source_location foo()
  {
    auto sl = std::source_location::current();
    // operations
    return sl;
  }

  int main()
  {
    func();
    // output ->
    //  file name: cpp20_changes.cpp
    //  function name: int main()
    //  line number: 552
    //  column number: 7
  }
*/

/*
                  --------------------------
                  | using enum declaration |
                  --------------------------
*/

/*
  enum class Color { Red, Green, Blue, White, Black };

  void f_red();
  void f_green();
  void f_blue();
  void f_white();
  void f_black();

  void func(Color color)
  {
    switch (color) {
    case Color::Red:    f_red();    break;
    case Color::Green:  f_green();  break;
    case Color::Blue:   f_blue();   break;
    case Color::White:  f_white();  break;
    case Color::Black:  f_black();  break;
    }
  }

  void bar(Color color)
  {
    switch (color) {
      using enum Color;

      case Red:    f_red();    break;
      case Green:  f_green();  break;
      case Blue:   f_blue();   break;
      case White:  f_white();  break;
      case Black:  f_black();  break;
    }
  }

  void baz(Color color)
  {
    switch (color) {
      using Color::Red, Color::Black;

      case Red:           f_red();    break;
      case Color::Green:  f_green();  break;
      case Color::Blue:   f_blue();   break;
      case Color::White:  f_white();  break;
      case Black:         f_black();  break;
    }
  }
*/

/*
  namespace ASpace {
    class Myclass {
    public:
      enum class Color { Red, Green, Blue, White, Black };
    };

    using enum Myclass::Color;

    // Color enumarators are in Myclass class scope.
    // when we have using enum declaration,
    // Color enumarators are visible in ASpace namespace scope.


    void foo(Myclass::Color);
  }

  namespace BSpace {
    using enum ASpace::Myclass::Color;

    // Color enumarators are now visible in BSpace namespace scope.
  }

  int main()
  {
    ASpace::Black;  // VALID
    BSpace::Red;    // VALID
    
    foo(BSpace::Green); // Argument Dependent Lookup (ADL)
    // `BSpace::Green` is become visible by 
    // `using enum ASpace::Myclass::Color;` declaration
    // so compiler will search `foo` identifier 
    // also in `ASpace` namespace
  }
*/

/*
              --------------------------------------
              | conditionally explicit constructor |
              --------------------------------------
*/

/*
  class Myclass {
  public:
    explicit Myclass(int);
  };

  int main()
  {
    Myclass m1 = 10;  // syntax error
    // error: conversion from 'int' to 
    // non-scalar type 'Myclass' requested

    // copy initialization is not allowed  
    // when explicit constructor is used
  }
*/

/*
  // all constructors can be explicit

  class Myclass {
  public:
    explicit Myclass(int, int);
  };

  int main()
  {
    Myclass m1 = { 11, 22 };  // syntax error
    // error: converting to 'Myclass' from initializer list 
    // would use explicit constructor 'Myclass::Myclass(int, int)'

    m1 = { 33, 44 };          // syntax error
    // error: converting to 'Myclass' from initializer list 
    // would use explicit constructor 'Myclass::Myclass(int, int)'
  }
*/

/*
  // explicit default constructor is also possible

  class Myclass {
  public:
    explicit Myclass();
  };

  Myclass foo()
  {
    return {};  // syntax error
    // error: converting to 'Myclass' from initializer list 
    // would use explicit constructor 'Myclass::Myclass()'
  } 

  int main()
  {
    Myclass m = {}; // syntax error
    // error: converting to 'Myclass' from initializer list 
    // would use explicit constructor 'Myclass::Myclass()'

    m = {};   // syntax error
    // error: converting to 'Myclass' from initializer list 
    // would use explicit constructor 'Myclass::Myclass()'
  }
*/

/*
  // explicit(EXPR) when EXPR evaluated as true,
  // Myclass(int) constructor will be an explicit constructor.
  // when EXPR evaluated as false, Myclass(int) constructor
  // will be a non-explicit constructor.

  class AClass {
  public:
    explicit(true) AClass(int);
  };

  class BClass {
  public:
    explicit(false) BClass(int);
  };

  int main()
  {
    AClass a1 = 10;  // syntax error
    // error: conversion from 'int' to 
    // non-scalar type 'AClass' requested

    BClass b1 = 20;  // VALID
  }
*/

/*
  #include <type_traits>  // std::is_integral

  template <typename T>
  class Myclass {
  public:
    explicit(std::is_integral_v<T>) Myclass(T);
  };

  int main()
  {
    Myclass<int> m1 = 12;       // syntax error
    // error: conversion from 'int' to 
    // non-scalar type 'Myclass<int>' requested

    Myclass<double> m2 = 12.34; // VALID
  }
*/

/*
  #include <type_traits>  // std::is_convertible

  class AClass {
  public:
    AClass();
    explicit AClass(int);
  };

  class BClass {
  public:
    BClass();
    BClass(int);
  };

  template <typename T>
  class Wrapper {
  private:
    T m_wrapped;
  public:
    Wrapper();

    template <typename U>
    Wrapper(U);
  };

  template <typename T>
  class Better_Wrapper {
  private:
    T m_wrapped;
  public:
    Better_Wrapper();

    template <typename U>
    explicit(!std::is_convertible_v<U, T>) Better_Wrapper(U);
  };


  int main()
  {
    // --------------------------------------------------

    AClass a1 = 10;          // syntax error
    // error: conversion from 'int' to 
    // non-scalar type 'AClass' requested

    Wrapper<AClass> w1 = 5;  // VALID

    // conversion from int to AClass is syntax error
    // but conversion from int to Wrapper<AClass> is VALID
    // which is not a wanted behavior!

    // --------------------------------------------------

    AClass a2 = 11;                     // syntax error
    // error: conversion from 'int' to 
    // non-scalar type 'AClass' requested

    Better_Wrapper<AClass> bw1 = 22;    // syntax error
    // error: conversion from 'int' to 
    // non-scalar type 'Better_Wrapper<AClass>' requested

    // when conversion from int to AClass is not allowed
    // conversion from int to 
    // Better_Wrapper<AClass> is also not allowed

    // --------------------------------------------------

    BClass b1 = 33;                     // VALID
    Better_Wrapper<BClass> bw2 = 44;    // VALID

    // when conversion from int to BClass is allowed
    // conversion from int to
    // Better_Wrapper<BClass> is also allowed

    // --------------------------------------------------
  }
*/

// ---------------------------------------------------
// ---------------------------------------------------
// ---------------------------------------------------
// ---------------------------------------------------
// ---------------------------------------------------
