#include <iostream>

/*
                            ==========
                            | ranges |
                            ==========
*/

/*
  #include <vector>
  #include <algorithm>    // std::sort, std::copy
  #include <iterator>     // std::ostream_iterator
  #include <ranges>       // std::ranges::sort, std::ranges::copy
  #include "../nutility.h"

  int main()
  {
    std::vector<int> vec;
    std::vector<int> vec2;
    rfill(vec, 15, Irand{10, 50});
    rfill(vec2, 15, Irand{10, 50});

    // --------------------------------------------------

    std::sort(vec.begin(), vec.end());

    std::copy(vec.begin(), 
              vec.end(), 
              std::ostream_iterator<int>{ std::cout, " " });
    // output -> 15 17 23 26 27 32 34 37 37 40 41 41 42 43 44

    std::cout << '\n';
    // --------------------------------------------------

    std::ranges::sort(vec2);
    std::ranges::copy(vec2, std::ostream_iterator<int>{ std::cout, " " });
    // output -> 10 11 21 23 26 28 31 32 39 41 41 44 45 46 47

    // --------------------------------------------------
  }
*/

/*
  #include <list>
  #include <ranges>
  #include <algorithm>

  #include "../nutility.h"

  int main()
  {
    using namespace std;

    std::list<int> ilist;
    rfill(ilist, 15, Irand{10, 50});

    std::ranges::sort(ilist); // syntax error
    // error: no match for call to '(const std::ranges::__sort_fn) 
    // (std::__cxx11::list<int>&)'
    // note: 'std::random_access_iterator_tag' 
    // is not a base of 'std::bidirectional_iterator_tag'
  }
*/

/*
  #include <concepts>
  #include <ranges>

  template <class T>
  concept Range = requires(T t)
  {
    std::ranges::begin(t);
    std::ranges::end(t);
  };
*/

/*
  // std::ranges::range is a standard concept

  #include <ranges>
  #include <concepts>
  #include <string>
  #include <string_view>

  template <std::ranges::range T>
  void Algorithm(T&& collection);

  void Algorithm_2(std::ranges::range auto&& collection)


  int main()
  {
    // --------------------------------------------------

    Algorithm(231); // syntax error
    // error: no matching function for call to 'Algorithm(int)'
    // note: the required expression 'std::ranges::_Cpo::begin(__t)' 
    // note: the required expression 'std::ranges::_Cpo::end(__t)'
    // are invalid

    // int type does not satisfy the range concept

    // --------------------------------------------------

    Algorithm(std::string{ "hello world" }); // VALID
    // std::string type is satisfies the range concept

    // --------------------------------------------------

    int arr[10]{};
    Algorithm(arr); // VALID
    // C-style array type satisfies the range concept

    // --------------------------------------------------

    using namespace std::literals;
    Algorithm("hello universe"sv); // VALID
    // std::string_view type satisfies the range concept

    // --------------------------------------------------
  }
*/

/*
  // all other range concepts subsume std::ranges::range concept

  #include <ranges>
  #include <concepts>
  #include <vector>
  #include <deque>
  #include <list>
  #include <forward_list>

  void algo(std::ranges::range auto&&)
  {
    std::cout << "algo(std::ranges::range auto&&)\n";
  }

  // concept input_range = 
  //    range<_Rng> && input_iterator<iterator_t<_Rng>>;
  // input_range concept subsumes range concept
  void algo(std::ranges::input_range auto&&)
  {
    std::cout << "algo(std::ranges::input_range auto&&)\n";
  }

  // concept forward_range = 
  //    range<_Rng> && forward_iterator<iterator_t<_Rng>>;
  // forward_range concept subsumes input_range concept
  void algo(std::ranges::forward_range auto&&)
  {
    std::cout << "algo(std::ranges::forward_range auto&&)\n";
  }

  // concept bidirectional_range = 
  //    range<_Rng> && bidirectional_iterator<iterator_t<_Rng>>;
  // bidirectional_range concept subsumes forward_range concept
  void algo(std::ranges::bidirectional_range auto&&)
  {
    std::cout << "algo(std::ranges::bidirectional_range auto&&)\n";
  }

  // concept contiguous_range = range<_Rng> && 
  //    contiguous_iterator<iterator_t<_Rng>> 
  // contiguous_range concept subsumes bidirectional_range concept
  void algo(std::ranges::contiguous_range auto&&)
  {
    std::cout << "algo(std::ranges::contiguous_range auto&&)\n";
  }

  // concept random_access_range = 
  //  range<_Rng> && random_access_iterator<iterator_t<_Rng>>;
  // random_access_range concept subsumes contiguous_range concept
  void algo(std::ranges::random_access_range auto&&)
  {
    std::cout << "algo(std::ranges::random_access_range auto&&)\n";
  }

  // Those are all function overloads.

  int main()
  {
    algo(std::vector<int>{});
    // output -> algo(std::ranges::contiguous_range auto&&)

    algo(std::deque<int>{});
    // output -> algo(std::ranges::random_access_range auto&&)

    algo(std::list<int>{});
    // output -> algo(std::ranges::bidirectional_range auto&&)

    algo(std::forward_list<int>{});
    // output -> algo(std::ranges::forward_range auto&&)
  }
*/

/*
  // std::ranges::sized_range is a standard concept

  #include <ranges>
  #include <concepts>
  #include <vector>
  #include <forward_list>

  void algo(std::ranges::range auto&&)
  {
    std::cout << "algo(std::ranges::range auto&&)\n";
  }

  void algo(std::ranges::forward_range auto&&)
  {
    std::cout << "algo(std::ranges::forward_range auto&&)\n";
  }

  void algo(std::ranges::contiguous_range auto&&)
  {
    std::cout << "algo(std::ranges::contiguous_range auto&&)\n";
  }

  // concept sized_range = range<_Rng> && requires(_Rng& __r) { 
  //  _RANGES size(__r); };
  // sized_range concept subsumes random_access_range concept
  // have also a requires expression, 
  // _RANGES size(__r) is a valid expression 
  // (can be calculated its size in constant time)
  void algo(std::ranges::sized_range auto&&)
  {
    std::cout << "algo(std::ranges::sized_range auto&&)\n";
  }

  int main()
  {
    // --------------------------------------------------

    algo(std::forward_list<int>{});
    // output -> algo(std::ranges::forward_range auto&&)

    // void algo(std::ranges::range auto&&)           is VALID
    // void algo(std::ranges::forward_range auto&&)   is VALID
    // void algo(std::ranges::sized_range auto&&)     NOT VALID

    // because of forward_range concept subsumes range concept
    // forward_range will be selected

    // --------------------------------------------------

    algo(std::vector<int>{}); // syntax error
    // error: call of overloaded 'algo(std::vector<int>)' is ambiguous

    // void algo(std::ranges::range auto&&)             is VALID
    // void algo(std::ranges::contiguous_range auto&&)  is VALID
    // void algo(std::ranges::sized_range auto&&)       is VALID

    // contiguous_range concept subsumes range concept
    // sized_range concept subsumes range concept

    // sized_range is NOT subsume contiguous_range concept
    // and contiguous_range is NOT subsume sized_range concept
    // so this call will be ambiguous

    // --------------------------------------------------
  }
*/

/*
  template <std::random_access_iterator I,
            std::sentinel_for<I> S,
            class Comp = std::ranges::less, 
            class Proj = std::identity>
  requires std::sortable<I, Comp, Proj>
  constexpr I sort(I first, S last, Comp comp = {}, Proj proj = {});
*/

/*
  template <typename Iter>
  void old_Print(Iter beg, Iter end)
  {
    while (beg != end)
      std::cout << *beg++ << ' ';
  }

  struct Null_Character {
    bool operator==(auto x) const { return *x == '\0'; }
  };

  template <typename Iter, typename Sentinel>
  void Print(Iter beg, Sentinel end)
  {
    while (beg != end)
      std::cout << *beg++ << ' ';
  }

  int main()
  {
    char str[100] = "hello world";

    Print(str, Null_Character{}); 
    // output -> h e l l o   w o r l d

    // no need to send the location of '\0' character at
    // the end of the char array as an Iter type.
    // so also no need to find(iterate over the array) it.
  }
*/

/*
  // ranges::sort is constexpr object from a class type 
  // that class type has operator() call function

  #include <ranges> 
  #include <algorithm>

  int main()
  {
    using namespace std;

    ranges::sort;
    // class _Sort_fn {};
    // inline constexpr _Sort_fn sort;
  }
*/

/*
                    ------------------------
                    | rangified algorithms |
                    ------------------------
*/

/*
  - paralel çalıştırılmaları mümkün değil.
  - numeric algoritmaların hepsi rangified değil. Zamanla eklenecek.
  - iterator parametreli overloadlarının ilk iki parametresi 
    aynı türden olmak zorunda değil.

    template <typename Iter, typename Sentinel>
    void algo(Iter first, Sentinel last);
      - Iter ve Sentinel aynı türden iseler -> common range

  - std::counted_iterator kaç kere arttırılabileceğini biliyor.
    dolayısıyla karşılaştırılmaya ihtiyacı yok. sentinel olarak 
    bu türün interface'indeki fonksiyonlar kullanılıp, karşılaştırma
    işlemi yapılmadan algoritma çalıştırılabilir.
*/

/*
  #include <iterator>
  #include <vector>
  #include <cstdio>

  int main()
  {
    using namespace std;

    // --------------------------------------------------

    vector<int> ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    counted_iterator iter{ ivec.begin(), 3 };

    while (iter.count() != 0)
      cout << *iter++ << ' ';
    // output -> 1 2 3

    std::putchar('\n');

    // --------------------------------------------------

    // rangified algorithmanın bu seneryo için kullanımı : 
    // default_sentinel ikinci parametre değişkeni olarak gönderilir ve
    // default_sentnel konu mankeni olarak kullanılır.

    for ( counted_iterator iter(ivec.begin(), 5); 
          iter != default_sentinel; ++iter)
      cout << *iter << ' ';
    // output -> 1 2 3 4 5


    // --------------------------------------------------
  } 
*/

/*
  #include <algorithm>
  #include <ranges>
  #include <vector>

  template <auto endval>  // Non-type template parameter
  struct EndStruct {
    bool operator==(auto pos) const { return *pos == endval; } 
  };

  int main()
  {
    std::vector<int> ivec{ 75, 93, 12, 8, 3, 9, 0, 4, 15, 82, 7 };

    // --------------------------------------------------

    // we know that there is `9` in vector
    // 2 iterator template parameter rangified algorithm(find) overload

    constexpr int val = 9;
    auto iter = std::ranges::find(ivec.begin(), EndStruct<val>{}, val);
    std::cout << *iter << '\n'; // output -> 9

    // --------------------------------------------------

    // 2 iterator template parameter rangified algorithm(sort) overload
    std::ranges::sort(ivec.begin(), EndStruct<9>{});
    // this algorithm will sort the vector until 9

    for (auto val : ivec)
      std::cout << val << ' ';
    // output -> 3 8 12 75 93 9 0 4 15 82 7

    std::putchar('\n');

    // --------------------------------------------------
  }
*/

/*
  unreachable_sentinel : end konumu isteyen bir algoritmaya 
    parametre olarak geçildiğinde fonksiyonun false döndürdüğünü
    derleyici görecek. 

  struct UnreachableSentinel_t {
    constexpr bool operator==(const auto&) const { return false; }
  };

  constexpr UnreachableSentinel_t UnreachableSentinel;
*/

/*
  #include <random>
  #include <vector>
  #include <numeric>    // std::ranges::iota
  #include <iterator>   // std::ostream_iterator
  #include <algorithm>  
  // ranges::generate, ranges::copy, ranges::shuffle, ranges::find

  int main()
  {
    using namespace std;

    // --------------------------------------------------

    vector<int> ivec(15);

    mt19937 eng{ random_device{}() };
    uniform_int_distribution dist{ 0, 100 };

    ranges::generate(ivec, [&]{ return dist(eng); });
    ranges::copy(ivec, ostream_iterator<int>{ cout, " " });
    // output -> 24 14 80 34 49 36 16 10 58 94 57 37 8 2 32

    std::putchar('\n');

    // --------------------------------------------------

    vector<int> ivec_2(15);

    std::ranges::iota(ivec_2, 0);
    ranges::copy(ivec_2, ostream_iterator<int>{ cout, " " });
    // output -> 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14

    std::putchar('\n');

    // --------------------------------------------------

    std::ranges::shuffle(ivec_2, eng);
    ranges::copy(ivec_2, ostream_iterator<int>{ cout, " " });
    // output -> 3 2 12 8 9 5 7 1 4 11 13 14 0 10 6

    std::putchar('\n');

    // --------------------------------------------------

    // we know that numbers in ivec_2 elements are [0, 15)
    int val = 7;

    auto iter = ranges::find(ivec_2.begin(), unreachable_sentinel, val);

    cout << format("val = {}, index = {}\n", val, iter - ivec_2.begin());
    // output -> val = 7, index = 6

    // --------------------------------------------------
  }
*/

/*
  // ranges algoritmaları, eski algoritmalara göre 
  // daha fazla enformasyon döndürüyorlar.

  #include <vector>
  #include <algorithm>  // ranges::for_each

  int main()
  {
    using namespace std;

    vector ivec{ 11, 22, 33, 44, 55, 66, 77, 88, 99, 110};

    auto result = ranges::for_each( ivec, 
                                    [](int x) { cout << x << ' '; });
    // output -> 11 22 33 44 55 66 77 88 99 110
    cout << '\n';

    cout << result.in - ivec.begin() << '\n'; // output -> 10
    // in -> input_iterator
    // out -> output_iterator
    // fun -> functor(callable)
  }
*/

/*
  #include <vector>
  #include <algorithm>  // std::copy, std::ranges::copy
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector vec1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    // --------------------------------------------------

    vector<int> vec2(20);
    auto iter = copy(vec1.begin(), vec1.end(), vec2.begin());

    // [vec2.begin(), iter) kopyalanan range.
    // eski yazma algoritmaları(örneğin copy) yazılan son 
    // indeksten bir sonraki indeksi döndürür.

    print(vec2.begin(), iter);
    // output -> 1 2 3 4 5 6 7 8 9 10

    print(vec2.begin(), vec2.end());
    // output -> 1 2 3 4 5 6 7 8 9 10 0 0 0 0 0 0 0 0 0 0

    // --------------------------------------------------

    vector<int> vec3(20);
    auto result = ranges::copy(vec1, vec3.begin());

    result.in;  
    // en son okunan konumun bir sonraki konumu
    result.out;   
    // yazılan konumun bir sonraki konumu
    // eski copy algoritması sadece out iterator döndürüyordu.

    auto vec1_endIter = vec1.end();
    cout << &*vec1_endIter << '\n';     // output -> 0x254bbf5f288
    cout << &*result.in << '\n';        // output -> 0x254bbf5f288

    auto vec3_writtenIter = vec3.begin() + vec1.size();
    cout << &*vec3_writtenIter << '\n';   // output -> 0x254bbf60808
    cout << &*result.out << '\n';         // output -> 0x254bbf60808

    print(vec1.begin(), result.in);
    // output -> 1 2 3 4 5 6 7 8 9 10

    print(vec3.begin(), result.out);
    // output -> 1 2 3 4 5 6 7 8 9 10

    print(vec3.begin(), vec3.end());
    // output -> 1 2 3 4 5 6 7 8 9 10 0 0 0 0 0 0 0 0 0 0

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <algorithm>  // std::ranges::copy
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector vec1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<int> vec2(20);

    auto [iter_source, iter_dest] = ranges::copy(vec1, vec2.begin());

    print(vec1.begin(), iter_source);
    // output -> 1 2 3 4 5 6 7 8 9 10

    print(vec2.begin(), vec2.end());
    // output -> 1 2 3 4 5 6 7 8 9 10 0 0 0 0 0 0 0 0 0 0

    print(vec2.begin(), iter_dest);
    // output -> 1 2 3 4 5 6 7 8 9 10
  }
*/

/*
            <---- check range_concepts.png ---->
         <---- check range_generic_functions.png ---->
        <---- check range_generic_functions_2.png ---->
        <---- check range_generic_functions_3.png ---->
        <---- check range_generic_functions_4.png ---->
        <---- check range_generic_functions_5.png ---->
      <---- check range_algorithms_return_types.png ---->
*/

/*
  #include <string_view>
  #include <vector>
  #include <algorithm>

  void print(std::string_view msg, auto beg, auto end)
  {
    std::cout << msg;

    for (auto pos = beg; pos != end; ++pos)
      std::cout << ' ' << *pos;

    std::cout << '\n';
  }

  int main()
  {
    using namespace std;

    vector inColl{ 1, 2, 3, 4, 5, 6, 7 };
    vector outColl{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    auto res = ranges::transform( inColl, 
                                  outColl.begin(),
                                  [](auto val){ return val * val; });


    print("processed in: ", inColl.begin(), res.in);
    // output -> processed in:  1 2 3 4 5 6 7

    print("rest of in: ", res.in, inColl.end());
    // output -> rest of in:

    print("written out: ", outColl.begin(), res.out);
    // output -> written out: 1 4 9 16 25 36 49

    print("rest of out: ", res.out, outColl.end());
    // output -> rest of out: 8 9 10
  }
*/

/*
              -----------------------------------
              | projection in ranges algorithms |
              -----------------------------------
*/

/*
  #include <vector>

  // --- std::find algorithm ---
  // in std::find algorithm, *iter's value is being used.
  template <typename InIter, typename T>
  InIter Find(InIter beg, InIter end, const T& val)
  {
    while (beg != end && *beg != val)
      ++beg;

    return beg;
  }

  // --- std::ranges::find algorithm ---
  // in std::ranges::find algorithm, *iter's value is sent 
  // to the proj function and the result of the proj function
  // is being used.
  template <typename InIter, typename T, typename Proj>
  InIter Range_Find(InIter beg, InIter end, const T& val, Proj proj)
  {
    while (beg != end && proj(*beg) != val)
      ++beg;

    return beg;
  }

  int main()
  {
    using namespace std;
    vector vec1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    auto iter = Range_Find(vec1.begin(), vec1.end(), 9, 
                          [](int x){ return x * x; });
    // *vec1.begin() is sent to lambda function and 1 * 1 = 1
    // 1 == 9 -> false
    // *(vec1.begin() + 1) is sent to lambda function and 2 * 2 = 4
    // 4 == 9 -> false
    // *(vec1.begin() + 2) is sent to lambda function and 3 * 3 = 9
    // 9 == 9 -> true
    // so, iter will be vec1.begin() + 2
    // *iter -> 3

    cout << *iter << '\n'; // output -> 3
  }
*/

/*
          <---- check not_related / std::invoke ---->
*/

/*
  #include <functional> // std::invoke

  template <typename InIter, typename T, typename Proj>
  InIter Range_Find(InIter beg, InIter end, const T& val, Proj proj)
  {
    while (beg != end){
      if (std::invoke(proj, *beg) == val)
        return beg;

      ++beg;
    }

    return end;
  }

  // we can pass a data member pointer or a callable 
  // to Range_Find's "proj" parameter
*/

/*
  // what if we don't want to use proj parameter
  // we want ranges::find function to operate like std::find algorithm.
  // To solve this problem, we can use std::identity

  // std::identity is a function object type, 
  // whose operator() returns its argument unchanged. 
  // https://en.cppreference.com/w/cpp/utility/functional/identity

  #include <functional> // std::invoke, std::identity(C++20)

  struct Identity {
    template <typename T>
    T operator()(T x) const { return x; }
  };

  template <typename InIter, typename T, typename Proj = std::identity>
  InIter Range_Find(InIter beg, 
                    InIter end, 
                    const T& val, 
                    Proj proj = {})
  {
    while (beg != end){
      if (std::invoke(proj, *beg) == val)
        return beg;

      ++beg;
    }

    return end;
  }
*/

/*
  #include <algorithm>  // std::ranges::sort, std::sort
  #include <vector>
  #include <cmath>      // std::abs
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector<int> ivec;

    rfill(ivec, 15, Irand{ -100, 100 });
    print(ivec);
    // output -> 84 -70 56 81 -84 37 -32 57 2 -38 98 -23 -84 -80 -74

    // --------------------------------------------------

    ranges::sort(ivec);
    // Comp = std::ranges::less
    // Proj = std::identity 

    print(ivec);
    // output -> -84 -84 -80 -74 -70 -38 -32 -23 2 37 56 57 81 84 98

    // --------------------------------------------------

    ranges::sort(ivec, {}, [](int x){ return abs(x); });
    // Comp = std::ranges::less 
    // Proj = [](int x){ return abs(x); }

    print(ivec);
    // output -> 2 -23 -32 37 -38 56 57 -70 -74 -80 81 -84 -84 84 98

    // --------------------------------------------------

    // acts like std::sort algorithm
    ranges::sort(ivec, [](int x, int y){ return abs(x) < abs(y); });
    // Comp = [](int x, int y){ return abs(x) > abs(y); }
    // Proj = std::identity

    print(ivec);
    // output -> 2 -23 -32 37 -38 56 57 -70 -74 -80 81 -84 -84 84 98

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <algorithm>  // ranges::generate, ranges::copy, ranges::sort
  #include <iterator>   // ostream_iterator
  #include "../nutility.h"

  struct Point {
    int m_x{};
    int m_y{};

    Point() = default;
    Point(int x, int y) : m_x{ x }, m_y{ y } {}

    // hidden friend operator<< function
    friend std::ostream& operator<<(std::ostream& os, const Point& p)
    {
      return os << "(" << p.m_x << ", " << p.m_y << ")";
    }
  };

  Point create_random_point()
  {
    Irand irand{ 10, 99 };
    return Point{ irand(), irand() };
  }

  int main()
  {
    using namespace std;
    vector<Point> pvec(5);

    // --------------------------------------------------

    ranges::generate(pvec, create_random_point);
    ranges::copy(pvec, ostream_iterator<Point>{ cout, " " });
    // output -> (25, 90) (30, 44) (62, 63) (79, 99) (79, 48)

    std::cout << '\n';

    // --------------------------------------------------
    
    ranges::sort(pvec, {}, &Point::m_x);
    // Comp = std::ranges::less
    // Proj = &Point::m_x
    ranges::copy(pvec, ostream_iterator<Point>{ cout, " " });
    // output -> (25, 90) (30, 44) (62, 63) (79, 99) (79, 48)
    
    std::cout << '\n';

    // --------------------------------------------------

    ranges::sort(pvec, {}, &Point::m_y);
    // Comp = std::ranges::less
    // Proj = &Point::m_y
    ranges::copy(pvec, ostream_iterator<Point>{ cout, " " });
    // output -> (30, 44) (79, 48) (62, 63) (25, 90) (79, 99)

    std::cout << '\n';

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <string>
  #include <algorithm>  // ranges::copy
  #include <iterator>   // ostream_iterator
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector<string> svec;

    rfill(svec, 5, []{ return rname() + ' ' + rfname(); });
    ranges::copy(svec, ostream_iterator<string>{ cout, "\n" });
    // output ->
    //  bilal yorgun
    //  azmi karaelmas
    //  engin karakoyun
    //  huseyin simsek
    //  pelin karayel

    // --------------------------------------------------
    
    size_t wanted_length = 15;
    if (auto iter = ranges::find(svec, wanted_length, 
        [](const string& s){ return s.size(); });
        iter != svec.end()){
      cout << "found : " << *iter << '\n';
    }
    else
      cout << "not found\n";
    // output -> found : engin karakoyun

    // --------------------------------------------------
  }
*/

/*
  #include <numeric>  // std::accumulate
  #include <vector>
  #include <string>
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    // --------------------------------------------------

    vector<string> svec;
    rfill(svec, 5, rname);
    print(svec);
    // output -> nazif cengiz aylin hande nevsin

    auto x = accumulate(svec.begin(), svec.end(), string{ "toplam = " });
    cout << x << '\n';
    // output -> toplam = nazifcengizaylinhandenevsin

    // --------------------------------------------------

    vector<int> ivec{ 4, 5, 2, 7, 9, 10, 3, 1, 6 };

    int start_val = 1;
    auto sum =  accumulate( ivec.begin(), 
                            ivec.end(),
                            start_val, 
                            [](int x, int y) { return x * y; });

    cout << sum << '\n';
    // output -> 453600

    // possible implementation
    // https://en.cppreference.com/w/cpp/algorithm/accumulate

    // template<class InputIt, class T, class BinaryOp>
    // constexpr  // since C++20
    // T accumulate(InputIt first, InputIt last, T init, BinaryOp op)
    // {
    //     for (; first != last; ++first)
    //         init = op(std::move(init), *first); // move since C++20
    // 
    //     return init;
    // }

    // --------------------------------------------------
  }
*/

/*
  #include <numeric>  // std::accumulate
  #include <vector>
  #include <string>
  #include <format>
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    // --------------------------------------------------

    vector<string> svec;
    rfill(svec, 5, rname);
    
    for (const auto& s : svec)
      cout << format("({:_<10}, {}) \n", s, s.size());
    // output -> 
    //  (tufan_____, 5)
    //  (gazi______, 4)
    //  (eylul_____, 5)
    //  (nasrullah_, 9)
    //  (semsit____, 6)


    const auto fn = [](size_t old_sum, const string& s2){ 
                        return old_sum + s2.length(); };
    
    auto total_size = accumulate( svec.begin(), 
                                  svec.end(), 
                                  0u, 
                                  fn);

    cout << total_size << '\n';  // output -> 29

    // --------------------------------------------------
  }
*/

/*
  // converting std::accumulate to a rangified algorithm

  #include <utility>      // std::move
  #include <functional>   // std::plus, std::invoke, std::identity
  #include <iterator>     
  // std::input_iterator, std::sentinel_for, std::iter_value
  #include <ranges>       
  // std::ranges::input_range, std::ranges::sentinel_for

  using namespace std;

  // -------------------------------------------------

  template <typename Iter, typename Init>
  Init Range_Accumulate_1(Iter beg, Iter end, Init init)
  {
    while (beg != end){
      init = move(init) + *beg;
      ++beg;
    }

    return init;
  }

  // -------------------------------------------------

  template <typename Iter, typename T_Sentinel, 
            typename Init, typename Op = std::plus<>>
  Init Range_Accumulate_2(
      Iter beg, T_Sentinel end, Init init, Op op = {})
  {
    while (beg != end){
      init = op(move(init), *beg);
      ++beg;
    }

    return init;
  }

  // -------------------------------------------------

  // added reqiures clause
  // Iter type should satisfy the input_iterator concept

  template <typename Iter, typename T_Sentinel, 
            typename Init, typename Op = plus<>>
  requires  input_iterator<Iter> && 
            sentinel_for<Iter, T_Sentinel>
  Init Range_Accumulate_3(
      Iter beg, T_Sentinel end, Init init, Op op = {})
  {
    while (beg != end){
      init = op(move(init), *beg);
      ++beg;
    }

    return init;
  }

  // -------------------------------------------------

  // instead of using requires clause, 
  // costrained template parameter is used.
  // same as the previous function template

  template <input_iterator Iter,
            sentinel_for<Iter> T_Sentinel,
            typename Init, 
            typename Op = plus<>>
  Init Range_Accumulate_4(
      Iter beg, T_Sentinel end, Init init, Op op = {})
  {
    while (beg != end){
      init = op(move(init), *beg);
      ++beg;
    }

    return init;
  }

  // -------------------------------------------------

  // used std::invoke to call the function object

  template <input_iterator Iter,
            sentinel_for<Iter> T_Sentinel,
            typename Init, 
            typename Op = plus<>>
  Init Range_Accumulate_5(
      Iter beg, T_Sentinel end, Init init, Op op = {})
  {
    while (beg != end){
      init = invoke(op, move(init), *beg);
      ++beg;
    }

    return init;
  }

  // -------------------------------------------------

  // added default argument to 3rd template parameter

  template <input_iterator Iter,
            sentinel_for<Iter> T_Sentinel,
            typename Init = iter_value_t<Iter>,
            typename Op = plus<>>
  Init Range_Accumulate_6(
      Iter beg, T_Sentinel end, Init init = {}, Op op = {})
  {
    while (beg != end){
      init = invoke(op, move(init), *beg);
      ++beg;
    }

    return init;
  }

  // -------------------------------------------------

  // Iterator parameter overload

  template <input_iterator Iter,
            sentinel_for<Iter> T_Sentinel,
            typename Init = iter_value_t<Iter>,
            typename Op = plus<>,
            typename Proj = identity>
  Init Range_Accumulate_7(
    Iter beg, T_Sentinel end, Init init = {}, Op op = {}, Proj proj = {})
  {
    while (beg != end){
      init = invoke(op, move(init), invoke(proj, *beg));
      ++beg;
    }

    return init;
  }

  // Range parameter overload

  template <ranges::input_range R,
            typename Init = ranges::range_value_t<R>,
            typename Op = plus<>,
            typename Proj = identity>
  Init Range_Accumulate_7(
    R&& r, Init init = {}, Op op = {}, Proj proj = {})
  {
    return Range_Accumulate_7(ranges::begin(r), 
                              ranges::end(r), 
                              move(init), 
                              move(op), 
                              move(proj));
  }

  // -------------------------------------------------
*/

/*
                      ----------------------
                      | std::ranges::views |
                      ----------------------
*/

/*
  - view bir range.
  - range yerine view oluşturulduğunda, 
    -> constant zaman taşıma ve kopyalama garantisi.
  - view'lar function objectler.
*/

/*
  #include <ranges>
  #include <vector>
  #include "../nutility.h"

  // namespace std::views = std::ranges::views (namespace alias)

  int main()
  {
    using namespace std;

    vector<int> ivec;
    rfill(ivec, 10, Irand{ 0, 999 });
    print(ivec);
    // output -> 23 354 433 851 345 220 424 430 852 623

    // --------------------------------------------------

    for (auto elem : views::take(ivec, 5))
      cout << elem << ' ';
    // output -> 23 354 433 851 345

    cout << '\n';

    // --------------------------------------------------

    for (auto x : views::filter(ivec, [](int x){ return x % 2 == 0; }))
      cout << x << ' ';
    // output -> 354 220 424 430 852

    cout << '\n';

    // --------------------------------------------------
  }
*/

/*
  #include <ranges>
  #include <vector>
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector<int> ivec;
    rfill(ivec, 10, Irand{ 0, 999 });
    print(ivec);
    // output -> 500 290 554 652 451 247 204 520 114 836
    
    // --------------------------------------------------

    auto view_obj = views::take(ivec, 5); 
    print(view_obj);
    // output -> 500 290 554 652 451

    auto view_obj2 = ranges::take_view(ivec, 5);
    print(view_obj2);
    // output -> 500 290 554 652 451

    // --------------------------------------------------
  }
*/

/*
  #include <ranges>
  #include <algorithm>  // ranges::for_each
  #include <vector>
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector<int> ivec;
    rfill(ivec, 10, Irand{ 0, 999 });
    print(ivec);
    // output -> 921 150 35 401 817 547 79 517 234 911

    // --------------------------------------------------

    ranges::for_each( views::take(ivec, 10), 
                      [](int x){ cout << x << ' '; });
    // output -> 921 150 35 401 817 547 79 517 234 911

    cout << '\n';

    // --------------------------------------------------

    auto view_obj = views::take(ivec, 10);
    ranges::for_each(view_obj, [](int x){ cout << x << ' '; });
    // output -> 921 150 35 401 817 547 79 517 234 911

    cout << '\n';

    // --------------------------------------------------

    ranges::for_each( ranges::take_view(ivec, 10), 
                      [](int x){ cout << x << ' '; });
    // output -> 921 150 35 401 817 547 79 517 234 911

    // --------------------------------------------------
  }
*/

/*
  #include <algorithm>  // ranges::for_each
  #include <vector>
  #include <ranges>
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector<int> ivec;
    rfill(ivec, 10, Irand{ 0, 999 });
    print(ivec);
    // output -> 676 283 483 407 563 727 469 492 527 967

    // (ivec | views::take(5)) == views::take(ivec, 5) -> true
    print(ivec | views::take(5));
    // output -> 676 283 483 407 563
  }
*/

/*
  - her view bir range olmak zorunda.
  - her range bir view olamaz.
  - bir range'in view'ı olabilmesi için 
    ranges::view isimli concept'i sağlaması(desteklemesi) gerekiyor.
*/

/*
  #include <vector>
  #include <ranges>     
  // ranges::views, ranges::views::reverse, ranges::for_each
  // ranges::views::take
  #include <algorithm>  // ranges::for_each
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector<int> ivec;
    rfill(ivec, 10, Irand{ 0, 999 });
    print(ivec);
    // output -> 540 953 86 932 455 328 322 257 641 471

    // --------------------------------------------------

    // "views::reverse(ivec)" isimlendirilebilir,
    // L value haline getirilebilir.

    auto view_obj = views::reverse(ivec);

    // --------------------------------------------------

    // "views::reverse(ivec)" bir algoritmaya gönderilebilir.

    ranges::for_each( views::reverse(ivec), 
                      [](int x){ cout << x << ' '; });
    // output -> 471 641 257 322 328 455 932 86 953 540

    cout << '\n';

    // --------------------------------------------------

    // eğer viewable_range conceptini sağlıyorsa 
    // ranged base for loop içinde kullanılabilir.

    for (auto elem : views::reverse(ivec))
      cout << elem << ' ';
    // output -> 471 641 257 322 328 455 932 86 953 540

    cout << '\n';

    // --------------------------------------------------

    // "views::take(ivec, 5)" ifadesinden elde edilen range
    // başka bir range adaptörüne argüman olarak geçilebilir.

    for (auto elem : views::reverse(views::take(ivec, 5)))
      cout << elem << ' ';
    // output -> 455 932 86 953 540

    // ivec vectorünün ilk 5 elemanı range olarak alındı.
    // alınan range ters çevrildi ve tekrar bir range olarak alındı.

    cout << '\n';

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <ranges> // views::reverse, views::take, views::filter
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector<int> ivec;
    rfill(ivec, 20, Irand{ 0, 999 });
    print(ivec);
    // output -> 
    //  573 825 584 170 733 223 733 275 584 635  
    //  874 851 127 920 952 304 635 131 681 702

    // --------------------------------------------------

    print(views::reverse(views::take(ivec, 10)));
    // 635 584 275 733 223 733 170 584 825 573

    // --------------------------------------------------

    for (auto val : 
            views::filter(views::reverse(views::take(ivec, 10)), 
                          [](int x){ return x % 2 == 0; }))
      cout << val << ' ';
    // output -> 584 170 584 

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <ranges> // views::reverse, views::take, views::filter
  #include "../nutility.h"

  int main()
  {
    using namespace std;

    vector<int> ivec;
    rfill(ivec, 20, Irand{ 0, 999 });
    print(ivec);
    // output -> 
    //  120 88 725 752 208 176 730 371 869 561  
    //  697 878 269 618 433 984 96 370 509 239

    // --------------------------------------------------

    print(ivec | views::take(10) | views::reverse );
    // output -> 561 869 371 730 176 208 752 725 88 120

    // --------------------------------------------------

    for (auto val : 
      ivec | views::take(10) | views::reverse | 
        views::filter([](int x){ return x % 2 == 0; }))
      cout << val << ' ';
    // output -> 730 176 208 752 88 120

    // --------------------------------------------------
  }
*/

/*
  #include <ranges>
  #include <vector>

  int main()
  {
    std::vector ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    auto view_obj = std::views::take(ivec, 5);

    std::cout << "sizeof(view_obj) = " << sizeof(view_obj) << '\n';
    // output -> sizeof(view_obj) = 16
    // holds 2 pointers

    static_assert(std::ranges::view<decltype(view_obj)>);
    // return type of std::views::take is satisfying
    // std::ranges::view concept
  }
*/

/*
  - range adaptörleri, öyle bir foksiyon nesnesi(functin object) ki,
    input olarak bir ya da birden fazla range alıp, belirli 
    niteliklere sahip bir range döndürüyor. Bu niteliklerin 
    en önemlisi döndürülen range'in range::view konseptini 
    sağlaması(desteklemesi) dolayısıyla bir view döndürüyor da 
    denilebilir.
*/

/*
                      -------------------
                      | lazy evaluation |
                      -------------------
*/

/*
  #include <ranges>
  #include <vector>

  int main()
  {
    std::vector ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    // --------------------------------------------------
    auto view_obj = std::views::filter(ivec, [](int x){
      std::cout << "- (1) lambda function called\n";
      return x % 3 == 0;
    });

    std::cout << "[0] main continues\n";
    auto iter = view_obj.begin();
    std::cout << "[1] main continues\n";

    // output ->
    //  [0] main continues
    //  - (1) lambda function called
    //  - (1) lambda function called
    //  - (1) lambda function called
    //  [1] main continues

    // to find view_obj.begin() iterator
    // needs to find the first element that satisfies the condition
    // (x % 3 == 0) in the ivec vector.
    // so lambda function is called for { 1, 2, 3 } elements.
    // lazy evaluation (not evaluated until needed)
    

    // --------------------------------------------------

    std::cout << "[2] main continues\n";

    auto view_obj2 = std::views::filter(ivec, [](int x){
      std::cout << "- (2) lambda function called\n";
      return x % 4 == 0;
    });

    auto iter2 = view_obj2.begin();

    std::cout << "[3] main continues\n";

    // output ->
    //  [2] main continues
    //  - (2) lambda function called
    //  - (2) lambda function called
    //  - (2) lambda function called
    //  - (2) lambda function called
    //  [3] main continues

    // to find view_obj.begin() iterator
    // needs to find the first element that satisfies the condition
    // (x % 4 == 0) in the ivec vector.
    // so lambda function is called for { 1, 2, 3, 4 } elements.
    // lazy evaluation (not evaluated until needed)
  }
*/

/*
  #include <ranges>
  #include <algorithm>  // std::copy_if, std::transform
  #include <vector>
  #include <iterator>   // std::back_inserter

  int main()
  {
    using namespace std;

    vector ivec{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<int> dest_vec;

    // we want to write square of the elements that are even
    // from ivec to the dest_vec.

    // --------------------------------------------------

    // ------- WAY 1 (using old algorithms) -------
    vector<int> temp;

    copy_if(ivec.begin(), ivec.end(), back_inserter(temp), 
            [](int x){ return x % 2 == 0; });

    transform(temp.begin(), temp.end(), back_inserter(dest_vec),
              [](int x){ return x * x; });

    for (auto elem : dest_vec)
      cout << elem << ' ';
    // output -> 4 16 36 64 100

    cout << '\n';

    // --------------------------------------------------

    // ------- WAY 2 (using ranges algorithms) -------

    vector<int> dest_vec2;

    auto view_obj = 
          ivec  | views::filter([](int x){ return x % 2 == 0; }) 
                | views::transform([](int x){ return x * x; });

    ranges::copy(view_obj, back_inserter(dest_vec2));

    for (auto elem : dest_vec2)
      cout << elem << ' ';
    // output -> 4 16 36 64 100

    // --------------------------------------------------
  }
*/

/*
  #include <ranges>
  #include "../nutility.h"  // isprime

  int main()
  {
    // std::views::iota() -> range factory 
    // did not get a range as an argument raturns a range

    // --------------------------------------------------

    auto view_range_obj = 
        std::views::iota(10) | std::views::take(5);

    for (auto elem : view_range_obj)
      std::cout << elem << ' ';
    // output -> 10 11 12 13 14

    std::cout << '\n';

    // --------------------------------------------------

    for (auto elem : std::views::iota(10, 20))
      std::cout << elem << ' ';
    // output -> 10 11 12 13 14 15 16 17 18 19

    std::cout << '\n';

    // --------------------------------------------------
    
    auto view_range_obj2 = 
                std::views::iota(10, 100)   | 
                std::views::reverse         |
                std::views::filter(isprime);
      
    for (auto elem : view_range_obj2)
      std::cout << elem << ' ';
    // output -> 
    // 97 89 83 79 73 71 67 61 59 53 47 43 41 37 31 29 23 19 17 13 11

    // --------------------------------------------------
  }
*/

/*
  #include <ranges>
  #include "../nutility.h"  // isprime

  int main()
  {
    auto ivec = 
          std::views::iota(10, 100)   | 
          std::views::reverse         |
          std::views::filter(isprime) |
          std::ranges::to<std::vector>(); // C++23 (std::ranges::to)

    std::cout << "ivec.size() = " << ivec.size() << '\n';
    // output -> ivec.size() = 21

    for (auto elem : ivec)
      std::cout << elem << ' ';
    // output ->
    // 97 89 83 79 73 71 67 61 59 53 47 43 41 37 31 29 23 19 17 13 11
  }
*/

/*
  #include <ranges>
  // ranges::input_range, range::range_value_t, 
  // ranges::begin, ranges::end
  #include <vector>

  template <std::ranges::input_range Range>
  std::ranges::range_value_t<Range> get_min(Range&& rng)
  {
    if (std::ranges::empty(rng)){
      return std::ranges::range_value_t<Range>{};
    }

    auto pos = std::ranges::begin(rng);
    auto min_val = *pos;

    while (++pos != std::ranges::end(rng)){
      if (*pos < min_val)
        min_val = *pos;
    }
    return min_val;
  }

  int main()
  {
    std::vector ivec{ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    auto min_val = get_min(ivec);

    std::cout << "min_val = " << min_val << '\n';
    // output -> min_val = 1
  } 
*/

/*
  #include <ranges>
  // ranges::views::repeat, ranges::views::take
  #include <vector>

  int main()
  {
    std::views::repeat(5) | std::views::take(10);
    // { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }

    std::views::empty<int>;
    // {}
  }
*/

/*
                            ----------
                            | common |
                            ----------
*/

/*
  #include <ranges>

  template <typename Iter, typename Sentinel>
  void call_argo(Iter beg, Sentinel end)
  {
    auto vw = std::views::common(std::ranges::subrange(beg, end));
    // ranges::subrange returns a range,
    // ranges::common converts the range to a common range

    // vw is a common range and can be passed to functions
    // that require a common range as an argument.
  }
*/

/*
  ranges::common is checking 
    - begin and end iterator types are same or different
    - the range is a view or not

  common range is using common_iterator type
*/

/*
  #include <list>
  #include <ranges>

  int main()
  {
    std::list<int> ilist(10);
    auto take_obj = std::views::take(ilist, 5);
    auto iota_obj = std::views::iota(35, 45);

    // --------------------------------------------------

    auto x = std::views::common(ilist);
    // x's type is a specialization of std::ranges::ref_view

    // ilist's begin and end iterator types are same 
    // but ilist is not a view 
    // so x's type is a specialization of std::ranges::ref_view

    // --------------------------------------------------

    auto y = std::views::common(take_obj);
    // y's type is a specialization of std::ranges::common_view

    // take_obj's begin and end iterator types are different 
    // but take_obj is a view
    // so y's type is a specialization of std::ranges::common_view

    // --------------------------------------------------

    auto z = std::views::common(iota_obj);
    // z's type is a specialization of std::ranges::iota_view

    // iota_obj's begin and end iterator types are same
    // and iota_obj is a view
    // so z's type is a specialization of std::ranges::iota_view

    // --------------------------------------------------
  }
*/

/*
  #include <ranges>     // ranges::subrange, ranges::views::common
  #include <vector>
  #include <algorithm>  // std::count

  template <auto end_val>
  struct Sentinel {
    bool operator==(auto pos) const { return *pos == end_val; }
  };

  int main()
  {
    using namespace std;

    vector<int> ivec(20);
    ivec[5] = -1;
    ivec[1] = 3;

    ranges::subrange sr(ivec.begin(), Sentinel<-1>{});
    // { 0, 3, 0, 0, 0 }

    auto common_vw = views::common(sr);
    // sr's begin and end iterator types are different
    // sr is a view
    // common_vw's type std::ranges::common_view<>

    auto N = count(common_vw.begin(), common_vw.end(), 0);

    cout << "N = " << N << '\n';
    // N = 4
    // There are 4 zeros in the subrange
  }
*/

/*
  #include <vector>
  #include <ranges> 
  #include <numeric>  // std::accumulate

  int main()
  {
    std::vector ivec{ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };

    auto view_obj = 
        ivec | std::views::filter([](int x){ return x % 10 == 7; });
    // { 7, 17, 37 }

    auto sum = std::accumulate(view_obj.begin(), view_obj.end(), 0);
    std::cout << sum << '\n'; // output -> 61  
  }
*/

/*
  #include <vector>
  #include <ranges>
  #include <numeric>  // std::accumulate

  int main()
  {
    std::vector ivec{ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };

    auto rng = 
        ivec | std::views::take_while([](int x){ return x < 20; });
      
    for (auto val : rng)
      std::cout << val << ' ';
    // output -> 2 3 5 7 11 13 17 19

    std::cout << '\n';

    // --------------------------------------------------

    std::accumulate(rng.begin(), rng.end(), 0); // syntax error
    // because of "rng" is not a common range, 
    // "rng.begin()" and "rng.end()" expressions types are different

    // --------------------------------------------------

    static_assert(std::ranges::common_range<decltype(rng)>);
    // error: static assertion failed 

    // rng's type is not satisfying ranges::common_range concept

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <list>
  #include <ranges> 
  // std::ranges::common_range, std::views::take, 
  // std::views::drop_while

  int main()
  {
    // --------------------------------------------------

    static_assert(std::ranges::common_range<std::vector<int>>);
    // ranges::common_range concept is satisfied

    static_assert(std::ranges::common_range<std::list<int>>); 
    // ranges::common_range concept is satisfied

    // --------------------------------------------------

    std::vector ivec{ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };

    auto rng = ivec | std::views::take(5);

    static_assert(std::ranges::common_range<decltype(rng)>);
    // ranges::common_range concept is satisfied

    // --------------------------------------------------

    auto f = [](int x){ return x < 10; };
    auto rng2 = ivec | std::views::drop_while(f);

    for (auto val : rng2)
      std::cout << val << ' ';
    // output -> 11 13 17 19 23 29 31 37

    static_assert(std::ranges::common_range<decltype(rng2)>);
    // ranges::common_range concept is satisfied

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <numeric>  // std::accumulate
  #include <ranges> 
  // views::take_while, views::common, ranges::common_range

  int main()
  {
    std::vector ivec{ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };

    auto rng = 
        ivec | std::views::take_while([](int x){ return x < 20; });
      
    for (auto val : rng)
      std::cout << val << ' ';
    // output -> 2 3 5 7 11 13 17 19

    std::cout << '\n';
      
    // --------------------------------------------------

    static_assert(std::ranges::common_range<decltype(rng)>);
    // error: static assertion failed 

    auto common_rng = std::views::common(rng);
    static_assert(std::ranges::common_range<decltype(common_rng)>);
    // ranges::common_range concept is satisfied

    auto sum = std::accumulate( common_rng.begin(), 
                                common_rng.end(), 
                                0);

    std::cout << "sum = " << sum << '\n'; 
    // output -> sum = 77

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <numeric>  // std::accumulate
  #include <ranges> 
  // views::take_while, views::common, ranges::common_range

  int main()
  {
    std::vector ivec{ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };

    auto rng = 
        ivec  | std::views::take_while([](int x){ return x < 10; })
              | std::views::common;
      
    for (auto val : rng)
      std::cout << val << ' ';
    // output -> 2 3 5 7
    std::cout << '\n';
      
    static_assert(std::ranges::common_range<decltype(rng)>);
    // ranges::common_range concept is satisfied

    auto sum = std::accumulate(rng.begin(), rng.end(), 0);

    std::cout << "sum = " << sum << '\n'; 
    // output -> sum = 17
  }
*/

/*
                          ------------
                          | subrange |
                          ------------
*/

/*
  - iki iterator argüman olarak subrange adaptörüne gönderip
    range konseptini sağlayan bir range nesnesi elde edilir.
*/

/*
  #include <vector>
  #include <ranges>  // ranges::subrange
  #include <format>

  int main()
  {
    using namespace std;

    vector ivec{ 1, 2, 3, 4, 5 };

    // --------------------------------------------------

    ranges::subrange sub_range1{ ivec };

    cout << "sizeof(sub_range1) = " << sizeof(sub_range1) << '\n';
    // output -> sizeof(sub_range1) = 16

    static_assert(ranges::view<decltype(sub_range1)>);
    // ranges::subrange type is satisfying ranges::view concept
    // ranges::subrange is a view

    for (const auto elem : sub_range1)
      cout << elem << ' ';
    // output -> 1 2 3 4 5
    cout << '\n';

    // --------------------------------------------------

    auto& r1 = sub_range1.front();
    auto& r2 = sub_range1.back();

    cout << format("r1 = {}, r2 = {}\n", r1, r2);
    // output -> r1 = 1, r2 = 5

    // --------------------------------------------------

    ranges::subrange sub_range2{ ivec.begin(), ivec.end() };

    for (const auto elem : sub_range2)
      cout << elem << ' ';
    // output -> 1 2 3 4 5

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <ranges>  // ranges::subrange
  #include <format>

  int main()
  {
    using namespace std;

    vector ivec{ 1, 2, 3, 4, 5 };

    // --------------------------------------------------

    ranges::subrange sub_range1{ ivec };

    auto& r1 = sub_range1.front();
    auto& r2 = sub_range1.back();

    r1 += 300;
    r2 += 300;

    for (const auto elem : ivec)
      cout << elem << ' ';
    // output -> 301 2 3 4 305

    // front() and back() functions return a reference to the
    // first and last elements of the vector

    // -------------------------------------------------- 
  }
*/

/*
  #include <vector>
  #include <ranges>

  template <auto end_val>
  class Sentinel {
  public:
    bool operator==(auto x) const { return *x == end_val; }
  };

  int main()
  {
    using namespace std;

    vector<int> ivec{ 1, 3, 5, 7, 2, 9 };
    ranges::subrange sub_range(ivec.begin(), Sentinel<7>{});

    for (const auto val : sub_range)
      cout << val << ' ';
    // output -> 1 3 5
  }
*/

/*
  #include <vector>
  #include <ranges> 
  // ranges::iota_view, ranges::range, ranges::subrange, ranges::to

  template <auto end_val>
  class Sentinel {
  public:
    bool operator==(auto x) const { return *x == end_val; }
  };

  void print(std::ranges::range auto&& r)
  {
    for (const auto& val : r)
      std::cout << val << ' ';
    std::cout << '\n';
  }

  int main()
  {
    //  --------------------------------------------------

    auto vw = std::ranges::iota_view{ 0, 10 };
    std::vector vec(vw.begin(), vw.begin() + 10);

    auto vec2 = 
      std::ranges::iota_view{ 0, 10 } | 
      std::ranges::to<std::vector>();

    // vec1 and vec2 are equivalent.

    // --------------------------------------------------

    std::ranges::subrange s1{ vec.begin(), vec.begin() + 7 };
    std::ranges::subrange s2{ vec.begin(), Sentinel<5>{} };

    print(s1);  // output -> 0 1 2 3 4 5 6
    print(s2);  // output -> 0 1 2 3 4

    // --------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <forward_list>
  #include <ranges>   // std::ranges::sized_range

  int main()
  {
    using namespace std;

    static_assert(std::ranges::sized_range<std::vector<int>>);
    // std::vector<int> type is satisfying 
    // std::ranges::sized_range concept

    static_assert(std::ranges::sized_range<std::forward_list<int>>);
    // error : static assertion failed
    // std::forward_list<int> type is NOT satisfying
    // std::ranges::sized_range concept
  }
*/

/*
  #include <vector>
  #include <ranges>   // ranges::subrange
  #include <list>
  #include <iterator> // std::next, std::prev

  int main()
  {
    // --------------------------------------------------

    std::vector ivec{ 2, 3, 5, 7, 11, 13 };

    std::ranges::subrange subrange_vec{ std::next(ivec.begin()),
                                        std::prev(ivec.end()) };  

    constexpr bool b1 = 
      std::ranges::sized_range<decltype(subrange_vec)>;
    // b1 -> true

    // --------------------------------------------------

    std::list ilist{ 2, 3, 5, 7, 11, 13 };

    std::ranges::subrange subrange_list1{ ilist.begin(),
                                          ilist.end(),
                                          ilist.size() };

    std::ranges::subrange subrange_list2{ std::next(ilist.begin()),
                                          std::prev(ilist.end())};


    constexpr bool b2 = 
      std::ranges::sized_range<decltype(subrange_list1)>;
    // b2 -> true
    
    constexpr bool b3 =
      std::ranges::sized_range<decltype(subrange_list2)>;
    // b3 -> true

    // --------------------------------------------------
  }
*/

/*
  #include <ranges>   // std::ranges::subrange
  #include <concepts> // std::same_as

  // the type of subranges depends only on the type of the iteratiors
  // wheter or not size() is provided

  int main()
  {
    int arr1[10]{};
    int arr2[20]{};

    using subrange_type1 = decltype(std::ranges::subrange{ arr1 });
    using subrange_type2 = decltype(std::ranges::subrange{ arr2 });

    static_assert(std::same_as<subrange_type1, subrange_type2>);
  }
*/

/*
  #include <ranges>  
  // ranges::subrange, ranges::random_access_range

  template <std::ranges::random_access_range Range>
  auto left_half(Range rng)
  {
    return std::ranges::subrange(
              std::begin(rng), 
              std::begin(rng) + std::ranges::size(rng) / 2));
  }

  template<std::ranges::random_access_range Range>
  auto right_half(Range rng)
  {
    return std::ranges::subrange(
              std::begin(rng) + std::ranges::size(rng) / 2,
              std::end(rng));
  }
*/

