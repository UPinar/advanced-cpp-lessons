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