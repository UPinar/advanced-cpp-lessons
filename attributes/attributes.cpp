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