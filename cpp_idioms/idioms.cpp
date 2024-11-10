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

// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
// -----------------------------------------------------
