#include <iostream>

/*
                        ================
                        | Strong Types |
                        ================
*/

/*
  class Kilogram {
  private:
    double m_value;

  public:
    class PreventUsage{}; // nested type
    // nested type can be used for 
    // preventing implicit conversions

    explicit constexpr Kilogram(PreventUsage, double val) 
      : m_value(val) {}
    // `explicit` for preventing copy initialization
    // `constexpr` for compile-time evaluation

    double get() const { return m_value; }

    // hidden friend operator+ global function
    friend constexpr Kilogram operator+(const Kilogram& lhs, 
                                        const Kilogram& rhs) 
    {
      return Kilogram{ PreventUsage{}, lhs.m_value + rhs.m_value };
    }
  };

  constexpr Kilogram operator"" _kg(long double val) 
  {
    return Kilogram{  Kilogram::PreventUsage{}, 
                      static_cast<double>(val) };
  }

  constexpr Kilogram operator"" _g(unsigned long long val) 
  {
    return Kilogram{  Kilogram::PreventUsage{}, 
                      static_cast<double>(val) / 1000 };
  }

  constexpr Kilogram operator"" _mg(unsigned long long val) 
  {
    return Kilogram{  Kilogram::PreventUsage{}, 
                      static_cast<double>(val) / 1'000'000 };
  }

  int main()
  {
    constexpr auto val = 2.9_kg + 1000_g + 500_mg;
    // val -> 3.9005 kg
  }
*/

/*
  #include <concepts> // std::same_as

  template <typename T>
  class NamedType {};

  using Kilogram = NamedType<double>;
  using Meter = NamedType<double>;

  int main()
  {
    constexpr auto b1 = std::same_as<Kilogram, Meter>; 
    // b1 -> true

    // both Kilogram and Meter are same types
    // which is not the desired behavior
  } 
*/

/*
  #include <concepts> // std::same_as

  template <typename T, typename Tag>
  class NamedType {};

  struct KilogramTag {};
  struct MeterTag {};
  using Kilogram = NamedType<double, KilogramTag>;
  using Meter = NamedType<double, MeterTag>;

  int main()
  {
    constexpr auto b1 = std::same_as<Kilogram, Meter>; 
    // b1 -> false

    // Kilogram and Meter are distinct types
  }
*/

/*
  #include <concepts> // std::same_as

  template <typename T, typename Tag>
  class NamedType {};

  using Kilogram = NamedType<double, struct KilogramTag>;
  using Meter = NamedType<double, struct MeterTag>;

  // incomplete type is used for Tag template parameter

  int main()
  {
    constexpr auto b1 = std::same_as<Kilogram, Meter>; 
    // b1 -> false

    // Kilogram and Meter are distinct types
  }
*/

/*
  #include <concepts> // std::same_as

  // default template argument for NTTP
  template <typename T, auto = []{}>
  class NamedType {};

  using Kilogram = NamedType<double>;
  using Meter = NamedType<double>;


  int main()
  {
    constexpr auto b1 = std::same_as<Kilogram, Meter>; 
    // b1 -> false

    // Kilogram and Meter are distinct types
  }
*/

/*
  #include <concepts> // std::same_as

  // default template argument for type parameter
  template <typename T, typename = decltype([]{})>
  class NamedType {};

  using Kilogram = NamedType<double>;
  using Meter = NamedType<double>;


  int main()
  {
    constexpr auto b1 = std::same_as<Kilogram, Meter>; 
    // b1 -> false

    // Kilogram and Meter are distinct types
  }
*/

/*
  #include <utility> // std::move

  template <typename T, typename Tag>
  class NamedType {
  private:
    T m_value;

  public:
    explicit NamedType(const T& val) : m_value(val) {}
    explicit NamedType(T&& val) : m_value(std::move(val)) {}

    T& get() { return m_value; }
    const T& get() const { return m_value; }
  };

  using Kilogram  = NamedType<double, struct Kilogram_Tag>;
  using Meter     = NamedType<double, struct Meter_Tag>;

  int main()
  {
    Kilogram k1{ 44.88 };
    Meter m1{ 166.66 };

    k1 = m1;    // syntax error
    // error: no match for 'operator=' (operand types are 
    // 'Kilogram' {aka 'NamedType<double, Kilogram_Tag>'} and 
    // 'Meter' {aka 'NamedType<double, Meter_Tag>'})

    k1 = 38.88; // syntax error
    // error: no match for 'operator=' (operand types are 
    // 'Kilogram' {aka 'NamedType<double, Kilogram_Tag>'} and 
    // 'double')
  }
*/

/*
  #include <ostream>  // std::ostream
  #include <utility>  // std::move  

  template <typename T, template<typename> typename>
  struct CRTP {
    T& underlying()
    { 
      return static_cast<T&>(*this); 
    }
    const T& underlying() const
    { 
      return static_cast<const T&>(*this); 
    }
  };

  template <typename T>
  struct Addable : CRTP<T, Addable> {
    T operator+(const T& other) const
    {
      return T(this->underlying().get() + other.get());
    }
  };

  template <typename T>
  struct Multiplicable : CRTP<T, Multiplicable> {
    T operator*(const T& other) const
    {
      return T(this->underlying().get() * other.get());
    }
  };

  template <typename T>
  struct Subtractable : CRTP<T, Subtractable> {
    T operator-(const T& other) const
    {
      return T(this->underlying().get() - other.get());
    }
  };

  template <typename T>
  struct Incrementable : CRTP<T, Incrementable> {
    T& operator+=(const T& other)
    {
      this->underlying().get() += other.get();
      return this->underlying();
    }
  };

  template <typename T>
  struct Printable : CRTP<T, Printable> {
    std::ostream& print(std::ostream& os) const
    {
      return os << this->underlying().get();
    }
  };

  template <typename T, 
            typename Param, 
            template<typename> typename... Skills>
  class NamedType : public Skills<NamedType<T, Param, Skills...>>... 
  {
  private:
    T m_value;

  public:
    explicit NamedType(const T& val) : m_value(val) {}
    explicit NamedType(T&& val) : m_value(std::move(val)) {}

    T& get() { return m_value; }
    const T& get() const { return m_value; }
  };

  template <typename T, 
            typename Param, 
            template<typename> typename... Skills>
  std::ostream& operator<<(std::ostream& os, 
                          const NamedType<T, Param, Skills...>& obj)
  {
    return obj.print(os);
  }

  using Length = NamedType< 
    double, 
    struct Length_Tag, 
    Addable, Multiplicable, Incrementable, Printable>;

  using Weight = NamedType< 
    double, 
    struct Weight_Tag, 
    Addable, Subtractable, Printable>;

  int main()
  {
    // ---------------------------------------------------

    Length len1{ 33.33 }, len2{ 44.44 }, len3{ 55.55 };

    std::cout << len1 + len2 << '\n';   // output: 77.77
    std::cout << len1 * len2 << '\n';   // output-> 1484.35

    len1 += len3;
    std::cout << len1 << '\n';          // output: 88.88

    // ---------------------------------------------------

    Weight w1{ 100.1 }, w2{ 200.2 }, w3{ 300.3 };

    std::cout << w1 + w2 << '\n';       // output: 300.3
    std::cout << w1 - w2 << '\n';       // output: -100.1

    // ---------------------------------------------------
  }
*/