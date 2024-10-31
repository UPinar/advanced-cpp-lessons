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