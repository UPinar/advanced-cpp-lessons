#include <iostream>

/*
                    ======================
                    | Exception Handling |
                    ======================
*/

/*
                ------------------------------
                | std::exception_ptr (C++11) |
                ------------------------------
*/

/*
  - bir hatayı yakalamak ve bu hatanın işlenmesini 
    başka bir bağlama(başka bir thread'de olabilir) aktarmak
    veya yakalanan hatanın daha sonra işlenebilmesi için
    saklama işlevi görmek.

  - std::exception_ptr nesneleri, polymorphic pointerlar gibi
    çalışıyor.
*/

/*
  // std::exception_ptr is a nullable type

  #include <exception>

  int main()
  {
    std::exception_ptr ep;

    if (ep == nullptr)
      std::cout << "ep is nullptr" << '\n';
    // output -> ep is nullptr
  }
*/

/*
  #include <string>
  #include <exception>  
  // std::exception_ptr, std::current_exception
  // std::rethrow_exception

  // when std::exception_ptr is copied 
  // dynamic type is not lost.
  void handle_exception(std::exception_ptr param_ep)
  {
    try {
      if (param_ep)
        std::rethrow_exception(param_ep);
    }
    catch (const std::out_of_range& ex) {
      std::cout << "std::out_of_range exception caught\n";
    }
    catch (const std::exception& ex) {
      std::cout << "std::exception caught\n";
    }
  }

  int main()
  {
    std::cout << "[0] - main started\n";

    std::exception_ptr ep;

    try {
      std::string str{ "hello world" };
      auto ch = str.at(100);
    }
    catch (...) {
      ep = std::current_exception();
    }

    handle_exception(ep);

    std::cout << "[1] - main continues\n";

    // output ->
    //  [0] - main started
    //  std::out_of_range exception caught
    //  [1] - main continues

    // exception's dynamic type is not lost !
  }
*/

/*
  #include <stdexcept>
  #include <thread>

  std::exception_ptr g_ep = nullptr;

  void func(int x)
  {
    std::cout << "func(int x) started, x = " << x << '\n';

    try {
      if (x % 2 == 0)
        throw std::invalid_argument{ "even argument exception" };
    }
    catch (...) {
      g_ep = std::current_exception();
    }

    std::cout << "func(int x) ends, x = " << x << '\n';
  }

  int main()
  {
    std::thread tx{ func, 10 };
    tx.join();

    try {
      if (g_ep)
        std::rethrow_exception(g_ep);
    }
    catch (const std::exception& ex) {
      std::cout << "std::exception caught : " 
                << ex.what() << '\n';
    }

    // output ->
    //  func(int x) started, x = 10
    //  func(int x) ends, x = 10
    //  std::exception caught : even argument exception

    // exception caught in "tx" thread and changing g_ep's value
    // with std::current_exception 
    // and after "tx" has been executed.
    // inside the main thread, exception that collected in "tx"
    // thread is rethrown and caught.
  }
*/

/*
  #include <vector>
  #include <mutex>      // std::lock_guard
  #include <thread>
  #include <stdexcept>  // std::runtime_error 
  #include <exception>  
  // std::exception_ptr, std::current_exception

  std::vector<std::exception_ptr> g_ex_vec;
  std::mutex g_mutex;

  void func_1()
  {
    throw std::runtime_error{ "exception from func_1" };
  }
  void func_2()
  {
    throw std::runtime_error{ "exception from func_2" };
  }

  void call_func1()
  {
    try {
      func_1();
    }
    catch (...) {
      std::lock_guard<std::mutex> guard{ g_mutex };
      g_ex_vec.push_back(std::current_exception());
    }
  }

  void call_func2()
  {
    try {
      func_2();
    }
    catch (...) {
      std::lock_guard<std::mutex> guard{ g_mutex };
      g_ex_vec.push_back(std::current_exception());
    }
  }

  int main()
  {
    std::thread tx1{ call_func1 };
    std::thread tx2{ call_func2 };

    tx1.join();
    tx2.join();

    for (const auto& ex : g_ex_vec) {
      try {
        if (ex != nullptr)
          std::rethrow_exception(ex);
      }
      catch (const std::exception& ex) {
        std::cout << "std::exception caught : " 
                  << ex.what() << '\n';
      }
    }

    // output ->
    //  std::exception caught : exception from func_2
    //  std::exception caught : exception from func_1
  }
*/

/*
                  ---------------------------
                  | std::make_exception_ptr |
                  ---------------------------
*/

/*
  #include <exception>  
  // std::make_exception_ptr, std::exception_ptr, 
  // std::current_exception

  #include <stdexcept>  // std::runtime_error

  template <typename E>
  std::exception_ptr Make_Exception_Ptr(E ex) noexcept
  {
    try {
      throw ex;
    }
    catch (...) {
      return std::current_exception();
    }
  }

  int main()
  {
    auto ep = Make_Exception_Ptr(std::runtime_error{ "error" });
  }
*/

/*
  #include <exception>  
  // std::rethrow_exception, std::make_exception_ptr
  // std::exception
  #include <stdexcept>  // std::runtime_error

  int main()
  {
    using namespace std;
    auto ep = make_exception_ptr(runtime_error{ "runtime error" });

    try {
      if (ep != nullptr)
        rethrow_exception(ep);
    }
    catch (const std::exception& ex) {
      cout << "exception caught : " << ex.what() << '\n';
    }
    // output -> exception caught : runtime error
  }
*/

/*
              ---------------------------------
              | std::nested_exception (C++11) |
              ---------------------------------
*/

/*
  - daha alt seviyeden fırlatılan bir hatayı üst seviyede
    yakalayıp, alt seviyedeki hatayı da sarmalayıp üst 
    seviye hatasını da üstüne ekleyerek tekrar fırlatma
    işlemi için kullanılır.

  - genel olarak bu sınıfı kullanan fonksiyonlar çağrılıyor.
    örneğin, std::throw_with_nested fonksiyonu.
*/

/*
  #include <exception>  
  #include <stdexcept>  // std::runtime_error
  #include <string>

  void low_level()
  {
    throw std::runtime_error{ "exception from low_level\n" };
  }

  void middle_level()
  {
    using namespace std;

    try {
      low_level();
    }
    catch (const std::exception& ex) {
      throw runtime_error{ 
        string{ "exception from middle_level\n" } + ex.what()};
    }
  }

  void top_level()
  {
    using namespace std;

    try {
      middle_level();
    }
    catch (const std::exception& ex) {
      throw runtime_error{ 
        string{ "exception from top_level\n" } + ex.what() };
    }
  }

  int main()
  {
    try{
      top_level();
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught : " << ex.what() << '\n';
    }

    // output -> 
    //  exception caught : exception from top_level
    //  exception from middle_level
    //  exception from low_level
  }
*/

/*
  #include <exception>  
  // std::throw_with_nested, std::rethrow_if_nested
  #include <stdexcept>  
  // std::runtime_error, std::length_error, std::out_of_range
  #include <string>

  void low_level()
  {
    throw std::runtime_error{ "exception from low_level\n" };
  }

  void middle_level()
  {
    using namespace std;

    try {
      low_level();
    }
    catch (const std::exception&) {
      std::throw_with_nested(
        std::length_error{ "exception from middle_level" });
    }
  }

  void top_level()
  {
    using namespace std;

    try {
      middle_level();
    }
    catch (const std::exception&) {
      std::throw_with_nested(
        std::out_of_range{ "exception from top_level" });
    }
  }

  void recursive_print_exception(const std::exception& ex)
  {
    std::cout << ex.what() << '\n';

    try {
      std::rethrow_if_nested(ex);
    }
    catch (const std::exception& ex) {
      std::cout << "nested exception : ";
      recursive_print_exception(ex);
    }
  }

  int main()
  {
    try {
      top_level();
    }
    catch (const std::exception& ex) {
      recursive_print_exception(ex);
    }

    // output -> 
    //  exception from top_level
    //  nested exception : exception from middle_level
    //  nested exception : exception from low_level
  }
*/