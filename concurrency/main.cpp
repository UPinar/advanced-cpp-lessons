#include <iostream>

/*
                          ===============
                          | Concurrency |
                          ===============
*/

/*
  concurrency : 
    Birden fazla işin aynı zaman diliminde gerçekleşmesi.
    Örneğin, tek çekirdekli bir işlemcinin belirli zaman dilimi 
    içinde iki işin kodlarını çalıştırması. Zaman paylaşımlı 
    olarak biraz ikinci işi, biraz birinci işi sonra tekrar iki 
    yine bir gibi..
  
  parallelism :
    Eş zamanlı olarak birden fazla işin gerçekleşmesi.
    Örneğin, çok çekirdekli bir işlemcinin veya birden 
    fazla işlemcinin farklı işleri çalıştırması.

  <--- check images/concurrency_vs_parallelism.png --->
  <--- check images/concurrent_parallelism.png --->
  <--- check images/concurrency_parallelism.png --->

  Imagine learning a new programming language by watching a video
  tutorial. You need to pause the video, apply what been said
  in code then continue watching. That's concurrency.

  Now you're a professional programmer and you enjoy listening
  to calm music while coding. That's parallelism.
*/

/*
                          ---------------
                          | std::thread |
                          ---------------
*/

/*
  #include <thread>

  int main()
  {
    std::cout << std::boolalpha;

    std::thread t1;   // default initialized thread object

    bool b1 = t1.joinable();
    // returns true, if the std::thread object 
    // identifies an active thread of execution
    // otherwise returns false

    std::cout << b1 << '\n';  // output -> false

    // -----------------------------------------------

    std::thread t2([]{});
    bool b2 = t2.joinable();

    std::cout << b2 << '\n';  // output -> true

    // -----------------------------------------------

    t2.join();
  }

  // Bir thread, henüz bir workload(iş yükü) almadığı için
  // non-joinable durumda olabilir ya da task'ini tamamladıktan
  // sonra hala hayatta olduğu için non-joinable durumda
  // olabilir.
*/

/*
          <--- check images/thread_constructors.png --->

  - has a noexcept default constructor
  - thread class is a move only class (copy members are deleted)
  - has a noexcept move constructor 
*/

/*
  // Eğer bir thread nesnesine bir iş yükü verip
  // "join" veya "detach" member functionlarından birini çağırmazsak
  // (thread nesnesi destroy edilirken joinable durumda ise)
  // hata oluşur, "std::terminate" çağrılır.

  #include <thread>

  int main()
  {
    auto fn = []{ std::cout << "hello from thread\n"; };

    std::thread tx{ fn };
    // output -> terminate called without an active exception
  }
*/

/*
  // Bir thread nesnesinin "join" fonksiyonu çağrıldığında
  // eğer thread'in iş yükü tamamlanmamışsa, 
  // çağıran thread(bu seneryoda main thread) 
  // iş yükünün tamamlanmasını bekler.

  // "join" fonksiyonu çağrılması, callable'ın(thread's workload)
  // çalışmasının bitmiş olduğu anlamına gelir. 

  // "join" fonksiyonu çağrıldıktan sonra, 
  // thread nesnesi non-joinable duruma geçer.

  #include <thread>

  int main()
  {
    auto fn = []{ std::cout << "hello from thread tx\n"; };

    std::thread tx{ fn };

    // ----> in this line 
    //  main thread + tx thread are running concurrently.

    tx.join();
    // output -> hello from thread tx
  }
*/

/*
  #include <vector>
  #include <thread>
  #include <syncstream>   // std::osyncstream

  int main()
  {
    using namespace std;

    // -----------------------------------------------
    std::cout << "[0] - main thread started\n";
    // -----------------------------------------------

    auto fn = [](int thread_id){ 
      osyncstream{ cout } << "thrad " << thread_id 
                          << " is running\n"; };

    // -----------------------------------------------

    vector<thread> t_vec;

    for (int i = 0; i < 5; ++i)
      t_vec.push_back(std::thread{ fn, i });

    // "std::thread{ fn, i }" is a PRValue expression
    // push_back(&&) overload will be called.

    for (auto& tx : t_vec)
      tx.join();
      
    // -----------------------------------------------
    std::cout << "[1] - main thread continues\n";
    // -----------------------------------------------

    vector<thread> t_vec2;

    for (int i = 5; i < 10; ++i)
      t_vec2.emplace_back(fn, i);


    for (auto& tx : t_vec2)
      tx.join();
    
    // -----------------------------------------------
    std::cout << "[2] - main thread finished\n";
    // -----------------------------------------------
  }

  // output ->
  //  [0] - main thread started
  //  thrad 0 is running
  //  thrad 3 is running
  //  thrad 1 is running
  //  thrad 2 is running
  //  thrad 4 is running
  //  [1] - main thread continues
  //  thrad 5 is running
  //  thrad 7 is running
  //  thrad 6 is running
  //  thrad 8 is running
  //  thrad 9 is running
  //  [2] - main thread finished
*/

/*
  #include <thread>
  #include <format>

  void foo(int x)
  {
    std::cout << "foo(int x), x = " << x << '\n';
  }

  void bar(int x, int y, int z)
  {
    using namespace std;
    cout << format("bar(int x, int y, int z)," 
                  " x = {}, y = {}, z = {}\n", x, y, z);
  }

  // Functor class(function object)
  class AClass {
  public:
    void operator()(int x) const
    {
      std::cout << "AClass::operator()(int x), x = " << x << '\n';
    }
  };

  class BClass {
  public:
    void func(int x) const
    {
      std::cout << "BClass::func(int x), x = " << x << '\n';
    }
  };

  class CClass {
  public:
    static void func(int x)
    {
      std::cout << "static CClass::func(int x), x = " << x << '\n';
    }
  };

  int main()
  {
    // -----------------------------------------------

    std::thread t1{ foo, 44 };
    t1.join();
    // output -> foo(int x), x = 44

    std::thread t2{ bar, 11, 22, 33 };
    t2.join();
    // output -> bar(int x, int y, int z), x = 11, y = 22, z = 33

    // -----------------------------------------------

    AClass a1;
    std::thread t3{ a1, 55 };
    t3.join();
    // output -> AClass::operator()(int x), x = 55

    // -----------------------------------------------

    BClass b1;
    std::thread t4{ &BClass::func, &b1, 66 };
    t4.join();
    // output -> BClass::func(int x), x = 66

    // -----------------------------------------------

    CClass c1;
    std::thread t5{ &CClass::func, 77 };
    t5.join();
    // output -> static CClass::func(int x), x = 77

    // -----------------------------------------------

    std::thread t6{ [](int x){ 
        std::cout << "[](int x), x = " << x << '\n'; }, 88 };
    t6.join();
    // output -> [](int x), x = 88
    
    // -----------------------------------------------
  }
*/

/*
  #include <list>
  #include <thread>
  #include <utility>  // std::move

  void func(std::list<int>&& list)
  {
    for (const auto ival : list)
      std::cout << ival << ' ';
    std::cout << '\n';
  }

  void foo(const std::list<int>& list)
  {
    for (const auto ival : list)
      std::cout << ival << ' ';
    std::cout << '\n';
  }


  int main()
  {
    // -----------------------------------------------

    std::list<int> list1{ 2, 4, 6, 8, 10, 12 };

    std::thread t1{ func, std::move(list1) };
    t1.join();
    // output -> 2 4 6 8 10 12

    std::cout << "list1.size() = " << list1.size() << '\n';
    // output -> list1.size() = 0
    // list1 is in moved-from state

    // -----------------------------------------------

    std::list<int> list2{ 1, 3, 5, 7, 9, 11 };

    std::thread t2{ foo, list2 };
    t2.join();
    // output -> 1 3 5 7 9 11

    // -----------------------------------------------
  }
*/

/*
  #include <list>
  #include <thread>

  void func(std::list<int>& list)
  {
    list.push_back(*list.begin());
    list.push_back(*list.begin() + 1);
    list.push_back(*list.begin() + 2);
  }

  int main()
  {
    std::list<int> list1{ 11, 22, 33, 44, 55 };

    std::thread tx{ func, list1 };
    tx.join();  // syntax error
    // error: static assertion failed: 
    // std::thread arguments must be invocable 
    // after conversion to rvalues
  }
*/

/*
  #include <list>
  #include <thread>
  #include <functional>   // std::ref

  void func(std::list<int>& list)
  {
    list.push_back(*list.begin());
    list.push_back(*list.begin() + 1);
    list.push_back(*list.begin() + 2);
  }

  int main()
  {
    std::list<int> list{ 11, 22, 33, 44, 55 };

    for (const auto ival : list)
      std::cout << ival << ' ';
    std::cout << '\n';
    // output -> 11 22 33 44 55

    std::thread tx{ func, std::ref(list) };

    tx.join();

    for (const auto ival : list)
      std::cout << ival << ' ';
    // output -> 11 22 33 44 55 11 12 13
  }
*/

/*
  #include <thread>
  #include <functional>   // std::ref
  #include <utility>      // std::move

  class Myclass {
  public:
    Myclass() = default;
    Myclass(const Myclass&)
    { 
      std::cout << "Myclass::Myclass(const Myclass&)\n"; 
    }
    Myclass(Myclass&&) noexcept
    { 
      std::cout << "Myclass::Myclass(Myclasss&&)\n"; 
    }
  };

  void foo(const Myclass& myclass){}

  void bar(Myclass&&){}

  int main()
  {
    Myclass m1;

    std::thread t1{ foo, m1 };
    t1.join();
    // output -> Myclass::Myclass(const Myclass&)
    // m1's copy is passed to foo function

    // -----------------------------------------------

    std::thread t2{ foo, std::ref(m1) };
    t2.join();
    // output -> (no output)
    // reference semantics is used

    // -----------------------------------------------

    std::thread t3{ bar, m1 };
    t3.join();
    // output -> Myclass::Myclass(const Myclass&)
    // m2's copy is passed to bar function

    // -----------------------------------------------

    std::thread t4{ bar, std::move(m1) };
    t4.join();
    // output -> Myclass::Myclass(Myclasss&&)

    // -----------------------------------------------
  }
*/

/*
  // when the thread's destructor is called,
  // if it is still joinable, std::terminate will be called.

  #include <exception>  // std::set_terminate
  #include <cstdlib>    // std::abort
  #include <thread>

  void foo()
  {
    std::cout << "foo() called\n";
  }

  void terminate_1()
  {
    std::cout << "terminate_1() called\n";
    std::abort();
  }

  int main()
  {
    std::cout << "[0] - main thread started\n";

    std::set_terminate(&terminate_1);

    {
      std::thread tx{ foo };
    }

    std::cout << "[1] - main thread finished\n";
  }

  // output ->
  //  [0] - main thread started
  //  terminate_1() called
*/

/*
  // calling "join" member function on a non-joinable thread
  // will throw an exception.

  #include <thread>

  int main()
  {
    std::thread tx;   
    // non-joinable thread(defult initialized)

    try {
      tx.join();
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught : " << ex.what() << '\n';
    }
    // output -> exception caught : Invalid argument
  }
*/

/*
  // calling "join" member function on a non-joinable thread
  // will throw an exception.

  #include <thread>

  void foo()
  {
    std::cout << "foo() called\n";
  }

  int main()
  {
    std::thread tx{ foo };
    tx.join();    // tx thread becomes non-joinable

    try {
      tx.join();
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught : " << ex.what() << '\n';
    }

    // output ->
    //  foo() called
    //  exception caught : Invalid argument
  }
*/

/*
  // calling "join" member function on a non-joinable thread
  // will throw an exception.

  #include <thread>

  void foo()
  {
    std::cout << "foo() called\n";
  }

  int main()
  {
    std::thread tx{ foo };
    tx.detach();    // tx thread becomes non-joinable

    try {
      tx.join();
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught : " << ex.what() << '\n';
    }

    // output ->
    //  exception caught : Invalid argument
    //  foo() called    ---> stack unwinding
  }
*/

/*
  #include <thread>
  #include <utility>  // std::move

  void foo()
  {
    std::cout << "foo() called\n";
  }

  int main()
  {
    std::thread t1{ foo };
    std::thread t2{ std::move(t1) };
    // t1 is moved to t2
    // t2 is now have foo function as its workload 
    // and t1 becomes non-joinable thread

    try {
      t1.join();
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught : " << ex.what() << '\n';
    }
    // output -> exception caught : Invalid argument

    t2.join();
    // output -> foo() called
  }
*/

/*
  #include <thread>
  #include <utility>  // std::move  

  void foo(){}

  int main()
  {
    // -------------------------------------------------------

    std::thread t1;
    std::thread t2{ foo };

    std::cout << "is t1 thread joinable ? " 
              << (t1.joinable() ? "yes" : "no") << '\n';
    // output -> is t1 thread joinable ? no

    std::cout << "is t2 thread joinable ? "
              << (t2.joinable() ? "yes" : "no") << '\n';
    // output -> is t2 thread joinable ? yes

    t2.join();

    // -------------------------------------------------------

    std::thread t3{ foo };
    std::thread t4;

    std::cout << "is t3 thread joinable ? "
              << (t3.joinable() ? "yes" : "no") << '\n';
    // output -> is t3 thread joinable ? yes

    std::cout << "is t4 thread joinable ? "
              << (t4.joinable() ? "yes" : "no") << '\n';
    // output -> is t4 thread joinable ? no

    t4 = std::move(t3);

    std::cout << "is t3 thread joinable ? "
              << (t3.joinable() ? "yes" : "no") << '\n';
    // output -> is t3 thread joinable ? no

    std::cout << "is t4 thread joinable ? "
              << (t4.joinable() ? "yes" : "no") << '\n';
    // output -> is t4 thread joinable ? yes

    t4.join();
    
    // -------------------------------------------------------
  }
*/

/*
  #include <thread>

  void foo()
  {
    for (int i = 0; i < 100; ++i)
      std::cout << '!';
  }

  int main()
  {
    std::cout << "[0] - main thread started\n";
    // --------------------------------------------------

    std::thread tx{ foo };

    for (size_t i = 0; i < 100; i++)
      std::cout << '*';

    // ----->
    // main and tx threads are running concurrently.

    tx.join();
    
    // --------------------------------------------------
    std::cout << "[1] - main thread finished\n";
  }

  // output ->
  //  [0] - main thread started
  //  ***********************!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //  !!!!!!!!!!!!!!!!!!!*********************************
  //  ***************************************
  //  *****[1] - main thread finished

  // '*' and '!' characters are written non-deterministically.
*/

/*
  #include <vector>
  #include <thread>

  void foo(char ch)
  {
    for (int i = 0; i < 100; ++i)
      std::cout << ch;
  }

  int main()
  {
    std::vector<std::thread> t_vec(26);

    for (size_t i = 0; i < t_vec.size(); ++i)
      t_vec[i] = std::thread{ foo, static_cast<char>(i + 'A') };

    for (auto& tx : t_vec)
      tx.join();
  }

  // output ->
  //  ICHGIIIEEELLNNNDDRRRRRRRRRRZZZZZZZZZZZZZZZZZZZZZZZ
  //  YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
  //  YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
  //  ZZZZZZZZZZZZZZZZZZZZZWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
  //  WWWWWWWWDDOOOOOUUUUUUUUUUUEEEEEEEEEEEEEELLLLLLLLLL
  //  AAAAACCNNIIIPVVVVRRRRRRRRRRRRRRRRRRRRRRIIIIVVCCCCC
  //  CCCXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXSS
  //  ... (non deterministic)
*/

/*
  - bütün threadler aynı process'e ait fakat 
    her bir thread'in kendi stack alanları vardır.

  - otomatik ömürlü değişkenler her bir thread'in 
    kendi stack alanında tutulur.
  - statik ömürlü değişkenler bütün threadler için aynıdır.
    ortak kullanılır. Örneğin std::cout nesni ortak kullanılır.
  - heapteki değişkenler ortak kullanılır.
  - fonksiyona referans semantiği ile geçilen değişkenler
    ortak kullanılır.

  - bütün threadler ortak kullanılan değişkenlere(shared variable)
    okuma amacıyla erişiyorlarsa bir problem olmaz.
  - threadlerden en az biri ortak kullanılan değişkene
    yazma amacıyla erişiyorsa, bu duruma race condition denir. 
    Race condition'a bağlı olarak data race problemi oluşursa
    (bir thread yazma amaçlı, diğer thread okuma amaçlı erişirse)
    tanımsız davranış(undefined behaviour) oluşur.

  - Yukarıdaki örnekte, std::cout global değişkenine 
    birden fazla thread yazma işlemi için erişiyor. 
    Bu durum race condition'dır.
    Fakat std::cout nesnesi thread-safe bir nesne olduğu için
    data race problemi, dolayısıyla tanımsız davranış oluşmaz.
*/

/*
                  ------------------------------
                  | std::this_thread namespace |
                  ------------------------------
*/

/*
                  -------------------------------
                  | std::this_thread::sleep_for |
                  -------------------------------
*/

/*
  #include <thread>
  #include <chrono>

  void foo(char ch)
  {
    using namespace std::literals;

    for (int i = 0; i < 100; ++i) {
      std::cout << ch;
      std::this_thread::sleep_for(60ms);
    }
  }

  int main()
  {
    std::vector<std::thread> t_vec(26);

    for (size_t i = 0; i < t_vec.size(); ++i)
      t_vec[i] = std::thread{ foo, static_cast<char>(i + 'A') };

    for (auto& tx : t_vec)
      tx.join();

    // output ->
    //  EGHDIJACLKMNBOPFQRSTUVWXYZZNVMUALHSBGPOFYETIDQJCR
    //  KWXDPFETQYROIKWXCLBJVZNMUASHGMAUYVNHJSGZBCLXITWOK
    //  RQDEPFUYNAMHSGZJTVBLXWIKORQEFCDPPUSMHYNABGLXZWKRT
    //  DQEFVICJOQMYSHXUEPWRNBCTLIAKDZGOVJFMYQJFPVEAXRULW
    //  DKCIGSBOZTNHGSNBHZOTKWLIREXFQADCMYUPJVNUPRYKWCELD
    //  XVAJMFZIGSQBHTOSBLRZIXWGYMKEJFCNHAPOQVTDULDQJUFZW
    //  ... (non deterministic)
  }
*/

/*
                  ----------------------------
                  | std::this_thread::get_id |
                  ----------------------------
*/

/*
  - thread sınıfının kendi "get_id" member function'ı ve 
    this_thread namespace'in "get_id" fonksiyonunun geri dönüş
    değeri bir tam sayı türü değil, "std::thread::id" türüdür.

  - "std::thread::id" sınıfı "std::thread" sınıfının 
    nested type'ıdır.
*/

/*
  #include <thread>       // std::this_thread::get_id
  #include <type_traits>  // std::is_same_v

  int main()
  {
    constexpr bool b1 = 
      std::is_same_v< decltype(std::this_thread::get_id()),
                      std::thread::id>;
    // b1 -> true
  }
*/

/*
  #include <thread>

  int main()
  {
    std::cout << std::boolalpha;

    std::thread t1([]{});
    std::thread t2([]{});

    std::thread::id id1 = t1.get_id();
    std::thread::id id2 = t2.get_id();

    t1.join();
    t2.join();

    std::cout << "id1 == id2 : " << (id1 == id2) << '\n';
    // output -> id1 == id2 : false
    std::cout << "id1 = " << id1 << '\n';   
    // output -> id1 = 2
    std::cout << "id2 = " << id2 << '\n';   
    // output -> id2 = 3
  }
*/

/*
  #include <vector>
  #include <thread>       // std::this_thread::get_id
  #include <syncstream>   // std::osyncstream

  void foo()
  {
    std::osyncstream{ std::cout } 
      << std::this_thread::get_id() << ' ';
  }

  int main()
  {
    std::vector<std::thread> t_vec(10);

    for (size_t i = 0; i < t_vec.size(); ++i)
      t_vec[i] = std::thread{ foo };

    for (auto& tx : t_vec)
      tx.join();

    // output ->  2 6 5 3 8 7 9 10 4 11
  }
*/

/*
  #include <thread>

  std::thread::id g_main_thread_id;

  void foo()
  {
    if (std::this_thread::get_id() == g_main_thread_id)
      std::cout << "foo() is called from main thread\n";
    else
      std::cout << "foo() is called from another thread\n";
  }

  int main()
  {
    g_main_thread_id = std::this_thread::get_id();

    std::thread tx{ foo };
    tx.join();
    // output -> foo() is called from another thread

    foo();
    // output -> foo() is called from main thread
  }
*/

/*
  #include <vector>
  #include <thread>   // std::this_thread::get_id
  #include <mutex>    // std::lock_guard

  std::mutex g_mtx;
  std::vector<std::thread::id> g_thread_id_vec;

  void foo()
  {
    std::lock_guard guard{ g_mtx };
    g_thread_id_vec.push_back(std::this_thread::get_id());
  }

  int main()
  {
    std::vector<std::thread> t_vec;

    for (size_t i = 0; i < 20; ++i)
      t_vec.emplace_back(foo);

    for (auto& tx : t_vec)
      tx.join();

    for (const auto& id : g_thread_id_vec)
      std::cout << id << ' ';
    // output -> 
    //  4 5 6 3 7 8 9 10 11 12 14 15 16 17 18 21 19 2 13 20
  }
*/

/*
                ---------------------------------
                | std::this_thread::sleep_until |
                ---------------------------------
*/

/*
  #include <thread>   // std::this_thread::sleep_until
  #include <chrono>   // std::chrono::system_clock
  #include <ratio>    // std::milli

  auto now()
  { 
    return std::chrono::steady_clock::now(); 
  }
  // returning std::chrono::steady_clock::time_point

  auto awake_time()
  {
    using std::chrono::operator""ms;
    return now() + 2000ms;
  }
  // returning std::chrono::steady_clock::time_point  

  int main()
  {
    using namespace std::chrono;
    std::cout << "[0] - main thread started\n" << std::flush;

    const auto start{ now() };
    std::this_thread::sleep_until(awake_time());

    duration<double, std::milli> elapsed{ now() - start };
    std::cout << "waited for " << elapsed << "\n";

    // output ->
    //  [0] - main thread started
    //  waited for 2010.87ms
  }
*/

/*
                    ---------------------------
                    | std::this_thread::yield |
                    ---------------------------
*/

/*
  #include <thread>   // std::this_thread::yield

  int main()
  {
    std::this_thread::yield();
    // bu threadin çalışması durup diğer threadlere öncelik verir.
  }
*/

/*
                    ------------------------
                    | exception in threads |
                    ------------------------
*/

/*
  #include <stdexcept>  // std::runtime_error
  #include <cstdlib>    // std::exit, EXIT_FAILURE
  #include <exception>  // std::set_terminate
  #include <thread>

  void foo(int x)
  {
    if (x % 5 == 0)
      throw std::runtime_error{ "division error from foo" };
  }

  void terminate_1()
  {
    std::cout << "terminate_1() called\n";
    std::exit(EXIT_FAILURE);
  }

  int main()
  {
    std::cout << "[0] - main thread started\n";

    std::set_terminate(&terminate_1);

    std::thread tx(foo, 10);
    tx.join();

    std::cout << "[1] - main thread finished\n";
  }

  // output ->
  //  [0] - main thread started
  //  terminate_1() called

  // because of the exception thrown from foo function,
  // std::terminate is called.
*/

/*
  #include <exception>  
  // std::exception_ptr, std::current_exception
  // std::rethrow_exception
  #include <stdexcept>  // std::invalid_argument
  #include <thread>

  std::exception_ptr g_eptr{ nullptr };

  void foo(int x)
  {
    std::cout << "foo(int x) started, x = " << x << '\n';

    try {
      if (x % 5 == 0)
        throw std::invalid_argument{ "invalid integer argument" };
    }
    catch (const std::exception&) {
      g_eptr = std::current_exception();
    }

    std::cout << "foo(int x) finished.\n";
  }

  int main()
  {
    std::thread tx{ foo, 10 };
    tx.join();

    try {
      if (g_eptr)
        std::rethrow_exception(g_eptr);
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught : " << ex.what() << '\n';
    }
    // output ->
    //  foo(int x) started, x = 10
    //  foo(int x) finished.
    //  exception caught : invalid integer argument
  }

  // an exception thrown and cathced in tx thread
  // than stored in g_eptr(global std::exception_ptr object)
  // that stored exception is rethrown and caught in main thread.
*/

/*
  #include <exception>  
  // std::exception_ptr, std::current_exception
  // std::rethrow_exception
  #include <stdexcept>  
  // std::runtime_error, std::out_of_range, std::invalid_argument
  #include <vector>
  #include <mutex>      // std::lock_guard
  #include <thread>

  std::vector<std::exception_ptr> g_eptr_vec; 
  std::mutex g_mtx;

  void func1()
  {
    throw std::runtime_error{ "runtime error from func1" };
  }

  void func2()
  {
    throw std::out_of_range{ "out of range error from func2" };
  }

  void func3()
  {
    throw std::invalid_argument{ "invalid argument from func3" };
  }

  void foo()
  {
    try {
      func1();
    }
    catch (...) {
      std::lock_guard guard{ g_mtx };
      g_eptr_vec.push_back(std::current_exception());
    }
  }

  void bar()
  {
    try {
      func2();
    }
    catch (...) {
      std::lock_guard guard{ g_mtx };
      g_eptr_vec.push_back(std::current_exception());
    }
  }

  void baz()
  {
    try {
      func3();
    }
    catch (...) {
      std::lock_guard guard{ g_mtx };
      g_eptr_vec.push_back(std::current_exception());
    }
  }

  int main()
  {
    std::thread t1{ foo };
    std::thread t2{ bar };
    std::thread t3{ baz };

    t1.join();
    t2.join();
    t3.join();

    for (const auto& eptr : g_eptr_vec) 
    {
      try {
        if (eptr)
          std::rethrow_exception(eptr); 
      }
      catch (const std::exception& ex) {
        std::cout << "exception caught : " << ex.what() << '\n';
      }
    }
    // output ->
    //  exception caught : out of range error from func2
    //  exception caught : runtime error from func1
    //  exception caught : invalid argument from func3
  }
*/

// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------

/*
  // dangling pointer

  #include <vector>
  #include <numeric>    // std::accumulate  
  #include <thread>

  void fn_load(const std::vector<int>* p_vec)
  {
    auto N = std::accumulate(p_vec->begin(), p_vec->end(), 0);
    std::cout << "N = " << N << '\n';
  }

  void foo()
  {
    std::vector<int> ivec(100'000, 12); // local vector object

    std::thread tx{ fn_load, &ivec };
    tx.detach();
  }

  int main()
  {
    using namespace std::literals;
    foo();
    std::this_thread::sleep_for(10s);
  }


  // foo fonksiyonu çağrıldığında, thread tx oluşturulur
  // ve workload olarak "fn_load" ve argüman olarak 
  // local "ivec" vector nesnesinin adresi gönderilir.

  // "tx" thread'inin workload'u olan "fn_load" fonksiyonu çalışırken,
  // "tx" thread'i detach edilir.
  // detach edilen "tx" threadinin workload'u 
  // arka planda çalışmaya devam eder ve 
  // bu esnada "foo" fonksiyonunun yerel değişkeni olan 
  // "ivec" nesnesini kullanır.

  // "tx" thread'i detach edildiği için, "foo" fonksiyonu
  // thread'in iş yükünün bitmesini beklemez ve sonlanır,
  // dolayısıyla yerel "ivec" nesnesinin ömrü sona erer.
  // Detach edilen thread'in iş yükü çalışmaya devam ederken
  // "ivec" nesnesi destroy edildiği için
  // ve iş yükünde bu nesneye erişildiği için
  // tanımsız davranış(undefined behaviour) oluşur.
*/

/*
  // dangling reference

  #include <vector>
  #include <numeric>    // std::accumulate  
  #include <thread>
  #include <functional> // std::ref

  void fn_load(const std::vector<int>& vec)
  {
    auto N = std::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "N = " << N << '\n';
  }

  void foo()
  {
    std::vector<int> ivec(100'000, 12); // local vector object

    std::thread tx{ fn_load, std::ref(ivec) };
    tx.detach();
  }

  int main()
  {
    using namespace std::literals;
    foo();
    std::this_thread::sleep_for(10s);
  }
*/

// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------

/*
                -----------------------------
                | move semantics in threads |
                -----------------------------
*/

/*
  #include <thread>
  #include <utility>  // std::move

  std::thread Make_Thread(int n)
  {
    auto fn = [n]()mutable {
      while (n--)
        std::cout << n << ' ';
    };

    return std::thread{ fn };
    // "std::thread{ fn }" is a PRValue expression
    // returning temporary object (Mandatory Copy Elision)
  }

  std::thread foo(std::thread tx_param)
  {
    return tx_param;
    // returning automatic storage duration object
    // move semantics will be applied.
  }

  // when foo is called with RValue or PRValue expression
  // Mandatory Copy Elision will occur.

  int main()
  {
    std::thread t1 = Make_Thread(10);

    auto t2 = std::move(t1);  
    // move semantics will be applied.

    auto t3 = foo(std::move(t2));
    // move semantics will be applied.

    t3.join();
    // output -> 9 8 7 6 5 4 3 2 1 0
  }
*/

// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------

/*
  #include <thread>

  int main()
  {
    std::cout << "hardware concurrency : " 
              << std::thread::hardware_concurrency() << '\n';
    // output -> 12

    // "std::thread::hardware_concurrency" is 
    // static inline member function of "std::thread" class.
  }
*/

// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------
// ---------------------------------------------------------

/*
                ------------------------------
                | thread local storage class |
                ------------------------------
*/

/*
  - global değişkenler, bütün threadlerin erişebileceği 
    statik ömürlü değişkenlerdir.
    (static storage duration)

  - thread local değişkenler, sadece tanımlandıkları 
    thread içerisinde erişilebilir ve threadin çalışması 
    bitene kadar kullanılabilirler.
    (thread storage duration)
  - thread local değişkenler için senkronizasyon 
    işlemlerine gerek duyulmaz.

  - "thread_local" bir keyword.
*/

/*
  #include <syncstream>   // std::osyncstream
  #include <string>
  #include <thread>

  thread_local int t_ival{ 0 };

  void func(const std::string& id)
  {
    t_ival += 10;

    std::osyncstream{ std::cout } 
      << "in thread " << id << ", t_ival = " << t_ival << '\n';
  }

  int main()
  {
    std::cout << "in main thread, t_ival = " << t_ival << '\n';

    std::thread t1{ func, "t1" };
    std::thread t2{ func, "t2" };

    t1.join();
    t2.join();

    std::cout << "in main thread, t_ival = " << t_ival << '\n';

    // output ->
    //  in main thread, t_ival = 0
    //  in thread t2, t_ival = 10
    //  in thread t1, t_ival = 10
    //  in main thread, t_ival = 0
  }
*/

/*
  #include <thread>

  thread_local int t_ival{ 0 };

  void func(int* p)
  {
    *p = 777;
    std::cout << "*p = " << *p << '\n';
    std::cout << "t_ival = " << t_ival << '\n';
  }

  int main()
  {
    std::cout << "main thread t_ival = " << t_ival << '\n';
    t_ival = 333;

    std::thread tx{ func, &t_ival };
    tx.join();

    std::cout << "main thread t_ival = " << t_ival << '\n';

    // output ->
    //  main thread t_ival = 0
    //  *p = 777            ----> main thread's t_ival
    //  t_ival = 0          ----> tx thread's t_ival
    //  main thread t_ival = 777
  }

  // sending main thread's thread local variable's address 
  // to "func" function as argument.
*/