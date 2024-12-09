#include <iostream>

/*
                          ==============
                          | C++ idioms |
                          ==============
*/

/*
  #include <stdexcept>

  class Member {
  public:
    Member(){}
    Member(int x)
    {
      if (x > 10)
        throw std::runtime_error("exception in Member::Member(int)");
    }
  };

  class AClass {
  private:
    Member m_Member;  // member object
  public:
    AClass(int ival) : m_Member(ival) 
    {
      try {

      }
      catch(const std::exception& ex) {
        std::cerr << "exception caught: " << ex.what() << std::endl;
      }
    }
  };

  class BClass {
  private:
    Member m_Member;  // member object
  public:
    // function try block
    BClass(int ival) try : m_Member(ival) 
    {
    }
    catch(const std::exception& ex) {
      std::cerr << "exception caught: " << ex.what() << '\n';
      // throw;  --> compiler will implicitly wrote this line
    }
  };
  int main()
  {
    // -----------------------------------------------------

    AClass a1(20);
    // terminate called after throwing 
    // an instance of 'std::runtime_error'
    // what():  exception in Member::Member(int)

    // exception CAN NOT be caught so std::terminate called 
    // std::terminate will call std::abort

    // -----------------------------------------------------

    BClass b1(20);
    // exception caught: exception in Member::Member(int)
    // terminate called after throwing 
    // an instance of 'std::runtime_error'
    // what():  exception in Member::Member(int)

    // exception will be caught in function-try block 
    // but inside catch block exception will be rethrown
    // because of rethrown exception is not been caught 
    // std::terminate will be called

    // -----------------------------------------------------

    try {
      BClass b2(20);
    }
    catch(const std::exception& ex) {
      std::cerr << "exception caught: " << ex.what() << '\n';
    }
    // output ->
    //  exception caught: exception in Member::Member(int)
    //  exception caught: exception in Member::Member(int)

    // -----------------------------------------------------
  }
*/

/*
  int foo(int x)
  try {
    int ival{ 15 };
    if (x > 10)
      throw std::runtime_error("exception in foo(int)");
  }
  catch(const std::exception& ex) {
    return x + ival;
    // error: 'ival' was not declared in this scope;
  }
*/

/*
                    ---------------------------
                    | exception tracker idiom |
                    ---------------------------
*/

/*
  // How to understand 
  // which member's constructor thrown an exception   
  // in Myclass's constructor

  #include <stdexcept>

  struct XClass {
    XClass(const char*)
    {
      throw std::runtime_error("exception in XClass(const char*)");
    }
  };

  struct YClass {
    YClass(const char*)
    {
      throw std::runtime_error("exception in YClass(const char*)");
    }
  };

  struct ZClass {
    ZClass(const char*)
    {
      throw std::runtime_error("exception in ZClass(const char*)");
    }
  };

  class Myclass {
  private:
    XClass m_XClass;
    YClass m_YClass;
    ZClass m_ZClass;
  public:
    Myclass();
  };
*/

/*
  #include <stdexcept>

  struct XClass {
    XClass(const char*)
    {
      // throw std::runtime_error("exception in XClass(const char*)");
    }
  };

  struct YClass {
    YClass(const char*)
    {
      throw std::runtime_error("exception in YClass(const char*)");
    }
  };

  struct ZClass {
    ZClass(const char*)
    {
      throw std::runtime_error("exception in ZClass(const char*)");
    }
  };

  class Myclass {
  private:
    XClass m_XClass;
    YClass m_YClass;
    ZClass m_ZClass;
    enum TrackerType { NONE, ONE, TWO, THREE };
  public:
    Myclass(TrackerType tracker = NONE)
    try : m_XClass((tracker = ONE, "world")),
          m_YClass((tracker = TWO, "galaxy")),
          m_ZClass((tracker = THREE, "universe"))
    { 
      std::cout << "Myclass::Myclass()\n";
    }
    catch(const std::exception& ex) {
      switch (tracker)
      {
      case ONE:
        std::cerr << "exception caught in XClass: " 
                  << ex.what() << '\n';
        break;
      case TWO:
        std::cerr << "exception caught in YClass: " 
                  << ex.what() << '\n';
        break;
      case THREE:
        std::cerr << "exception caught in ZClass: " 
                  << ex.what() << '\n';
        break;
      }
    };
  };

  // comma operator will generate its right operands value.
  // m_XClass((tracker = ONE, "world"))
  // "ONE" will assign to "tracker" parameter variable
  // "world" will be passed to XClass constructor

  int main()
  {
    try {
      Myclass m1;
    }
    catch(const std::exception& ex) {
      std::cerr << "exception caught: " << ex.what() << '\n';
    }

    // output ->
    //  exception caught in YClass: exception in YClass(const char*)
    //  exception caught: exception in YClass(const char*)
  }
*/

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

/*
  void foo()
  {
    throw;
    // rethrow statement can be used also not inside catch block
  }

  int main()
  {
    foo();
    // output -> terminate called without an active exception

    // because there is no exception thrown 
    // rethrow statement will call std::terminate function.
  }
*/

/*
  #include <stdexcept>

  void foo()
  {
    throw;
  }

  void func(int x)
  {
    try {
      if (x > 10)
        throw std::out_of_range{"out of range exception"};
    }
    catch(...){
      foo();
    }
  };

  int main()
  {
    try {
      func(22);
    }
    catch(const std::out_of_range& ex) {
      std::cerr << "exception caught: " << ex.what() << '\n';
    }
    // output -> exception caught: out of range exception
  }
*/

/*
                  ------------------------------
                  | exception dispatcher idiom |
                  ------------------------------
*/

/*
  class a_exception {};
  class b_exception {};
  class c_exception {};

  void f1()
  {
    throw b_exception{};
  }

  void f2()
  {
    throw c_exception{};
  }

  void f3()
  {
    throw a_exception{};
  }

  void handle_exception()
  {
    try{
      throw;  // rethrow statement
    }
    catch(const a_exception& ex) {
      std::cerr << "exception caught: a_exception\n";
    }
    catch(const b_exception& ex) {
      std::cerr << "exception caught: b_exception\n";
    }
    catch(const c_exception& ex) {
      std::cerr << "exception caught: c_exception\n";
    }
  }

  int main()
  {
    std::cout << "[0] - main started\n";

    try {
      f1();
    }
    catch(...) {
      handle_exception();
    }

    std::cout << "[1] - main continues\n";

    try {
      f2();
    }
    catch(...) {
      handle_exception();
    }

    std::cout << "[2] - main continues\n";

    try {
      f3();
    }
    catch(...) {
      handle_exception();
    }

    std::cout << "[2] - main ends\n";

    // output ->
    //  [0] - main started
    //  exception caught: b_exception
    //  [1] - main continues
    //  exception caught: c_exception
    //  [2] - main continues
    //  exception caught: a_exception
    //  [2] - main ends
  }
*/

/*
                -------------------------------
                | polymorphic exception idiom |
                -------------------------------
*/

/*
  - elimizde oluşturulmuş bir exception nesnesi var.
    (geçici nesne de olabilir)
    bu exception nesnesi, exception hiyerarşisi içinde
    child class'lardan biri olabilir.

  - bu exception nesnesi bir fonksiyona gönderilmek
    ve fonksiyonun, gönderilen exception nesnesi türünden 
    bir exception throw etmesi isteniliyor.
    (burada amaç dinamik exception nesnesinin türünü korumak)
*/

/*
  class Exception_Base {};
  class Exception_Der1    : public Exception_Base {};
  class Exception_Der1_1  : public Exception_Der1 {};

  void func(Exception_Base& ex)
  {
    throw ex;   // object slicing 
  }

  int main()
  {
    Exception_Der1 ex1;

    try {
      func(ex1);  
    }
    catch (Exception_Der1_1) {
      std::cout << "Exception_Der1_1 caught\n";
    }
    catch (Exception_Der1) {
      std::cout << "Exception_Der1 caught\n";
    }
    catch (Exception_Base) {
      std::cout << "Exception_Base caught\n";
    }
    catch (...) {
      std::cout << "Unknown exception caught\n";
    }
    // output -> Exception_Base caught
  }
*/

/*
  // polymorphic Base class
  class Exception_Base {
  public:
    virtual ~Exception_Base() = default;   // virtual destructor
    virtual void raise() { throw *this; }
  };

  class Exception_Der1    : public Exception_Base {
  public:
    void raise() override { throw *this; }
  };
  class Exception_Der1_1  : public Exception_Der1 {
  public:
    void raise() override { throw *this; }
  };

  void func(Exception_Base& ex)
  {
    ex.raise();   // virtual dispatch
  }

  int main()
  {
    Exception_Der1_1 ex1;

    try {
      func(ex1);  
    }
    catch (Exception_Der1_1) {
      std::cout << "Exception_Der1_1 caught\n";
    }
    catch (Exception_Der1) {
      std::cout << "Exception_Der1 caught\n";
    }
    catch (Exception_Base) {
      std::cout << "Exception_Base caught\n";
    }
    catch (...) {
      std::cout << "Unknown exception caught\n";
    }

    // output -> Exception_Der1_1 caught
  }
*/

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

/*
  namespace ASpace {
    class Myclass {};

    void swap(Myclass& lhs, Myclass& rhs)
    {
      std::cout << "ASpace::swap(Myclass&, Myclass&)\n";
    }
  }

  int main()
  {
    ASpace::Myclass m1, m2;

    swap(m1, m2); // output -> ASpace::swap(Myclass&, Myclass&)
    // unqualified name lookup for "swap" identifier
    // start from main function's block scope       -> NOT FOUND
    // - unqualified lookup global namespace scope  -> NOT FOUND
    // - ADL in ASpace namespace scope              -> FOUND
  }
*/

/*
  #include <vector>

  namespace ASpace {
    class Myclass {};

    void foo(std::vector<Myclass>)
    {
      std::cout << "ASpace::foo(std::vector<Myclass>)\n";
    }

    enum Color { BLUE, BLACK };
    void bar(Color)
    {
      std::cout << "ASpace::bar(Color)\n";
    }
  }

  int main()
  {
    // -----------------------------------------------------

    ASpace::Myclass mx;
    std::vector<ASpace::Myclass> vec;

    foo(vec); // output -> ASpace::foo(std::vector<Myclass>)
    // unqualified name lookup for "foo" identifier
    // start from main function's block scope       -> NOT FOUND
    // - unqualified lookup global namespace scope  -> NOT FOUND
    // - ADL in ASpace namespace scope              -> FOUND

    // because of std::vector<Myclass> argument type is related 
    // with ASpace namespace, ADL will find foo function in ASpace
    // namespace scope.

    // -----------------------------------------------------

    bar(ASpace::BLUE); // output -> ASpace::bar(Color)

    // unqualified name lookup for "bar" identifier
    // start from main function's block scope       -> NOT FOUND
    // - unqualified lookup global namespace scope  -> NOT FOUND
    // - ADL in ASpace namespace scope              -> FOUND

    // -----------------------------------------------------
  }
*/

/*
  namespace ASpace {
    class Myclass {};

    void foo(Myclass)
    {
      std::cout << "ASpace::foo(Myclass)\n";
    }
  }

  void foo(ASpace::Myclass)
  {
    std::cout << "foo(ASpace::Myclass)\n";
  }

  int main()
  {
    ASpace::Myclass mx;

    foo(mx); // syntax error 
    // error: call of overloaded 'foo(ASpace::Myclass&)' is ambiguous

    // unqualified name lookup for "foo" identifier
    // start from main function's block scope       -> NOT FOUND
    // - unqualified lookup global namespace scope  -> FOUND
    // - ADL in ASpace namespace scope              -> FOUND

    // because of 2 viable function found, ambiguity error.
    // so there is order between,
    // - unqualified name lookup in global namespace scope 
    // - ADL in ASpace namespace scope.
  }
*/

/*
                  ------------------------
                  | ADL + fallback idiom |
                  ------------------------
*/

/*
namespace ASpace {
  class XClass {};
  class YClass {};

  void swap(XClass& lhs, XClass& rhs)
  {
    std::cout << "ASpace::swap(XClass&, XClass&)\n";
  }
}

template <typename T>
void func(T x, T y)
{
  using std::swap;
  swap(x, y);
}

// name lookup for "swap" identifier
// start from func template's block scope       -> NOT FOUND
// - unqualified lookup global namespace scope  -> NOT FOUND
// - ADL for T type in its namespace scope 

// if ADL will find a function named "swap" 
// in T's namespace scope then it will be used.

// if ADL WON'T find a function named "swap" 
// in T's namespace scope becuase of using std::swap declaration
// std::swap will be used.

int main()
{
  ASpace::XClass x1, x2;
  ASpace::YClass y1, y2;

  func(x1, x2); // output -> ASpace::swap(XClass&, XClass&)

  func(y1, y2);
  // unqualified lookup for "swap" identifier 
  // starts from func function block scope            -> NOT FOUND
  // - unqualified lookup global namespace scope      -> NOT FOUND
  // - ADL in ASpace namespace scope                  -> FOUND

  // context control failed because of swap function's argument type
  // so std::swap will be used. 
}
*/

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

/*
                    ------------------------
                    | hidden friend idioms |
                    ------------------------
*/

/*
  class Myclass {
  public:
    friend void foo(int); // hidden friend function
    // "foo" function is in namespace scope 
    // that Myclass is defined (global namespace scope in this case)

    // but unqualified "foo" identifier can only be found with ADL
    // unqualified name lookup in global namespace scope 
    // will NOT FOUND.
  };

  int main()
  {
    // -----------------------------------------------------

    foo(12);  // syntax error
    // error: 'foo' was not declared in this scope

    // unqualified name lookup for "foo" identifier
    // start from main function's block scope       -> NOT FOUND
    // - unqualified lookup global namespace scope  -> NOT FOUND

    // -----------------------------------------------------

    Myclass mx;
    foo(mx);
    // unqualified name lookup for "foo" identifier
    // start from main function's block scope       -> NOT FOUND
    // - unqualified lookup global namespace scope  -> NOT FOUND
    // - ADL in Myclass namespace scope             -> FOUND

    // -----------------------------------------------------
  }
*/

/*
  class Myclass {
  public:
    friend void swap(Myclass&, Myclass&)  // hidden friend function
    {
      std::cout << 
        "Myclass_hidden_friend - swap(Myclass&, Myclass&)\n";
    }
  };

  int main()
  {
    Myclass m1, m2;

    swap(m1, m2); 
    // output -> Myclass_hidden_friend - swap(Myclass&, Myclass&)

    // unqualified name lookup for "swap" identifier
    // start from main function's block scope       -> NOT FOUND
    // - unqualified lookup global namespace scope  -> NOT FOUND
    // - ADL in Myclass namespace scope             -> FOUND
  }
*/

/*
  struct AStruct {};
  struct BStruct {};
  struct CStruct {};
  struct DStruct {};
  struct EStruct {};
  struct FStruct {};

  AStruct operator+(const AStruct&, const AStruct&);
  BStruct operator+(const BStruct&, const BStruct&);
  CStruct operator+(const CStruct&, const CStruct&);
  DStruct operator+(const DStruct&, const DStruct&);
  EStruct operator+(const EStruct&, const EStruct&);
  FStruct operator+(const FStruct&, const FStruct&);

  class Myclass {};

  int main()
  {
    Myclass m1, m2;

    auto m3 = m1 + m2;  // syntax error
    // error: no match for 'operator+' 
    // (operand types are 'Myclass' and 'Myclass')

    // candidate: 'AStruct operator+(const AStruct&, const AStruct&)'
    // no known conversion for arg1 from 'Myclass' to 'const AStruct&'

    // candidate: 'BStruct operator+(const BStruct&, const BStruct&)'
    // no known conversion for arg1 from 'Myclass' to 'const BStruct&'

    // candidate: 'CStruct operator+(const CStruct&, const CStruct&)'
    // no known conversion for arg1 from 'Myclass' to 'const CStruct&'

    // candidate: 'DStruct operator+(const DStruct&, const DStruct&)'
    // no known conversion for arg1 from 'Myclass' to 'const DStruct&'

    // candidate: 'EStruct operator+(const EStruct&, const EStruct&)'
    // no known conversion for arg1 from 'Myclass' to 'const EStruct&'

    // candidate: 'FStruct operator+(const FStruct&, const FStruct&)'
    // no known conversion for arg1 from 'Myclass' to 'const FStruct&'

    // name lookup for "operator+" identifier
    // will start from Myclass class scope          -> NOT FOUND
    // - unqualified lookup global namespace scope  -> FOUND

    // (6 candidate function found - context control failed)
    // increase compile time because of
    // global operator+ functions
  }
*/

/*
  // hidden friend functions are 
  // not visible in global namespace scope
  // only can be found with ADL

  struct AStruct {
    friend AStruct operator+(const AStruct&, const AStruct&);
  };

  struct BStruct {
    friend BStruct operator+(const BStruct&, const BStruct&);
  };

  struct CStruct {
    friend CStruct operator+(const CStruct&, const CStruct&);
  };

  struct DStruct {
    friend DStruct operator+(const DStruct&, const DStruct&);
  };

  class Myclass {};

  int main()
  {
    Myclass m1, m2;
    auto m3 = m1 + m2;  // syntax error
    // error: no match for 'operator+' 
    // (operand types are 'Myclass' and 'Myclass')

    // name lookup for "operator+" identifier
    // will start from Myclass class scope          -> NOT FOUND
    // - unqualified lookup global namespace scope  -> NOT FOUND
    
    // because of operator+ functions are hidden friend functions
    // compiler will not see them in the global namespace scope
  }
*/

/*
  namespace ASpace {
    struct XStruct {};

    struct YStruct {
      operator XStruct ()
      {
        return XStruct{};
      }
    };

    void foo(XStruct)
    {
      std::cout << "ASpace::foo(XStruct)\n";
    }
  }

  namespace BSpace {
    struct XStruct {
      friend void foo(XStruct)
      {
        std::cout << "hidden friend XStruct - ASpace::foo(XStruct)\n";
      }
    };

    struct YStruct {
      operator XStruct ()
      {
        return XStruct{};
      }
    };
  }

  int main()
  {
    // -----------------------------------------------------

    ASpace::XStruct x1;
    ASpace::YStruct y1;

    foo(x1);  // output -> ASpace::bar(XStruct)
    foo(y1);  // output -> ASpace::bar(XStruct)
    // because of implicit conversion from YStruct to XStruct
    // foo can also be called with ASpace::YStruct object

    // -----------------------------------------------------

    BSpace::XStruct x2;
    BSpace::YStruct y2;

    foo(x2);  // output -> hidden friend XStruct - ASpace::foo(XStruct)
    foo(y2);  // syntax error
    // error: 'foo' was not declared in this scope

    // -----------------------------------------------------
  }
*/

/*
                      ---------------------
                      | scope guard idiom |
                      ---------------------
*/

/*
  #include <utility>    // std::move
  #include <stdexcept>  // std::runtime_error

  template <typename Func>
  class scope_guard {
  private:
    Func m_Func;
    bool m_call{ true };
  public:
    scope_guard(Func func) noexcept  
      : m_Func(func) {}

    scope_guard(scope_guard&& other)  
      : m_Func(std::move(other.m_Func)), 
        m_call(other.m_call) {}

    ~scope_guard()
    {
      if (m_call)
        m_Func();
    }

    scope_guard(const scope_guard&) = delete;
    scope_guard& operator=(const scope_guard&) = delete;

    void dismiss() noexcept { m_call = false; }
  };

  void cleanup()
  {
    std::cout << "cleanup called\n";
  }

  void throw_exception()
  {
    throw std::runtime_error("exception in throw_exception()");
  }
  int main()
  {
    std::cout << "[0] - main started\n";

    {
      scope_guard sg{ cleanup };
    }

    std::cout << "[1] - main continues\n";

    // -----------------------------------------------------

    try {
      scope_guard sg{ cleanup };
      throw_exception();
    }
    catch(const std::exception& ex) {
      std::cerr << "exception caught: " << ex.what() << '\n';
    }

    // -----------------------------------------------------

    std::cout << "[2] - main ends\n";

    // output ->
    //  [0] - main started
    //  cleanup called
    //  [1] - main continues
    //  cleanup called
    //  exception caught: exception in throw_exception()
    //  [2] - main ends
  }
*/

/*
  #include <utility>    // std::move
  #include <stdexcept>  // std::runtime_error

  template <typename Func>
  class scope_guard {
  private:
    Func m_Func;
    bool m_call{ true };
  public:
    scope_guard(Func func) noexcept  
      : m_Func(func) {}

    scope_guard(scope_guard&& other)  
      : m_Func(std::move(other.m_Func)), 
        m_call(other.m_call) {}

    ~scope_guard()
    {
      if (m_call)
        m_Func();
    }

    scope_guard(const scope_guard&) = delete;
    scope_guard& operator=(const scope_guard&) = delete;

    void dismiss() noexcept { m_call = false; }
  };

  void cleanup()
  {
    std::cout << "cleanup called\n";
  }

  // factory function returns scope_guard object
  template <typename Func>
  scope_guard<Func> make_scope_guard(Func func) noexcept
  {
    return scope_guard<Func>(func);
  }  

  int main()
  {
    std::cout << "[0] - main started\n";

    {
      auto g = make_scope_guard(cleanup);
    }

    std::cout << "[1] - main continues\n";

    // output ->
    //  [0] - main started
    //  cleanup called
    //  [1] - main continues
  }
*/

/*
                  ------------------------------
                  | return type resolver idiom |
                  ------------------------------
*/

/*
  class AClass{};
  class BClass{};
  class CClass{};

  class Myclass {
  public:
    explicit operator int() const;
    operator AClass() const;
    operator BClass() const;
    operator CClass() const;
  };

  int main()
  {
    Myclass m1;
    int ival = m1;  // syntax error
    // error: cannot convert 'Myclass' to 'int' in initialization
    // note: explicit conversion function was not considered

    int ival2 = static_cast<int>(m1);  // VALID
  }
*/

/*
  class String {
  public:
    operator int() const
    {
      std::cout << "String::operator int()\n";
      return 1;
    }
    operator double() const
    {
      std::cout << "String::operator double()\n";
      return 1.0;
    }
    operator long long() const
    {
      std::cout << "String::operator long long()\n";
      return 1LL;
    }
  };

  int main()
  {
    String s1;

    int ival = s1;        // output -> String::operator int()
    double dval = s1;     // output -> String::operator double()
    long long lval = s1;  // output -> String::operator long long()
  }
*/

/*
  #include <string> // std::stoi, std::stoll, std::stod
  #include <format>

  class String {
  private:
    std::string m_str;
  public:
    String(const char* str) : m_str(str) {}

    operator int() const { return std::stoi(m_str); }
    operator double() const { return std::stod(m_str); }
    operator long long() const { return std::stoll(m_str); }
  };

  int main()
  {
    using namespace std;

    String s1("13.21");

    int ival = s1;
    double dval = s1;
    long long lval = s1;

    cout << format("int = {}, double = {}, long long = {}\n",
                    ival, dval, lval);
    // output -> int = 13, double = 13.21, long long = 13
  }
*/

/*
  #include <vector>
  #include <list>
  #include <set>

  class RangeContainer {
  private:
    const int m_start;
    const int m_end; 
  public:
    RangeContainer(int start, int end) : m_start(start), m_end(end) 
    {
      if (m_start > m_end)
        throw std::runtime_error("invalid range values");
    }

    template <typename Con>
    operator Con() const
    {
      Con con;

      for (int i{ m_start }; i < m_end; ++i)
        con.insert(con.end(), i);

      return con;
    }
  };

  int main()
  {
    // -----------------------------------------------------

    std::vector<int> ivec = RangeContainer(10, 25);
    std::cout << "ivec.size() = " << ivec.size() << '\n';
    // output -> ivec.size() = 15

    for (auto elem : ivec)
      std::cout << elem << ' ';
    std::cout << '\n';
    // output -> 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24

    // -----------------------------------------------------

    std::set<int> iset = RangeContainer(5, 14);
    std::cout << "iset.size() = " << iset.size() << '\n';
    // output -> iset.size() = 9

    for (auto elem : iset)
      std::cout << elem << ' ';
    std::cout << '\n';
    // output -> 5 6 7 8 9 10 11 12 13

    // -----------------------------------------------------

    std::list<int> ilist = RangeContainer(-5, 12);
    std::cout << "ilist.size() = " << ilist.size() << '\n';
    // output -> ilist.size() = 17

    for (auto elem : ilist)
      std::cout << elem << ' ';
    std::cout << '\n';
    // output -> -5 -4 -3 -2 -1 0 1 2 3 4 5 6 7 8 9 10 11

    // -----------------------------------------------------
  }
*/

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

/*
  in polymorphic classes, Base class's destructor should be 
    - public virtual 
    - protected non-virtual
*/

/*
  // polymorphic Base class
  class Base {
  public:
    ~Base() { std::cout << "Base::~Base()\n"; }
    virtual void foo(){}
  };

  class Der : public Base {
  public:
    ~Der() { std::cout << "Der::~Der()\n"; }
  };

  int main()
  {
    Base* p = new Der;
    // "operator new" function will allocate sizeof(Der) bytes memory
    // void* returned from "operator new" function 
    // will be casted to `this` pointer in Der::Der() constructor

    delete p; // undefined behavior(UB)
    // Base::~Base() will be called
    // because Base class's destructor is not virtual
    // so it will not call Der::~Der() destructor
    // which will cause UB 

    // output -> Base::~Base()
  }
*/

/*
  // polymorphic Base class
  class Base {
  public:
    virtual ~Base() { std::cout << "Base::~Base()\n"; }
  };

  class Der : public Base {
  public:
    ~Der() { std::cout << "Der::~Der()\n"; }
  };

  int main()
  {
    Base* p = new Der;
    delete p;
    // output -> 
    //  Der::~Der()     ---> virtual dispatch
    //  Base::~Base()
  }
*/

/*
  // polymorphic Base class
  class Base {
  public:
    virtual void foo(){}

  protected:
    ~Base() { std::cout << "Base::~Base()\n"; }
  };

  class Der : public Base {
  public:
    ~Der() { std::cout << "Der::~Der()\n"; }
  };

  int main()
  {
    // -----------------------------------------------------

    Base* p_base = new Der;
    delete p_base; // syntax error
    // error: 'Base::~Base()' is protected within this context

    // -----------------------------------------------------

    Der* p_der = new Der;
    delete p_der; // VALID
    // output ->
    //  Der::~Der()
    //  Base::~Base()

    // -----------------------------------------------------
  }
*/

/*
              -------------------------------------
              | non virtual interface idiom (NVI) |
              -------------------------------------
*/

/*
  - Derived classes virtual functions,
    should not be in public section of the class

  - Derived classes non-virtual functions, 
    should call its virtual functions.

  - interface ile implementasyonu birbirinden ayırarak
    daha stabil bir interface sağlar.
  - taban sınıfın implementasyon üzerinde kontrolü olur.
    virtual implementasyon fonksiyonu çağrılmadan önce 
    virtual olmayan fonksiyonda belirli kontroller yapılabilir.
*/

/*
  class Old_Base {
  public:
    virtual void foo();
    virtual void bar();
  };

  class New_Base {
  private:
    virtual void foo_implementation();
    virtual void bar_implementation();
  public:
    void foo()
    {
      foo_implementation(); 
      // virtual dispatch can be applied
    }
    void bar()
    {
      bar_implementation();
      // virtual dispatch can be applied
    }
  };
*/

/*
  #include <string>

  class Animal {
  private:
    virtual std::string get_sound() const = 0;
  public:
    void speak() const
    {
      std::cout << get_sound() << '\n';
    }
  };

  class Cat : public Animal {
  private:
    virtual std::string get_sound() const override
    {
      return "meow";
    }
  };

  class Dog : public Animal {
  private:
    virtual std::string get_sound() const override
    {
      return "woof";
    }
  };
*/

/*
                  ------------------------------
                  | fragile base class problem |
                  ------------------------------
*/

/*
  #include <set>
  #include <iterator> // std::distance

  // Set class is a container adaptor of std::set
  class Set {
  private:
    std::set<int> m_set;

    virtual void add_impl(int i) = 0;
    virtual void add_range_impl(const int* begin, const int* end) = 0;

  public:
    void add(int i)
    {
      m_set.insert(i);
      add_impl(i);
    }

    void add_range(const int* begin, const int* end)
    {
      // -------------------- (1) OLD CODE ---------------------
      // m_set.insert(begin, end); 
      // -------------------------------------------------------

      // -------------------- (2) NEW CODE ---------------------
      while (begin != end)
        m_set.insert(*begin++);
      // -------------------------------------------------------

      add_range_impl(begin, end);   
      // because in new code begin pointer increased until end pointer
      // "add_range_impl" function will be called with wrong arguments.
    } 
  };

  class CountingSet : public Set {
  private:
    int m_count;

    virtual void add_impl(int i) override
    {
      ++m_count;
    }

    virtual 
    void add_range_impl(const int* begin, const int* end) override
    {
      m_count += std::distance(begin, end);
    }
  public:
    int get_count() const { return m_count; }
  };

  int main()
  {
    CountingSet cs;

    int arr[] = { 1, 2, 3, 4, 5 };
    cs.add_range(arr, arr + 5);

    std::cout << "cs.get_count() = " << cs.get_count() << '\n';
    // output -> cs.get_count() = 0
  }
*/

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

/*
  #include <tuple>  // std::get
  #include <string>

  int main()
  {
    using namespace std;

    tuple<int, double, string> t1{ 10, 3.14, "hello" };

    get<0>(t1);

    get<3>(t1);     // syntax error
    // error: '__type_pack_element' index is out of range

    get<long>(t1);  // syntax error
    // error : static assertion failed 
    // the type T in std::get<T> must occur exactly once in the tuple
  }
*/

/*
  #include <tuple>
  #include <string>

  int main()
  {
    using namespace std;

    using id = int;
    using wage = double;
    using name = string;

    tuple<id, wage, name> t1{ 10, 3.14, "hello" };
  }
*/

/*
  #include <tuple>  // std::get
  #include <string>

  int main()
  {
    using namespace std;

    enum tuple_types{ ID, WAGE, NAME };

    tuple<int, double, string> tx{ 10, 3.14, "hello" };

    get<ID>(tx);
    get<WAGE>(tx);
    get<NAME>(tx);
  }
*/

/*
                      ---------------------
                      | std::tuple idioms |
                      ---------------------
*/

/*
  #include <tuple>  // std::tie
  #include <string>

  std::tuple<int, double, std::string> func()
  {
    return { 10, 3.14, "hello" };
  }

  int main()
  {
    // -----------------------------------------------------

    int ival = 11;
    double dval = 22.2;
    std::string str = "hello";

    auto tx = std::tie(ival, dval, str);
    // std::tie returns tuple object 
    // which contains references of its arguments
    // "tx"'s data type is std::tuple<int&, double&, std::string&>

    // -----------------------------------------------------

    tie(ival, dval, str) = func();

    std::cout << "ival = " << ival << '\n';
    // output -> ival = 10
    std::cout << "dval = " << dval << '\n';
    // output -> dval = 3.14
    std::cout << "str = " << str << '\n';
    // output -> str = hello

    // -----------------------------------------------------
  }
*/

/*
  #include <tuple>

  class Date {
  private:
    int m_d;
    int m_m;
    int m_y;
  public:
    friend bool operator<(const Date& d1, const Date& d2)
    {
      using namespace std;

      tuple{ d1.m_y, d1.m_m, d1.m_d } < tuple{ d2.m_y, d2.m_m, d2.m_d };
    }
  };
*/

/*
  #include <tuple>  // std::tie

  int main()
  {
    // -----------------------------------------------------

    int x = 10, y = 20, z = 30, t = 40;

    int temp = x;
    x = y;
    y = z;
    z = t;
    t = temp;

    std::cout << "x = " << x << '\n';   // output -> x = 20
    std::cout << "y = " << y << '\n';   // output -> y = 30
    std::cout << "z = " << z << '\n';   // output -> z = 40
    std::cout << "t = " << t << '\n';   // output -> t = 10

    // -----------------------------------------------------

    int a = 10, b = 20, c = 30, d = 40;

    std::tie(a, b, c, d) = std::tuple{ b, c, d, a };

    std::cout << "a = " << a << '\n';   // output -> a = 20
    std::cout << "b = " << b << '\n';   // output -> b = 30
    std::cout << "c = " << c << '\n';   // output -> c = 40
    std::cout << "d = " << d << '\n';   // output -> d = 10

    // -----------------------------------------------------
  }
*/

/*
  #include <tuple>  // std::apply(C++17)

  int sum(int x, int y, int z)
  {
    return x + y + z;
  }

  int main()
  {
    std::tuple tx{ 10, 20, 30 };

    // -----------------------------------------------------

    auto result_1 = sum(std::get<0>(tx), 
                        std::get<1>(tx), 
                        std::get<2>(tx));

    std::cout << "result_1 = " << result_1 << '\n';
    // output -> result_1 = 60

    // -----------------------------------------------------

    auto result_2 = std::apply(sum, tx);

    std::cout << "result_2 = " << result_2 << '\n';
    // output -> result_2 = 60

    // -----------------------------------------------------
  }
*/

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

/*
  // some enum types is being used as bitmasks

  enum Color { YELLOW = 0x1, RED = 0x2, BLUE = 0x4 };
  // unscoped enum type

  int main()
  {
    // -----------------------------------------------------

    auto purple = Color::RED | Color::BLUE;
    auto purple_2 = RED | BLUE;
    // Those 2 lines are equivalent.

    // enumarator's(RED, BLUE, YELLOW) scope 
    // is unscoped enum's(Color) scope
    // which is global namespace scope in this case.

    auto green  = Color::YELLOW | Color::BLUE; 
    auto orange = Color::YELLOW | Color::RED;

    // -----------------------------------------------------

    // purple, orange and green's data type is `int`

    // -----------------------------------------------------
  }
*/

/*
  enum class Color { YELLOW = 0x1, RED = 0x2, BLUE = 0x4 };
  // scoped enum type(enum class)

  int main()
  {
    auto purple = Color::RED | Color::BLUE;     // syntax error
    auto green  = Color::YELLOW | Color::BLUE;  // syntax error
    auto orange = Color::YELLOW | Color::RED;   // syntax error
    
    // error: no match for 'operator|' 
    //  (operand types are 'Color' and 'Color')
  }
*/

/*
  #include <type_traits>  // std::underlying_type

  enum class Color { YELLOW = 0x1, RED = 0x2, BLUE = 0x4 };
  // scoped enum type(enum class)

  constexpr Color operator|(Color lhs, Color rhs)
  {
    using u_type = std::underlying_type_t<Color>;

    return static_cast<Color>(static_cast<u_type>(lhs) | 
                              static_cast<u_type>(rhs));
  }

  int main()
  {
    constexpr auto purple = Color::RED | Color::BLUE;
    // purple -> (Color)6

    constexpr auto green  = Color::YELLOW | Color::BLUE;
    // green -> (Color)5

    constexpr auto orange = Color::YELLOW | Color::RED;
    // orange -> (Color)3
  }
*/

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

/*
  // command line arguments will be hold in string_view vector

  #include <vector>
  #include <string_view>

  int main(int argc, char** argv)
  {
    std::vector<std::string_view> args{ argv, argv + argc };
    // std::vector has a range constructor.
    // one-liner version

    for (auto arg : args) {
      std::cout << arg << '\n';
    }

    // input -> prog.exe hello world
    // output -> 
    //  prog.exe
    //  hello
    //  world
  }
*/

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

/*
  // converting the string that printf will write to stream
  // to a std::string object, so we can use printf function's 
  // formatting capabilities.

  #include <cstdio>   // std::printf
  #include <string>   // std::string
  #include <cstdarg>  // va_list, va_start, va_end

  // va_list types
  // va_start, va_arg, va_end, va_copy MACROS
  // vsnprintf_s, vsprintf, vfprintf functions
  std::string printf_to_str(const char* fmt, ...)
  {
    va_list args;
    va_list args2;

    va_start(args, fmt);
    va_copy(args2, args);

    std::string str( vsnprintf(nullptr, 0, fmt, args2) + 1, '\0');
    // fill constructor

    va_end(args2);
    vsprintf(str.data(), fmt, args);
    va_end(args);

    str.pop_back();
    return str;
  }

  int main()
  {
    char str[] = "hello world";
    int ival = 22;
    double dval = 3.14;

    printf("%s and %d and %.2f\n", str, ival, dval);
    // output -> hello world and 22 and 3.14

    auto s1 = printf_to_str("%s and %d and %.2f", str, ival, dval);

    std::cout << '[' << s1 << "]\n";
    // output -> [hello world and 22 and 3.14]
  }
*/

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

/*
            ------------------------------------------
            | PIMPL idiom(Pointer to Implementation) |
            ------------------------------------------
*/

/*
  diğer isimler :
    - opaque pointer idiom
    - handle-body idiom
    - d-pointer idiom
    - cheshire cat idiom
    - compiler firewall idiom

  - sınıfın interface ve implementasyonu arasındaki bağımlılığı
    ortadan kaldırır.

  - sınıfın private veri elemanları ve private fonksiyonları
    sınıfın implementasyon sınıfına taşınır.

  - derleme zamanınını düşürür.
*/

/*
  class Myclass {
  private:
    int m_i1;
    int m_i2;
    double m_d1;
  };

  // Question : 
  //  when a data member added to or removed from Myclass,
  //  does binary compatibility will be broken?

  // Answer : 
  //  Yes, class object's layout will be changed and
  //  sizeof(Myclass) will be changed
*/

/*
  class Myclass {
  public:
    void func(int);

  private:
    void func(double);
  };

  // Question : 
  //  why Myclass's private member functions are are also 
  //  been removed from Myclass's interface?
  // Answer : because of function overloading.

  // function overload resolution is done before access control
*/

/*
        <---- check ../not_related/deep constness ---->
*/

/*
              <--- check ../headers/student.h --->
              <--- check ../src/student.cpp --->
*/

/*
  #include "../headers/student.h"

  int main()
  {
    Student s1("hello", "world");
    s1.add_grade(66);
    s1.add_grade(77);
    s1.add_grade(88);

    s1.print();
    // output -> name = hello, surname = world, grades = 66 77 88
  }
*/

/*
                    ---------------------
                    | Fast-PIMPLE idiom |
                    ---------------------
*/

/*
  #include <cstddef>      // std::size_t, std::max_align_t
  #include <type_traits>  
  // std::aligned_storage, std::alignment_of
  #include <string>

  using namespace std;

  // ---------------------------------------------------

  // placement.h
  // ------------ 

  template <typename T>
  void placement_new(void* buffer, std::size_t buffer_size)
  {
    new(buffer)T();
  }

  template <typename T>
  T* placement_cast(void* buffer)
  {
    return reinterpret_cast<T*>(buffer);
  }

  template <typename T>
  void placement_delete(void* buffer)
  {
    placement_cast<T>(buffer)->~T();
  }

  // ---------------------------------------------------

  // logger.h
  // ----------

  class Logger {
  private:
    static constexpr std::size_t m_size{ 1024 };
    aligned_storage<m_size, alignment_of_v<max_align_t>> m_impl{};

  public:
    Logger();
    ~Logger();
    void log(const std::string& msg);
  };

  // ---------------------------------------------------

  // logger.cpp
  // ------------

  // #include "logger.h"
  // #include "placement.h"
  #include <fstream>

  class Logger_Impl {
  private:
    std::ofstream m_ofs;

  public:
    void log(const std::string& msg)
    {
      m_ofs << "message : " <<  msg << std::endl;
      std::cout << "message : " <<  msg << std::endl;
    }
  };

  Logger::Logger()
  {
    static_assert( m_size >= sizeof(Logger_Impl));

    placement_new<Logger_Impl>(&m_impl, sizeof(Logger_Impl));
  }

  Logger::~Logger()
  {
    placement_delete<Logger_Impl>(&m_impl);
  }

  void Logger::log(const std::string& msg)
  {
    placement_cast<Logger_Impl>(&m_impl)->log(msg);
  }

  // ---------------------------------------------------
*/

/*
                      -------------------
                      | copy-swap idiom |
                      -------------------
*/

/*
  - sınıfın atama operatör fonksiyonlarının yazılmasında 
    kullanılıyor. (copy assignment, move assignment)
*/

/*
  // when strong exception guarantee is needed
  // strong_assign function can be used.

  template <typename T>
  T& strong_assign(T& dest, T& source)
  {
    using std::swap;
    swap(dest, source);
    return dest;
  }
*/

/*
  #include <cstddef>    // std::size_t
  #include <algorithm>  // std::copy_n, std::swap
  #include <utility>    // std::exchange

  template <typename T>
  class Dynamic_Arr {
  private:
    T* mp_arr;
    std::size_t m_size;

  public:
    Dynamic_Arr(T* p_arr, std::size_t size)
      : mp_arr{ p_arr }, m_size{ size } {}

    Dynamic_Arr(const Dynamic_Arr& other)
      : mp_arr{ new T[other.m_size] }, m_size{ other.m_size }
    {
      std::copy_n(other.mp_arr, other.m_size, mp_arr);
    }

    Dynamic_Arr(Dynamic_Arr&& other)
      : mp_arr{ std::exchange(other.mp_arr, nullptr) }, 
        m_size{ other.m_size }

    ~Dynamic_Arr()
    {
      if (mp_arr)
        delete[] mp_arr;
    }

    T& operator[](std::size_t idx) 
    { 
      return mp_arr[idx]; 
    }
    const T& operator[](std::size_t idx) const 
    { 
      return mp_arr[idx]; 
    }

    std::size_t size() const 
    { 
      return m_size; 
    }

    // hidden friend swap function
    friend void swap(Dynamic_Arr& lhs, Dynamic_Arr& rhs) noexcept
    {
      using std::swap;
      swap(lhs.mp_arr, rhs.mp_arr);
      swap(lhs.m_size, rhs.m_size);
    }

    // -----------------------------------------------------

    // ----------------- OLD WAY -----------------
    // Dynamic_Arr& operator=(const Dynamic_Arr& other)
    // {
    //   Dynamic_Arr temp_arr(other);
    //   swap(*this, temp_arr);
    //   return *this;
    // }

    // ----------------- NEW WAY -----------------
    Dynamic_Arr& operator=(Dynamic_Arr arr_copy)
    {
      swap(*this, arr_copy);
      return *this;
    }

    // -----------------------------------------------------
  };
*/

/*
                    -------------------------
                    | attorney-client idiom |
                    -------------------------
*/

/*
  class Myclass {
  private:
    friend class AClass;

    void foo()
    {
      std::cout << "Myclass::foo() - private\n";
    }
    void bar()
    {
      std::cout << "Myclass::bar() - private\n";
    }
    void baz()
    {
      std::cout << "Myclass::baz() - private\n";
    }
  };

  class AClass {
  public:
    void func(Myclass& m)
    {
      m.foo();
      m.bar();
      m.baz();
    }
  };

  int main()
  {
    AClass a1;
    Myclass m1;

    a1.func(m1);
    // output ->
    //  Myclass::foo() - private
    //  Myclass::bar() - private
    //  Myclass::baz() - private
  }
*/

/*
  // what if we want to give an access to ONLY "foo" function
  // NOT "bar" and "baz" functions

  // also can be applied to data members

  class Myclass {
  private:
    int m_x{ 11111 };

    friend class Attorney;

    void foo()
    {
      std::cout << "Myclass::foo() - private\n";
    }
    void bar()
    {
      std::cout << "Myclass::bar() - private\n";
    }
    void baz()
    {
      std::cout << "Myclass::baz() - private\n";
    }
  };

  class Attorney {
    friend class Client;
    static void foo(Myclass& m) { m.foo(); }
    static int get_x(const Myclass& m) { return m.m_x; }
  };

  class Client {
  public:
    void foo(Myclass& m) { Attorney::foo(m); }
    int get_x(Myclass& m) { return Attorney::get_x(m); }
  };

  int main()
  {
    Myclass m1;
    Client c1;

    c1.foo(m1); 
    // output -> Myclass::foo() - private
    
    int val = c1.get_x(m1); 
    std::cout << "val = " << val << '\n';
    // output -> val = 11111
  }
*/

/*
                      -----------------------
                      | nifty counter idiom |
                      -----------------------
*/

/*
  - farklı modüllerde bulunan statik ömürlü nesnelerin
    (global değişkenler ve sınıfların statik veri elemanları)
    hayata gelme sıraları öngürelemez.(static initialization fiasco)

  - bir modüldeki statik ömürlü bir varlık, constructorı içinde
    başka bir modüldeki statik ömürlü bir varlığı kullanıyorsa
    kullanmasın! çünkü hayata gelme sırası öngörülemez ve 
    çöp değerde kullanma riski oluşabilir.

  - örneğin cin, cout, cerr, wcin, wcout, wcerr...
*/

/*
  class Myclass {
  public:
    Myclass()
    {
      std::cout << "Myclass::Myclass()\n";    
      // "std::cout" is a static storage duration object.
    }

    ~Myclass()
    {
      std::cout << "Myclass::~Myclass()\n";
      // "std::cout" is a static storage duration object.
    }
  };

  Myclass g_m1; // global Myclass object
  // g_m1 is a static storage duration object.

  // output ->
  //  Myclass::Myclass()
  //  Myclass::~Myclass()

  // before global Myclass object's construction,
  // "std::cout" object has already been constructed.

  // "std::cout" object are still alive
  // after global Myclass object's destruction.
*/

/*
              <--- check cpp_idioms/stream.h --->
              <--- check cpp_idioms/stream.cpp --->
*/

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------

