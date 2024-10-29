#include <iostream>

/*
                        ================
                        | `bit` module |
                        ================
*/

/*
  #include <bit>      // str::rotl, std::rotr
  #include <cstdint>  // uint16_t
  #include <format>

  int main()
  {
    // --------------------------------------------------

    constexpr auto elem1 = std::rotl(24U, 1);
    // elem1 -> 48U

    constexpr auto elem2 = std::rotr(24U, 1);
    // elem2 -> 12U

    // --------------------------------------------------

    uint16_t x = 0b0000'0000'0011'1011;

    std::cout << std::format("{0:016b} {0:8}\n", x);
    // output -> 0000000000111011       59

    std::cout << std::format("{0:016b} {0:8}\n", std::rotl(x, 2));
    // output -> 0000000011101100      236

    std::cout << std::format("{0:016b} {0:8}\n", std::rotl(x, -2));
    // output -> 1100000000001110     49166

    std::cout << std::format("{0:016b} {0:8}\n", std::rotr(x, 3));
    // output -> 0110000000000111    24583

    // --------------------------------------------------
  }
*/

/*
  #include <bit>      // str::bit_floor, std::bit_ceil
  #include <cstdint>  // uint16_t
  #include <format>

  int main()
  {
    using namespace std;
    
    // --------------------------------------------------

    constexpr auto elem1 = std::bit_floor(24U);
    // elem1 -> 16U

    // If elem1 is not zero, calculates the largest 
    // integral power of two that is not greater than elem1

    constexpr auto elem2 = std::bit_ceil(24U);
    // elem2 -> 32U

    // Calculates the smallest integral power of two 
    // that is not smaller than elem2. 

    // -------------------------------------------------- 

    uint16_t x = 0b0000'0000'0011'1011;

    cout << format("{0:016b} {0:8}\n", x);
    // output -> 0000000000111011       59

    cout << format("{0:016b} {0:8}\n", bit_floor(x));
    // output -> 0000000000100000       32

    cout << format("{0:016b} {0:8}\n", bit_ceil(x));
    // output -> 0000000001000000       64

    // --------------------------------------------------
  }
*/

/*
  #include <bit>     // std::has_single_bit
  #include <bitset>

  int main()
  {
    using namespace std;
    cout << boolalpha;    

    // How to check if a number is a power of 2
    // (if only 1 bit is set in the binary representation)

    // "x && !(x & (x - 1))" expression is used to check
    // if a number is a power of 2 or not

    // --------------------------------------------------

    uint32_t val = 567;

    cout << std::bitset<32>(val) << '\n';
    // output -> 00000000000000000000001000110111

    cout << std::bitset<32>(val & (val - 1)) << '\n';
    // output -> 00000000000000000000001000110110

    // --------------------------------------------------

    uint32_t val2 = 1024;

    cout << std::bitset<32>(val2) << '\n';
    // output -> 00000000000000000000010000000000

    cout << std::bitset<32>(val2 & (val2 - 1)) << '\n';
    // output -> 00000000000000000000000000000000

    // --------------------------------------------------

    uint32_t val3 = 0;

    cout << std::bitset<32>(val3) << '\n';
    // output -> 00000000000000000000000000000000

    cout << std::bitset<32>(val3 & (val3 - 1)) << '\n';
    // output -> 00000000000000000000000000000000

    // --------------------------------------------------

    cout << has_single_bit(val) << '\n';  // output -> false
    cout << has_single_bit(val2) << '\n'; // output -> true
    cout << has_single_bit(val3) << '\n'; // output -> false

    // --------------------------------------------------
  }
*/

/*
  #include <bit>    
  // std::countl_zero, std::countl_one
  // std::countr_zero, std::countr_one

  int main()
  {
    using namespace std;

    // --------------------------------------------------

    uint16_t val = 0b0000'0000'0011'1011;
    cout << countl_zero(val) << '\n';   // output -> 10

    uint16_t val2 = 0b1111'1100'0011'0000;
    cout << countl_one(val2) << '\n';   // output -> 6

    // --------------------------------------------------

    uint16_t val3 = 0b0000'0000'1110'0000;
    cout << countr_zero(val3) << '\n';  // output -> 5

    uint16_t val4 = 0b0000'0000'1111'1111;
    cout << countr_one(val4) << '\n';   // output -> 8

    // --------------------------------------------------
  }
*/

/*
  #include <bit>  // std::popcount

  int main()
  {
    using namespace std;

    uint16_t val = 0b1111'0000'0011'1011;

    cout << "set bit count = " << popcount(val) << '\n';
    // output -> set bit count = 9
  }
*/

/*
  #include <bit>  // std::bit_width

  // maximum kaç bitlik bir sayı ile temsil edilebilir.

  int main()
  {
    using namespace std;

    uint16_t val = 0b1111'0000'0011'1011;
    cout << "bit width = " << bit_width(val) << '\n';
    // output -> bit width = 16

    uint16_t val2 = 0b0001'0000'0011'1011;
    cout << "bit width = " << bit_width(val2) << '\n';
    // output -> bit width = 13

    uint16_t val3 = 0b0000'0000'0000'1011;
    cout << "bit width = " << bit_width(val3) << '\n';
    // output -> bit width = 4
  }
*/

/*
  #include <bit>  // std::rotl
  #include <bitset>

  int main()
  {
    // --------------------------------------------------

    int ival{ 345 };
    auto x = std::rotl(ival, 5);  // syntax error
    // note: the expression 'std::__is_unsigned_integer<_Tp>::value 
    // [with _Tp = int]' evaluated to 'false'

    // --------------------------------------------------

    std::bitset<16> bx{};
    auto y = std::rotl(bx, 5);    // syntax error
    // note: the expression 'std::__is_unsigned_integer<_Tp>::value 
    // [with _Tp = std::bitset<16>]' evaluated to 'false'
    
    // --------------------------------------------------
  }
*/

/*
  // bazı durumlarda bir değişkenin türünü değiştirip 
  // o değişkenin bitlerini 
  // başka bir türden değişkenin bitleri gibi kullanmak istiyoruz.

  // Nasıl yapılmaz.

  int main()
  {
    float f1 = 2.7263784f;

    // --------------------------------------------------

    auto elem1 = static_cast<int>(f1);  // VALID
    // accepting data loss

    std::cout << elem1 << '\n';  // output -> 2

    // --------------------------------------------------

    auto elem2 = *reinterpret_cast<int*>(&f1);  // UB
    auto elem3 = *(int*)&f1;                    // UB

    // --------------------------------------------------

    union Fint {
      float f;
      int i;
    };

    Fint fn = { f1 };
    auto elem4 = fn.i;  // UB in C++, VALID in C

    // --------------------------------------------------
  }
*/

/*
  // Nasıl Yapılır.

  #include <bit>      // std::bit_cast
  #include <cstring>  // std::memcpy
  #include <cstdint>  // std::uint16_t, std::uint64_t

  struct AStruct {
    std::uint16_t u1;
    std::uint16_t u2;
  }

  int main()
  {
    float f1 = 2.7263784f;

    // --------------------------------------------------
    // before C++20 

    int ival;
    std::memcpy(&ival, &f1, sizeof(f1));

    // --------------------------------------------------
    // since C++20

    int elem6 = std::bit_cast<int, float>(f1);
    unsigned int elem7 = std::bit_cast<unsigned int, float>(f1);

    constexpr auto elem8 = std::bit_cast<unsigned int, float>(18.2f);
    // elem8 -> 1100061082

    auto elem9 = std::bit_cast<long long, float>(f1); // syntax error
    // target type and source type must have the same size
    // note: the expression 'sizeof (_To) == sizeof (_From) 
    // [with _To = long long int; _From = float]' 
    // evaluated to 'false'

    // --------------------------------------------------

    AStruct a1 = { 243U, 9283U };
    auto uval = std::bit_cast<unsigned, AStruct>(a1);

    auto uval2 = std::bit_cast<uint16_t, AStruct>(f1); // syntax error
    auto uval2 = std::bit_cast<uint64_t, AStruct>(a1); // syntax error
    
    // --------------------------------------------------
  }
*/

/*
  #include <bit>  // std::endian enum class

  int main()
  {
    int x = 1;

    // --------------------------------------------------
    // run-time check 

    if (*(char*)&x == 1)
      std::cout << "little endian\n";
    else
      std::cout << "big endian\n";

    // --------------------------------------------------
    // compile-time check

    static_assert(std::endian::native == std::endian::little);

    static_assert(std::endian::native == std::endian::big);
    // static_assert failed

    // --------------------------------------------------

    if constexpr (std::endian::native == std::endian::little)
      ; // execution environment is little endian
    else if constexpr (std::endian::native == std::endian::big)
      ; // execution environment is big endian
    else 
      ; //execution environment is mixed

    // --------------------------------------------------
  }
*/