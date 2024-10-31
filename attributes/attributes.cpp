#include <iostream>

/*
                          ==============
                          | attributes |
                          ==============
*/

/*
                -----------------------------------
                | [[nodiscard]] attribute (C++17) |
                -----------------------------------
*/

/*
  - bir fonksiyonun varlık nedeni bir değer hesaplamak ise.

  - fonksiyonun geri dönüş değeri tamamlayıcı bilgi değil ise,
  - fonksiyonun geri dönüş değeri çağıran kodun discard 
    etmek isteyeceği bir hata kodu değil ise,
  
  [[nodiscard]] attribute kullanılmalıdır.
*/

/*
  // C++20 adding string literal 
  [[nodiscard ("foo return value should not be discarded")]] 
  int foo(int x) { return x; }

  int main()
  {
    // ---------------------------------------------------

    foo(10);
    // warning: ignoring return value of 'int foo(int)', 
    // declared with attribute 'nodiscard': 
    // 'foo return value should not be discarded'

    // ---------------------------------------------------

    (void)foo(10);
    // if we want to discard the return value
    // we can cast "foo(10)" expression to void type

    // ---------------------------------------------------
  }
*/

/*
  class [[nodiscard]] Myclass {};

  Myclass foo();
  Myclass bar();

  int main()
  {
    auto m = foo(); // VALID

    foo();
    // warning: ignoring returned value of type 'Myclass', 
    // declared with attribute 'nodiscard': 

    bar();
    // warning: ignoring returned value of type 'Myclass', 
    // declared with attribute 'nodiscard': 
  }
*/

/*
  enum [[nodiscard]] ErrorCode { Error_1, Error_2, Error_3 };

  ErrorCode foo() { return Error_1; }
  ErrorCode bar() { return Error_2; }

  int main()
  {
    foo();
    // warning: ignoring returned value of type 'ErrorCode', 
    // declared with attribute 'nodiscard'

    bar();
    // warning: ignoring returned value of type 'ErrorCode', 
    // declared with attribute 'nodiscard'
  }
*/

/*
  class Myclass {
  public:
    [[nodiscard]] int foo() { return 10; }
  };

  int main()
  {
    Myclass m;

    m.foo();
    //  warning: ignoring return value of 'int Myclass::foo()', 
    // declared with attribute  nodiscard'
  }
*/

/*
  class Myclass {
  public:
    [[nodiscard]] Myclass(int);
  };

  int main()
  {
    Myclass{ 10 };
    // warning: ignoring return value of 'Myclass::Myclass(int)', 
    // declared with attribute 'nodiscard'

    static_cast<Myclass>(12);
    // warning: ignoring return value of 'Myclass::Myclass(int)', 
    // declared with attribute 'nodiscard' 
  }
*/

/*
  class [[nodiscard]] Myclass {};

  Myclass foo();
  Myclass& bar();
  Myclass* baz();

  int main()
  {
    foo();
    // warning: ignoring returned value of type 'Myclass', 
    // declared with attribute 'nodiscard': 

    bar();  // no warning

    baz();  // no warning
  }
*/

/*    
                  ----------------------------
                  | [[deprecated]] attribute |
                  ----------------------------
*/

/*
  struct [[deprecated]] AStruct {
    int m_x, m_y, m_z;
  };

  int main()
  {
    AStruct a;
    // warning: 'AStruct' is deprecated 
  }
*/

/*
  struct [[deprecated("use struct AStruct")]] AStruct {
    int m_x, m_y, m_z;
  };

  struct BStruct {
    int m_x, m_y, m_z;
  };

  int main()
  {
    AStruct a;
    // warning: 'AStruct' is deprecated: use struct AStruct
  }
*/

/*
  [[deprecated]] typedef int myint_32;
  using myint_64 [[deprecated]] = unsigned long long;

  int main()
  {
    myint_32 x;
    // warning: 'myint_32' is deprecated

    myint_64 y;
    // warning: 'using myint_64 = long long unsigned int' is deprecated
  }
*/

/*
  enum Color {
    Red, 
    Green ,
    Yellow [[deprecated]] = Green,
    Blue
  };

  int main()
  {
    auto c = Yellow;  // warning: 'Yellow' is deprecated
  }
*/

/*
  [[deprecated("use bar instead of foo")]]
  void foo() {}

  int main()
  {
    foo();
    // warning: 'void foo()' is deprecated: use bar instead of foo
  }
*/

/*
  // some_header.h
  // ----------------
  void foo(int);


  // main.cpp
  // ----------------
  // #include "some_header.h"

  [[deprecated]]
  void foo(int);  
  // function redeclaration with [[deprecated]] attribute

  int main()
  {
    foo(10);  
    // warning: 'void foo(int)' is deprecated
  }
*/

/*
  struct Data {
    int m_a, m_b, m_c;
    [[deprecate]] int m_d;
  };

  int main()
  {
    Data d1{};

    d1.m_a = 2;

    d1.m_d = 10;  
    // warning: 'deprecate' attribute directive ignored
  }
*/

/*
  namespace [[deprecated]] old_version {
    void foo() {}
  }

  int main()
  {
    old_version::foo();
    // warning: 'old_version' is deprecated
  }
*/

/*
            ----------------------------------------------
            | [[likely]] [[unlikely]] attributes (C++20) |
            ----------------------------------------------
*/

/*
  enum class Color { White, Blue, Black, Red, Magenta };

  void f_white();
  void f_blue();
  void f_black();
  void f_red();
  void f_magenta();

  void foo(Color c)
  {
    switch (c)
    {
      using enum Color;
    case White:   f_white();    break;
    case Blue:    f_blue();     break;
    [[likely]]  case Black:   f_black();    break;
    case Red:     f_red();      break;
    [[unlikely]] case Magenta: f_magenta();  break;
    }
  }
*/

/*
  #include <cmath>

  constexpr double power(double x, long long n) noexcept
  {
    if (n > 0) [[likely]]
      return x * pow(x, n - 1);
    else [[unlikely]]
      return 1.;
  }

  constexpr long long factorial(long long n) noexcept
  {
    if (n > 1) [[likely]]
      return n * factorial(n - 1);
    else [[unlikely]]
      return 1;
  }

  constexpr double cosin(double x) noexcept
  {
    constexpr long long precision{ 16LL };
    double y{};

    for (auto n{ 0LL }; n < precision; n += 2LL) [[likely]]
      y += power(x, n) / (n & 2LL ? -factorial(n) : factorial(n));

    return y;
  }
*/

/*
                  ------------------------------
                  | [[maybe_unused]] attribute |
                  ------------------------------
*/

/*
  // internal linkage
  [[maybe_unused]] 
  static int foo(int x) 
  {
    return x * x;
  }

  // "foo" function can only be used in this module
  // it has internal linkage

  // if "foo" function is not used in this module
  // compiler will generate a warning
  // to suppress this warning, [[maybe_unused]] attribute can be used.
*/

/*
  int main()
  {
    int a = 23;
    [[maybe_unused]] int b = 45;
  }
*/

/*
  #include <cassert>

  [[maybe_unused]] void foo([[maybe_unused]] bool param_1,
                            [[maybe_unused]] bool param_2)
  {
    [[maybe_unused]] bool b1 = param_1 && param_2;
    assert(b1);
    // in release mode, assert is compiled out, and b is unused
    // no warning because it is declared [[maybe_unused]]
  }

  // static_assert  -> compile time check
  // assert macro   -> runtime check
*/

/*
  struct [[maybe_unused]] AStruct;

  [[maybe_unused]] typedef AStruct* AStructPtr;
  using AStructPtr [[maybe_unused]] = AStruct*;

  struct BStruct {
    int m_x, m_y, m_z;
    [[maybe_unused]] int m_w;
  };
*/

/*
  #include <tuple>

  int main()
  {
    [[maybe_unused]]
    auto[ival, dval, cstr] = std::make_tuple(1, 4.5, "hello");
  }
*/

/*
                  --------------------------
                  | [[noreturn]] attribute |
                  --------------------------
*/

/*
  [[noreturn]] void Exit(int);
  // this function will never return
  // we might use this function for throwing an exception
*/

/*
  #include <stdexcept>  // std::runtime_error

  [[noreturn]] 
  void bar()
  {
    throw std::runtime_error{ "error" };
  }

  int main()
  {
    try 
    {
      bar();
      // unreachable code
    }
    catch(const std::exception& ex)
    {
      // rechable code
    }
  }
*/

/*
  [[noreturn]] 
  void bar(int i)
  {
    if (i < 0)
      throw std::runtime_error{ "error" };

    // if "i >= 0" it will be undefined behavior(UB)
    // because [[noreturn]] attribute is saying compiler that
    // this function will never return
    // but if "i >= 0" it will return
  }
*/

/*
                  -----------------------------
                  | [[fallthrough]] attribute |
                  -----------------------------
*/

/*
  void f1();
  void f2();
  void f3();

  void func(int x)
  {
    switch (x)
    {
    case 1: f1(); [[fallthrough]];
    case 2: f2(); break;
    case 3: f3(); break;
    }

    // when case (x == 1) is true, 
    // we want both f1 and f2 to be executed
    // for declaring that we do not forgot break statement
    // we can use [[fallthrough]] attribute
  }
*/

/*
  bool isleap(int);

  constexpr int day_of_year(int d, int m, int y)
  {
    int sum = d;

    switch(m - 1){
    case 11: sum += 30;                   [[fallthrough]];
    case 10: sum += 31;                   [[fallthrough]];
    case 9:  sum += 30;                   [[fallthrough]];
    case 8:  sum += 31;                   [[fallthrough]];
    case 7:  sum += 31;                   [[fallthrough]];
    case 6:  sum += 30;                   [[fallthrough]];
    case 5:  sum += 31;                   [[fallthrough]];
    case 4:  sum += 30;                   [[fallthrough]];
    case 3:  sum += 31;                   [[fallthrough]];
    case 2:  sum += isleap(y) ? 29 : 28;  [[fallthrough]];
    case 1:  sum += 31;
    }

    return sum;
  }
*/