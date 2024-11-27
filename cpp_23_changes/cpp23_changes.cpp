#include <iostream>

/*
                      ======================
                      | Changes with C++23 |
                      ======================
*/

/*
                  -----------------------------
                  | explicit object parameter |
                  -----------------------------
*/

/*
  class Myclass {
  public:
    void func();  
    // non-static member function
    // have a hidden parameter(Myclass*)

    void func() const;
    // const non-static member function
    // have a hidden parameter(const Myclass*)
  };
*/

/*
  class Myclass {
  private:
    int m_x{};

  public:
    Myclass(int x = 0) : m_x(x) {}

    void setX(int x)
    { 
      m_x = x; 
    }
    void printX() const 
    { 
      std::cout << m_x << '\n'; 
    }
  };

  int main()
  {
    Myclass m1(11);
    m1.printX(); // output -> 11

    m1.setX(22);
    m1.printX(); // output -> 22
  }
*/

/*
  class Myclass {
  private:
    int m_x{};

  public:
    Myclass(int x = 0) : m_x(x) {}

    void setX(this Myclass& self, int x)
    { 
      self.m_x = x;
    }

    void printX(this const Myclass& self) 
    { 
      std::cout << self.m_x << '\n'; 
    }
  };

  int main()
  {
    Myclass m1(11);
    m1.printX(); // output -> 11

    m1.setX(22);
    m1.printX(); // output -> 22
  }
*/

/*
  class Myclass {
  public:
    void func_1(this Myclass&);
    void func_2(this const Myclass&);
    void func_3(this Myclass&&);
    void func_4(this const Myclass&&);
  };
*/

/*
  class Myclass {
  public:
    void func() &
    { 
      std::cout << "Myclass::func() &\n";
    }    
    // lvalue reference qualifier
    // can be invoked with Lvalue class objects

    void func() const &
    {
      std::cout << "Myclass::func() const &\n";
    }
    // lvalue reference qualifier

    void func() &&
    {
      std::cout << "Myclass::func() &&\n";
    }  
    // rvalue reference qualifier
    // can be invoked with Rvalue class objects

    void func() const &&
    {
      std::cout << "Myclass::func() const &&\n";
    }
    // rvalue reference qualifier 
  };

  int main()
  {
    Myclass m1;
    const Myclass m2;

    m1.func();            // output -> Myclass::func() &
    m2.func();            // output -> Myclass::func() const &
    Myclass{}.func();     // output -> Myclass::func() &&
    std::move(m2).func(); // output -> Myclass::func() const &&
  }
*/

/*
  // explicit object parameter syntax 
  // is an alternative to reference qualifier syntax

  class Myclass {
  public:
    void func(this Myclass&)
    { 
      std::cout << "Myclass::func(this Myclass&)\n";
    }
    void func(this const Myclass&)
    {
      std::cout << "Myclass::func(this const Myclass&)\n";
    }
    void func(this Myclass&&)
    {
      std::cout << "func(this Myclass&&)\n";
    }  
    void func(this const Myclass&&)
    {
      std::cout << "Myclass::func(this const Myclass&&)\n";
    }
  };

  int main()
  {
    Myclass m1;
    const Myclass m2;

    m1.func();
    // output -> Myclass::func(this Myclass&)
    m2.func(); 
    // output -> Myclass::func(this const Myclass&)    
    Myclass{}.func();
    // output -> Myclass::func(this Myclass&&)
    std::move(m2).func();
    // output -> Myclass::func(this const Myclass&&)
  }
*/

/*
  class Myclass {
  private:
    int m_x{};

  public:
    Myclass(int x = 0) : m_x(x) {}

    void setX(this Myclass& self, int x)
    { 
      // ------------------------------------------------

      m_x = 11; // syntax error
      // error: invalid use of non-static data member 'Myclass::m_x'

      // ------------------------------------------------

      this->m_x = 22; // syntax error
      // error: 'this' is unavailable for 
      // explicit object member functions

      // ------------------------------------------------

      // `this` pointer and data members can only be used 
      // with qualified name of explicit object parameter
      // CAN NOT be used with unqualified names.

      // ------------------------------------------------
    }
  };
*/

/*
  class Myclass {
  public:
    int m_x{ 11 };

    void func(this Myclass self_copy)
    {
      self_copy.m_x = 22;
    }
  };

  int main()
  {
    Myclass m1;
    std::cout << "m1.m_x = " << m1.m_x << '\n';
    // output -> m1.m_x = 11

    m1.func();
    std::cout << "m1.m_x = " << m1.m_x << '\n';
    // output -> m1.m_x = 11
  }
*/

/*
  // explicit object parameter proposal

  #include <optional> // std::bad_optional_access
  #include <utility>  // std::move, std::forward

  // -----------------------------------------------------

  // WITHOUT explicit object parameter

  template <typename T>
  class Optional {
  public:
    bool has_value() const;

    // version of value for non-const lvalues
    constexpr T& value() &
    {
      if (has_value())
        return this->m_value;
      
      throw std::bad_optional_access{};
    }

    // version of value for const lvalues
    constexpr const T& value() const &
    {
      if (has_value())
        return this->m_value;

      throw std::bad_optional_access{};
    }

    // version of value for non-const rvalues
    constexpr T&& value() &&
    {
      if (has_value())
        return std::move(this->m_value);

      throw std::bad_optional_access{};
    }

    // version of value for const rvalues
    constexpr const T&& value() const &&
    {
      if (has_value())
        return std::move(this->m_value);

      throw std::bad_optional_access{};
    }
  };

  // -----------------------------------------------------

  // WITH explicit object parameter

  template <typename T>
  class Optional_2 {
  public:
    bool has_value() const;

    template <class Self>
    constexpr auto&& value(this Self&& self)
    {
      if (self.has_value())
        return std::forward<Self>(self).m_value;

      throw std::bad_optional_access{};
    }
  };

  // -----------------------------------------------------
*/

/*
  #include <utility> // std::move

  struct Myclass {

    template <typename Self>
    void func(this Self&& self); // forwarding reference parameter
  };

  int main()
  {
    Myclass m1;
    const Myclass m2;

    // -----------------------------------------------------
    m1.func();  
    // Self(template parameter) will be is deduced as Myclass&

    // self(function parameter)'s type will be 
    // reference collapsing (& && -> &) -> Myclass&

    // -----------------------------------------------------

    m2.func();
    // Self(template parameter) will be deduced to const Myclass&

    // self(function parameter)'s type will be
    // reference collapsing (& && -> &) -> const Myclass&

    // -----------------------------------------------------

    std::move(m1).func();
    // Self(template parameter) will be deduced to Myclass

    // self - function parameter's type will be Myclass&&

    // -----------------------------------------------------

    std::move(m2).func();
    // Self(template parameter) will be deduced to const Myclass

    // self - function parameter's type will be const Myclass&&

    // -----------------------------------------------------
  }
*/

/*
  #include <typeinfo>

  struct Base {
    template <typename Self>
    void func(this Self&& self)
    {
      std::cout << typeid(self).name() << '\n';
    }
  };

  struct Derived : Base {};

  int main()
  {
    Base b1;
    b1.func();    // output -> struct Base

    Derived d1;
    d1.func();    // output -> struct Derived
  }
*/

/*
  // compile time'da tür bilgisi korunuyor.

  #include <typeinfo>

  struct Base {
    template <typename Self>
    void func(this Self&& self)
    {
      std::cout << typeid(self).name() << '\n';
    }
  };

  struct Derived_1 : Base {};
  struct Derived_2 : Derived_1 {};

  int main()
  {
    Derived_2 d2;
    d2.func();    // output -> struct Derived_2
  }
*/

/*
  Advantages of explicit object parameter:

  - for const overloading we have to write two functions
    but with explicit object parameter we can write one 
    function template that can handle both const and non-const
    objects.
  - for 4 reference qualifier function overloads we have to 
    write 4 functions but with explicit object parameter we 
    can write one function template that can handle all 4 cases.
  - alternative to CRTP pattern.
  - this pointer is passed by value(copying).
  - creating recursive lambda functions very easily.
*/

/*
  // CRTP pattern

  template <typename Der>
  struct postfix_incrementable {
    Der operator++(int)
    {
      auto& self = static_cast<Der&>(*this);

      Der temp(self);
      ++self;
      return temp;
    }
  };

  struct Mystruct : postfix_incrementable<Mystruct> {
    Mystruct& operator++();
  };
*/

/*
  // explicit object parameter

  struct postfix_incrementable {
    template <typename Self>
    auto operator++(this Self&& self, int)
    {
      auto temp(self);
      ++self;
      return temp;
    }
  };

  struct Mystruct : postfix_incrementable {
    Mystruct& operator++();
  };
*/

/*
  int main()
  {
    auto fn = [](this auto&& self, int N){
      if (N == 0)
        return 1;
      else
        return N * self(N - 1);
    };

    constexpr auto val = fn(10);   // val -> 3628800
  }
*/

/*
  // greatest common divisor

  int main()
  {
    auto fn_gcd = [](this auto self, int a, int b) -> int {
      return b == 0 ? a : self(b, a % b);
    };

    constexpr auto val = fn_gcd(12, 40) << '\n';  // val -> 6
  }
*/