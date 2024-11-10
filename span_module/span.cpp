#include <iostream>

/*
                        =================
                        | <span> module |
                        =================
*/

/*
  - string_view is a non-owning range to a string
  - span is a non-owning range to a contiguous sequence of objects
*/

/*
  #include <cstddef>

  template <typename T, std::size_t N = SIZE_MAX>
  class Static_Span {
  private:
    T* p_begin;
  };

  // what if our span will sometimes point to a fixed size array, 
  // and sometimes size will be calculated at runtime.
  // we can use template parameter with a default argument.
  // when template argument provided, it will declare that
  // the size is fixed at compile time, and when not provided
  // it will declare that the size is calculated at runtime.
*/

/*
  #include <span>
  #include <vector>

  int main()
  {
    using namespace std;

    int arr[] = { 1, 2, 3, 4, 5 };
    vector ivec = { 1, 2, 3, 4, 5 };

    // -------------------------------------------------

    span s1 = arr; 
    // s1's type is span<int, 5U>

    std::cout << "sizeof(s1) = " << sizeof(s1) << '\n';
    // output -> sizeof(s1) = 8

    // static extent, span is holding only 1 pointer variable.

    // -------------------------------------------------

    span s2 = { ivec.begin(), 3 };
    // s2's type is span<int, SIZE_MAX>

    std::cout << "sizeof(s2) = " << sizeof(s2) << '\n';
    // output -> sizeof(s2) = 16

    // dynamic extent, span is holding 2 pointer variables.

    // -------------------------------------------------
  }
*/

/*
  template <typename T>
  void func(std::span<T> s);
*/

/*
  #include <vector>
  #include <span>

  int main()
  {
    using namespace std;

    vector<int> ivec(20);

    span sp(ivec.begin(), 5);

    // -------------------------------------------------

    sp.extent; // compile time constant 

    // if static span, extent will be the size of the fixed sized range.
    // if dynamic span, extent will be SIZE_MAX.

    // -------------------------------------------------

    sp.size();

    // if static span, size and extent will be the same.
    // if dynamic span, size will be the size of the range, 
    // and extent will be SIZE_MAX.

    // -------------------------------------------------
  }
*/

/*
  inline constexpr std::size_t dynamic_extent = 
      std::numeric_limits<std::size_t>::max();

  inline constexpr std::size_t dynamic_extent = -1;
*/

/*
  #include <span>
  #include <array>

  int main()
  {
    using namespace std;

    // -------------------------------------------------

    span<int, 5> sp1; // syntax error
    // error: no matching function for call to 
    // 'std::span<int, 5>::span()'

    // if static extent, span classes default ctor is not declared.

    // -------------------------------------------------

    span<int, 0> sp2;
    cout << "sp2.extent = " << sp2.extent << '\n';  
    // output -> sp2.extent = 0

    // -------------------------------------------------

    span<int> sp3;  // VALID
    cout << "sp3.extent = " << sp3.extent << '\n';
    // output -> sp3.extent = 18446744073709551615 (SIZE_MAX)

    cout << "sp3.size() = " << sp3.size() << '\n';
    // output -> sp3.size() = 0

    constexpr bool b1 = (sp3.extent == dynamic_extent);
    // b1 -> true (dynamic extent)

    // if dynamic extent, span class is default constructible.

    // -------------------------------------------------

    int arr[]{ 1, 2, 3, 4, 5, 6 };

    span<int> sp4{ arr };
    // "sp4"'s type is span<int>
    cout << "sp4.extent = " << sp4.extent << '\n';
    // output -> sp4.extent = 18446744073709551615 (SIZE_MAX)

    constexpr bool b2 = (sp4.extent == dynamic_extent);
    // b2 -> true (dynamic extent)

    // -------------------------------------------------

    span sp5{ arr }; // CTAD
    // "sp5"'s type is span<int, 6U>
    cout << "sp5.extent = " << sp5.extent << '\n';
    // output -> sp5.extent = 6

    constexpr bool b3 = (sp5.extent == dynamic_extent);
    // b3 -> false (static extent)
    
    // -------------------------------------------------

    array<int, 3> arr2{ 1, 2, 3 };

    span<int, 3> sp6{ arr2 };
    cout << "sp6.extent = " << sp6.extent << '\n';
    // output -> sp6.extent = 3

    constexpr bool b4 = (sp6.extent == dynamic_extent);
    // b4 -> false (static extent)

    // -------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <cassert>
  #include <span>

  int main()
  {
    std::vector<int> ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // -------------------------------------------------

    std::span<int, 7> sp1{ ivec };
    assert(sp1.size() == ivec.size());  // holds

    constexpr bool b1 = (sp1.extent == std::dynamic_extent);
    // b1 -> false (static extent)

    // -------------------------------------------------

    std::span<int, 10> sp2{ ivec }; // undefined behavior(UB)
    // used static extent but initialized with an object 
    // that its size can be changed.

    std::span<int, 5> sp3{ ivec };  // undefined behavior(UB)
    // used static extent but initialized with an object
    // that its size can be changed.

    // -------------------------------------------------

    std::span<int, 7> sp4(ivec, 7);
    // no matching function for call to 
    // 'std::span<int, 7>::span(std::vector<int>&, int)'

    // -------------------------------------------------

    std::span<int, 5> sp5{ ivec.begin(), 5 }; // VALID

    // -------------------------------------------------

    std::span<int, 3> sp6{ ivec.begin(), 5 }; // undefined behavior(UB)

    std::span<int, 8> sp7{ ivec.begin(), 5 }; // undefined behavior(UB)

    // -------------------------------------------------

    std::span<int, 5> sp8{ ivec.begin() }; // syntax error
    // error: no matching function for call to 
    // 'std::span<int, 5>::span(<brace-enclosed initializer list>)'

    // -------------------------------------------------

    std::span<int, 5> sp9(ivec.begin()); // syntax error
    // error: no matching function for call to 
    // 'std::span<int, 5>::span(std::vector<int>::iterator)'

    // -------------------------------------------------
  }
*/

/*
  #include <array>
  #include <span>

  int main()
  {
    int C_arr[10];
    std::array arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    // -------------------------------------------------

    std::span<int> sp1{ C_arr };
    constexpr bool b1 = (sp1.extent == std::dynamic_extent);
    // b1 -> true (dynamic extent)

    // -------------------------------------------------

    std::span<int> sp2{ arr };
    constexpr bool b2 = (sp2.extent == std::dynamic_extent);
    // b2 -> true (dynamic extent)

    // -------------------------------------------------

    std::span<int, 5> sp3{ C_arr }; // syntax error
    // error: no matching function for call to 
    // 'std::span<int, 5>::span(<brace-enclosed initializer list>)'

    // using static extent with NOT VALID range.

    // -------------------------------------------------

    std::span<int, 5> sp4{ arr };   // syntax error
    // error: no matching function for call to 
    // 'std::span<int, 5>::span(<brace-enclosed initializer list>)'

    // using static extent with NOT VALID range.

    // -------------------------------------------------

    std::span<int, 5> sp5{ arr.data(), 5 };

    constexpr bool b3 = (sp5.extent == std::dynamic_extent);
    // b3 -> false (static extent)

    // -------------------------------------------------
  }
*/

/*
  #include <span>

  int main()
  {
    std::span<const int>;
    // we CAN'T CHANGE the elements of this span.(read-only)

    std::span<int>;
    // we CAN CHANGE the elements of this span.

    const std::span<int>;
    // we CAN'T CHANGE span object itself. 
    // non-const member functions are not callable.
  }
*/

/*
  #include <vector>
  #include <span>

  int main()
  {
    std::vector<int> ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // -------------------------------------------------

    std::span<const int> sp1{ ivec };
    constexpr bool b1 = sp1.extent == std::dynamic_extent;
    // b1 -> true (dynamic extent)

    sp1.back() = 10; // syntax error
    // error: assignment of read-only location 
    // 'sp1.std::span<const int>::back()'

    sp1[0] = 11;     // syntax error
    // error: assignment of read-only location 
    // 'sp1.std::span<const int>::operator[](0)'

    // -------------------------------------------------

    std::span<int> sp2{ ivec };
    constexpr bool b2 = sp2.extent == std::dynamic_extent;
    // b2 -> true (dynamic extent)

    sp2.back() = 10; // VALID
    sp2[0] = 11;     // VALID

    // -------------------------------------------------

    std::span<int> sp3{ sp1 };  // syntax error
    // CAN NOT convert from 
    // 'std::span<const int>' to 'std::span<int>'

    // -------------------------------------------------

    std::span<const long> sp4{ sp1 };  // syntax error
    // CAN NOT convert from
    // 'std::span<const int>' to 'std::span<const long>'

    // -------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <span>

  int main()
  {
    std::vector<int> ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::span<long long> sp1{ ivec };  // syntax error
    // type check is done in compile time. 
    // range elements are `int` type.
  }
*/

/*
  #include <array>
  #include <span>
  #include <vector>
  #include <concepts> // std::same_as

  int main()
  {
    std::array arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::vector ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    std::span<int> vec_span{ ivec };  // VALID
    constexpr bool b1 = vec_span.extent == std::dynamic_extent;
    // b1 -> true (dynamic extent)

    std::span<int> arr_span{ arr };   // VALID
    constexpr bool b2 = arr_span.extent == std::dynamic_extent;
    // b2 -> true (dynamic extent)

    static_assert(std::same_as< decltype(vec_span), 
                                decltype(arr_span) >);  // VALID
  }
*/

/*
  #include <array>
  #include <span>

  int main()
  {
    std::array arr_1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::array arr_2{ 0, 9, 18 };

    // -------------------------------------------------

    const std::span<int> c_sp1{ arr_1 }; 
    // span itself is const

    constexpr bool b1 = (c_sp1.extent == std::dynamic_extent);
    // b1 -> true (dynamic extent)

    c_sp1[0] = 10;  // VALID

    c_sp1 = arr_2;  // syntax error
    // error: passing 'const std::span<int>' 
    // as 'this' argument discards qualifiers

    // -------------------------------------------------

    std::span<const int> sp2{ arr_2 };   
    // span elements(viewed elements) are const

    constexpr bool b2 = (sp2.extent == std::dynamic_extent);
    // b2 -> true (dynamic extent)

    sp2[0] = 10;    // syntax error
    // error: assignment of read-only location 
    // 'sp2.std::span<const int>::operator[](0)'

    sp2 = arr_2;    // VALID

    // -------------------------------------------------
  }
*/

/*
  #include <span>
  #include <vector>

  template <typename T, std::size_t N>
  void print_span(std::span<T, N> sp)
  {
    std::cout << "size = " << sp.size() << '\n';

    if constexpr (N == std::dynamic_extent)
      std::cout << "dynamic extent\n";
    else
      std::cout << "fixed/static extent\n";

    for (const auto& elem : sp)
      std::cout << elem << ' ';
    std::cout << '\n';
  }

  int main()
  {
    // -------------------------------------------------

    int arr[]{ 1, 2, 3 };
    std::span sp1{ arr };     
    // sp1's type is span<int, 3U>

    print_span(sp1);
    // output ->
    //  size = 3
    //  fixed/static extent
    //  1 2 3

    // -------------------------------------------------

    print_span(std::span{ arr });
    // output ->
    //  size = 3
    //  fixed/static extent
    //  1 2 3

    // -------------------------------------------------

    std::vector ivec{ 2, 4, 6, 8, 10 };
    std::span sp2{ ivec };    
    // sp2's type is span<int, SIZE_MAX>

    print_span(sp2);
    // output ->
    //  size = 5
    //  dynamic extent
    //  2 4 6 8 10

    // -------------------------------------------------

    print_span(std::span{ ivec });
    // output ->
    //  size = 5
    //  dynamic extent
    //  2 4 6 8 10

    // -------------------------------------------------

    print_span(arr);  // syntax error (can't deduce template arguments)
    // error: no matching function for call to 
    // 'print_span(int [3])'

    print_span(ivec); // syntax error (can't deduce template arguments)
    // error: no matching function for call to 
    // 'print_span(std::vector<int, std::allocator<int> >&)'

    // -------------------------------------------------
  }
*/

/*
  // span is a non-owning range to a contiguous sequence of objects

  #include <span>

  int main()
  {
    int arr[]{ 1, 2, 3, 4, 5 };

    std::span sp(arr);

    for (auto& elem : arr)
      elem *= 10;

    for (auto elem : sp)
      std::cout << elem << ' ';
    // output -> 10 20 30 40 50
  }
*/

/*
  #include <span>

  int main()
  {
    int arr[]{ 1, 2, 3, 4, 5 };

    std::span sp(arr);

    for (auto& elem : sp)
      elem *= 10;

    for (auto elem : arr)
      std::cout << elem << ' ';
    // output -> 10 20 30 40 50
  }
*/

/*
  #include <vector>
  #include <span>

  std::vector<int> get_vec()
  {
    return { 1, 2, 3, 4, 5 };
  }

  int main()
  {
    std::span sp{ get_vec().begin(), 5 };

    // "get_vec()" is RValue expression
    // because that it did not bind to any reference,
    // after the expression is evaluated, it will be destroyed.
    // so span objects pointer will become dangling pointer.

    for (auto elem : sp)
      std::cout << elem << ' '; // undefined behavior(UB)
  }
*/

/*
  #include <vector>
  #include <span>

  auto get_spn()
  {
    std::vector<int> ivec{ 1, 2, 3, 4, 5 };
    return std::span{ ivec };
    // returning a span object that points to a local object.
    // which will be destroyed after the function returns.
    // so span objects pointer will become dangling pointer.
  }

  int main()
  {
    auto sp = get_spn();

    for (auto elem : sp)
      std::cout << elem << ' '; // undefined behavior(UB)
  }
*/

/*
  #include <span>
  #include <vector>

  void print_span(std::span<int> sp)
  {
    for (auto elem : sp)
      std::cout << elem << ' ';
    std::cout << '\n';
  }

  int main()
  {
    using namespace std;

    vector ivec{ 1, 2, 3, 4, 5 };
    span sp(ivec);

    cout << "ivec.capacity() = " << ivec.capacity() << '\n';  
    // output -> ivec.capacity() = 5

    print_span(sp);
    // output -> 1 2 3 4 5

    for (int i = 0; i < 10; ++i)
      ivec.push_back(i);

    cout << "ivec.capacity() = " << ivec.capacity() << '\n';  
    // output -> ivec.capacity() = 20

    // -------------------------------------------------

    print_span(sp); // undefined behavior(UB) 
    // reallocation makes the span object's pointer dangling.

    // -------------------------------------------------

    sp = ivec;
    // assignment to span object with a new range.
    print_span(sp); // VALID
    // output -> 1 2 3 4 5 0 1 2 3 4 5 6 7 8 9

    // -------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <span>

  int main()
  {
    std::vector<int> ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::cout << "vec.size() = " << ivec.size() << '\n';
    std::cout << "vec.capacity() = " << ivec.capacity() << '\n';
    // output ->
    //  vec.size() = 9
    //  vec.capacity() = 9

    std::span sp1{ ivec };
    ivec.insert(ivec.end(), { 10, 11, 12, 13, 14, 15 });

    std::cout << "vec.size() = " << ivec.size() << '\n';
    std::cout << "vec.capacity() = " << ivec.capacity() << '\n';
    // output ->
    //  vec.size() = 15
    //  vec.capacity() = 18

    std::cout << sp1[0] << '\n'; // undefined behavior(UB)
    // reallocation makes the span object's pointer dangling.
  }
*/

/*
  #include <span>
  #include <vector>

  int main()
  {
    std::cout <<std::boolalpha;

    std::vector ivec = { 1, 2, 3, 4, 5 };
    std::span sp(ivec);

    std::cout << "sp.size() = " << sp.size() << '\n';
    std::cout << "sp.size_bytes() = " << sp.size_bytes() << '\n';
    std::cout << "sp.extent = " << sp.extent << '\n';
    // output ->
    //  sp.size() = 5
    //  sp.size_bytes() = 20
    //  sp.extent = 18446744073709551615 (SIZE_MAX)

    // -------------------------------------------------

    std::cout << sp[0] << '\n';       // output -> 1

    sp[0] *= 10;
    std::cout << ivec[0] << '\n';     // output -> 10
    std::cout << sp.front() << '\n';  // output -> 10

    ++sp.front(); 
    std::cout << ivec[0] << '\n';     // output -> 11

    // -------------------------------------------------

    sp.back() *= 10;
    std::cout << ivec.back() << '\n'; // output -> 50

    // -------------------------------------------------

    std::cout << "sp.empty() = " << sp.empty() << '\n';
    // output -> sp.empty() = false
    
    // -------------------------------------------------

    for (size_t i{}; i < sp.size(); ++i)
      std::cout << *(sp.data() + i) << ' ';
    // output -> 11 2 3 4 50
    
    // -------------------------------------------------
  }
*/

/*
  // span is a view(satisfies the ranges::view concept)

  #include <vector>
  #include <span>
  #include <ranges>

  int main()
  {
    std::vector ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::span sp1{ ivec };

    static_assert(std::ranges::view<decltype(sp1)>);  // VALID
  }
*/

/*
  #include <ranges>
  #include <span>

  int main()
  {
    std::span sp{ "hello galaxy" };
    static_assert(std::ranges::view<decltype(sp)>);  // VALID

    for (auto ch : (sp | std::views::drop(6) | std::views::take(4)))
      std::cout << ch << ' ';
    // output -> g a l a
  }

  // null('\0') character at the end of the string(const char array)
  // is also included in the span.
*/

/*
                        -----------------
                        | span::subspan |
                        -----------------
*/

/*
  #include <span>

  template <typename T, std::size_t N>
  requires requires (T x) { std::cout << x; }
  void print_span(std::span<T, N> sp)
  {
    for (size_t i{}; i < sp.size(); ++i)
      std::cout << sp[i] << ' ';
    std::cout << '\n';
  }

  int main()
  {
    int arr[10]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // -------------------------------------------------

    std::span sp1{ arr }; 
    // sp1's type is span<int, 10U>

    constexpr bool b1 = (sp1.extent == std::dynamic_extent);
    // b1 -> false (static extent)

    print_span(sp1);
    // output -> 0 1 2 3 4 5 6 7 8 9

    // -------------------------------------------------

    auto sp2 = sp1.subspan(3, 4); 
    // sp2's type is span<int, SIZE_MAX>

    constexpr bool b2 = (sp2.extent == std::dynamic_extent);
    // b2 -> true (dynamic extent)

    print_span(sp2);
    // output -> 3 4 5 6

    // -------------------------------------------------

    auto sp3 = sp1.subspan(3);
    // sp3's type is span<int, SIZE_MAX>

    constexpr bool b3 = (sp3.extent == std::dynamic_extent);
    // b3 -> true (dynamic extent)

    print_span(sp3);
    // output -> 3 4 5 6 7 8 9

    // -------------------------------------------------
  }
*/

/*
  #include <span>
  #include <ranges>

  int main()
  {
    int arr[10]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::span sp1{ arr };

    for (auto elem : sp1)
      std::cout << elem << ' ';
    std::cout << '\n';
    // output -> 0 1 2 3 4 5 6 7 8 9

    for (auto elem : sp1 | std::views::reverse)
      std::cout << elem << ' ';
    std::cout << '\n';
    // output -> 9 8 7 6 5 4 3 2 1 0

    for (auto iter = sp1.cbegin(); iter != sp1.cend(); ++iter)
      std::cout << *iter << ' ';
    std::cout << '\n';
    // output -> 0 1 2 3 4 5 6 7 8 9

    for (auto iter = sp1.crbegin(); iter != sp1.crend(); ++iter)
      std::cout << *iter << ' ';
    std::cout << '\n';  
    // output -> 9 8 7 6 5 4 3 2 1 0
  }
*/

/*
  #include <span>

  template <typename T, std::size_t N>
  void print_span(std::span<T, N> sp)
  {
    for (size_t i{}; i < sp.size(); ++i)
      std::cout << sp[i] << ' ';
    std::cout << '\n';
  }

  int main()
  {
    int arr[10]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // -------------------------------------------------

    std::span sp1{ arr };     // sp1's type is span<int, 10U>
    constexpr bool b1 = (sp1.extent == std::dynamic_extent);
    // b1 -> false (static extent)

    std::cout << "sp1.size() = " << sp1.size() << '\n';
    // output -> sp1.size() = 10
    print_span(sp1);  // output -> 0 1 2 3 4 5 6 7 8 9

    // -------------------------------------------------

    auto sp2 = sp1.first<3>();  // sp2's type is span<int, 3U>
    constexpr bool b2 = (sp2.extent == std::dynamic_extent);
    // b2 -> false (static extent)

    std::cout << "sp2.size() = " << sp2.size() << '\n';
    // output -> sp2.size() = 3
    print_span(sp2);  // output -> 0 1 2

    // -------------------------------------------------

    auto sp3 = sp1.first(3);    // sp3's type is span<int, SIZE_MAX>
    constexpr bool b3 = (sp3.extent == std::dynamic_extent);
    // b3 -> true (dynamic extent)

    std::cout << "sp3.size() = " << sp3.size() << '\n';
    print_span(sp3);  // output -> 0 1 2

    // -------------------------------------------------

    auto sp4 = sp1.last<4>();   // sp4's type is span<int, 4U>
    constexpr bool b4 = (sp4.extent == std::dynamic_extent);
    // b3 -> false (static extent)

    std::cout << "sp4.size() = " << sp4.size() << '\n';
    // output -> sp4.size() = 4
    print_span(sp4);  // output -> 6 7 8 9

    // -------------------------------------------------

    auto sp5 = sp1.last(4);     // sp5's type is span<int, SIZE_MAX>
    constexpr bool b5 = (sp5.extent == std::dynamic_extent);
    // b5 -> true (dynamic extent)

    std::cout << "sp5.size() = " << sp5.size() << '\n';
    // output -> sp5.size() = 4
    print_span(sp5);  // output -> 6 7 8 9

    // -------------------------------------------------
  }
*/

/*
  // std::views::counted range adaptor returns a std::span object.

  #include <span>
  #include <vector>
  #include <ranges> // std::same_as, std::views::counted

  int main()
  {
    using namespace std;

    vector ivec{ 2, 5, 7, 9, 3, 6, 4 };

    auto spn = views::counted(ivec.begin(), 5);

    static_assert(same_as<decltype(spn), span<int>>); // VALID
  }
*/