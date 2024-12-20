#include <iostream>

/*
              ------------------------------------
              | template<auto N> syntax (C++17) |
              ------------------------------------
*/

/*
  template <auto N>
  class Myclass {}; // class template

  template <auto N>
  void foo();       // function template

  // non-type template parameter
*/

/*
  // since C++20
  template <auto N>
  class Myclass {};

  // before C++17
  template <typename T, T N>
  class Myclass_before17 {};

  int main()
  {
    Myclass<5> m1;
    Myclass<9> m2;
    Myclass<3.24> m3;   
    // C++20 -> nttp can be a floating-point type

    Myclass_before17<int, 5> m3;
    Myclass_before17<int, 9> m4;
    Myclass_before17<char, 'A'> m5;
    Myclass_before17<long, 4L> m6;
  }
*/

/*
  template <int N>
  class Myclass {};

  template <long N>
  class Myclass {};

  // error: template parameter 'int N'
  // note: redeclared here as 'long int N'

  // can not create same named classes with 
  // different type of non-type template parameters
*/

/*
  template <typename T, int N>
  class Myclass {};

  int main()
  {
    Myclass<int, 5> m1;
    Myclass<long, 4L> m2;
    Myclass<char, 'A'> m3;
    // different specialization of the same class template
  }
*/

/*
  #include <type_traits>  // std::is_same

  template<auto N>
  class Myclass {};

  // without using second template parameter
  // we can create different specializations of the same class template
  // with template <auto N> syntax

  int main()
  {
    Myclass<5> m1;
    Myclass<5L> m2;

    static_assert(std::is_same_v<decltype(m1), decltype(m2)>);  
    // static assert failed.
  }
*/

/*
  template <auto N>
  void foo(); // function template

  int main()
  {
    foo<5>();
    foo<5L>();
    foo<3.4>(); // since C++20 -> nttp can be a floating-point type
  }
*/

/*
  template <typename T, T val>
  constexpr T elem = val;   // variable template

  template <auto val>
  constexpr auto elem_2 = val;

  int main()
  {
    // --------------------------------------------------

    constexpr auto x = elem<int, 5>;
    // x -> 5, x's type is int

    constexpr auto y = elem<char, 'A'>;
    // y -> (char)65, y's type is char

    constexpr auto z = elem<double, 3.14>;  // since C++20
    // z -> 3.14, z's type is double

    // --------------------------------------------------

    constexpr auto x2 = elem_2<5>;
    // x2 -> 5, x2's type is int

    constexpr auto y2 = elem_2<'A'>;
    // y2 -> (char)65, y2's type is char

    constexpr auto z2 = elem_2<3.14>;  // since C++20
    // z2 -> 3.14, z2's type is double

    // --------------------------------------------------
  }
*/

/*
  C++20 -> literal type can be used as an argument of nttp
*/

/*
  template <auto val>
  class Myclass {};

  int main()
  {
    Myclass<decltype([]{})> m1;
  }

  // aradan sonraki ornegi buraya ekle.!
*/

/*
  template <decltype([](){})>
  class Myclass{};

  // "decltype([](){})" is a literal type
*/

/*
  // using template non-type parameter with default argument
  template <auto = [](){}>
  class Myclass {};

  // "[](){}" is an expression, not a type.
  // "[](){}" is lambda expression, it will create a temporary object 

  template <int = 5>
  class AClass {};

  template <double = 5.6>   // C++20
  class BClass {};
*/

/*
  #include <type_traits>  // std::is_same

  template <auto = [](){}>
  class Myclass {};

  int main()
  {
    Myclass m1;
    Myclass m2;

    // "m1" and "m2"'s types are different because
    // every lambda expression creates a different type.

    static_assert(std::is_same_v<decltype(m1), decltype(m2)>);
    // static assert failed.
  }
*/

/*
                    -----------------------
                    | lazy initialization |
                    -----------------------
*/

/*
  class Myclass {
  public:
    Myclass()
    {
      std::cout << "Myclass constructor, this = " << this << '\n';
    }
    void foo() 
    {
      std::cout << "Myclass::foo(),      this = " << this << '\n';
    }
  };

  Myclass& get_instance()
  {
    static Myclass m;
    return m;
  }
  // static local variable "m"'s dynamic initialization happens
  // when program flow gets inside "get_instance" function 
  // for the first time

  // when "get_instance" function is called for the first time
  // m object is initialized and returned.
  // when "get_instance" function is called for the second time,
  // same m object will be returned.
  // This is called Lazy initialization.

  // static local variable's initialization is thread-safe

  int main()
  {
    std::cout << "[0] main started\n";

    get_instance().foo();

    std::cout << "[1] main continues\n";

    get_instance().foo();
    get_instance().foo();

    // output ->
    //  [0] main started
    //  Myclass constructor, this = 0x7ff6a39ff0a0
    //  Myclass::foo(),      this = 0x7ff6a39ff0a0
    //  [1] main continues
    //  Myclass::foo(),      this = 0x7ff6a39ff0a0
    //  Myclass::foo(),      this = 0x7ff6a39ff0a0
  }
*/

/*
  template <typename T>
  struct Lazy_Init {
    constexpr Lazy_Init() = default;
    // objects which are Lazy_Init class type 
    // can be constant initialized.

    T& get_lazy()
    {
      static T global_obj;
      return global_obj;
    }
  };

  int main()
  {
    Lazy_Init<int> obj_1;
    Lazy_Init<int> obj_2;

    obj_1.get_lazy()++;
    obj_1.get_lazy()++;
    obj_1.get_lazy()++;

    std::cout << obj_2.get_lazy() << '\n';  // output -> 3

    // we can not use 2 different global Lazy_Init objects 
    // are from same specialization of Lazy_Init class template
  }
*/

/*
  // -------------- WAY 1 --------------

  template <typename Tag, typename T>
  struct Lazy_Init {
    constexpr Lazy_Init() = default;
    // objects which are Lazy_Init class type 
    // can be constant initialized.

    T& get_lazy()
    {
      static T global_obj;
      return global_obj;
    }
  };

  struct Tag_1;   // forward declaration (incomplete type)
  struct Tag_2;   // forward declaration (incomplete type)

  int main()
  {
    Lazy_Init<Tag_1, int> obj_1;
    Lazy_Init<Tag_2, int> obj_2;

    obj_1.get_lazy()++;
    obj_1.get_lazy()++;
    obj_1.get_lazy()++;

    std::cout << obj_2.get_lazy() << '\n';  // output -> 0
  }
*/

/*
  // -------------- WAY 2 --------------
  // lambda idiom that solves static initialization order fiasco

  template <typename T, auto = []{}>
  struct Lazy_Init {
    constexpr Lazy_Init() = default;
    // objects which are Lazy_Init class type 
    // can be constant initialized.

    T& get_lazy()
    {
      static T global_obj;
      return global_obj;
    }
  };

  struct Tag_1;   // forward declaration (incomplete type)
  struct Tag_2;   // forward declaration (incomplete type)

  int main()
  {
    Lazy_Init<int> obj_1;
    Lazy_Init<int> obj_2;

    obj_1.get_lazy()++;
    obj_1.get_lazy()++;
    obj_1.get_lazy()++;

    std::cout << obj_2.get_lazy() << '\n';  // output -> 0
  }
*/

/*
                    -------------------------
                    | refactoring functions |
                    -------------------------
*/

/*
  // we want to refactor AggregateAndDisplay function

  #include <map>
  #include <string>
  #include <utility>  // std::make_pair

  void AggregateAndDisplay( const std::map<int, std::string>& source, 
                            const std::map<int, std::string>& dest)
  {
    auto aggregated_map = dest;

    for (const auto& source_entry : source)
    {
      auto dest_pos = aggregated_map.find(source_entry.first);

      if (dest_pos == aggregated_map.end())
        aggregated_map.insert(std::make_pair(source_entry.first, 
                                            source_entry.second));
      else
        aggregated_map[source_entry.first] = 
          source_entry.second + " or " + dest_pos->second;
    }
    
    for (const auto& entry : aggregated_map)
      std::cout << "available translations for "
                << entry.first << " -> " << entry.second << '\n';
  }

  // ----------------------------------------------

  // create IIFE idiom
  // learning output(return value) of the function
  void refactor_1(const std::map<int, std::string>& source, 
                  const std::map<int, std::string>& dest)
  {
    [&](){
      auto aggregated_map = dest;

      for (const auto& source_entry : source)
      {
        auto dest_pos = aggregated_map.find(source_entry.first);

        if (dest_pos == aggregated_map.end())
          aggregated_map.insert(std::make_pair(source_entry.first, 
                                              source_entry.second));
        else
          aggregated_map[source_entry.first] = 
            source_entry.second + " or " + dest_pos->second;
      }
    }();

    for (const auto& entry : aggregated_map)
      std::cout << "available translations for "
                << entry.first << " -> " << entry.second << '\n';
    // aggregated_map is not visible here
    // it will be the return value of the refactored function
  }

  // ----------------------------------------------

  // remove '&'(capture all by ref) from lambda expression
  // learning inputs of the lambda expression
  void refactor_2(const std::map<int, std::string>& source, 
                  const std::map<int, std::string>& dest)
  {
    [](){
      auto aggregated_map = dest;

      for (const auto& source_entry : source)
      {
        auto dest_pos = aggregated_map.find(source_entry.first);

        if (dest_pos == aggregated_map.end())
          aggregated_map.insert(std::make_pair(source_entry.first, 
                                              source_entry.second));
        else
          aggregated_map[source_entry.first] = 
            source_entry.second + " or " + dest_pos->second;
      }
    }();
    // dest and source are not visible so they both will be parameters
  }

  // ----------------------------------------------

  auto refactored_function( const std::map<int, std::string>& source, 
                            const std::map<int, std::string>& dest)
  {
    auto aggregated_map = dest;

    for (const auto& source_entry : source)
    {
      auto dest_pos = aggregated_map.find(source_entry.first);

      if (dest_pos == aggregated_map.end())
        aggregated_map.insert(std::make_pair(source_entry.first, 
                                            source_entry.second));
      else
        aggregated_map[source_entry.first] = 
          source_entry.second + " or " + dest_pos->second;
    }

    return aggregated_map;
  }

  void refactored_AggregateAndDisplay(
                const std::map<int, std::string>& source, 
                const std::map<int, std::string>& dest)
  {
    auto aggregated_map = refactored_function(source, dest);

    for (const auto& entry : aggregated_map)
      std::cout << "available translations for "
                << entry.first << " -> " << entry.second << '\n';
  }
*/

/*
          ---------------------------------------------------
          | default declaration of special member functions |
          ---------------------------------------------------
*/

/*
  // implicitly declared defaulted special member functions
  // are non-static inline public member functions

  class Myclass {
  private:
    Myclass(const Myclass&) = default;

  protected:
    Myclass(Myclass&&) = default;
  };

  // special member functions can be in private or protected area
  // when they are declared by the user
*/

/*
  class Myclass {
  public:
    Myclass& operator=(const Myclass&)& = default;
  };

  // user declared defaulted special member functions
  // can be reference qualified
*/

/*
  class AClass {
  public:
    AClass() noexcept(false) = default;   // NOT no-throw guaranteed
  };

  class BClass {
  public:
    BClass() noexcept = default;  // no-throw guaranteed
  };

  // user declared defaulted special member functions
  // can have `noexcept` specifier
  // and `noexcept(false)` is also valid
*/

/*
      -----------------------------------------------------------
      | (template argument deduction) and (auto type deduction) |
      -----------------------------------------------------------
*/

/*
  template <typename T>
  void func(T, T) {}

  int main()
  {
    func(5, 5);     // VALID

    func(5, 3.4);   // syntax error
    // error: no matching function for call to 'func(int, double)'
    // note: deduced conflicting types for parameter 'T' 
    // ('int' and 'double')

    func<double>(5, 3.4);   // VALID
    // defining template argument type explicitly 
  }
*/

/*
  template <typename T>
  void foo(T, T) {}

  template <typename T>
  void func(T&, T&) {}

  int main()
  {
    // --------------------------------------------------

    foo("hello", "world");    // VALID
    foo("hello", "universe"); // VALID

    // T -> const char* (array to pointer conversion)

    // --------------------------------------------------

    func("hello", "world");   // VALID
    // T -> const char[6]

    func("hello", "universe");  // syntax error
    // error: no matching function for call to 
    // 'func(const char [6], const char [9])'

    // because of T& parameter type,
    // array to pointer conversion does not happen

    // --------------------------------------------------
  }
*/

/*
  - auto type deduction and template argument deduction are same.
    (1 exception -> initializer_list)

  - in template argument deduction, 
    deduction is done for template parameter.
    not the function parameter variable.

    template <typename T>
    void func(T x);
    - deduction will be done for 'T' not 'x'
*/

/*
  template <typename T>
  void func(T x);

  int main()
  {
    func(10);       // template argument deduction
    auto val = 10;  // auto type deduction
  }
*/

/*
  template <typename T>
  void func(T& r);

  int main()
  {
    int arr[20]{};

    func(arr);
    // deduction for T -> int[20]

    // compiler written specialization 
    // void func(int (&r)[20]);


    auto& r_arr = arr; 
    // deduction for auto -> int[20] 
    // (same as template argument deduction)
  }
*/

/*
  template <typename T>
  void func(T r);

  int main()
  {
    auto elem = { 1, 2, 3, 4, 5 };
    // deduction for auto -> std::initializer_list<int>

    func({1, 2, 3, 4, 5});  // syntax error
    // error: no matching function for call to 
    // 'func(<brace-enclosed initializer list>)'

    // only exception between auto type deduction and 
    // template argument deduction
  }
*/

/*
  template <typename T>
  void func(T&& x);

  int main()
  {
    // --------------------------------------------------

    int x = 5;
    func(x);  
    // when L value is passed to a function
    // T will be deduced as L value reference type

    // deduction for T -> int& 
    // int& && -> int& (reference collapse)
    // 'x'(parameter variable) data type is int&

    // --------------------------------------------------

    func(10); 
    // when R value is passed to a function
    // T will not be deduce to a reference type

    // deduction for T -> int
    // no reference collapsing happens
    // 'x'(parameter variable) data type is int&&

    // --------------------------------------------------
  }
*/

/*
  template <typename T>
  void func(T&& param_1, T param_2);

  int main()
  {
    // --------------------------------------------------

    int a{}, b{};

    func(a, b); // syntax error
    // for 1st parameter -> deduction for T -> int&
    // for 2nd parameter -> deduction for T -> int

    // --------------------------------------------------

    func(10, 20); // VALID
    // for 1st parameter -> deduction for T -> int
    // for 2nd parameter -> deduction for T -> int

    // --------------------------------------------------
  }
*/

/*
  template <typename T>
  class TypeTeller;

  template <typename T>
  void func_Rref(T&& Rref_param)
  {
    TypeTeller<T> t;
  }

  template <typename T>
  void func_Lref(T& Lref_param)
  {
    TypeTeller<T> t;
  }

  template <typename T>
  void func_NOref(T& NOref_param)
  {
    TypeTeller<T> t;
  }

  int bar(int) { return 1; }

  int main()
  {
    // --------------------------------------------------

    int ival{};
    func_Rref(ival); // syntax error
    //  error: 'TypeTeller<int&> t' has incomplete type
    // deduction for T -> int& (also TypeTeller<int&>)

    // --------------------------------------------------

    func_Rref(12); // syntax error
    //  error: 'TypeTeller<int> t' has incomplete type
    // deduction for T -> int (also TypeTeller<int>)

    // --------------------------------------------------

    int arr[20]{};
    func_Lref(arr); // syntax error
    // error: 'TypeTeller<int [20]> t' has incomplete type
    // deduction for T -> int[20] (also TypeTeller<int[20]>)

    // --------------------------------------------------

    func_NOref(bar); // syntax error
    // error: 'TypeTeller<int(int)> t' has incomplete type
    // deduction for T -> int(int) (also TypeTeller<int(int)>)

    // "bar" will decay to a pointer(function pointer) type

    // --------------------------------------------------

    func_Lref(bar); // syntax error
    // error: 'TypeTeller<int(int)> t' has incomplete type
    // deduction for T -> int(int) (also TypeTeller<int(int)>)

    // "Lref_param" parameter variable's type is int(&)(int) 
    
    // --------------------------------------------------
  }
*/

/*
                      ----------------------
                      | std::type_identity |
                      ----------------------
*/

/*
  #include <type_traits>  // std::type_identity

  template <typename T>
  struct TypeIdentity {
    using type = T;
  };

  template <typename T>
  using TypeIdentity_t = TypeIdentity<T>::type;

  // we want 
  //  - deduction for first parameter but not for the second one
  //  - second parameter's type will be same as first parameter's type

  template <typename T>
  void func(T param_1, TypeIdentity_t<T> param_2);

  int main()
  {
    TypeIdentity_t<int> x{};
    // "x"'s data type is int

    func(5.2, 3); // VALID
    // deduction for T -> double
    // param_1's data type is double
    // param_2's data type is double
  }
*/

/*
                    -------------------------
                    | `std::iota` algorithm | 
                    -------------------------
*/

/*
  #include <numeric>  // std::iota
  #include <vector>
  #include <cstdio>   // std::putchar

  class Date {
  private:
    int m_day, m_month, m_year;
  public:
    Date(int, int, int);
    Date& operator++();
     // prefix increment generates LValue expression
    Date operator++(int);
    // postfix increment generates PRValue expression
  };

  int main()
  {
    // --------------------------------------------------

    std::vector<int> ivec(10);
    std::iota(ivec.begin(), ivec.end(), 0);

    for (auto elem : ivec)
      std::cout << elem << ' ';
    std::putchar('\n');
    // output -> 0 1 2 3 4 5 6 7 8 9

    // --------------------------------------------------

    std::vector<Date> date_vec(10);
    std::iota(date_vec.begin(), date_vec.end(), Date{1, 1, 2021});  
     
    // --------------------------------------------------
  }
*/

/*
                      -------------------
                      | namespace alias |
                      -------------------
*/

/*
  #include <chrono>
  #include <ranges>
  #include <algorithm>

  namespace ASpace {
    namespace BSpace {
      namespace CSpace {
        int a;
      }
    }
  }

  int main()
  {
    // --------------------------------------------------

    namespace chr = std::chrono;  // namespace alias

    // --------------------------------------------------

    namespace CS = ASpace::BSpace::CSpace;
    // namespace alias for nested namespace
    CS::a = 5;

    // --------------------------------------------------

    namespace rng = std::ranges;
    // rng::for_each

    // --------------------------------------------------

    std::ranges::views::chunk();
    std::views::chunk();  // VALID because of namespace alias
    // namespace std::views = std::ranges::views;

    // --------------------------------------------------
  }
*/

/*
                    ---------------------------
                    | ADL and two step lookup |
                    ---------------------------
*/

/*
  When a function is called with a non quialified name,
  std::begin -> qualified, begin -> non qualified
  and one of function's argument is in a namespace
  function identifier(tag) will also be lookedup in that namespace
*/

/*
  namespace ASpace {
    struct AStruct {};
  }

  namespace BSpace {
    struct BStruct {};
    void begin(const BStruct&)
    {
      std::cout << "BSpace::begin(const BStruct&) called\n";
    }
  }

  template <typename C>
  void func(C con)
  {
    begin(con);
  }

  int main()
  {
    // --------------------------------------------------

    func(ASpace::AStruct{}); // syntax error
    // error: 'begin' was not declared in this scope

    // begin is not a visible function and 
    // there is no "begin" function in ASpace namespace
    // can be found with Argument Dependent Lookup

    // --------------------------------------------------

    func(BSpace::BStruct{}); // VALID
    // output -> BSpace::begin(const BStruct&) called

    // begin function is found with Argument Dependent Lookup

    // --------------------------------------------------
  }
*/

/*
  //  When "begin" function is NOT visible or 
  //  CAN NOT found with Argument Dependent Lookup,
  //  we want std::begin function to be called.

  template <typename C>
  void func(C con)
  {
    using std::begin;  
    // std::begin is visible, injected into the function scope

    begin(con);
  }

  // if "begin" can be called with ADL, it will be called
  // if not "std::begin" will be called,
*/

/*
                        ---------------
                        | std::invoke | 
                        ---------------
*/

/*
  #include <functional>  // std::invoke

  int func(int x, int y)
  {
    std::cout << "func(int, int) called\n";
    return x * y + 5;
  }

  int main()
  {
    auto ret_val = std::invoke(func, 10, 20);
    std::cout << "ret_val = " << ret_val << '\n';
    // output ->
    //  func(int, int) called
    //  ret_val = 205
  }
*/

/*
  #include <functional>  // std::invoke

  class Myclass {
  public:
    int foo(int x) 
    { 
      std::cout << "Myclass::foo(int x), x = " << x << '\n';
      return x * 1000;
    }
  };

  int main()
  {
    using namespace std;

    Myclass mx;
    int ival{ 22 };
    auto ret_val = invoke(&Myclass::foo, mx, ival);
    // 2nd parameter is the hidden parameter of the member function

    cout << "ret_val = " << ret_val << '\n';
    // output ->
    //  Myclass::foo(int x), x = 22
    //  ret_val = 22000
  }
*/

/*
  #include <functional>  // std::invoke

  class Myclass {
  public:
    int foo(int x) 
    { 
      std::cout << "Myclass::foo(int x), x = " << x << '\n';
      return x * 1000;
    }
  };

  int main()
  {
    Myclass mx;

    // --------------------------------------------------

    int (Myclass::*fptr)(int) = &Myclass::foo;
    // auto = &Myclass::foo;
    (mx.*fptr)(22);
    // output -> Myclass::foo(int x), x = 22

    // ".*" member (member access through pointer to member) operator's 
    // precedence is lower than "()" (function call operator)

    // --------------------------------------------------

    // calling a member function with ".*" operator 
    // creates a verbose code

    Myclass* p_m = new Myclass;
    auto fp = &Myclass::foo;

    ((*p_m).*fp)(33);
    // output -> Myclass::foo(int x), x = 33
    
    delete p_m;

    // --------------------------------------------------

    // when a function will be called
    // with an address of a non-static member function by using 
    // a function pointer, better using std::invoke

    Myclass mx2;
    Myclass* p_mx3 = new Myclass;
    auto fp2 = &Myclass::foo;

    std::invoke(fp2, mx2, 44);
    // output -> Myclass::foo(int x), x = 44

    std::invoke(fp2, *p_mx3, 55);  
    // output -> Myclass::foo(int x), x = 55
    std::invoke(fp2, p_mx3, 66);  
    // output -> Myclass::foo(int x), x = 66

    // we can use "*p_mx3" or "p_mx3" with std::invoke

    delete p_mx3;

    // --------------------------------------------------
  }
*/

/*
  #include <functional>   // std::invoke

  struct Mystruct {
    int m_x{ 10 }, m_y{ 20 };
  };

  int main()
  {
    // --------------------------------------------------

    Mystruct mystruct_obj;
    &mystruct_obj.m_x;  
    // data type of "&mystruct_obj.m_x" expression is `int*`

    int* type_1 = &mystruct_obj.m_x;

    // --------------------------------------------------

    &Mystruct::m_x; // data member pointer
    // data type of "&Mystruct::m_x" expression is `int Mystruct::*`

    int Mystruct::* data_member_ptr_x = &Mystruct::m_x;

    // --------------------------------------------------

    // how to reach "mystruct_obj" object's "m_x" data member
    // by using "data_member_ptr_x" data member pointer ?

    mystruct_obj.*data_member_ptr_x = 44;
    std::cout << mystruct_obj.m_x << '\n';  // output -> 44

    // --------------------------------------------------

    // std::invoke can also be used with data member pointer

    std::invoke(data_member_ptr_x, mystruct_obj) = 55;

    std::cout << mystruct_obj.m_x << '\n';  // output -> 55

    // --------------------------------------------------
  }
*/

/*
                        -----------------
                        | std::exchange |
                        -----------------
*/

/*
  #include <utility>  
  // std::move, std::forward, std::exchange
  #include <type_traits>  
  // std::is_nothrow_move_constructible
  // std::is_nothrow_assignable

  template <class T, class U = T>
  constexpr T exchange(T& obj, U&& new_value) 
  noexcept( std::is_nothrow_move_constructible_v<T> && 
            std::is_nothrow_assignable_v<T&, U>)
  {
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
  }

  // std::exchange        C++14
  // constexpr            C++20
  // noexcept specifier   C++23

  // bir nesneye değer atayarak, nesnenin değeri değiştirilir.
  // eski değer geri döndürülür.
*/

/*
  #include <utility>  // std::exchange

  int main()
  {
    int x = 20;
    auto y = std::exchange(x, 30);

    std::cout << "x = " << x << '\n';   // output -> x =  30
    std::cout << "y = " << y << '\n';   // output -> y =  20
  }
*/

/*
  #include <utility>  // std::exchange
  #include <string>

  int main()
  {
    std::string str{ "hello world" };
    auto y = std::exchange(str, "hello universe");

    std::cout << "str = " << str << '\n';   
    // output -> str = hello universe

    std::cout << "y = " << y << '\n';
    // output -> y = hello world
  }
*/

/*
  // generally used in move members

  #include <utility>  // std::exchange

  class Myclass {
  private:
    int m_value;

  public:
    Myclass(Myclass&& other) noexcept
      : m_value{ std::exchange(other.m_value, 0) } {}
    
    Myclass& operator=(Myclass&& other) noexcept
    {
      if (this != &other) // self-assignment check
        m_value = std::exchange(other.m_value, 0);
      
      return *this;
    }
  };
*/

/*
  // possibly same assembly codes

  #include <utility>  // std::exchange

  char* strcpy_1(char* p_dest, const char* p_src)
  {
    while ((*p_dest++ = *p_src++))
      ; // null statement

    return p_dest;
  }

  char* strcpy_2(char* p_dest, const char* p_src)
  {
    for(;;)
    {
      auto src_char = std::exchange(p_src, p_src + 1);
      auto dest_char = std::exchange(p_dest, p_dest + 1);
      *dest_char = *src_char;

      if (*dest_char == '\0')
        break;
    }

    return p_dest;
  }
*/

/*
  #include <vector>
  #include <utility>  // std::exchange
  #include "../nutility.h"

  int main()
  {
    std::vector ivec = { 1, 2, 3, 4, 5, 6 };

    auto old_ivec = std::exchange(ivec, { 11, 22, 33, 44, 55 });

    print(ivec);  
    // output -> 11 22 33 44 55
    print(old_ivec);     
    // output -> 1 2 3 4 5 6
  }
*/

/*
  #include <format>   
  #include <utility>  // std::exchange

  int main()
  {
    for ( auto cnt = 0LL, a = 0LL, b = 1LL; 
          cnt < 15; 
          a = std::exchange(b, a + b), ++cnt )
      std::cout << std::format("fib[{}] = {}\n", cnt, a);

    // output ->
    //  fib[0] = 0
    //  fib[1] = 1
    //  fib[2] = 1
    //  fib[3] = 2
    //  fib[4] = 3
    //  fib[5] = 5
    //  fib[6] = 8
    //  fib[7] = 13
    //  fib[8] = 21
    //  fib[9] = 34
    //  fib[10] = 55
    //  fib[11] = 89
    //  fib[12] = 144
    //  fib[13] = 233
    //  fib[14] = 377
  }
*/

/*
  #include <utility>  // std::exchange

  void foo()
  {
    std::cout << "foo()\n";
  }

  void bar()
  {
    std::cout << "bar()\n";
  }

  int main()
  {
    auto f1 = &foo;
    f1();   // output -> foo()

    auto f2 = std::exchange(f1, &bar);
    f2();   // output -> foo()
    f1();   // output -> bar()
  }
*/

/*
                          --------------
                          | std::clamp |
                          --------------
*/

/*
  #include <functional>   // std::less

  template <class T>
  constexpr const T& clamp( const T& val, 
                            const T& low, const T& high)
  {
    return clamp(val, low, high, std::less{});
  }

  template <class T, class Compare>
  constexpr const T& clamp( const T& val, 
                            const T& low, const T& high, 
                            Compare comp)
  {
    return comp(val, low) ? low : comp(high, val) ? high : val;
  }

  // when local variable is bind to a reference,
  // which is the return value of a function,
  // it will be a reference to a temporary object
  // dangling reference!
*/

/*
  #include <algorithm>  // std::clamp

  int main()
  {
    int ival  = 22;
    int low   = 11;
    int high  = 33;

    auto res = std::clamp(ival, low, high);
    std::cout << "res = " << res << '\n';  
    // output -> res = 22

    ival = 5;
    res = std::clamp(ival, low, high);
    std::cout << "res = " << res << '\n';
    // output -> res = 11

    ival = 44;
    res = std::clamp(ival, low, high);
    std::cout << "res = " << res << '\n';
    // output -> res = 33
  }
*/

/*
  #include <array>
  #include <algorithm>  // std::clamp, std::transform

  int main()
  {
    using namespace std;

    array i_arr{ -3, 0, -5, 2, 7, -1, 4, 6, -2, 3 };

    for (const int elem : i_arr)
      cout << elem << ' ';
    // output -> -3 0 -5 2 7 -1 4 6 -2 3

    cout << '\n';

    int low = -1, high = 3;

    transform(begin(i_arr), end(i_arr), begin(i_arr), 
              [low, high](int val) { 
                return std::clamp(val, low, high); 
              });

    for (const int elem : i_arr)
      cout << elem << ' ';
    // output -> -1 0 -1 2 3 -1 3 3 -1 3
  }
*/

/*
                      ------------------
                      | deep constness |
                      ------------------
*/

/*
  int main()
  {
    int x = 10;
    int y = 20;

    // --------------------------------------------------

    int* p = &x;

    p = &y;   // VALID
    *p = 11;  // VALID

    // --------------------------------------------------

    int* const cp = &x;

    cp = &y;  // syntax error
    // error: assignment of read-only variable 'cp'
    *cp = 22; // VALID

    // --------------------------------------------------

    const int* p2 = &x;
    
    p2 = &y;  // VALID
    *p2 = 33; // syntax error
    // error: assignment of read-only location '* p2'

    // --------------------------------------------------

    const int* const cp2 = &x;

    cp2 = &y;  // syntax error
    // error: assignment of read-only variable 'cp2'

    *cp2 = 44; // syntax error
    // error: assignment of read-only location '*(const int*)cp2'

    // --------------------------------------------------
  }
*/

/*
  #include <vector>

  int main()
  {
    std::vector ivec = { 1, 2, 3, 4, 5, 6 };

    // --------------------------------------------------

    std::vector<int>::iterator iter = ivec.begin();
    *iter = 10;   // VALID
    ++iter;       // VALID

    // --------------------------------------------------

    const std::vector<int>::iterator c_iter = ivec.begin();
    // class object is `const`

    ++c_iter;     // syntax error
    // error: passing 
    // 'const std::vector<int, std::allocator<int> >::iterator' 
    // as 'this' argument discards qualifiers

    // operator++ function is non-const member function
    // const class object can not call non-const member function

    *c_iter = 20; // VALID

    // --------------------------------------------------

    std::vector<int>::const_iterator iter_2 = ivec.begin();

    ++iter_2;     // VALID

    *iter_2 = 30; // syntax error
    // error: assignment of read-only location 
    // 'normal_iterator<const int*, 
    // std::vector<int, std::allocator<int>>>::operator*()'

    // --------------------------------------------------
  }
*/

/*
  #include <memory>   // std::unique_ptr, std::make_unique

  class AClass {
  public:
    void foo();         // non-const member function
    void bar()const;    // const member function
  };

  class BClass {
  private:
    std::unique_ptr<AClass> mp_AClass;

  public: 
    BClass() : mp_AClass{ std::make_unique<AClass>() } {}

    // non-const member function
    void f1() 
    {
      mp_AClass->foo();   // VALID
      mp_AClass->bar();   // VALID
    }

    // const member function
    void f2()const 
    {
      mp_AClass->foo();   // VALID
      mp_AClass->bar();   // VALID
    }
  };

  // PROBLEM : 
  // calling const member functions of non const member object
  // is valid which is not a desired behavior.
*/

/*
  // ------- SOLUTION 1 -------

  #include <memory>   // std::unique_ptr, std::make_unique

  class AClass {
  public:
    void foo();         // non-const member function
    void bar()const;    // const member function
  };

  class BClass {
  private:
    std::unique_ptr<AClass> mp_AClass;

    const AClass* get_pAClass() const
    {
      return mp_AClass.get();
    }

    AClass* get_pAClass()
    {
      return mp_AClass.get();
    }

  public: 
    BClass() : mp_AClass{ std::make_unique<AClass>() } {}

    // non-const member function
    void f1() 
    {
      get_pAClass()->foo();   // VALID
      get_pAClass()->bar();   // VALID
    }

    // const member function
    void f2()const 
    {
      get_pAClass()->foo();   // syntax error
      // error: passing 'const AClass' as 'this' argument 
      // discards qualifiers 

      get_pAClass()->bar();   // VALID
    }
  };
*/

/*
  // ------- SOLUTION 2 -------

  template <typename T>
  class deep_const_pointer {
  private:
    T* mp_obj;

  public:
    explicit deep_const_pointer(T* p) : mp_obj{ p } {}

    const T& operator*() const { return *mp_obj; }
    T& operator*() { return *mp_obj; }

    const T* operator->() const { return mp_obj; }  
    T* operator->() { return mp_obj; }
  };


  class AClass {
  public:
    void foo();
    void bar() const;
  };

  class BClass {
  private:
    deep_const_pointer<AClass> mp_AClass;

  public:
    // non-const member function
    void f1() 
    {
      mp_AClass->foo();   // VALID
      mp_AClass->bar();   // VALID
    }

    // const member function
    void f2() const 
    {
      mp_AClass->foo();   // syntax error
      // error: passing 'const AClass' as 'this' argument 
      // discards qualifiers

      mp_AClass->bar();   // VALID
    }
  };
*/

/*
                    -------------------------
                    | file buffer to stream |
                    -------------------------
*/

/*
  #include <fstream>  // std::ifstream

  int main()
  {
    std::ifstream ifs{ "../regex_module/floats.txt" };

    std::cout << ifs.rdbuf() << '\n';
    // output ->
    //  hello
    //  12.4
    //  +15.76985
    //  -1.2
    //  world
    //  9.87348654
    //  999990.12
    //  -1275634.333334
    //  galaxy
    //  93.333

    // global "operator<<" overload that takes 
    // std::filebuf* as a parameter.
    // this function will pass the buffer to the stream.

    // --------------------------------------------------

    // when a file wants to be copied
    // ifstream objects buffer can be passed to ofstream object

    std::ifstream ifs2{ "../regex_module/floats.txt" };
    std::ofstream ofs{ "copy.txt" };

    ofs << ifs2.rdbuf();

    // copy.txt ->
    //  hello
    //  12.4
    //  +15.76985
    //  -1.2
    //  world
    //  9.87348654
    //  999990.12
    //  -1275634.333334
    //  galaxy
    //  93.333

    // --------------------------------------------------
  }
*/

/*
            ------------------------------------------
            | std::accumulate algorithm in <numeric> |
            ------------------------------------------
*/

/*
  #include <vector>
  #include <cstddef>    // std::size_t
  #include <random>     // std::mt19937, std::random_device
  #include <algorithm>  // std::generate
  #include <numeric>    // std::accumulate

  using namespace std::literals;

  int main()
  {
    std::mt19937 eng;
    std::uniform_int_distribution dist{ 0ULL, 50ULL };

    constexpr std::size_t N = 1'000'000ULL;
    std::vector<unsigned long long> uvec(N);

    std::generate(uvec.begin(), uvec.end(), 
                  [&eng, &dist]{ return dist(eng); }); 

    auto sum1 = std::accumulate(uvec.begin(), uvec.end(), 0ULL);
    // sum1's data type is unsigned long long

    auto sum2 = std::accumulate(uvec.begin(), uvec.end(), 0);
    // sum2's data type is int

    auto sum3 = std::accumulate(uvec.begin(), uvec.end(), "hello"s);
    // sum3's data type is std::string
  }
*/

/*
  #include <vector>
  #include <numeric>  // std::accumulate
  #include <string>

  using namespace std::literals;

  int main()
  {
    std::vector ivec{ 11, 33, 55, 77, 99 };

    auto res = std::accumulate(
                  ivec.begin(), ivec.end(), ""s); // syntax error
    // error: no match for 'operator+' 
    // (operand types are 'std::basic_string<char>'} and 'int')
  }
*/

/*
  #include <vector>
  #include <numeric>  // std::accumulate
  #include <string>

  using namespace std::literals;

  int main()
  {
    std::vector ivec{ 11, 33, 55, 77, 99 };

    // --------------------------------------------------

    auto res = std::accumulate(
                  ivec.begin(), ivec.end(), ""s,
                  [](std::string str, int val) {
                    return str + std::to_string(val); });
    std::cout << res << '\n';   
    // output -> 1133557799

    // --------------------------------------------------

    res = std::accumulate(
                  ivec.begin(), ivec.end(), "00"s,
                  [](std::string str, int val) {
                    return str + '-' + std::to_string(val); });
    std::cout << res << '\n';   
    // output -> 00-11-33-55-77-99
    
    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <numeric>    // std::accumulate
  #include <string>
  #include <functional> // std::multiplies(function object)

  using namespace std::literals;

  int main()
  {
    std::vector ivec{ 1, 2, 3 };

    // --------------------------------------------------

    auto res = std::accumulate( ivec.begin(), ivec.end(), 
                                1, std::multiplies{});
    std::cout << res << '\n'; 
    // output -> 6
    // init = 1 -> 1 * 1 = 1
    // init = 1 -> 1 * 2 = 2
    // init = 2 -> 2 * 3 = 6

    // --------------------------------------------------

    res = std::accumulate(ivec.begin(), ivec.end(), 1, 
                  [](int x, int y) { return x * x + y * y; });
    std::cout << res << '\n';
    // output -> 73
    //  init = 1 -> (1 * 1 + 1 * 1) = 2
    //  init = 2 -> (2 * 2 + 2 * 2) = 8 
    //  init = 8 -> (8 * 8 + 3 * 3) = 73

    // --------------------------------------------------
  }
*/

/*
                  ----------------------------
                  | std::transform algorithm |
                  ----------------------------    
*/

/*
  #include <vector>
  #include <algorithm>  // std::transform, std::copy
  #include <iterator>   // std::ostream_iterator

  int main()
  {
    std::vector ivec1{ 1, 2, 3, 4, 5 };
    std::vector<int> ivec2(ivec1.size());

    std::transform(ivec1.begin(), ivec1.end(), 
                  ivec2.begin(), 
                  [](int x){ return x * x; });

    std::copy(ivec1.begin(), ivec1.end(), 
              std::ostream_iterator<int>{ std::cout, " " });
    // output -> 1 2 3 4 5 
    std::cout << '\n';

    std::copy(ivec2.begin(), ivec2.end(), 
              std::ostream_iterator<int>{ std::cout, " " });
    // output -> 1 4 9 16 25
  }
*/

/*
  #include <vector>
  #include <list>
  #include <deque>  
  #include <algorithm>    // std::transform, std::copy
  #include <iterator>   
  // std::back_inserter, std::ostream_iterator
  #include <functional>   // std::multiplies(function object)

  int main()
  {
    std::vector<int> ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::list<int> ilist{ 11, 22, 33, 44, 55, 66, 77, 88, 99 };
    std::deque<int> ideque;

    // --------------------------------------------------

    std::transform( 
      ivec.begin(), ivec.end(),                     // source_1
      ilist.begin(),                                // source_2
      std::back_inserter(ideque),                   // destination
      [](int x, int y) { return x * x + y * y; });  // BinaryPred

    std::copy(ivec.begin(), ivec.end(), 
              std::ostream_iterator<int>{ std::cout, " " });
    std::cout << '\n';
    // output -> 1 2 3 4 5 6 7 8 9

    std::copy(ilist.begin(), ilist.end(), 
              std::ostream_iterator<int>{ std::cout, " " });
    std::cout << '\n';
    // output -> 11 22 33 44 55 66 77 88 99

    std::copy(ideque.begin(), ideque.end(), 
              std::ostream_iterator<int>{ std::cout, " " });
    std::cout << '\n';
    // output -> 122 488 1098 1952 3050 4392 5978 7808 9882

    // --------------------------------------------------

    std::transform( ivec.begin(), ivec.end(),     // source_1
                    ilist.begin(),                // source_2
                    ivec.begin(),                 // destination
                    std::multiplies{});           // BinaryPred

    std::copy(ivec.begin(), ivec.end(), 
              std::ostream_iterator<int>{ std::cout, " " });
    // output -> 11 44 99 176 275 396 539 704 891
    std::cout << '\n';

    // --------------------------------------------------
  }
*/

/*
                --------------------------------
                | std::inner_product algorithm |
                --------------------------------
*/

/*
  #include <numeric>    // std::inner_product
  #include <vector>
  #include <string>
  #include <functional> // std::plus, std::equal_to

  int main()
  {
    std::vector<std::string> svec1{ 
        "aa", "bb", "cc", "dd", "ee", "ab", "cd", "ef", "gh" };
      
    std::vector<std::string> svec2{ 
        "aa", "cb", "cc", "ef", "gg", "xx", "yy", "ef", "zz" };

    // to find out which elements 
    // are common in both vectors with same index

    auto count = std::inner_product( 
                    svec1.begin(), svec1.end(), 
                    svec2.begin(), 
                    0,
                    std::plus{}, std::equal_to{});

    std::cout << "count = " << count << '\n'; 
    // output -> count = 3

    // transform  => equal_to
    // reduce     => plus
  }
*/

/*
                  ------------------------------
                  | std::partial_sum algorithm |
                  ------------------------------
*/

/*
  #include <vector>
  #include <numeric>    // std::partial_sum
  #include <iterator>   // std::ostream_iterator
  #include <functional> // std::multiplies

  int main()
  {
    std::vector ivec{ 1, 5, 9, 12, 17 };

    // ------------------------------------------------------

    std::partial_sum( 
        ivec.begin(), ivec.end(), 
        std::ostream_iterator<int>{ std::cout, " " });
    // output -> 1 6 15 27 44
    // (1), (1 + 6), (7 + 9), (15 + 12), (27 + 17)
    // default binary predicate is std::plus{}

    std::cout << '\n';

    // ------------------------------------------------------

    std::partial_sum( 
        ivec.begin(), ivec.end(), 
        std::ostream_iterator<int>{ std::cout, " " },
        std::multiplies{});
    // output -> 1 5 45 540 9180
    // (1), (1 * 5), (5 * 9), (45 * 12), (540 * 17)

    std::cout << '\n';

    // ------------------------------------------------------

    std::partial_sum( 
        ivec.begin(), ivec.end(), 
        std::ostream_iterator<int>{ std::cout, " " },
        [](int a, int b){ return a * a + b; });
    // output -> 1 6 45 2037 4149386

    //  (1), (1 * 1 + 5), (6 * 6 + 9), 
    //  (45 * 45 + 12), (2037 * 2037 + 17)

    // ------------------------------------------------------
  }
*/


// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------