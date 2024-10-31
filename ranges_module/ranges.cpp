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