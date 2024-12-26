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

  - thread local değişkenler namespace scope ya da block scope
    içerisinde tanımlanabilirler. 
    namespace scope içerisinde tanımlanan thread local değişkenler
    threadin ömrü boyunca yaşarlar.
    block scope içerisinde tanımlanan thread local değişkenler
    fonksiyonun çalışma süresince yaşarlar.
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

/*
  #include <thread>
  #include <string>
  #include <syncstream>   // std::osyncstream

  thread_local int gt_ival{ 0 };

  void func(const std::string& thread_name)
  {
    ++gt_ival;  
    // no need for synchronization 
    // when thread_local variable is used.

    std::osyncstream{ std::cout }
      << "gt_ival in thread " << thread_name 
      << " = " << gt_ival << '\n';

    // synchronization for std::cout object will be done 
    // with std::osyncstream class.
  }

  int main()
  {
    std::thread t1{ func, "t1" };
    std::thread t2{ func, "t2" };
    std::thread t3{ func, "t3" };
    std::thread t4{ func, "t4" };

    {
      std::osyncstream{ std::cout }
        << "gt_ival in main thread = " << gt_ival << '\n';
    }

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    {
      std::osyncstream{ std::cout }
        << "gt_ival in main thread = " << gt_ival << '\n';
    }

    // output ->
    //  gt_ival in main thread = 0
    //  gt_ival in thread t1 = 1
    //  gt_ival in thread t4 = 1
    //  gt_ival in thread t2 = 1
    //  gt_ival in thread t3 = 1
    //  gt_ival in main thread = 0
  }
*/

/*
  #include <thread>
  #include <string>
  #include <mutex>  // std::lock_guard, std::mutex

  int g_ival{ 0 };
  std::mutex g_mtx;

  void func(const std::string& thread_name)
  {
    std::lock_guard guard{ g_mtx };

    ++g_ival;  

    std::cout << "g_ival in thread " << thread_name 
      << " = " << g_ival << '\n';
  }

  int main()
  {
    std::thread t1{ func, "t1" };
    std::thread t2{ func, "t2" };
    std::thread t3{ func, "t3" };
    std::thread t4{ func, "t4" };

    {
      std::lock_guard guard{ g_mtx };
      std::cout << "g_ival in main thread = " << g_ival << '\n';
    }

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    {
      std::lock_guard guard{ g_mtx };
      std::cout << "g_ival in main thread = " << g_ival << '\n';
    }

    // output ->
    //  g_ival in main thread = 0
    //  g_ival in thread t3 = 1
    //  g_ival in thread t1 = 2
    //  g_ival in thread t4 = 3
    //  g_ival in thread t2 = 4
    //  g_ival in main thread = 4
  }
*/

/*
  - lock sınıfları RAII idiom'unu implemente ederler.
  - constructor'larında bir mutex edinip, lock ederler.
  - destructor'larında ise mutex'i unlock ederler.
  - bir exception gönderilirse exception yakalandığında 
    stack unwinding sürecinde destructor çağrılır ve
    mutex unlock edilir.
*/

/*
  #include <mutex> 
  #include <thread>

  std::mutex g_mtx;

  void foo()
  {
    // ---------------------------------------------------
    int ival = 0;           
    // local variable (automatic storage duration)
    // no need for synchronization
    // all threads have their own stack space.

    static int s_ival = 0;  
    // static local variable (static storage duration)
    // need to be synchronized
    // shared among all threads (shared variable)

    thread_local int t_ival = 0;
    // thread local variable in block scope (thread storage duration)
    // no need for synchronization

    // ---------------------------------------------------

    ++ival;
    ++t_ival;

    
    g_mtx.lock();     // -----> critical section starts 

    ++s_ival;
    std::cout << "thread id = " << std::this_thread::get_id() 
              << "\nautomatic storage ival = " << ival 
              << "\nthread storage t_ival  = " << t_ival
              << "\nstatic storage s_ival  = " << s_ival << "\n\n";

    g_mtx.unlock();   // -----> critical section ends
    
    // ++s_ival and std::cout objects are in a critical section
    // because they both are shared among all threads.
    
    // when a thread enters to a critical section,
    // it will lock the mutex object,
    // so other threads can not enter the critical section
    // before the mutex is unlocked.
    // this is called mutual exclusion.
    // if unlock is not called, the mutex will be locked forever,
    // which is called deadlock.

    // after the thread is done with the critical section,
    // it will unlock the mutex and changes that has been done 
    // will be published, so another thread 
    // will be aware of that changes and 
    // can acquire the lock and enter the critical section.

    // ---------------------------------------------------
  }

  int main()
  {
    std::thread t1{ foo };
    std::thread t2{ foo };
    std::thread t3{ foo };
    std::thread t4{ foo };

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // output ->
    //  thread id = 4
    //  automatic storage ival = 1
    //  thread storage t_ival  = 1
    //  static storage s_ival  = 1
    //  
    //  thread id = 5
    //  automatic storage ival = 1
    //  thread storage t_ival  = 1
    //  static storage s_ival  = 2
    //  
    //  thread id = 3
    //  automatic storage ival = 1
    //  thread storage t_ival  = 1
    //  static storage s_ival  = 3
    //  
    //  thread id = 2
    //  automatic storage ival = 1
    //  thread storage t_ival  = 1
    //  static storage s_ival  = 4
  }
*/

/*
  #include <thread>
  #include <string>
  #include <syncstream>   // std::osyncstream
  #include <vector>

  thread_local std::string g_t_str{ "Hello" };

  void func(const std::string& str)
  {
    g_t_str += str;

    std::osyncstream o_cout{ std::cout };
    o_cout  << "thread id = " << std::this_thread::get_id() 
            << ", g_t_str = " << g_t_str << '\n';

    o_cout << "address of g_t_str = " << &g_t_str << "\n\n";

  }

  int main()
  {
    const char* const str_arr[] = { "World", "Galaxy", "Universe" };

    std::vector<std::thread> t_vec;

    for (auto str : str_arr)
      t_vec.emplace_back(func, str);

    for (auto& tx : t_vec)
      tx.join();
    // output ->
    //  thread id = 3, g_t_str = HelloGalaxy
    //  address of g_t_str = 0x299837aa558
    //  
    //  thread id = 2, g_t_str = HelloWorld
    //  address of g_t_str = 0x299837aa708
    //  
    //  thread id = 4, g_t_str = HelloUniverse
    //  address of g_t_str = 0x299837aa0d8
  }
*/

/*
  #include <thread>
  #include <string>
  #include <syncstream>   // std::osyncstream
  #include <vector>

  std::string g_str{ "Hello" };   // global shared variable
  std::mutex mtx;

  void func(const std::string& str)
  {
    std::lock_guard guard{ mtx };

    g_str += str;

    std::cout << "thread id = " << std::this_thread::get_id() 
              << ", g_str = " << g_str << '\n';

    std::cout << "address of g_str = " << &g_str << "\n\n";
  }

  int main()
  {
    const char* const str_arr[] = { "World", "Galaxy", "Universe" };

    std::vector<std::thread> t_vec;

    for (auto str : str_arr)
      t_vec.emplace_back(func, str);

    for (auto& tx : t_vec)
      tx.join();
    // output ->
    //  thread id = 4, g_str = HelloUniverse
    //  address of g_str = 0x7ff61f72a020
    //  
    //  thread id = 3, g_str = HelloUniverseGalaxy
    //  address of g_str = 0x7ff61f72a020
    //  
    //  thread id = 2, g_str = HelloUniverseGalaxyWorld
    //  address of g_str = 0x7ff61f72a020
  }
*/

/*
  #include <syncstream>   // std::osyncstream
  #include <thread>       // std::this_thread::sleep_for
  #include <chrono>

  class Myclass {
  public:
    Myclass() 
    { 
      std::osyncstream{ std::cout } 
        << "Myclass::Myclass(), this = " << this << '\n';
    }

    ~Myclass() 
    { 
      std::osyncstream{ std::cout } 
        << "Myclass::~Myclass(), this = " << this << '\n';
    }
  };

  void foo()
  {
    std::osyncstream{ std::cout } 
      << "foo() started\n";

    thread_local Myclass t_m1;  

    std::osyncstream{ std::cout } 
      << "foo() finished\n";
  }

  void bar()
  {
    using namespace std::chrono_literals;

    std::osyncstream{ std::cout } 
      << "bar() started\n";

    foo();
    std::this_thread::sleep_for(2s);

    std::osyncstream{ std::cout } 
      << "bar() finished\n";
  }

  int main()
  {
    std::cout << "[0] - main thread started\n";

    std::thread tx{ bar };
    tx.join();

    std::cout << "[1] - main thread finished\n";
  }

  // output ->
  //  [0] - main thread started
  //  bar() started
  //  foo() started
  //  Myclass::Myclass(), this = 0x144d4f90ba8
  //  foo() finished
  //  bar() finished
  //  Myclass::~Myclass(), this = 0x144d4f90ba8
  //  [1] - main thread finished
*/

/*
  #include <syncstream>   // std::osyncstream
  #include <thread>

  class Myclass {
  public:
    Myclass() 
    { 
      std::osyncstream{ std::cout } 
        << "Myclass::Myclass(), this = " << this << '\n';
    }

    ~Myclass() 
    { 
      std::osyncstream{ std::cout } 
        << "Myclass::~Myclass(), this = " << this << '\n';
    }
  };

  void foo()
  {
    std::osyncstream{ std::cout } 
      << "foo() started\n";

    thread_local Myclass t_m1;  

    std::osyncstream{ std::cout } 
      << "foo() finished\n";
  }

  void bar()
  {
    using namespace std::chrono_literals;

    std::osyncstream{ std::cout } 
      << "bar() started\n";

    foo();

    std::osyncstream{ std::cout } 
      << "bar() finished\n";
  }

  int main()
  {
    std::cout << "[0] - main thread started\n";
    
    std::thread t1{ bar };
    std::thread t2{ bar };
    std::thread t3{ bar };

    t1.join();
    t2.join();
    t3.join();

    std::cout << "[1] - main thread finished\n";
  }

  // output ->
  //  [0] - main thread started
  //  bar() started
  //  bar() started
  //  foo() started
  //  bar() started
  //  foo() started
  //  Myclass::Myclass(), this = 0x21c48387c08    : (1)
  //  foo() started
  //  Myclass::Myclass(), this = 0x21c48388048    : (2)
  //  foo() finished
  //  bar() finished
  //  foo() finished
  //  bar() finished
  //  Myclass::~Myclass(), this = 0x21c48388048   : (~1)
  //  Myclass::Myclass(), this = 0x21c48387be8    : (3)
  //  foo() finished
  //  bar() finished
  //  Myclass::~Myclass(), this = 0x21c48387c08   : (~2)
  //  Myclass::~Myclass(), this = 0x21c48387be8   : (~3)
  //  [1] - main thread finished
*/

/*
  #include <mutex>
  #include <thread>
  #include <vector>

  int g_ival;       // global shared variable
  std::mutex g_mtx;

  void func(char ch)
  {
    std::lock_guard guard{ g_mtx };

    ++g_ival;
    std::cout << "ch = " <<ch << " - " << g_ival << '\n';
  }

  int main()
  {
    std::vector<std::thread> t_vec;

    for (char ch = 'a'; ch <= 'z'; ++ch)
      t_vec.emplace_back(func, ch);

    for (auto& tx : t_vec)
      tx.join();

    // output ->
    //  ch = a - 1
    //  ch = c - 2
    //  ch = d - 3
    //  ch = e - 4
    //  ch = b - 5
    //  ch = r - 6
    //  ch = g - 7
    //  ch = h - 8
    //  ch = i - 9
    //  ch = j - 10
    //  ch = k - 11
    //  ch = l - 12
    //  ch = m - 13
    //  ch = n - 14
    //  ch = o - 15
    //  ch = p - 16
    //  ch = q - 17
    //  ch = f - 18
    //  ch = t - 19
    //  ch = u - 20
    //  ch = v - 21
    //  ch = x - 22
    //  ch = w - 23
    //  ch = y - 24
    //  ch = z - 25
    //  ch = s - 26

    // threads(sent characters) are executed 
    // in a non-deterministic order.
  }
*/

/*
  #include <mutex>
  #include <thread>
  #include <vector>

  thread_local int g_t_ival{};     
  // thread local variable in global namespace scope
  std::mutex g_mtx;

  void func(char ch)
  {
    ++g_t_ival;

    std::lock_guard guard{ g_mtx };
    std::cout << "ch = " <<ch << " - " << g_t_ival << '\n';
  }

  int main()
  {
    std::vector<std::thread> t_vec;

    for (char ch = 'a'; ch <= 'z'; ++ch)
      t_vec.emplace_back(func, ch);

    for (auto& tx : t_vec)
      tx.join();

    // output ->
    //  ch = a - 1
    //  ch = c - 1
    //  ch = l - 1
    //  ch = f - 1
    //  ch = e - 1
    //  ch = g - 1
    //  ch = h - 1
    //  ch = i - 1
    //  ch = d - 1
    //  ch = j - 1
    //  ch = k - 1
    //  ch = b - 1
    //  ch = m - 1
    //  ch = n - 1
    //  ch = o - 1
    //  ch = p - 1
    //  ch = q - 1
    //  ch = r - 1
    //  ch = s - 1
    //  ch = t - 1
    //  ch = u - 1
    //  ch = v - 1
    //  ch = w - 1
    //  ch = y - 1
    //  ch = x - 1
    //  ch = z - 1

    // threads(sent characters) are executed 
    // in a non-deterministic order.
  }
*/

/*
  #include <random>
  // std::uniform_int_distribution, std::mt19937 
  #include <thread>


  thread_local std::mt19937 g_t_eng{ 1928394854u };
  // thread local variable in global namespace scope

  void func()
  {
    std::uniform_int_distribution dist{ 100, 999 };

    for (int i = 0; i < 10; ++i) 
      std::cout << dist(g_t_eng) << ' ';
    std::cout << '\n';
  }

  int main()
  {
    std::thread t1{ func };
    t1.join();
    // output -> 994 125 359 471 607 267 861 524 161 790

    std::thread t2{ func };
    t2.join();
    // output -> 994 125 359 471 607 267 861 524 161 790
  }
*/

/*
  #include <random>
  // std::uniform_int_distribution, std::mt19937 
  #include <thread>
  #include <syncstream>   // std::osyncstream


  thread_local std::mt19937 g_t_eng{ 1928394854u };
  // thread local variable in global namespace scope

  void func()
  {
    std::uniform_int_distribution dist{ 100, 999 };
    // local variable, no need for synchronization

    std::osyncstream o_cout{ std::cout };

    for (int i = 0; i < 10; ++i) 
      o_cout << dist(g_t_eng) << ' ';
    o_cout << '\n';
  }

  int main()
  {
    std::thread t1{ func };
    std::thread t2{ func };
    std::thread t3{ func };

    t1.join();
    t2.join();
    t3.join();

    // output ->
    //  994 125 359 471 607 267 861 524 161 790
    //  994 125 359 471 607 267 861 524 161 790
    //  994 125 359 471 607 267 861 524 161 790
  }
*/

/*
  #include <random>
  // std::uniform_int_distribution, std::mt19937 
  #include <thread>
  #include <mutex>

  std::mt19937 g_t_eng{ 1928394854u };  // shared global variable 
  std::mutex g_mtx;

  void func()
  {
    std::uniform_int_distribution dist{ 100, 999 };
    // local variable, no need for synchronization

    std::lock_guard guard{ g_mtx };

    for (int i = 0; i < 10; ++i) 
      std::cout << dist(g_t_eng) << ' ';
    std::cout  << '\n';
  }

  int main()
  {
    std::thread t1{ func };
    std::thread t2{ func };
    std::thread t3{ func };

    t1.join();
    t2.join();
    t3.join();

    // output ->
    //  994 125 359 471 607 267 861 524 161 790
    //  358 728 381 593 210 693 916 181 245 281
    //  174 447 864 551 163 846 497 229 970 331
  }
*/

/*
                      ----------------------
                      | std::jthread class |
                      ----------------------
*/

/*
  #include <thread>   // std::jthread

  void func(){}

  int main()
  {
    std::cout << "[0] - main thread started\n";

    {
      std::jthread jt1{ func };
    }

    std::cout << "[1] - main thread finished\n";
  }
  // output ->
  //  [0] - main thread started
  //  [1] - main thread finished

  // no need to call join or detach member functions
  // for std::jthread object.
*/

/*
  #include <thread>
  #include <stdexcept>  // std::runtime_error

  void func(){}

  int main()
  {
    std::cout << "[0] - main thread started\n";

    {
      try {
        std::thread t1{ func };
        throw std::runtime_error{ "__error__" };
        t1.join();
      }
      catch (const std::exception& ex) {
        std::cout << "exception caught : " << ex.what() << '\n';
      }
    }

    std::cout << "[1] - main thread finished\n";
  }

  // output ->
  //  [0] - main thread started
  //  terminate called without an active exception

  // because of the exception thrown before calling join
  // member function of the std::thread object,
  // join member function can not be called and
  // std::terminate is called.
*/

/*
  #include <thread>     // std::jthread
  #include <stdexcept>  // std::runtime_error

  void func(){}

  int main()
  {
    std::cout << "[0] - main thread started\n";

    {
      try {
        std::jthread jt1{ func };
        throw std::runtime_error{ "__error__" };
      }
      catch (const std::exception& ex) {
        std::cout << "exception caught : " << ex.what() << '\n';
      }
    }

    std::cout << "[1] - main thread finished\n";
  }

  // output ->
  //  [0] - main thread started
  //  exception caught : __error__
  //  [1] - main thread finished

  // in stack unwinding process, destructor of the std::jthread
  // will call join member function wrapped std::thread object
*/

/*
  // Concurrency in Action book jthread implementation 

  #include <thread>
  #include <utility>  // std::forward, std::move

  class JThread {
  private:
    std::thread m_thread;

  public:
    JThread() noexcept = default;

    template <typename Callable, typename... Args>
    explicit JThread(Callable&& func, Args&&... args)
      : m_thread{ std::forward<Callable>(func), 
                  std::forward<Args>(args)... } 
    {}

    explicit JThread(std::thread other_thread) noexcept
      : m_thread{ std::move(other_thread) } 
    {}

    // move constructor
    JThread(JThread&& other_JThread) noexcept
      : m_thread{ std::move(other_JThread.m_thread) } 
    {}

    // move assignment operator
    JThread& operator=(JThread&& other_JThread) noexcept
    {
      if (joinable())
        join();

      m_thread = std::move(other_JThread.m_thread);
      return *this;
    }

    JThread& operator=(std::thread other_thread) noexcept
    {
      if (joinable())
        join();

      m_thread = std::move(other_thread);
      return *this;
    }

    ~JThread() noexcept
    {
      if (joinable())
        join();
    }

    void swap(JThread& other_JThread) noexcept
    {
      m_thread.swap(other_JThread.m_thread);
    }

    std::thread::id get_id() const noexcept
    {
      return m_thread.get_id();
    }

    bool joinable() const noexcept
    {
      return m_thread.joinable();
    }

    void join()
    {
      m_thread.join();
    }

    void detach()
    {
      m_thread.detach();
    }

    std::thread& as_thread() noexcept
    {
      return m_thread;
    }

    const std::thread& as_thread() const noexcept
    {
      return m_thread;
    }
  };
*/

/*
  #include <thread>
  #include <stop_token>   // std::stop_source, std::stop_token
  #include <chrono>
  #include <syncstream>   // std::osyncstream

  int main()
  {
    using namespace std::literals;

    std::stop_source st_src;

    std::thread t1{ [s_token = st_src.get_token()](){
        while (!s_token.stop_requested()) 
        {
          std::cout.put('*');
          std::this_thread::sleep_for(100ms);
        }
      }
    };

    std::thread t2{ [s_token = st_src.get_token()](){
        while (!s_token.stop_requested()) 
        {
          std::cout.put('_');
          std::this_thread::sleep_for(100ms);
        }
      }
    };

    std::thread t3{ [s_token = st_src.get_token()](){
        while (!s_token.stop_requested()) 
        {
          std::cout.put('!');
          std::this_thread::sleep_for(100ms);
        }
      }
    };

    std::this_thread::sleep_for(2s);

    std::osyncstream{ std::cout } 
      << "\nsending stop request to thread\n";

    st_src.request_stop();

    std::cout << "thread stopped\n";

    t1.join();
    t2.join();
    t3.join();
  }
  // output ->
  //  *!_!*_*!__*!_*!*_!!*_*!_*!_!_**!__!*_!*_*!_!**!_*_!*_!*_!
  //  sending stop request to thread
  //  thread stopped
*/

/*
  std::jthread class have those member functions 
  which are not available in std::thread class.
  - request_stop
  - get_stop_source
  - get_stop_token

  if std::jthread's workload's first parameter's type is 
  std::stop_token;
  std::jthread will create a std::stop_source object
  gets a std::stop_token object from that std::stop_source object
  with "get_token" member function and passes 
  that std::stop_token object to its workload as an argument.
*/

/*
  #include <thread>
  #include <chrono>
  #include <stop_token>   // std::stop_token
  #include <syncstream>   // std::osyncstream

  int main()
  {
    using namespace std::literals;

    std::jthread jt1{
      [](std::stop_token s_token){
        while (!s_token.stop_requested()) 
        {
          std::cout.put('*');
          std::this_thread::sleep_for(100ms);
        }
      }
    };

    std::jthread jt2{
      [](std::stop_token s_token){
        while (!s_token.stop_requested()) 
        {
          std::cout.put('_');
          std::this_thread::sleep_for(100ms);
        }
      }
    };

    std::jthread jt3{
      [](std::stop_token s_token){
        while (!s_token.stop_requested()) 
        {
          std::cout.put('!');
          std::this_thread::sleep_for(100ms);
        }
      }
    };

    std::this_thread::sleep_for(2s);

    std::osyncstream{ std::cout } 
      << "\nsending stop request to thread\n";

    jt1.request_stop();
    jt2.request_stop();
    jt3.request_stop();

    std::cout << "thread stopped\n";
  }
  // output ->
  //  *!_!_*_!**!__*!_*!_*!_!*!*_!*_!_*_*!_!*!*__!*_!*!_**_!_*!
  //  sending stop request to thread
  //  thread stopped
*/

/*
                    ---------------------------
                    | mutual exclusion(mutex) |
                    ---------------------------
*/

/*
  threadler genel olarak paylaşılan nesneleri kullanıyorlar.

  bütün threadler, paylaşılan değişkene okuma amaçlı(read-only)
  erişiyorlar ise bu durumda bir sorun oluşmaz ve senkronizasyon
  mekanizmasına ihtiyaç duyulmaz.

  en az 1 thread yazma amaçlı kullanıyorsa, data race problemi
  oluşabilir.

  örneğin bir thread, paylaşılan bir değişkene yazma işlemi 
  gerçekleştiriyor ve işlem bitmeden başka bir thread 
  bu değişkeni okumaya çalışıyor.
  Değişken half written state'te bulunduğu için okuma işlemi
  tanımsız davranış(undefined behavior) oluşurturur.

  farklı threadlerdeki akışlar deterministik değil.
  threadlerin tasklarının öncelik sırası program için 
  bir fark yaratmıyor ise, race condition benign(iyi huylu)
*/

/*
  int g_x = 55;

  void print_abs(int val){}

  void bar()
  {
    if (g_x > 0)
      print_abs(g_x);
    else
      print_abs(-g_x);
  }

  - bir thread bar fonksiyonunda "g_x" değişkenin okuyup 
    değişkenin negatif olduğunu anlayıp "print_abs" 
    fonksiyonuna -g_x değerini gönderiyor.
    
  - "print_abs" fonksiyonun kodu koşmaya başlamadan önce 
    başka bir thread ortak kullanılan "g_x" değişkeninin 
    değerini pozitif bir değer yapıyor. 
    dolayısıyla bir önceki threadin gönderdiği -g_x değeri
    fonksiyonun kodu koşmaya başladığında yanlış bir değer oluyor.

  - veya işletim sistemi ve değişkenin türüne bağlı olarak 
    yazma işlemi henüz bitmemiş ve ilk thread'den çağrılan 
    "print_abs" fonksiyonunda "g_x" değişkeni okunmaya 
    çalışıldığında half written state'te bulunan bir 
    değişken okunmaya çalışılmış olur(tanımsız davranış).  
*/

/*
  #include <vector>

  std::vector<int> g_ivec;

  void foo()
  {
    if (!g_ivec.empty())
      auto val = g_ivec.front();
  }

  // calling "front" member function of an empty vector
  // is undefined behavior(UB).

  - g_ivec paylaşımlı bir değişken ve 1 elemana sahipken 
    ilk thread "foo" fonksiyonunu çağırıyor if döngüsünde
    g_ivec'in boş olmadığı anlaşılıyor. 
    Tam bu esnada başka bir thread g_ivec'in elemanını
    siliyor ve g_ivec boşalıyor.
    Bu durumda ilk thread "front" member function'u 
    çağrıldığında tanımsız davranış(UB) oluşur.
*/

/*
  critical section : 
    öyle bir kod bloğu var ki bu bloğu birden fazla thread'in
    aynı anda koşmasını istemiyoruz.
  
  mutual exclusion :
    threadlerin bir mutex'i birlikte(mutual) olarak kullanması
    ve bir thread critical section'a girdiğinde diğer threadlerin
    hep birlikte beklemesi.

  mutex : 
    iki state'e sahip olabilir.
    - locked    (kilit bir thread tarafında edinilmiş(acquired).)
    - unlocked  (kilit serbest bırakılmış(released).)
*/

/*
  #include <mutex>

  std::mutex g_mtx;

  void foo()
  {
    g_mtx.lock();
    // critical section
    // critical section
    // critical section
    g_mtx.unlock();
  }
*/

/*
  #include <vector>
  #include <thread>

  long long int g_llval{};

  void foo()
  {
    for(int i = 0; i < 1'000'000; ++i)
      ++g_llval;  
  }

  int main()
  {
    std::vector<std::thread> t_vec;

    for (int i = 0; i < 10; ++i)
      t_vec.emplace_back(foo);

    for (auto& tx : t_vec)
      tx.join();

    std::cout << "g_llval = " << g_llval << '\n';
    // output -> g_llval = 1580276

    // normally g_llval should be 10'000'000, it is 1'580'276
  }
*/

/*
  #include <vector>
  #include <thread>
  #include <mutex>

  long long int g_llval{};
  std::mutex g_mtx;

  void foo()
  {
    for(int i = 0; i < 1'000'000; ++i) 
    {
      g_mtx.lock();
      ++g_llval;        // critical section
      g_mtx.unlock();
    }
  }

  int main()
  {
    std::vector<std::thread> t_vec;

    for (int i = 0; i < 10; ++i)
      t_vec.emplace_back(foo);

    for (auto& tx : t_vec)
      tx.join();

    std::cout << "g_llval = " << g_llval << '\n';
    // output -> g_llval = 10000000
  }
*/

/*
  - external synchronization : 
    örneğin standart kütüphanenin container'ları thread-safe değil.
    ve bu container'ları kullanırken external synchronization
    mekanizmaları kullanmamız gerekiyor.

  - internal synchronization :
    örneğin bir sınıf yazıyoruz ve sınıfın kendi içinde thread-safe
    olmasını sağlıyoruz buna internal synchronization denir.
*/

/*
  #include <vector>
  #include <string>
  #include <mutex>

  std::vector<std::string> g_svec;
  std::mutex g_mtx;

  void foo()
  {
    g_mtx.lock();
    g_svec.push_back("Hello");  // critical section
    g_mtx.unlock();
  }
  // external synchronization
*/

/*
  #include <list>
  #include <mutex>

  class InternalSync_List {
  private:
    std::list<int> m_list;
    std::mutex m_mtx;

  public:
    // non const member function
    void push_back(int val)   
    {
      m_mtx.lock();
      m_list.push_back(val);  // critical section
      m_mtx.unlock();
    } 

    // const member function
    void print() const
    {
      m_mtx.lock();   // syntax error
      // error: passing 'const std::mutex' as 'this' argument 
      // discards qualifiers

      for (const auto val : m_list)   // critical section
        std::cout << val << ' ';      // critical section

      m_mtx.unlock(); // syntax error
      // error: passing 'const std::mutex' as 'this' argument 
      // discards qualifiers
    }
  };
*/

/*
  #include <list>
  #include <mutex>

  class InternalSync_List {
  private:
    std::list<int> m_list;
    mutable std::mutex m_mtx;

  public:
    // non const member function
    void push_back(int val)   
    {
      m_mtx.lock();
      m_list.push_back(val);  // critical section
      m_mtx.unlock();
    } 

    // const member function
    void print() const
    {
      m_mtx.lock();  

      for (const auto val : m_list)   // critical section
        std::cout << val << ' ';      // critical section

      m_mtx.unlock();
    }

    // m_list is not thread-safe and std::cout object
    // is global shared variable, 
    // so they are in a critical section.
  };
*/

/*
                      --------------------
                      | std::mutex class |
                      --------------------
*/

/*
  // std::mutex class is non-copyable and non-movable.

  #include <mutex>
  #include <utility>  // std::move

  int main()
  {
    std::mutex mtx;

    // --------------------------------------------------------

    auto mtx2 = mtx;  // syntax error
    // error: use of deleted function 
    // 'std::mutex::mutex(const std::mutex&)'

    // std::mutex is a non-copyable class
    // copy constructor is deleted.

    // --------------------------------------------------------

    auto mtx3 = std::move(mtx);  
    // error: use of deleted function 
    // 'std::mutex::mutex(const std::mutex&)'

    // std::mutex class is non-movable
    // move constructor is not declared.

    // --------------------------------------------------------
  }
*/

/*
  #include <mutex>

  std::mutex g_mtx;

  int main()
  {
    g_mtx.lock();
    // when "lock" function can not acquire the mutex object,
    // it will blocked and wait for acquiring the mutex object.

    while (g_mtx.try_lock())
    {
      // if "try_lock" function can acquire the mutex object,
      // it will return true and enter the critical section.
      // if it can not acquire the mutex object, 
      // it will return false and do some other work.
    }
  }
*/

/*
  #include <mutex>
  #include <thread>

  int g_counter{};
  std::mutex g_counter_mtx;

  void try_to_increase()
  {
    for (int i = 0; i < 100'000; ++i) 
    {
      if (g_counter_mtx.try_lock()) 
      {
        ++g_counter;
        g_counter_mtx.unlock();
      }
    }
  }

  int main()
  {
    std::thread t_arr[10];

    for (int i = 0; i < 10; ++i)
      t_arr[i] = std::thread{ try_to_increase };

    for (auto& tx : t_arr)
      tx.join();

    std::cout << "g_counter increased " 
              << g_counter << " times\n";
    // output -> g_counter increased 83452 times
  }
*/

/*
  #include <thread>   // std::this_thread::sleep_for
  #include <mutex>    // std::mutex
  #include <chrono> 

  using namespace std::literals;

  std::mutex g_mtx;

  void task_1()
  {
    std::cout << "task_1 is trying to lock the mutex\n";
    g_mtx.lock();
    std::cout << "task_1 locked the mutex\n";

    std::this_thread::sleep_for(500ms);

    std::cout << "task_1 is unlocking the mutex\n";
    g_mtx.unlock();
  }

  void task_2()
  {
    std::this_thread::sleep_for(100ms);

    std::cout << "task_2 is trying to lock the mutex\n";
    while (!g_mtx.try_lock())
    {
      std::cout << "task_2 can not lock the mutex\n";
      std::this_thread::sleep_for(100ms);
    }
    std::cout << "task_2 locked the mutex\n";

    std::cout << "task_2 is unlocking the mutex\n";
    g_mtx.unlock();
  }

  int main()
  {
    std::thread t1{ task_1 }; 
    std::thread t2{ task_2 };

    t1.join();
    t2.join();

    // output ->
    //  task_1 is trying to lock the mutex
    //  task_1 locked the mutex
    //  task_2 is trying to lock the mutex
    //  task_2 can not lock the mutex
    //  task_2 can not lock the mutex
    //  task_2 can not lock the mutex
    //  task_2 can not lock the mutex
    //  task_1 is unlocking the mutex
    //  task_2 locked the mutex
    //  task_2 is unlocking the mutex
  }
*/

/*
                      ----------------------
                      | deadlock scenarios |
                      ----------------------
*/

/*
  #include <mutex>
  #include <thread>

  std::mutex g_mtx1;
  std::mutex g_mtx2;

  void task_1()
  {
    using namespace std::literals;

    g_mtx1.lock();
    std::this_thread::sleep_for(100ms);
    g_mtx2.lock();

    std::cout << "task_1()\n";
    g_mtx1.unlock();
    g_mtx2.unlock();
  }

  void task_2()
  {
    using namespace std::literals;

    g_mtx2.lock();
    std::this_thread::sleep_for(100ms);
    g_mtx1.lock();

    std::cout << "task_2()\n";
    g_mtx2.unlock();
    g_mtx1.unlock();
  }

  int main()
  {
    std::thread t1{ task_1 };
    std::thread t2{ task_2 };

    t1.join();
    t2.join();

    // 0ms 
    //  - t1 locks g_mtx1 and waits for 100ms
    //  - t2 locks g_mtx2 and waits for 100ms

    // 100ms 
    //  - t1 will try to lock g_mtx2 which has already locked by t2 
    //  - t2 will try to lock g_mtx1 which has already locked by t1

    // deadlock situation (undefined behavior)
  }
*/

/*
  #include <mutex>
  #include <thread>

  std::mutex g_mtx1;
  std::mutex g_mtx2;

  void task_1()
  {
    using namespace std::literals;

    g_mtx1.lock();
    std::this_thread::sleep_for(100ms);
    g_mtx2.lock();

    std::cout << "task_1()\n";
    g_mtx1.unlock();
    g_mtx2.unlock();
  }

  void task_2()
  {
    using namespace std::literals;

    g_mtx1.lock();
    std::this_thread::sleep_for(100ms);
    g_mtx2.lock();

    std::cout << "task_2()\n";
    g_mtx2.unlock();
    g_mtx1.unlock();
  }

  int main()
  {
    std::thread t1{ task_1 };
    std::thread t2{ task_2 };

    t1.join();
    t2.join();

    // output ->
    //  task_1()
    //  task_2()

    // NO deadlock situation
  }
*/

/*
  #include <mutex>

  class Myclass {
  private:
    std::mutex m_mtx;

  public:
    void func_1()
    {
      m_mtx.lock();       // trying to lock, already locked mutex
      // critical section
      m_mtx.unlock();
    }

    void func_2()
    {
      m_mtx.lock();
      // critical section
      m_mtx.unlock();
    }

    void func_3()
    {
      m_mtx.lock();
      func_1();       // ----> 
      m_mtx.unlock();
    }
  };

  int main()
  {
    Myclass m1;
    m1.func_3();
    // deadlock situation (undefined behavior)
  }
*/

/*
  #include <mutex>
  #include <syncstream>   // std::osyncstream
  #include <chrono>
  #include <thread>       // std::jthread

  std::mutex g_mtx1;
  std::mutex g_mtx2;

  void foo()
  {
    using namespace std::literals;

    auto o_cout = std::osyncstream{ std::cout };
    auto t_id = std::this_thread::get_id();

    // --------------------------------------------------------

    o_cout << "thread " << t_id << " trying to lock g_mtx1\n";
    g_mtx1.lock();
    o_cout  << "thread " << t_id << " locked g_mtx1\n";

    std::this_thread::sleep_for(400ms);

    // --------------------------------------------------------

    o_cout << "thread " << t_id << " trying to lock g_mtx2\n";
    g_mtx2.lock();
    o_cout << "thread " << t_id << " locked g_mtx2\n";

    // --------------------------------------------------------

    g_mtx1.unlock();
    o_cout << "thread " << t_id << " unlocked g_mtx1\n";
    g_mtx2.unlock();
    o_cout << "thread " << t_id << " unlocked g_mtx2\n";
  }

  void bar()
  {
    using namespace std::literals;

    auto o_cout = std::osyncstream{ std::cout };
    auto t_id = std::this_thread::get_id();

    // --------------------------------------------------------

    o_cout << "thread " << t_id << " trying to lock g_mtx2\n";
    g_mtx2.lock();
    o_cout  << "thread " << t_id << " locked g_mtx2\n";

    std::this_thread::sleep_for(400ms);

    // --------------------------------------------------------

    o_cout << "thread " << t_id << " trying to lock g_mtx1\n";
    g_mtx1.lock();
    o_cout << "thread " << t_id << " locked g_mtx1\n";

    // --------------------------------------------------------

    g_mtx1.unlock();
    o_cout << "thread " << t_id << " unlocked g_mtx1\n";
    g_mtx2.unlock();
    o_cout << "thread " << t_id << " unlocked g_mtx2\n";
  }

  int main()
  {
    std::jthread jt1{ foo };
    std::jthread jt2{ bar };

    // deadlock situation
  }
*/

/*
  #include <vector>
  #include <mutex>
  #include <utility>    // std::swap
  #include <thread>     // std::jthread

  class Buffer {
  private:
    std::vector<char> m_data = std::vector<char>(16 * 1024 * 1024);
    mutable std::mutex m_mtx;

  public:
    Buffer() = default;
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    void swap(Buffer& other) noexcept
    {
      if (&other == this)
        return;

      m_mtx.lock();
      other.m_mtx.lock();
      std::swap(m_data, other.m_data);  // critical section
      other.m_mtx.unlock();
      m_mtx.unlock();
    }
  };

  Buffer g_buf1;
  Buffer g_buf2;

  int main()
  {
    std::jthread jt1([](){
      for (int i = 0; i < 100'000; ++i)
        g_buf1.swap(g_buf2);
    });

    std::jthread jt2([](){
      for (int i = 0; i < 100'000; ++i)
        g_buf2.swap(g_buf1);
    });

    // deadlock situation
  }
*/

/*
                  ------------------------------
                  | std::recursive_mutex class |
                  ------------------------------
*/

/*
  // std::recursive_mutex class can be locked multiple times
  // without causing a deadlock situation.

  #include <mutex>  // std::recursive_mutex
  #include <thread>

  std::recursive_mutex g_rmtx;
  int g_counter{};

  void recursive_func(char ch, int N)
  {
    if (N < 0)
      return;

    g_rmtx.lock();
    std::cout << ch << " - " << g_counter++ << '\n';
    recursive_func(ch, N - 1);
    g_rmtx.unlock();
  }

  int main()
  {
    std::thread t1{ recursive_func, 'x', 10 };
    std::thread t2{ recursive_func, 'y', 10 };

    t1.join();
    t2.join();

    // output ->
    //  x - 0
    //  x - 1
    //  x - 2
    //  x - 3
    //  x - 4
    //  x - 5
    //  x - 6
    //  x - 7
    //  x - 8
    //  x - 9
    //  x - 10
    //  y - 11
    //  y - 12
    //  y - 13
    //  y - 14
    //  y - 15
    //  y - 16
    //  y - 17
    //  y - 18
    //  y - 19
    //  y - 20
    //  y - 21
  }
*/

/*
  #include <mutex>    // std::recursive_mutex, std::lock_guard

  class DatabaseAccess {
  private:
    // std::mutex m_db_mtx; --> deadlock situation
    std::recursive_mutex m_db_mtx;

  public:
    void create_table()
    {
      std::lock_guard guard{ m_db_mtx };
      // critical section code...
    }

    void insert_data()
    {
      std::lock_guard guard{ m_db_mtx };
      // critical section code...
    }

    void create_table_and_insert_data()
    {
      std::lock_guard guard{ m_db_mtx };
      create_table();
      // critical section code...
    }
  };

  int main()
  {
    DatabaseAccess db1;
    db1.create_table_and_insert_data();   // deadlock

    // when "create_table_and_insert_data" function is called,
    // it will lock the data member mutex object 
    // and call "create_table" member function,
    // which will try to lock the mutex object again
    // so that will cause a deadlock situation.

    // when std::recursive_mutex is used instead of std::mutex,
    // deadlock situation will not occur.
  }
*/

/*
  #include <thread>       // std::jthread
  #include <mutex>        // std::recursive_mutex, std::lock_guard
  #include <syncstream>   // std::osyncstream

  auto o_cout = std::osyncstream{ std::cout };

  class Myclass {
  private:
    mutable std::recursive_mutex m_rmtx;

  public:
    void func()
    {
      std::lock_guard guard{ m_rmtx };
      o_cout  << std::this_thread::get_id() 
              << " - func started\n";

      foo();
      o_cout  << std::this_thread::get_id() 
              << " - func finished\n";
    }

    void foo()
    {
      std::lock_guard guard{ m_rmtx };
      o_cout  << std::this_thread::get_id() 
              << " - foo started\n";

      o_cout  << std::this_thread::get_id() 
              << " - foo finished\n";
    }
  };

  void g_function()
  {
    Myclass mx;
    mx.func();
  }

  int main()
  {
    std::jthread jt1{ g_function };
    std::jthread jt2{ g_function };

    // output ->
    //  2 - func started
    //  2 - foo started
    //  2 - foo finished
    //  2 - func finished
    //  3 - func started
    //  3 - foo started
    //  3 - foo finished
    //  3 - func finished
  }
*/

/*
                  --------------------------
                  | std::timed_mutex class |
                  --------------------------
*/

/*
  std::timed_mutex class have 
    - try_lock_for      -> duration parameter
    - try_lock_until    -> time_point parameter
  member functions.
*/

/*
  #include <mutex>  // std::timed_mutex
  #include <thread> 

  int g_counter{};
  std::timed_mutex g_tmtx;

  void increment()
  {
    using namespace std::literals;

    auto id = std::this_thread::get_id();

    if (g_tmtx.try_lock_for(50ms))
    {
      ++g_counter;
      std::this_thread::sleep_for(10ms);
      std::cout << "thread " << id 
                << " is in critical section\n";
      g_tmtx.unlock();
    }
    else
      std::cout << "thread " << id 
                << " is NOT in critical section\n";
  }

  int main()
  {
    std::thread t1{ increment };
    std::thread t2{ increment };

    t1.join();
    t2.join();

    std::cout << "g_counter = " << g_counter << '\n';

    // output ->
    //  thread 3 is in critical section
    //  thread 2 is in critical section
    //  g_counter = 2
  }
*/

/*
                    -------------------------
                    | std::lock_guard class |
                    -------------------------
*/

/*
  // std::lock_guard class is non-copyable and non-movable.

  #include <mutex>    // std::lock_guard
  #include <thread>
  #include <utility>  // std::move

  std::mutex g_mtx;

  int main()
  {
    std::lock_guard guard(g_mtx);

    // --------------------------------------------------------

    auto guard2 = guard;  // syntax error
    //  error: use of deleted function 
    // 'std::lock_guard<_Mutex>::lock_guard(
    //    const std::lock_guard<_Mutex>&)

    // std::lock_guard is a non-copyable class
    // copy constructor is deleted.

    // --------------------------------------------------------

    auto guard3 = std::move(guard); // syntax error

    // error: use of deleted function 
    // 'std::lock_guard<_Mutex>::lock_guard(
    //    const std::lock_guard<_Mutex>&) 

    // std::lock_guard class is non-movable
    // move constructor is not declared.

    // --------------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <mutex>    // std::lock_guard
  #include <thread>   // std::jthread

  unsigned long long g_counter{};
  std::mutex g_mtx;

  void foo()
  {
    for (int i = 0; i < 1'000'000; ++i)
    {
      std::lock_guard<std::mutex> guard{ g_mtx };
      ++g_counter;
    }
  }

  int main()
  {
    {  
      std::vector<std::jthread> jt_vec;

      for (int i = 0; i < 16; ++i)
        jt_vec.emplace_back(foo);
    }

    std::cout << "g_counter = " << g_counter << '\n';
    // output -> g_counter = 16000000
  }
*/

/*
  #include <mutex>  
  // std::mutex, std::recursive_mutex, std::timed_mutex
  // std::lock_guard

  std::mutex g_mtx;
  std::recursive_mutex g_rmtx;
  std::timed_mutex g_tmtx;

  int main()
  {
    using namespace std;

    lock_guard<mutex> guard1{ g_mtx };
    lock_guard<recursive_mutex> guard2{ g_rmtx };
    lock_guard<timed_mutex> guard3{ g_tmtx };

    lock_guard guard4{ g_mtx };   // CTAD
    lock_guard guard5{ g_rmtx };  // CTAD
    lock_guard guard6{ g_tmtx };  // CTAD
  }
*/

/*
  #include <mutex>    // std::lock_guard

  std::mutex g_mtx;

  int main()
  {
    {
      std::lock_guard guard{ g_mtx };
      // std::lock_guard's constructor will be called
      // and g_mtx's "lock" member function will be called

      // g_mtx's "unlock" member function will be called 
      // at the end of the scope.
    }
  }
*/

/*
  #include <mutex>    // std::lock_guard

  std::mutex g_mtx;

  int main()
  {
    g_mtx.lock();

    {
      std::lock_guard guard{ g_mtx }; 
    }

    // because of g_mtx object is locked
    // and std::lock_guard's constructor will also call 
    // g_mtx's "lock" member function,
    // deadlock situation will occur. (undefined behavior)
  }
*/

/*
  // std::adopt_lock is a constexpr variable (empty struct)

  #include <mutex>    // std::lock_guard

  std::mutex g_mtx;

  int main()
  {
    g_mtx.lock();

    // g_mtx object is already locked

    {
      std::lock_guard guard{ g_mtx, std::adopt_lock };
      // if g_mtx object is already locked, 
      // it won't call "lock" member function of g_mtx object
      // in its constructor.

      // lock_guard object's destructor will call "g_mtx" 
      // object's "unlock" member function.
    }
  }
*/

/*
  #include <mutex>      
  #include <thread>     // std::jthread 
  #include <stdexcept>  // std::invalid_argument

  std::mutex g_mtx;

  void func(int x)
  {
    g_mtx.lock();

    try {
      std::cout << "thread " << std::this_thread::get_id() 
                << " locked the mutex\n";

      std::cout << "x = " << x << '\n';

      if (x % 3 != 0)
        throw std::invalid_argument{ "x is not divisible by 3" };

      g_mtx.unlock();
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
  }

  int main()
  {
    std::jthread jt1{ func, 5 };
    std::jthread jt2{ func, 10 };

    // first thread that gets into the "func" will lock the mutex 
    // and will throw an exception.
    // after exception thrown because of it is not possible 
    // to unlock the mutex anymore, second thread that gets in to 
    // "func" will be blocked forever. (deadlock situation)
  }
*/

/*
  #include <mutex>      // std::lock_guard
  #include <thread>     // std::jthread
  #include <stdexcept>  // std::invalid_argument

  std::mutex g_mtx;

  void func(int x)
  {
    std::lock_guard<std::mutex> guard{ g_mtx };

    try {
      std::cout << "thread " << std::this_thread::get_id() 
                << " locked the mutex\n";

      std::cout << "x = " << x << '\n';

      if (x % 3 != 0)
        throw std::invalid_argument{ "x is not divisible by 3" };
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
  }

  int main()
  {
    std::jthread jt1{ func, 5 };
    std::jthread jt2{ func, 10 };

    // output ->
    //  thread 3 locked the mutex
    //  x = 10
    //  exception caught: x is not divisible by 3
    //  thread 2 locked the mutex
    //  x = 5
    //  exception caught: x is not divisible by 3

    // because of std::lock_guard object's destructor 
    // will be called in stack unwinding phase,
    // it will unlock the mutex object.
    // so second thread WON'T be blocked.
  }
*/

/*
  #include <mutex>    // std::lock_guard
  #include <thread>   // std::jthread
  #include <string>

  std::mutex g_mtx;

  void print_str(const std::string& str)
  {
    std::lock_guard guard{ g_mtx };

    for (const auto ch : str)
      std::cout << ch << ' ';

    std::cout << '\n';
  }

  int main()
  {
    std::cout << "[0] - main thread started\n";

    {
      std::jthread jt1{ print_str, "world" };
      std::jthread jt2{ print_str, "galaxy" };
      std::jthread jt3{ print_str, "universe" };
    }

    std::cout << "[1] - main thread finished\n";
  }

  // output ->
  //  [0] - main thread started
  //  w o r l d
  //  g a l a x y
  //  u n i v e r s e
  //  [1] - main thread finished
*/

/*
  // technique used before std::osyncstream class.

  #include <sstream>    // std::stringstream
  #include <string>     // std::to_string
  #include <mutex>      // std::lock_guard
  #include <thread>     // std::jthread
  #include <vector>

  struct Cout : std::stringstream {
    static inline std::mutex m_mtx;

    ~Cout()
    {
      std::lock_guard guard{ m_mtx };

      // critical section
      std::cout << rdbuf();   
      std::cout.flush();
      // critical section    
    } 
  };

  void print(const std::string& str)
  {
    Cout{} << "str : " << str << '\n';
  }

  int main()
  {
    std::vector<std::jthread> jt_vec(5);

    for (int i = 0; i < 5; ++i)
      jt_vec[i] = std::jthread{ print, "_" + std::to_string(i)  };

    // output ->
    //  str : _3
    //  str : _4
    //  str : _1
    //  str : _2
    //  str : _0
  }
*/

/*
              --------------------------------------
              | std::lock global function template |
              --------------------------------------
*/

/*
  template <typename T1, typename T2, typename... Ts>
  void Lock(T1&, T2&, Ts&...);

  - geçilen tüm mutex objelerini kilitler ve deadlock avoidence
    algoritmasını kullanıp deadlock durumunu engeller.
*/

/*
  #include <mutex>      // std::lock
  #include <thread>     // std::jthread

  std::mutex g_mtx1;
  std::mutex g_mtx2;

  void foo()
  {
    std::lock(g_mtx1, g_mtx2);
    std::cout << "foo locked g_mtx1 and g_mtx2\n";

    g_mtx1.unlock();
    g_mtx2.unlock();
  }

  void bar()
  {
    std::lock(g_mtx1, g_mtx2);
    std::cout << "bar locked g_mtx1 and g_mtx2\n";

    g_mtx1.unlock();
    g_mtx2.unlock();
  }

  int main()
  {
    std::jthread jt1{ foo };
    std::jthread jt2{ bar };

    // output ->
    //  foo locked g_mtx1 and g_mtx2
    //  bar locked g_mtx1 and g_mtx2
  }
*/

/*
                    --------------------------
                    | std::scoped_lock class |
                    --------------------------
*/

/*
  - effectively replaces std::lock_guard
  - non copyable and non movable class template
  - deadlock avoidance algorithm is being used 
    when locking multiple mutex objects.
*/

/*
  #include <mutex>
  #include <thread>

  std::mutex g_mtx1;
  std::timed_mutex g_tmtx1;

  int main()
  {
    using namespace std;

    scoped_lock guard1{ g_mtx1, g_tmtx1 };
    scoped_lock<mutex, timed_mutex> guard2{ g_mtx1, g_tmtx1 };
    // Those 2 lines are equivalent.
  }
*/

/*
                    --------------------------
                    | std::unique_lock class |
                    --------------------------
*/

/*
  - unique_lock nesnesi, sarmaladığı mutexi
    ömrü bitene kadar (destructor'ı çağrılana kadar)
    birden fazla kere kilitleyebilir ve serbest bırakabilir.

  - diğer RAII nesneleri(std::lock_guard, std::scoped_lock) gibi
    eğer sarmaladığı mutex kilitli ise desturctor'ı çağrıldığında
    mutexi serbest bırakır.

  - kopyalanamaz(non-copyable) fakat taşınabilir(movable) 
    bir sınıftır.

  - mutex'in lock, try_lock, try_lock_for, try_lock_until
    member function'larını çağırabilir.
*/

/*
  // std::unique_lock is non-copyable but movable class.

  #include <mutex>    // std::unique_lock
  #include <thread>
  #include <utility>  // std::move

  std::mutex g_mtx;

  int main()
  {
    std::unique_lock guard(g_mtx);

    // --------------------------------------------------------

    auto guard2 = guard;  // syntax error
    // error: use of deleted function 
    // 'std::unique_lock<_Mutex>::unique_lock(
    //    const std::unique_lock<_Mutex>&) 

    // std::unique_lock is a non-copyable class
    // copy constructor is deleted.

    // --------------------------------------------------------

    auto guard3 = std::move(guard); // VALID

    // std::unique_lock is a movable class

    // --------------------------------------------------------
  }
*/

/*
  // if one of those themes will not going to be used,
  // better using std::lock_guard or std::scoped_lock

  #include <mutex>  
  // std::unique_lock, std::adopt_lock, std::defer_lock,
  // std::try_to_lock

  std::mutex g_mtx; 

  void func1()
  {
    std::unique_lock guard(g_mtx);
    // constructor will lock the mutex.
    // destructor will unlock the mutex.
  }

  void func2()
  {
    g_mtx.lock();   
    std::unique_lock guard(g_mtx, std::adopt_lock);
    // getting ownership of already locked mutex.
    // destructor will unlock the mutex.
  }

  void func3()
  {
    std::unique_lock guard(g_mtx, std::defer_lock);
    // constructor will not lock the mutex.
    // user will lock the mutex manually.
    // destructor will unlock the mutex.
  }

  void func4()
  {
    std::unique_lock guard(g_mtx, std::try_to_lock);
    // constructor will try to lock the mutex.

    // user should check if the mutex is locked or not.
    if (guard);             // operator bool() member function
    if (guard.owns_lock()); // owns_lock() member function

    // if locked successfully, destructor will unlock the mutex.
  }
*/

/*
  #include <mutex>    // std::unique_lock, std::timed_mutex

  std::timed_mutex g_tmtx;

  void func()
  {
    using namespace std;
    using namespace std::literals;

    unique_lock<timed_mutex> guard(g_tmtx, 500ms);
    // will try to lock the mutex for 500ms
  }
*/

/*
  #include <mutex>    // std::unique_lock, std::defer_lock
  #include <thread>   // std::jthread

  unsigned g_counter{};
  std::mutex g_mtx;

  void foo()
  {
    std::unique_lock guard(g_mtx, std::defer_lock);
    if (guard)
      std::cout << "mutex locked\n";
    else
      std::cout << "mutex not locked\n";
  }

  int main()
  {
    std::jthread jt1{ foo };    // output -> mutex not locked
  }
*/

/*
  #include <mutex>    // std::unique_lock, std::defer_lock
  #include <thread>   // std::jthread

  unsigned g_counter{};
  std::mutex g_mtx;

  void foo()
  {
    std::unique_lock guard(g_mtx, std::defer_lock);
    guard.lock();

    if (guard)
      std::cout << "mutex locked\n";
    else
      std::cout << "mutex not locked\n";
  }

  int main()
  {
    std::jthread jt1{ foo };  // output -> mutex locked
  }
*/

/*
  #include <mutex>    // std::unique_lock, std::adopt_lock
  #include <thread>   // std::jthread

  unsigned g_counter{};
  std::mutex g_mtx;

  void foo()
  {
    g_mtx.lock();
    std::unique_lock guard(g_mtx, std::adopt_lock);
    
    if (guard.owns_lock())
      std::cout << "mutex locked\n";
    else
      std::cout << "mutex not locked\n";
  }

  int main()
  {
    std::jthread jt1{ foo };    // output -> mutex locked
  }
*/

/*
  #include <mutex>    // std::unique_lock, std::timed_mutex
  #include <thread>   // std::jthread

  unsigned g_counter{};
  std::timed_mutex g_tmtx;

  void foo()
  {
    using namespace std::literals;

    std::unique_lock guard(g_tmtx, 10ms);
    
    if (guard.owns_lock())
      std::cout << "mutex locked\n";
    else
      std::cout << "mutex not locked\n";
  }

  int main()
  {
    std::jthread jt1{ foo };    // output -> mutex locked
  }
*/

/*
  #include <mutex>    // std::unique_lock, std::timed_mutex
  #include <thread>   // std::jthread

  unsigned g_counter{};
  std::timed_mutex g_tmtx;

  void foo()
  {
    using namespace std::literals;

    std::unique_lock guard(g_tmtx, std::defer_lock);
    // NOT CRITICAL SECTION CODE
    // NOT CRITICAL SECTION CODE
    // NOT CRITICAL SECTION CODE
    // NOT CRITICAL SECTION CODE

    guard.lock();
    ++g_counter;
    // CRITICAL SECTION CODE
    // CRITICAL SECTION CODE
    // CRITICAL SECTION CODE

    guard.unlock();
    // NOT CRITICAL SECTION CODE
    // NOT CRITICAL SECTION CODE
    // NOT CRITICAL SECTION CODE

    guard.lock();
    ++g_counter;
    // CRITICAL SECTION CODE
    // CRITICAL SECTION CODE
    // CRITICAL SECTION CODE
  }

  int main()
  {
    {
      std::jthread jt1{ foo };
    }

    std::cout << "g_counter = " << g_counter << '\n';
    // output -> g_counter = 2
  }
*/

/*
                -----------------------------------
                | std::shared_mutex class (C++17) |
                -----------------------------------
*/

/*
  - birden fazla thread okuma işlemi yaparken
    bir thread yazma işlemi yapabilir.

  - okuma işlemi için aynı kilidin kopyası(shared lock) 
    threadler arasında paylaşılabilirken, 
    yazma işleminde tek bir kilit(exclusive lock) kullanılır.

  - std::shared_mutex'i exclusive edinmek için, std::lock_guard veya 
    std::unique_lock ile sarmalanır.(Yazma işlemi)

  - shared(paylaşımlı) edinmek için std::shared_lock ile sarmalanır.
    (Okuma işlemi)

  - mutex'in paylaşımlı olarak edinebilmesi için, shared_lock
    nesnesi oluşturulur.
     - mutexin kilitlenmemiş olması gerekiyor veya
     - exclusive olarak kilitlenmemiş olması gerekiyor.

  - kilit paylaşımlı olarak edinildiğinde diğer okuma amaçlı 
    threadler de kilidi paylaşımlı olarak edinebilirler.
*/

/*
  #include <shared_mutex>     // std::shared_lock
  #include <vector>
  #include <mutex>            // std::scoped_lock
  #include <chrono>    
  #include <thread>           
  // std::this_thread::sleep_for, std::jthread
  #include <syncstream>       // std::osyncstream

  using namespace std::literals;

  std::shared_mutex g_smtx;
  int g_counter{};

  void writer()
  {
    // exclusive lock
    std::scoped_lock guard{ g_smtx };
    g_counter = 100;
  }

  void reader()
  {
    std::this_thread::sleep_for(5ms);

    // shared lock
    std::shared_lock guard{ g_smtx };

    std::osyncstream{ std::cout } 
      << "thread " << std::this_thread::get_id() 
      << " is reading, g_counter = " << g_counter << '\n';

    std::this_thread::sleep_for(1s);
  }

  int main()
  {
    std::cout << "[0] - main thread started\n";

    {
      std::vector<std::jthread> jt_vec;
      jt_vec.reserve(10);

      jt_vec.emplace_back(writer);
      for (int i = 0; i < 9; ++i)
        jt_vec.emplace_back(reader);
    }

    std::cout << "[1] - main thread finished\n";

    // output ->
    //  [0] - main thread started
    //  thread 5 is reading, g_counter = 100
    //  thread 3 is reading, g_counter = 100
    //  thread 7 is reading, g_counter = 100
    //  thread 9 is reading, g_counter = 100
    //  thread 11 is reading, g_counter = 100
    //  thread 4 is reading, g_counter = 100
    //  thread 10 is reading, g_counter = 100
    //  thread 6 is reading, g_counter = 100
    //  thread 8 is reading, g_counter = 100
    //  [1] - main thread finished
  }
*/

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

/*
  #include <thread>
  #include <vector>

  class Myclass {
  public:
    Myclass()
    {
      std::cout << "Myclass::Myclass(), this = " 
                << this << '\n';
    }

    ~Myclass()
    {
      std::cout << "Myclass::~Myclass(), this = " 
                << this << '\n';
    }
  };

  void func()
  {
    static Myclass sm1;   
    // static local variables are thread-safe
  }

  int main()
  {
    std::vector<std::jthread> jt_vec;

    for (int i = 0; i < 100; ++i)
      jt_vec.emplace_back(func);

    // output ->
    //  Myclass::Myclass(), this = 0x7ff7b8d750a0
    //  Myclass::~Myclass(), this = 0x7ff7b8d750a0
  }
*/

/*
              ----------------------------------
              | std::once_flag, std::call_once |
              ----------------------------------
*/

/*
  #include <mutex>  // std::once_flag, std::call_once

  template <typename Callable, class ...Args>
  void CallOnce(std::once_flag& of, Callable& fn, Args&&... args);

  - std::call_once fonksiyon şablonu, 
    gönderilen callable'ın, 
    gönderilen flag nesnesini kullanarak 
    sadece 1 defa çağrılmasını garanti ediyor(thread-safe).

  - en tipik kullanım seneryosu, multithread programlarda 
    dinamik ömürlü nesnelerin sadece 1 defa initialize edilmesi.
    örneğin, singleton
*/

/*
  #include <thread>   // std::jthread
  #include <vector>

  void func_once()
  {
    std::cout << "func_once() called\n";
  }

  void func()
  {
    static auto fn = []{ func_once(); return 0; }();
  }

  int main()
  {
    std::vector<std::jthread> jt_vec;

    for (int i = 0; i < 50; ++i)
      jt_vec.emplace_back(func);

    // output -> func_once() called
  }
*/

/*
  #include <memory>   // std::unique_ptr, std::make_unique
  #include <mutex>    // std::once_flag, std::call_once  
  #include <cassert>  // assert
  #include <vector>
  #include <thread>   // std::jthread

  std::unique_ptr<int> g_uptr;
  std::once_flag g_of;

  void func_initializer(int val)
  {
    g_uptr = std::make_unique<int>(111);
    std::cout << "func_initializer called, val = " << val << '\n';
  }

  int& get_value(int id)
  {
    std::call_once(g_of, func_initializer, id);
    return *g_uptr;
  }

  void foo(int id)
  {
    const int& r = get_value(id);
    assert(r == 111);
  }

  int main()
  {
    std::vector<std::jthread> jt_vec;

    for (int i = 0; i < 10; ++i)
      jt_vec.emplace_back(foo, i);

    // output -> func_initializer called, val = 0
  }
*/

/*
  #include <mutex>        // std::once_flag, std::call_once
  #include <vector>
  #include <thread>       // std::jthread
  #include <syncstream>   // std::osyncstream

  std::once_flag g_of;

  void func()
  {
    auto id = std::this_thread::get_id();  
    std::osyncstream{ std::cout } 
      << "func started with thread id = " << id << '\n';

    std::call_once(g_of, [id](){
      std::cout << "thread " << id << " called\n";
    });
  }

  int main()
  {
    std::vector<std::jthread> jt_vec;
    jt_vec.reserve(10);

    for (int i = 0; i < 10; ++i)
      jt_vec.emplace_back(func);
    // output ->
    //  func started with thread id = 2
    //  func started with thread id = 5
    //  thread 2 called
    //  func started with thread id = 6
    //  func started with thread id = 3
    //  func started with thread id = 7
    //  func started with thread id = 8
    //  func started with thread id = 4
    //  func started with thread id = 9
    //  func started with thread id = 10
    //  func started with thread id = 11
  }
*/

/*
  #include <mutex>  // std::once_flag, std::call_once
  #include <chrono>
  #include <thread> // std::this_thread::sleep_for, std::jthread

  using namespace std::literals;

  std::once_flag g_once_flag;

  void foo()
  {
    std::this_thread::sleep_for(100ms);

    std::call_once(g_once_flag, [](){ 
      std::cout << "registered in foo\n"; 
    });
  }

  void bar()
  {
    std::this_thread::sleep_for(100ms);

    std::call_once(g_once_flag, [](){ 
      std::cout << "registered in bar\n"; 
    });
  }

  int main()
  {
    std::jthread jt_arr[10];

    for (int i = 0; i < 10; ++i)
      jt_arr[i] = i % 2 ? std::jthread{ foo } 
                        : std::jthread{ bar };
    // output -> registered in bar
  }
*/

/*
  #include <mutex>      // std::once_flag, std::call_once
  #include <syncstream> // std::osyncstream
  #include <vector>
  #include <thread>

  class Singleton {
  private:
    static std::once_flag m_once_flag;
    static Singleton* m_instance;
    Singleton() = default;

  public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static void init() { m_instance = new Singleton(); }
    static Singleton* get_instance()
    {
      std::call_once(m_once_flag, Singleton::init);
      return m_instance;
    }
  };

  Singleton* Singleton::m_instance{};
  std::once_flag Singleton::m_once_flag;

  void func()
  {
    std::osyncstream{ std::cout } 
      << Singleton::get_instance() << '\n';
  }

  int main()
  {
    std::vector<std::jthread> jt_vec;
    jt_vec.reserve(5);

    for (int i = 0; i < 5; ++i)
      jt_vec.emplace_back(func);

    // output ->
    //  0x13a578fd7f0
    //  0x13a578fd7f0
    //  0x13a578fd7f0
    //  0x13a578fd7f0
    //  0x13a578fd7f0
  }
*/

/*
  // alternative way to implement Singleton class
  // without using std::call_once and std::once_flag

  #include <syncstream> // std::osyncstream
  #include <vector>
  #include <thread>

  class Singleton {
  private:
    static Singleton* m_instance;
    Singleton() = default;

  public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static void init() { m_instance = new Singleton(); }
    static Singleton* get_instance()
    {
      static int _ = [](){ Singleton::init(); return 0; }(); 
      return m_instance;
    }
  };

  Singleton* Singleton::m_instance{};

  void func()
  {
    std::osyncstream{ std::cout } 
      << Singleton::get_instance() << '\n';
  }

  int main()
  {
    std::vector<std::jthread> jt_vec;
    jt_vec.reserve(5);

    for (int i = 0; i < 5; ++i)
      jt_vec.emplace_back(func);

    // output ->
    //  0x15240b491b0
    //  0x15240b491b0
    //  0x15240b491b0
    //  0x15240b491b0
    //  0x15240b491b0
  }
*/

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

/*
  - bir threadden başka bir thread'e 
    sonuç(değer veya bir exception) aktarılabilir.

  - üretilecek sonucu hazırlayan sınıf, std::promise
  - sonucun iletilmesi ve kullanıdırlmasını sağlayan sınıf,
    std::future
*/

/*
  #include <future>   // std::future, std::promise

  template <typename T>
  class Future {};

  // template parametre değişkeninin türü, 
  // üretilmesi beklenen sonucun türüdür.
  // herhangi bir sonuç üretilmeyecek ise void türü kullanılır.

  // partial specialization for L value reference types
  template <typename T>
  class Future<T&>;    

  // explicit specialization for void type
  template <>
  class Future<void>;

  // ------------------------------------------------------

  template <typename T>
  class Promise {};

  // template parametre değişkeninin türü,
  // Promise sınıfının shared state'te set ediceği değerin türüdür.

  // ------------------------------------------------------
*/

/*
  #include <future>   // std::promise, std::future
  #include <string> 

  int main()
  {
    std::promise<int> pr1;
    std::promise<std::string> pr2;
    std::promise<double> pr3;

    auto ft1 = pr1.get_future();  
    // ft1 will deduce to std::future<int>
    auto ft2 = pr2.get_future();  
    // ft2 will deduce to std::future<std::string>

    std::future<double> ft3 = pr3.get_future();
  }
*/

/*
  #include <future>   // std::promise

  int main()
  {
    std::promise<int> pr;
    auto ft = pr.get_future();
    
    pr.set_value(111);

    std::cout << ft.get() << '\n';    // output -> 111
  }
*/

/*
  #include <future>     // std::promise
  #include <stdexcept>  // std::runtime_error
  #include <exception>  // std::make_exception_ptr

  int main()
  {
    using namespace std;

    std::promise<int> pr;
    auto ft = pr.get_future();

    pr.set_exception(make_exception_ptr(runtime_error{ "error" }));

    try {
      auto val = ft.get();
    }
    catch (const exception& ex) {
      cout << "exception caught: " << ex.what() << '\n';
    }
    // output -> exception caught: error
  }
*/

/*
  #include <future>     // std::promise, std::future
  #include <thread>     // std::jthread
  #include <utility>    // std::move
  #include <functional> // std::ref

  double get_square(double dval) { return dval * dval; }

  void produce_1(std::promise<double> prom, double dval)
  {
    prom.set_value(get_square(dval));
  }

  void produce_2(std::promise<double>& prom, double dval)
  {
    prom.set_value(get_square(dval));
  }

  void produce_3(std::promise<double>&& prom, double dval)
  {
    prom.set_value(get_square(dval));
  }

  int main()
  {
    // ------------------------------------------------------

    std::promise<double> pr1;
    std::future<double> ftr1 = pr1.get_future();

    std::jthread jt1{ produce_1, std::move(pr1), 3.14 };
    // move semantics

    std::cout << "calculate value = " << ftr1.get() << '\n';
    // output -> calculate value = 9.8596

    // ------------------------------------------------------

    std::promise<double> pr2;
    std::future<double> ftr2 = pr2.get_future();

    std::jthread jt2{ produce_2, std::ref(pr2), 5.52 };
    // reference semantics (lvalue reference)

    std::cout << "calculate value = " << ftr2.get() << '\n';
    // output -> calculate value = 30.4704

    // ------------------------------------------------------

    std::promise<double> pr3;
    std::future<double> ftr3 = pr3.get_future();

    std::jthread jt3{ produce_3, std::move(pr3), 7.77 };
    // reference semantics (rvalue reference)

    std::cout << "calculate value = " << ftr3.get() << '\n';
    // output -> calculate value = 60.3729

    // ------------------------------------------------------
  }
*/

/*
  #include <future>     // std::promise, std::future
  #include <utility>    // std::move
  #include <thread>     // std::jthread

  void sum_square(std::promise<int>&& prom, int x, int y)
  {
    prom.set_value(x * x + y * y);
  } 

  // function object(functor class)
  struct Sum_Square {
    void operator()(std::promise<int>&& prom, int x, int y)
    {
      prom.set_value(x * x + y * y);
    }
  };

  int main()
  {
    int x = 5, y = 12;

    std::promise<int> pr1;
    std::promise<int> pr2;

    std::future<int> ft1 = pr1.get_future();
    std::future<int> ft2 = pr2.get_future();

    std::jthread jt1{ sum_square, std::move(pr1), x, y };
    std::jthread jt2{ Sum_Square{}, std::move(pr2), x, y };

    std::cout << "result_1 = " << ft1.get() << '\n';
    // output -> result_1 = 169
    std::cout << "result_2 = " << ft2.get() << '\n';
    // output -> result_2 = 169
  }
*/

/*
  #include <future>     // std::promise, std::future
  #include <utility>    // std::move
  #include <thread>     // std::jthread
  #include <stdexcept>  // std::invalid_argument
  #include <exception>  // std::current_exception

  void sum_square(std::promise<int>&& prom, int x, int y)
  {
    try {
      if (x < 0 || y < 0)
        throw std::invalid_argument{ "argument is negative" };
    }
    catch (const std::exception& ex) {
      auto eptr = std::current_exception();
      prom.set_exception(eptr);
      return;
    }

    prom.set_value(x * x + y * y);
  } 

  int main()
  {
    int x = 5, y = 12;

    // -----------------------------------------------------

    std::promise<int> pr1;
    std::future<int> ft1 = pr1.get_future();
    std::jthread jt1{ sum_square, std::move(pr1), x, y };

    try {
      auto val = ft1.get();
      std::cout << "result = " << val << '\n';
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
    // output -> result = 169

    // -----------------------------------------------------

    std::promise<int> pr2;
    std::future<int> ft2 = pr2.get_future();
    std::jthread jt2{ sum_square, std::move(pr2), -x, y };

    try {
      auto val = ft2.get();
      std::cout << "result = " << val << '\n';
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
    // output -> exception caught: argument is negative

    // -----------------------------------------------------
  }
*/

/*
  // using std::make_exception_ptr function template

  #include <future>     // std::promise, std::future
  #include <utility>    // std::move
  #include <thread>     // std::jthread
  #include <stdexcept>  // std::invalid_argument
  #include <exception>  // std::make_exception_ptr

  void sum_square(std::promise<int>&& prom, int x, int y)
  {
    if (x < 0 || y < 0) 
    {
      prom.set_exception( std::make_exception_ptr(
          std::invalid_argument{ "argument is negative" }));
      return;
    }

    prom.set_value(x * x + y * y);
  } 

  int main()
  {
    int x = 5, y = 12;

    // -----------------------------------------------------

    std::promise<int> pr1;
    std::future<int> ft1 = pr1.get_future();
    std::jthread jt1{ sum_square, std::move(pr1), x, y };

    try {
      auto val = ft1.get();
      std::cout << "result = " << val << '\n';
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
    // output -> result = 169

    // -----------------------------------------------------

    std::promise<int> pr2;
    std::future<int> ft2 = pr2.get_future();
    std::jthread jt2{ sum_square, std::move(pr2), -x, y };

    try {
      auto val = ft2.get();
      std::cout << "result = " << val << '\n';
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
    // output -> exception caught: argument is negative

    // -----------------------------------------------------
  }
*/

/*
  // when future object's get member function is called
  // for the second time, it will throw an exception.

  #include <future>    // std::promise, std::future

  int main()
  {
    std::promise<double> prom;
    auto ftr = prom.get_future();

    prom.set_value(3.14);
    std::cout << ftr.get() << '\n';   // output -> 3.14

    try {
      auto val = ftr.get();
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
    // output -> 
    //  exception caught: std::future_error: No associated state
  }
*/

/*
  // when promise object's get_future member function is called
  // for the second time, it will throw an exception.

  #include <future>    // std::promise, std::future

  int main()
  {
    std::promise<double> prom;
    auto ftr1 = prom.get_future();

    try {
      auto ftr2 = prom.get_future();
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
    // output ->
    //  exception caught: 
    //  std::future_error: Future already retrieved
  }
*/

/*
  std::future class'es member functions
    - "wait", will block the thread until the result is ready. 
      no return value.
    - "wait_for(duration)", 
      returns std::future_status enum class
    - "wait_until(time_point)", 
      returns std::future_status enum class

  enum future_status {
    ready,      : value is ready
    time_out,   : time has passed, value is still not ready
    deferred    : workload will run synchronously,
                  value will be ready when "get" is called
  };
*/

/*
  #include <future>   // std::promise, std::future
  #include <utility>  // std::move
  #include <chrono>
  #include <thread>   
  // std::this_thread::sleep_for, std::jthread

  using namespace std::literals;  

  void foo(std::promise<double> prom)
  {
    std::this_thread::sleep_for(500ms);
    prom.set_value(3.14);
  }

  int main()
  {
    std::promise<double> pr;
    std::future<double> ftr = pr.get_future();

    std::jthread jt{ foo, std::move(pr) };

    std::future_status status{};
    do {
      status = ftr.wait_for(150ms);
      std::cout << "work...\n";
    } while(status != std::future_status::ready);

    std::cout << "result = " << ftr.get() << '\n';

    // output ->
    //  work...
    //  work...
    //  work...
    //  work...
    //  result = 3.14
  }
*/

/*
  #include <future>   
  // std::promise, std::future, std::future_status
  #include <thread>   // std::jthread
  #include <chrono>
  #include <format>

  using namespace std::literals;

  unsigned long long fibonacci(unsigned long long N)
  {
    return N  < 3 ? 1 : fibonacci(N - 1) + fibonacci(N - 2);
  }

  void produce( std::promise<unsigned long long> prom, 
                unsigned long long N)
  {
    prom.set_value(fibonacci(N));
  }

  int main()
  {
    std::promise<unsigned long long> pr;
    std::future<unsigned long long> ft = pr.get_future();

    unsigned long long val = 45ULL;

    std::jthread jt{ produce, std::move(pr), val };

    while(ft.wait_for(20ms) == std::future_status::timeout)
      std::cout.put('.');

    auto result = ft.get();

    std::cout << 
      std::format("\nfibonacci({}) = {}\n", val, result);

    // output -> 
    //  ...................................................
    //  ........................
    //  fibonacci(45) = 1134903170
  }
*/

/*
  #include <future>   // std::promise
  #include <chrono>

  void print_status(std::future_status status)
  {
    using enum std::future_status;

    switch (status) {
      case ready:
        std::cout << "ready\n"; break;
      case timeout:
        std::cout << "timeout\n"; break;
      case deferred:
        std::cout << "deferred\n"; break;
    }
  }

  using namespace std::literals;

  int main()
  {
    std::promise<int> pr; 
    auto ftr = pr.get_future();

    print_status(ftr.wait_for(20ms));   
    // output -> timeout

    pr.set_value(111);

    print_status(ftr.wait_for(20ms));
    // output -> ready
  }
*/

/*
                        --------------
                        | std::async |
                        --------------
*/

/*
  std::async(callable, args...)
    default policy

  std::async(std::launch::async, callable, args...)
    async (for running asynchronously) policy

  std::async(std::launch::deferred, callable, args...)
    deferred (for running synchronously) policy

  std::async( std::launch::async | std::launch::deferred, 
              callable, args...)
    compiler will decide the policy
*/

/*
  #include <future>   // std::async

  int func(int x, int y)
  {
    return x * x + y;
  }

  int main()
  {
    auto ftr = std::async(func, 11, 22);
    std::cout << ftr.get() << '\n'; // output -> 143
  }
*/

/*
  #include <future>   // std::async, std::launch

  int func(int x, int y)
  {
    return x * x + y;
  }

  int main()
  {
    // ----------------------------------------------------

    auto ftr2 = std::async(std::launch::async, func, 11, 22);

    // function will run [asynchronously] (create another thread)
    // if thread can not be created, it will throw an exception.

    // ----------------------------------------------------

    auto ftr3 = std::async(std::launch::deferred, func, 33, 44);
    // lazy evaluation, function will run [synchronously]

    // when "get" member function is called, 
    // function will run [synchronously] in main thread.

    ftr3.get(); // function's code will execute now.

    // ----------------------------------------------------

    using namespace std;

    auto ftr1 = std::async(func, 55, 66);
    auto ftr4 = async(launch::async | launch::deferred, 
                      func, 77, 88);
    // Those 2 lines are equivalent.

    // compiler will decide the policy
    // no way that it will throw an exception.

    // ----------------------------------------------------
  }
*/

/*
  #include <future>   // std::future, std::async
  #include <thread>   // std::jthread
  #include <chrono>

  using namespace std::literals;

  unsigned long long fibonacci(unsigned long long N)
  {
    return N  < 3 ? 1 : fibonacci(N - 1) + fibonacci(N - 2);
  }

  int main()
  {
    using namespace std;

    // ---------------------------------------------------------

    auto tp_start = chrono::steady_clock::now();

    auto result = fibonacci(42ULL) + fibonacci(43ULL);
    // both function are working synchronously

    auto tp_end = chrono::steady_clock::now();

    cout << "result = " << result << '\n';
    cout  << "time passed = " 
          << chrono::duration<double>(tp_end - tp_start).count()
          << " seconds\n";

    // output ->
    //  result = 701408733
    //  time passed = 2.14846 seconds

    // ---------------------------------------------------------

    tp_start = chrono::steady_clock::now();

    auto ftr = std::async(fibonacci, 42ULL);
    auto N = fibonacci(43ULL);
    // functions are working asynchronously
    result = ftr.get() + N;

    tp_end = chrono::steady_clock::now();

    cout << "result = " << result << '\n';
    cout  << "time passed = " 
      << chrono::duration<double>(tp_end - tp_start).count()
      << " seconds\n";

    // output ->
    //  result = 701408733
    //  time passed = 1.36927 seconds

    // ---------------------------------------------------------
  }
*/

/*
  #include <future>   // std::async, std::launch
  #include <thread>   // std::this_thread::sleep_for

  int main()
  {
    using namespace std::chrono_literals;
    using d_sec = std::chrono::duration<double>;

    std::chrono::time_point start = std::chrono::steady_clock::now();

    auto eager = std::async(std::launch::async, []{
      return std::chrono::steady_clock::now();
    });

    auto lazy = std::async(std::launch::deferred, []{
      return std::chrono::steady_clock::now();
    });

    std::this_thread::sleep_for(1s);

    auto deferred_sec = 
      duration_cast<d_sec>(lazy.get() - start).count();
    // because of the std::launch::deferred policy(lazy evaluation)
    // workload will execute when "get" member function is called
    // in"deferred_sec" variable's initialization.
    // So (std::this_thread::sleep_for(1s)) in main thread
    // will be added to duration.

    auto eager_sec =
      duration_cast<d_sec>(eager.get() - start).count();
    // because of the std::launch::async policy
    // workload has already been executed asynchronously
    // in a different thread 
    // so (std::this_thread::sleep_for(1s)) in main thread
    // will not be added to duration.

    std::cout << "duration for deferred = " 
              << deferred_sec << " seconds\n";
    // output -> duration for deferred = 1.00106 seconds

    std::cout << "duration for eager = "
              << eager_sec << " seconds\n";
    // output -> duration for eager = 0.0006041 seconds
  }
*/

/*
  #include <future>  // std::async, std::launch

  void func()
  {
    std::cout << "func() called\n";
  }

  int main()
  {
    auto ftr = std::async(std::launch::deferred, func);
    // NO output because of the lazy evaluation
    // "get" member function has not been called.
  }
*/

/*
  #include <future>  // std::async, std::launch

  void func()
  {
    std::cout << "func() called\n";
  }

  int main()
  {
    auto ftr = std::async(std::launch::deferred, func);

    ftr.get();  // output -> func() called
  }
*/

/*
  #include <future>  // std::async, std::launch

  void func()
  {
    std::cout << "func() called\n";
  }

  int main()
  {
    std::cout << "[0] - main thread started\n";

    {
      auto ftr = std::async(std::launch::async, func);
    }

    std::cout << "[1] - main thread finished\n";
  }
  // output -> 
  //  [0] - main thread started
  //  func() called
  //  [1] - main thread finished
*/

/*
  #include <future>  // std::async, std::launch

  void func()
  {
    std::cout << "func() called\n";
  }

  int main()
  {
    std::cout << "[0] - main thread started\n";
    
    std::async(std::launch::async, func);
    // warning: ignoring return value of ... 
    // declared with attribute 'nodiscard'
    
    // because of "std::async(std::launch::async, func)" 
    // is a temporary object, its destructor will be called 
    // at the end of this line.
    // so basically it will work like a synchronous function call.

    std::cout << "[1] - main thread finished\n";
  }

  // output -> 
  //  [0] - main thread started
  //  func() called
  //  [1] - main thread finished
*/

/*
  #include <chrono> // std::chrono::milliseconds
  #include <random>   
  // std::mt19937, std::random_device, 
  // std::uniform_int_distribution
  #include <thread> // std::this_thread::sleep_for
  #include <future> // std::async

  int task(char ch)
  {
    std::mt19937 eng{ std::random_device{}() };
    std::uniform_int_distribution dist{ 20, 200 };

    int total_duration{};

    for (int i = 0; i < 10; ++i) 
    {
      auto dur = std::chrono::milliseconds{ dist(eng) };
      std::this_thread::sleep_for(dur);
      std::cout.put(ch).flush();
      total_duration += static_cast<int>(dur.count());
    }

    return total_duration;
  }

  int func_1() { return task('*'); }
  int func_2() { return task('!'); }
  int func_3() { return task('.'); }

  int main()
  {
    auto ftr1 = std::async(std::launch::async, func_1);
    auto ftr2 = std::async(std::launch::async, func_2);
    auto ftr3 = std::async(std::launch::async, func_3);

    auto N = ftr1.get() + ftr2.get() + ftr3.get();
    std::cout << "\nN = " << N << '\n';

    // output ->
    //  .!*!.!.*.!*.!*..*.!.!*.!*!!***
    //  N = 3184
  }
*/

/*
  #include <map>
  #include <cstddef>    // std::size_t
  #include <string>     // std::erase_if(std::string)
  #include <future>     // std::async
  #include <algorithm>  // std::sort, std::count_if
  #include <iomanip>    // std::quoted
  #include <format>

  std::map<char, std::size_t> histogram(const std::string& str)
  {
    std::map<char, std::size_t> ch_map{};

    for (const auto ch : str)
      ++ch_map[ch];

    return ch_map;
  }

  std::string get_sorted(std::string str)
  {
    std::sort(str.begin(), str.end());
    std::erase_if(str, [](const char ch){ 
        return std::isspace(ch); 
      });

    return str;
  }

  bool is_vowel(char ch)
  {
    using namespace std::literals;
    return "aeiouAEIOU"s.contains(ch);
  }

  std::size_t count_vowel(const std::string& str)
  {
    return std::count_if(str.begin(), str.end(), &is_vowel);
  } 

  int main()
  {
    std::string sline = { "istanbul ankara izmir bursa" };

    auto hist = std::async(histogram, sline);
    auto sorted_string = std::async(get_sorted, sline);
    auto vowel_count = std::async(count_vowel, sline);

    for (const auto& [ch, count] : hist.get())
      std::cout << std::format("'{}' -> {}\n", ch, count);
    // output ->
    //  ' ' -> 3
    //  'a' -> 5
    //  'b' -> 2
    //  'i' -> 3
    //  'k' -> 1
    //  'l' -> 1
    //  'm' -> 1
    //  'n' -> 2
    //  'r' -> 3
    //  's' -> 2
    //  't' -> 1
    //  'u' -> 2
    //  'z' -> 1

    std::cout << "sorted string = "
              << std::quoted(sorted_string.get()) << '\n';
    // output -> sorted string = "aaaaabbiiiklmnnrrrsstuuz"

    std::cout << "total vowels = " << vowel_count.get() << '\n';
    // output -> total vowels = 10
  }
*/

/*
  // if a value set to a shared state, "get" member function
  // will return the value, 
  // if an exception set to a shared state, "get" member function
  // will throw the exception.

  #include <future>
  #include <cmath>      // std::pow
  #include <stdexcept>  // std::invalid_argument
  #include <vector>
  #include <string>     // std::to_string

  double get_power(double base, double exp)
  {
    if (exp < 0)
      throw std::invalid_argument{ 
        std::string{ "negative exponent: " } + std::to_string(exp) };

    return std::pow(base, exp);
  }

  int main()
  {
    using namespace std;
    std::vector<std::future<double>> ftr_vec;

    for (int i = 0; i < 10; ++i)
      ftr_vec.emplace_back(async( launch::async, 
                                  get_power, 
                                  1.1, 
                                  (i % 2  ? i * 2.2 : i * -2.2)));

    std::cout << std::fixed;

    for (auto& ftr : ftr_vec)
    {
      try {
        std::cout << ftr.get() << '\n'; 
      }
      catch (const std::exception& ex) {
        std::cout << "exception caught: " << ex.what() << '\n';
      }
    }
    // output ->
    //  1.000000
    //  1.233286
    //  exception caught: negative exponent: -4.400000
    //  1.875822
    //  exception caught: negative exponent: -8.800000
    //  2.853117
    //  exception caught: negative exponent: -13.200000
    //  4.339577
    //  exception caught: negative exponent: -17.600000
    //  6.600475
  }
*/

/*
                    ----------------------
                    | std::shared_future |
                    ----------------------
*/

/*
  std::shared_future : 
    - "get" member function can be called multiple times.
    - copyable
    
*/

/*
  #include <future>       // std::promise, std::shared_future
  #include <thread>       // std::jthread 
  #include <utility>      // std::move
  #include <syncstream>   // std::osyncstream

  // Sum_Square functor class(function object)
  struct Sum_Square {
    void operator()(std::promise<int>&& prom, int x, int y)
    {
      prom.set_value(x * x + y * y);
    }
  };

  void func(std::shared_future<int> sftr)
  {
    std::osyncstream{ std::cout }
      << "thread id = " << std::this_thread::get_id() 
      << ", result = " << sftr.get() << '\n';
  }

  int main()
  {
    std::promise<int> prom;
    std::shared_future<int> s_ftr = prom.get_future();

    std::jthread jtx{ Sum_Square{}, std::move(prom), 5, 12 };

    std::jthread jt1{ func, s_ftr };
    std::jthread jt2{ func, s_ftr };
    std::jthread jt3{ func, s_ftr };
    std::jthread jt4{ func, s_ftr };
    std::jthread jt5{ func, s_ftr };

    // output ->
    //  thread id = 7, result = 169
    //  thread id = 4, result = 169
    //  thread id = 6, result = 169
    //  thread id = 3, result = 169
    //  thread id = 5, result = 169
  }
*/

/*
                      ----------------------
                      | std::packaged_task |
                      ----------------------
*/

/*
  - std::packaged_task sınıfı türünden bir nesne, çoğunlukla
    asynchrone çağrı yapmak için, bir callable'ı sarmalıyor.

  - std::packaged_task, bir task'i, daha sonradan aynı veya farklı
    bir thread'de çalıştırılabilir bir callable halinde paketliyor.
*/

/*
  #include <future>   // std::packaged_task

  int sum(int a, int b)
  {
    return a + b;
  }

  int main()
  {
    std::packaged_task<int(int, int)> task1;  // default init
    std::packaged_task<int(int, int)> task2{ sum };
    std::packaged_task task3{ sum };  // CTAD

    task2(11, 22);    // operator() function
  }
*/

/*
  #include <future>   // std::packaged_task, std::future

  int sum(int a, int b)
  {
    std::cout << "sum() called\n";
    return a + b;
  }

  int main()
  {
    std::packaged_task<int(int, int)> task{ sum };
    std::future<int> ftr = task.get_future();

    std::packaged_task task2{ sum };        // CTAD
    std::future ftr2 = task2.get_future();  // CTAD

    task(11, 22);
    task2(33, 44);
    // tasks are run synchronously in main thread

    std::cout << "result = " << ftr.get() << '\n';
    std::cout << "result = " << ftr2.get() << '\n';
    // output ->
    //  sum() called
    //  sum() called
    //  result = 33
    //  result = 77
  }
*/

/*
  // std::packaged_task is a non-copyable class

  #include <future>   // std::packaged_task, std::future
  #include <thread>

  int sum(int a, int b)
  {
    return a + b;
  }

  int main()
  {
    std::packaged_task task{ sum };
    std::future ftr = task.get_future();

    // ---------------------------------------------------

    auto task2 = task;  // syntax error
    // error: use of deleted function 
    //  'packaged_task<_Res(_ArgTypes ...)>::packaged_task(
    //          const packaged_task<_Res(_ArgTypes ...)>&)
    // copy constructor is deleted

    // ---------------------------------------------------

    std::thread tx{ task, 11, 22 };   // syntax error
    // error: no matching function for call to 
    // 'tuple<packaged_task<int(int, int)>, int, int>::tuple(
    //        packaged_task<int(int, int)>&, int, int)'

    // because of std::packaged_task is not copyable
    // it can not be passed by value to a std::thread constructor.

    tx.join();
    
    // ---------------------------------------------------
  }
*/

/*
  // std::packaged_task is a move-only class

  #include <future>   // std::packaged_task, std::future
  #include <thread>
  #include <utility>  // std::move

  int sum(int a, int b)
  {
    return a + b;
  }

  int main()
  {
    std::packaged_task task{ sum };
    std::future ftr = task.get_future();

    std::thread tx{ std::move(task), 11, 22 };
    // task is run asynchronously in tx thread

    std::cout << "result = " << ftr.get() << '\n';
    // output -> result = 33

    tx.join();
  }
*/

/*
  #include <future>   // std::packaged_task, std::future

  int main()
  {
    std::packaged_task task{ [](int x){ return x * 10; }};
    auto ftr = task.get_future();

    task(35); 
    // task run synchronously in main thread

    auto result = ftr.get();

    std::cout << "result = " << result << '\n';
    // output -> result = 350
  }
*/

/*
  #include <future>   // std::packaged_task, std::future

  int main()
  {
    std::packaged_task<int(int,int)> task;  // default init

    try {
      task(11, 22);
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
    // output -> 
    //  exception caught: std::future_error: No associated state
  }
*/

/*
  #include <future>       // std::packaged_task, std::future
  #include <functional>   // std::bind, std::placeholders
  #include <thread>       // std::thread

  using fn_type = int(int, int);

  int sum(int a, int b)
  {
    return a + b;
  }

  void task_lambda()
  {
  
    std::packaged_task<fn_type> task{ 
      [](int a, int b) { return a * a + b * b; } 
    };

    auto ftr = task.get_future();
    task(3, 4);

    std::cout << "task_lambda result = " 
              << ftr.get() << '\n';
  }

  void task_bind()
  {
    using fn_type_bind = int(int);

    std::packaged_task<fn_type_bind> task{
      std::bind(sum, 6, std::placeholders::_1)
    };

    auto ftr = task.get_future();
    task(8);

    std::cout << "task_bind result = " 
              << ftr.get() << '\n';
  }

  void task_thread()
  {
    std::packaged_task<fn_type> task{ sum };
    auto ftr = task.get_future();

    std::thread tx{ std::move(task), 11, 22 };
    tx.join();

    std::cout << "task_thread result = " 
              << ftr.get() << '\n';
  }

  int main()
  {
    task_lambda();  // output -> task_lambda result = 25
    // synchronous task (main thread)

    task_bind();    // output -> task_bind result = 14
    // synchronous task (main thread)

    task_thread();  // output -> task_thread result = 33
    // asynchronous task (tx thread)
  }
*/

/*
  #include <future>       // std::packaged_task, std::future
  #include <thread>       // std::jthread
  #include <syncstream>   // std::osyncstream

  int fib(int N)
  {
    return (N < 3) ? 1 : fib(N - 1) + fib(N - 2);
  }

  int main()
  {
    std::packaged_task task_fib{ fib };
    auto ftr = task_fib.get_future();

    std::jthread jtx{ std::move(task_fib), 45 };
    // asynchronous task (jtx thread)

    std::osyncstream{ std::cout } 
      << "main thread is waiting jtx task to end...\n";
    std::cout << "result = " << ftr.get() << '\n';

    // output ->
    //  main thread is waiting jtx task to end...
    //  result = 1134903170
  }
*/

/*
  #include <utility>      // std::pair
  #include <future>       // std::packaged_task
  #include <vector> 
  #include <syncstream>   // std::osyncstream
  #include <thread>       // std::jthread
  #include <functional>   // std::ref

  using ii_pair = std::pair<int, int>;
  using fn_type = int(int, int);

  void func(std::packaged_task<fn_type>& task, 
            const std::vector<ii_pair>& pair_vec)
  {
    std::osyncstream os{ std::cout };

    for (const auto [elem1, elem2] : pair_vec) 
    {
      auto ftr = task.get_future();
      task(elem1, elem2);

      os  << elem1 << " * " << elem1 << " + " << elem2 
          << " = " << ftr.get() << '\n';

      task.reset(); 
      // for using same future object more than once
    }
  }

  int main()
  {
    std::vector<ii_pair> pair_vec;
    pair_vec.reserve(5);

    pair_vec.emplace_back(3, 4);
    pair_vec.emplace_back(5, 6);
    pair_vec.emplace_back(7, 8);
    pair_vec.emplace_back(9, 10);
    pair_vec.emplace_back(11, 12);

    std::packaged_task<fn_type> task{ 
      [](int a, int b) { return a * a + b * b; } 
    };

    std::jthread jtx{ func, std::ref(task), pair_vec };
    // asynchronous task (jtx thread)

    func(task, pair_vec);
    // synchronous task (main thread)

    // output ->
    //  3 * 3 + 4 = 25
    //  5 * 5 + 6 = 61
    //  7 * 7 + 8 = 113
    //  9 * 9 + 10 = 181
    //  11 * 11 + 12 = 265
    //  3 * 3 + 4 = 25
    //  5 * 5 + 6 = 61
    //  7 * 7 + 8 = 113
    //  9 * 9 + 10 = 181
    //  11 * 11 + 12 = 265
  }
*/

/*
  #include <future>   // std::packaged_task, std::future  
  #include <vector>
  #include <utility>  // std::move
  #include <thread>   // std::jthread

  struct Sum_Functor{
    int operator()(int first, int last) const
    {
      int sum{};
      for (int i = first; i <= last; ++i)
        sum += i;

      return sum;
    }
  };

  int main()
  {
    using fn_type = int(int, int);

    Sum_Functor sum_1, sum_2, sum_3;

    std::packaged_task task1{ sum_1 }, task2{ sum_2 }, 
                                        task3{ sum_3 };

    auto ftr1 = task1.get_future();
    auto ftr2 = task2.get_future();
    auto ftr3 = task3.get_future();

    std::vector<std::packaged_task<fn_type>> task_vec;
    task_vec.reserve(5);

    task_vec.push_back(std::move(task1));
    task_vec.push_back(std::move(task2));
    task_vec.push_back(std::move(task3));

    int first = 1, last = 100;
    while (!task_vec.empty())
    {
      auto task = std::move(task_vec.back());
      task_vec.pop_back();

      std::jthread tx{ std::move(task), first++, last-- };
      tx.detach();
    }

    auto total_sum = ftr1.get() + ftr2.get() + ftr3.get();
    std::cout << "total sum = " << total_sum << '\n';
    // output -> total sum = 14847
  }
*/

/*
                    ----------------------
                    | condition variable |
                    ----------------------
*/

/*
  spurious wakeups : 
    condition variable beklenilen event gerçekleştiği zaman
    bekleyen thread'in bunu kaçırmamasını garanti eder.
    belli zamanlarda bekleyen thread event gerçekleşmeden
    uyandırılabilir, bu duruma spurious wakeup denir.
    Dolayısıyla thread'in uyandığında beklenilen event'in 
    gerçekleşip gerçekleşmediğini kontrol etmesi gerekir.
*/

/*
  // naive approach to producer-consumer problem

  #include <mutex>        // std::scoped_lock, std::unique_lock
  #include <chrono>
  #include <thread>       // std::jthread
  #include <syncstream>   // std::osyncstream

  using namespace std::literals;

  int g_shared_variable{};
  std::mutex g_mtx;

  void producer()
  {
    std::this_thread::sleep_for(1500ms);

    std::scoped_lock guard{ g_mtx };
    g_shared_variable = 111;
  }


  void consumer()
  {
    std::unique_lock guard{ g_mtx };

    while(g_shared_variable == 0) {
      std::cout << "event is not ready\n";
      guard.unlock();
      std::this_thread::yield();
      std::this_thread::sleep_for(300ms);
      guard.lock();
    }

    // guard is still locked here
    std::cout << "g_shared_variable = " 
              << g_shared_variable << '\n';
  }

  int main()
  {
    std::jthread jt1{ producer };
    std::jthread jt2{ consumer };

    // output ->
    //  event is not ready
    //  event is not ready
    //  event is not ready
    //  event is not ready
    //  event is not ready
    //  g_shared_variable = 111
  }
*/

/*
  #include <chrono>
  #include <string>
  #include <thread>   // std::this_thread::sleep_for
  #include <mutex>    // std::scoped_lock, std::unique_lock
  #include <format>
  #include <thread>   // std::jthread

  using namespace std::literals;

  std::string   g_shared_data{};
  bool          g_updated_flag{ false };
  std::mutex    g_data_mtx;
  bool          g_completed_flag{ false };
  std::mutex    g_completed_mtx;

  // producer
  void receive_data()
  {
    for (int i = 1; i <= 5; ++i) 
    {
      std::cout << "receive_data thread is waiting data...\n";
      std::this_thread::sleep_for(500ms);

      std::scoped_lock guard(g_data_mtx);

      g_shared_data += std::format("chunk{:<2} ", i);
      std::cout << "recieved data : " << g_shared_data << '\n';
      g_updated_flag = true;
    }

    std::cout << "receive data thread is done\n";

    std::scoped_lock guard(g_completed_mtx);
    g_completed_flag = true;
    
  }

  // consumer_1 (will check g_updated_flag)
  void display_progress()
  {
    while (true) {
      std::cout << "display_progress thread is waiting data...\n";
      std::unique_lock guard(g_data_mtx);
      
      while (!g_updated_flag) {
        guard.unlock();
        std::this_thread::yield();
        std::this_thread::sleep_for(100ms);
        guard.lock();
      }

      g_updated_flag = false;
      std::cout << "displaying progress : " 
                << g_shared_data << '\n';
      guard.unlock();

      std::scoped_lock guard2(g_completed_mtx);
      if (g_completed_flag) {
        std::cout << "display_progress thread is done\n";
        break;
      }
    }
  }

  // consumer_2 (will check g_completed_flag)
  void process_data()
  {
    while (true) {
      std::cout << "process_data thread is waiting data...\n";

      std::unique_lock guard(g_completed_mtx);

      while(!g_completed_flag) {
        guard.unlock();
        std::this_thread::yield();
        std::this_thread::sleep_for(200ms);
        guard.lock();
      }

      guard.unlock();

      std::scoped_lock guard2(g_data_mtx);
      std::cout << "processing started : " 
                << g_shared_data << '\n';
      break;
    }
  }

  int main()
  {
    std::jthread jtx{ receive_data};
    std::jthread jtx1{ display_progress };
    std::jthread jtx2{ process_data };

    // output ->
    //  process_data thread is waiting data...
    //  display_progress thread is waiting data...
    //  receive_data thread is waiting data...
    //  recieved data : chunk1
    //  receive_data thread is waiting data...
    //  displaying progress : chunk1
    //  display_progress thread is waiting data...
    //  recieved data : chunk1  chunk2
    //  receive_data thread is waiting data...
    //  displaying progress : chunk1  chunk2
    //  display_progress thread is waiting data...
    //  recieved data : chunk1  chunk2  chunk3
    //  receive_data thread is waiting data...
    //  displaying progress : chunk1  chunk2  chunk3
    //  display_progress thread is waiting data...
    //  recieved data : chunk1  chunk2  chunk3  chunk4
    //  receive_data thread is waiting data...
    //  displaying progress : chunk1  chunk2  chunk3  chunk4
    //  display_progress thread is waiting data...
    //  recieved data : chunk1  chunk2  chunk3  chunk4  chunk5
    //  receive data thread is done
    //  displaying progress : chunk1  chunk2  chunk3  chunk4  chunk5
    //  display_progress thread is done
    //  processing started : chunk1  chunk2  chunk3  chunk4  chunk5
  }
*/

/*
  #include <condition_variable>
  #include <mutex>    // std::scoped_lock
  #include <thread>   // std::jthread
  #include <chrono>

  using namespace std::literals;

  int                     g_data{};         // shared variable
  bool                    g_ready_flag{};   // shared variable
  std::mutex              g_mtx;
  std::condition_variable g_cv;

  void producer()
  {
    {
      std::scoped_lock guard{ g_mtx };
      std::this_thread::sleep_for(1500ms);
      g_data = 111;
      g_ready_flag = true;
    }

    g_cv.notify_one();
  }

  void consumer()
  {
    {
      std::unique_lock guard{ g_mtx };
      g_cv.wait(guard, []{ return g_ready_flag; });

      // when "wait" function is called, firstly,  
      // predicate will be called to check if  
      // "notify_one" function has already been called.
    }

    std::cout << "g_data = " << g_data << '\n';
  }

  int main()
  {
    std::jthread jt1{ producer };
    std::jthread jt2{ consumer };

    // output -> g_data = 111
  }
*/

/*
  #include <condition_variable>
  #include <chrono>
  #include <syncstream>   // std::osyncstream
  #include <string>
  #include <mutex>        // std::unique_lock, std::scoped_lock
  #include <thread>       // std::jthread

  using namespace std::literals;

  std::condition_variable g_cv;
  std::mutex              g_mtx;  
  int                     g_data{};

  void waits(const std::string& id)
  {
    std::osyncstream os{ std::cout };

    std::unique_lock guard{ g_mtx };
    os << id << " is waiting...\n";

    g_cv.wait(guard, []{ return g_data == 111; });
    // when "wait" function is called,
    // first thing it does is checking the predicate
    // is already satisfied or not.
    // if "g_data" has already been set to 111
    // program will continue executing,
    // if not, it will start waiting until waking up again.
    // Everytime it wakes up, it will control the predicate 
    // if it is not satisfied, will sleep(wait) again
    // until waking up and saw that predicate is satisfied.

    os << id << " woke up... g_data = " << g_data << '\n';
  }

  void signals(const std::string& id)
  {
    std::osyncstream os{ std::cout };

    os << id << " is notifying\n";
    g_cv.notify_all();    
    // waking up consumers but predicate is not satisfied yet.
    std::this_thread::sleep_for(1s);

    {
      std::scoped_lock guard{ g_mtx };
      g_data = 111;
      os << id << " is notifying.\n";
    }

    g_cv.notify_all();
  }

  int main()
  {
    std::jthread  jt1{ waits, "consumer_1" },
                  jt2{ waits, "consumer_2" },
                  jt3{ waits, "consumer_3" },
                  jt4{ waits, "consumer_4" },
                  jt5{ signals, "producer" };
    // output ->
    //  producer is notifying
    //  producer is notifying.
    //  consumer_2 is waiting...
    //  consumer_2 woke up... g_data = 111
    //  consumer_1 is waiting...
    //  consumer_1 woke up... g_data = 111
    //  consumer_3 is waiting...
    //  consumer_3 woke up... g_data = 111
    //  consumer_4 is waiting...
    //  consumer_4 woke up... g_data = 111
  }
*/

/*
  #include <mutex>      // std::unique_lock, std::scoped_lock
  #include <condition_variable>
  #include <string>
  #include <iomanip>    // std::quoted
  #include <chrono>
  #include <thread>     // std::jthread

  using namespace std::literals;

  std::mutex                  g_mtx;
  std::condition_variable     g_cv;
  std::string                 g_str;    // shared variable

  void reader()
  {
    std::cout << "reader thread is locking the mutex\n";
    std::unique_lock guard{ g_mtx };
    std::cout << "reader thread has locked the mutex\n";

    std::cout << "reader thread started sleeping\n";
    g_cv.wait(guard, []{ return !g_str.empty(); });
    std::cout << "reader thread wakes up\n";

    std::cout << "g_str = " << std::quoted(g_str) << '\n';
    std::cout << "reader thread is unlocking the mutex\n";
  }

  void writer()
  {
    {
      std::cout << "writer thread is locking the mutex\n";

      std::scoped_lock guard{ g_mtx };
      std::cout << "writer thread has locked the mutex\n";

      std::cout << "writer thread modifying g_str\n";
      g_str = "hello";

      std::cout << "writer thread is unlocking the mutex\n";  
    }

    std::cout << "writer thread is notifying\n";
    g_cv.notify_one();
  }

  int main()
  {
    std::jthread jt1{ reader };
    std::this_thread::sleep_for(800ms);
    std::jthread jt2{ writer };

    // output ->
    //  reader thread is locking the mutex
    //  reader thread has locked the mutex
    //  reader thread started sleeping
    //  writer thread is locking the mutex
    //  writer thread has locked the mutex
    //  writer thread modifying g_str
    //  writer thread is unlocking the mutex
    //  writer thread is notifying
    //  reader thread wakes up
    //  g_str = "hello"
    //  reader thread is unlocking the mutex
  }
*/

/*
          ----------------------------------------------
          | concurrency in standart library algorithms |
          ----------------------------------------------
*/

/*
        <---- check not_related/std::accumulate ---->
*/

/*
  #include <vector>
  #include <cstddef>    // std::size_t
  #include <random>     // std::mt19937, std::random_device
  #include <algorithm>  // std::generate
  #include <chrono>     // std::chrono::steady_clock
  #include <numeric>    // std::accumulate
  #include <future>     // std::async, std::launch::async

  using ui64 = unsigned long long;

  ui64 partial_accumulate(const ui64* p_first, const ui64* p_last)
  {
    return std::accumulate(p_first, p_last, 0ULL);
  }

  ui64 concurrent_accumulate(const std::vector<ui64>& vec)
  {
    const auto p_first = vec.data();
    const auto vec_size = vec.size();

    auto ftr1 = std::async( std::launch::async, 
                            partial_accumulate, 
                            p_first, p_first + vec_size / 4);

    auto ftr2 = std::async( std::launch::async,
                            partial_accumulate,
                            p_first + (vec_size / 4), 
                            p_first + (vec_size / 2));

    auto ftr3 = std::async( std::launch::async, 
                            partial_accumulate, 
                            p_first + (vec_size / 2),
                            p_first + 3 * (vec_size / 4));

    auto ftr4 = std::async( std::launch::async,
                            partial_accumulate,
                            p_first + 3 * (vec_size / 4),
                            p_first + vec_size);

    return ftr1.get() + ftr2.get() + ftr3.get() + ftr4.get();
  }

  int main()
  {
    std::mt19937 eng;
    std::uniform_int_distribution dist{ 0ULL, 50ULL };

    constexpr std::size_t N = 200'000'000ULL;
    std::vector<ui64> uvec(N);

    std::generate(uvec.begin(), uvec.end(), 
                  [&eng, &dist]{ return dist(eng); }); 

    // -------------------------------------------------------

    auto tp_start = std::chrono::steady_clock::now();
    auto sum = std::accumulate(uvec.begin(), uvec.end(), 0ULL);
    auto tp_end = std::chrono::steady_clock::now();

    std::cout << "accumulate duration without threads = " 
      << std::chrono::duration<double>(tp_end - tp_start) 
      << '\n';
    // output -> accumulate duration without threads = 1.32412s

    // -------------------------------------------------------

    auto tp_start_conc = std::chrono::steady_clock::now();
    auto sum_conc = concurrent_accumulate(uvec);
    auto tp_end_conc = std::chrono::steady_clock::now();

    std::cout << "accumulate duration with threads = " 
      << std::chrono::duration<double>(tp_end_conc - tp_start_conc) 
      << '\n';
    // output -> accumulate duration with threads = 0.117924s

    // -------------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <cstddef>    // std::size_t
  #include <random>     // std::mt19937, std::random_device
  #include <algorithm>  // std::generate
  #include <chrono>     // std::chrono::steady_clock
  #include <numeric>    // std::accumulate
  #include <future>     // std::packaged_task
  #include <thread>     // std::jthread
  #include <utility>    // std::move

  using ui64 = unsigned long long;

  ui64 partial_accumulate(const ui64* p_first, const ui64* p_last)
  {
    return std::accumulate(p_first, p_last, 0ULL);
  }

  ui64 concurrent_accumulate(const std::vector<ui64>& vec)
  {
    using fn_type = ui64(const ui64*, const ui64*);
    using task_t = std::packaged_task<fn_type>;

    task_t task1(partial_accumulate);
    task_t task2(partial_accumulate);
    task_t task3(partial_accumulate);
    task_t task4(partial_accumulate);

    auto ftr1 = task1.get_future();
    auto ftr2 = task2.get_future();
    auto ftr3 = task3.get_future();
    auto ftr4 = task4.get_future();

    auto p_first = vec.data();
    auto vec_size = vec.size();

    std::jthread jt1( std::move(task1), 
                      p_first, 
                      p_first + (vec_size / 4));

    std::jthread jt2( std::move(task2),
                      p_first + (vec_size / 4),
                      p_first + (vec_size / 2));
    
    std::jthread jt3( std::move(task3),
                      p_first + (vec_size / 2),
                      p_first + 3 * (vec_size / 4));

    std::jthread jt4( std::move(task4),
                      p_first + 3 * (vec_size / 4),
                      p_first + vec_size);

    return ftr1.get() + ftr2.get() + ftr3.get() + ftr4.get();
  }

  int main()
  {
    std::mt19937 eng;
    std::uniform_int_distribution dist{ 0ULL, 50ULL };

    constexpr std::size_t N = 400'000'000ULL;
    std::vector<ui64> uvec(N);

    std::generate(uvec.begin(), uvec.end(), 
                  [&eng, &dist]{ return dist(eng); }); 

    // -------------------------------------------------------

    auto tp_start = std::chrono::steady_clock::now();
    auto sum = std::accumulate(uvec.begin(), uvec.end(), 0ULL);
    auto tp_end = std::chrono::steady_clock::now();

    std::cout << "accumulate duration without threads = " 
      << std::chrono::duration<double>(tp_end - tp_start) 
      << '\n';
    // output -> accumulate duration without threads = 2.65025s

    // -------------------------------------------------------

    auto tp_start_conc = std::chrono::steady_clock::now();
    auto sum_conc = concurrent_accumulate(uvec);
    auto tp_end_conc = std::chrono::steady_clock::now();

    std::cout << "accumulate duration with threads = " 
      << std::chrono::duration<double>(tp_end_conc - tp_start_conc) 
      << '\n';
    // output -> accumulate duration with threads = 0.275826s

    // -------------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <cstddef>    // std::size_t
  #include <random>     // std::mt19937, std::random_device
  #include <algorithm>  // std::generate
  #include <chrono>
  #include <numeric>    // std::accumulate, std::reduce
  #include <future>
  #include <execution>  // std::reduce

  using ui64 = unsigned long long;

  int main()
  {
    std::mt19937 eng;
    std::uniform_int_distribution dist{ 0ULL, 50ULL };

    constexpr std::size_t N = 400'000'000ULL;
    std::vector<ui64> uvec(N);

    std::generate(uvec.begin(), uvec.end(), 
                  [&eng, &dist]{ return dist(eng); }); 

    // -------------------------------------------------------

    auto tp_start = std::chrono::steady_clock::now();

    auto sum = std::accumulate( uvec.begin(), uvec.end(), 0ULL, 
                      [](auto a, auto b){ return a * a + b * b; });

    auto tp_end = std::chrono::steady_clock::now();

    std::cout << "accumulate duration without threads = " 
      << std::chrono::duration<double>(tp_end - tp_start) 
      << '\n';
    // output -> accumulate duration without threads = 3.07354s

    // -------------------------------------------------------

    auto tp_start_conc = std::chrono::steady_clock::now();

    auto sum_conc = std::reduce(std::execution::par, 
                                uvec.begin(), uvec.end(), 0ULL, 
                      [](auto a, auto b){ return a * a + b * b; });

    auto tp_end_conc = std::chrono::steady_clock::now();

    std::cout << "accumulate duration with threads = " 
      << std::chrono::duration<double>(tp_end_conc - tp_start_conc) 
      << '\n';
    // output -> accumulate duration with threads = 2.34186s

    // -------------------------------------------------------
  }
*/

/*
  #include <execution>    
  // std::execution::sequenced_policy             (C++17)
  // std::execution::parallel_policy              (C++17)
  // std::execution::parallel_unsequenced_policy  (C++17)
  // std::execution::unsequenced_policy           (C++20)
  #include <vector>
  #include <algorithm>    // std::sort

  int main()
  {
    std::vector<int> ivec(100000);

    // ---------------------------------------------------

    std::sort(std::execution::sequenced_policy{}, 
              ivec.begin(), ivec.end());
    std::sort(std::execution::seq,
              ivec.begin(), ivec.end());
    // Those 2 lines are equivalent.

    // ---------------------------------------------------

    std::sort(std::execution::parallel_policy{}, 
              ivec.begin(), ivec.end());
    std::sort(std::execution::par,
              ivec.begin(), ivec.end());
    // Those 2 lines are equivalent.

    // ---------------------------------------------------

    std::sort(std::execution::parallel_unsequenced_policy{}, 
              ivec.begin(), ivec.end());
    std::sort(std::execution::par_unseq,
              ivec.begin(), ivec.end());
    // Those 2 lines are equivalent.

    // ---------------------------------------------------

    
    std::sort(std::execution::unsequenced_policy{}, 
              ivec.begin(), ivec.end());
    std::sort(std::execution::unseq, 
              ivec.begin(), ivec.end());  
    // Those 2 lines are equivalent.

    // ---------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <algorithm>  // std::sort
  #include <execution>  // std::execution::seq

  int main()
  {
    std::vector<int> ivec(100'000);

    std::sort(ivec.begin(), ivec.end());    // overload (1)

    std::sort(std::execution::seq, 
              ivec.begin(), ivec.end());    // overload (2)

    // both overload are running the algorithm sequencial 
    // in the same thread
    // difference between those 2 overload
    // when an exception thrown in overload (1), it CAN BE caught.
    // when an exception thrown in overload (2), 
    //  std::terminate will be called. 
  }
*/

/*
  #include <vector>
  #include <algorithm>
  #include <execution>  // std::execution::seq
  #include <stdexcept>  // std::runtime_error

  int main()
  {
    std::vector ivec{ 11, 22, 33, 44, 55, 66, 77, 88, 99 };

    try {
      std::sort(ivec.begin(), ivec.end(), 
              [](int a, int b){ 
                if (a % 2 == b % 2) 
                  throw std::runtime_error{ "error in std::sort "};
                return a < b;
              });
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
    // output -> exception caught: error in std::sort
  }
*/

/*
  #include <vector>
  #include <algorithm>
  #include <execution>  // std::execution::seq
  #include <stdexcept>  // std::runtime_error
  #include <exception>  // std::set_terminate
  #include <cstdlib>    // std::abort

  void terminate_func()
  {
    std::cout << "terminate_func is called\n";
    std::cout << "abort will be called\n";
    std::abort();
  }

  int main()
  {
    std::set_terminate(terminate_func);
    std::vector ivec{ 11, 22, 33, 44, 55, 66, 77, 88, 99 };

    try {
      std::sort(std::execution::seq, ivec.begin(), ivec.end(), 
              [](int a, int b){ 
                if (a % 2 == b % 2) 
                  throw std::runtime_error{ "error in std::sort "};
                return a < b;
              });
    }
    catch (const std::exception& ex) {
      std::cout << "exception caught: " << ex.what() << '\n';
    }
  }
  // output ->
  //  terminate_func is called
  //  abort will be called
*/

/*
  std::execution::parallel_policy, argüman olarak gönderildiğinde
  algoritma koşulların uygun olmaması sebebi ile
  std::execution::sequenced_policy olarak da çalıştırılabilir.

  std::execution::parallel_unsequenced_policy : 
    vektörizasyon ve ilave threadler arasındaki aktarımlar
    için müsaade verilmiş olunur.

  std::execution::unsequenced_policy :
    vektörizasyon kullanılır fakat ayrı threadler kullanılmaz.

  parallel policy'lerde doğrusal bir yapı olma zorunluluğu 
  ortadan kalkar. Öğelerin hangi sırayla işleme sokulacağı 
  belirsizdir. dolayısıyla senkronizasyon işlemi programcının 
  sorumluluğundadır.
*/

/*
  #include <vector>
  #include <random>
  // std::mt19937, std::uniform_int_distribution
  #include <algorithm>  // std::generate, std::sort
  #include <execution>
  // std::execution::seq, std::execution::unseq,
  // std::execution::par, std::execution::par_unseq
  #include <chrono>     
  // std::chrono::steady_clock, std::chrono::duration

  namespace exec = std::execution;  // namespace alias
  using dsec = std::chrono::duration<double>;

  int main()
  {
    constexpr std::size_t N = 10'000'000;
    std::vector<double> dvec(N);

    std::mt19937 eng;
    std::uniform_real_distribution dist{ -50.0, 50.0 };
    std::generate(dvec.begin(), dvec.end(), 
                  [&eng, &dist]{ return dist(eng); });

    // -----------------------------------------------------

    auto tp_start = std::chrono::steady_clock::now(); 
    std::sort(std::execution::seq, 
              dvec.begin(), dvec.end());
    auto tp_end = std::chrono::steady_clock::now(); 

    std::cout << dsec(tp_end - tp_start).count() << "seconds\n";
    // output -> 3.4896seconds

    // -----------------------------------------------------

    tp_start = std::chrono::steady_clock::now(); 
    std::sort(std::execution::unseq, 
              dvec.begin(), dvec.end());
    tp_end = std::chrono::steady_clock::now(); 

    std::cout << dsec(tp_end - tp_start).count() << "seconds\n";
    // output -> 2.01572seconds

    // -----------------------------------------------------

    tp_start = std::chrono::steady_clock::now(); 
    std::sort(std::execution::par, 
              dvec.begin(), dvec.end());
    tp_end = std::chrono::steady_clock::now(); 

    std::cout << dsec(tp_end - tp_start).count() << "seconds\n";
    // output -> 2.00061seconds
    
    // -----------------------------------------------------

    tp_start = std::chrono::steady_clock::now(); 
    std::sort(std::execution::par_unseq, 
              dvec.begin(), dvec.end());
    tp_end = std::chrono::steady_clock::now(); 

    std::cout << dsec(tp_end - tp_start).count() << "seconds\n";
    // output -> 1.9991seconds

    // -----------------------------------------------------
  }
*/

/*
        <---- check images/accumulate_parallel.png ---->
*/

/*
  #include <random>     // std::mt19937
  #include <vector>
  #include <execution>
  #include <algorithm>  // std::for_each
  #include <cstddef>    // std::size_t
  #include <mutex>      // std::scoped_lock
  #include <iostream>

  std::mutex g_mtx;

  int main()
  {
    std::cout << std::boolalpha;

    constexpr std::size_t N = 10'000'000;

    std::vector<unsigned int> ivec1(N);
    std::vector<unsigned int> ivec2(N);

    std::mt19937 eng;

    eng.seed(111u);
    std::for_each(std::execution::par, ivec1.begin(), ivec1.end(), 
                  [&eng](unsigned int& x){
                    (void)std::scoped_lock{ g_mtx };
                    x = eng();
                  });

    eng.seed(111u);
    std::for_each(std::execution::par, ivec2.begin(), ivec2.end(), 
                  [&eng](unsigned int& x){
                    (void)std::scoped_lock{ g_mtx };
                    x = eng();
                  });

    std::cout << "ivec1 == ivec2 : " 
              << (ivec1 == ivec2) << '\n';
    // output -> ivec1 == ivec2 : false (not guaranteed)
    
    // -----------------------------------------------------  

    std::sort(ivec1.begin(), ivec1.end());  
    std::sort(ivec2.begin(), ivec2.end());  

    std::cout << "ivec1 == ivec2 : " 
          << (ivec1 == ivec2) << '\n';
    // output -> ivec1 == ivec2 : true

    // -----------------------------------------------------  
 }
*/

/*
                    -------------------------
                    | std::reduce algorithm |
                    -------------------------
*/

/*
  std::reduce is a concurrent version of std::accumulate.
*/

/*
  // differences between std::accumulate, std::reduce

  #include <numeric>    // std::accumulate, std::reduce
  #include <vector>

  int main()
  {
    std::vector ivec{ 11, 33, 55, 77, 99 };

    std::accumulate(ivec.begin(), ivec.end());  // syntax error
    //  error: no matching function for call to 
    // 'accumulate(std::vector<int, std::allocator<int>>::iterator, 
    //             std::vector<int, std::allocator<int> >::iterator)'

    std::reduce(ivec.begin(), ivec.end());
    std::accumulate(ivec.begin(), ivec.end(), int{});
    // Those 2 lines are equivalent.
  }
*/

/*
  std::accumulate only have 
    input iterator(InIter) parameter overload.
  std::reduce also have 
    forward iterator(ForIter) parameter overload.
*/

/*
  #include <vector>
  #include <random>   
  // std::mt19937, std::uniform_int_distribution
  #include <execution>
  #include <algorithm>  // std::generate
  #include <numeric>    // std::reduce
  #include <chrono>     // std::chrono::steady_clock

  namespace exec = std::execution;
  using namespace std::chrono;

  int main()
  {
    std::vector<unsigned int> ivec(100'000'000);
    std::mt19937 eng;
    std::uniform_int_distribution dist{ 0u, 10u };

    std::generate(exec::par, 
                  ivec.begin(), ivec.end(), 
                  [&]{ return dist(eng); });


    // ----------------------------------------------------

    auto tp_start = std::chrono::steady_clock::now(); 

    auto result = 
      std::reduce(exec::seq, 
                  ivec.begin(), ivec.end(), 
                  0u, 
                  [](auto a, auto b){ return a * a + b * b; });

    auto tp_end = std::chrono::steady_clock::now();

    std::cout 
      << duration_cast<milliseconds>(tp_end - tp_start).count() 
      << " milliseconds\n";
    // exec::seq output -> 603 milliseconds
    // exec::par output -> 556 milliseconds

    std::cout << "result = " << result << '\n';
    // output -> result = 1451211306

    // ----------------------------------------------------
  }
*/

/*
              -----------------------------------
              | std::transform_reduce algorithm |
              -----------------------------------
*/

/*
        <---- check not_related/std::transform ---->
        <---- check not_related/std::inner_product ---->
*/

/*
  // parallel version of std::inner_product algorithm

  #include <numeric>    // std::transform_reduce
  #include <vector>
  #include <execution>
  #include <functional> // std::plus, std::multiplies

  int main()
  {
    std::vector<int> ivec1{ 1, 2, 3, 4, 5 };
    std::vector<int> ivec2{ 6, 7, 8, 9, 10 };
    // (1 * 6) + (2 * 7) + (3 * 8) + (4 * 9) + (5 * 10) = 130

    // ------------------------------------------------

    auto result = std::transform_reduce( 
        std::execution::par,
        ivec1.begin(), ivec1.end(),
        ivec2.begin(),
        0);

    std::cout << "inner product of x and y = " 
              << result << '\n';
    // output -> inner product of x and y = 130

    // ------------------------------------------------

    result = std::transform_reduce( 
        std::execution::par,
        ivec1.begin(), ivec1.end(),
        ivec2.begin(),
        0,
        std::plus{}, std::multiplies{});

    std::cout << "inner product of x and y = " 
              << result << '\n';
    // output -> inner product of x and y = 130

    // ------------------------------------------------
  }
*/

/*
  #include <numeric>    
  // std::inner_product, std::transform_reduce
  #include <vector>
  #include <string>
  #include <functional> // std::plus, std::equal_to
  #include <execution>

  int main()
  {
    std::vector<std::string> svec1{ 
        "aa", "bb", "cc", "dd", "ee", "ab", "cd", "ef", "gh" };
      
    std::vector<std::string> svec2{ 
        "aa", "cb", "cc", "ef", "gg", "xx", "yy", "ef", "zz" };

    // to find out which elements 
    // are common in both vectors with same index

    auto count = std::transform_reduce( 
                    std::execution::par,
                    svec1.begin(), svec1.end(), 
                    svec2.begin(), 
                    0,
                    std::plus{}, std::equal_to{});

    std::cout << "count = " << count << '\n'; 
    // output -> count = 3

    // transform  => equal_to
    // reduce     => plus
  }
*/

/*
      -------------------------------------------------------
      | std::inclusive_scan, std::exclusive_scan algorithms |
      -------------------------------------------------------
*/

/*
            <--- check not_related/std::partial_sum --->
*/

/*
  #include <vector>
  #include <numeric>    
  // std::inclusive_scan, std::exclusive_scan
  #include <iterator>   // std::ostream_iterator
  #include <functional> // std::multiplies
  #include <execution>

  int main()
  {
    std::vector ivec{ 1, 5, 9, 12, 17 };

    // ------------------------------------------------------

    std::inclusive_scan( 
        std::execution::par,
        ivec.begin(), ivec.end(), 
        std::ostream_iterator<int>{ std::cout, " " });
    // output -> 1 6 15 27 44
    // (1), (1 + 6), (7 + 9), (15 + 12), (27 + 17)

    std::cout << '\n';

    // ------------------------------------------------------

    std::exclusive_scan( 
        std::execution::par,
        ivec.begin(), ivec.end(), 
        std::ostream_iterator<int>{ std::cout, " " },
        100);
    // output -> 100 101 106 115 127
    // (100), (100 + 1), (100 + 6), (100 + 15), (100 + 27)

    // ------------------------------------------------------
  }
*/

// --------------------------------------------------------
// --------------------------------------------------------
// --------------------------------------------------------
// --------------------------------------------------------
// --------------------------------------------------------

/*
  #include "thread_pool.h"
  #include <syncstream>   // std::osyncstream
  #include <thread>       // std::this_thread::sleep_for
  #include <chrono>       // std::chrono::milliseconds
  #include <cstddef>      // std::size_t

  using namespace std::chrono_literals;

  void task_fn()
  {

    std::osyncstream{ std::cout } 
      << "thread id = " << std::this_thread::get_id() 
      << " started running the task\n";

    std::this_thread::sleep_for(300ms);

    std::osyncstream{ std::cout } 
      << "thread id = " << std::this_thread::get_id() 
      << " finished running the task\n";
  }

  int main()
  {
    const auto thread_count = std::thread::hardware_concurrency();
    std::cout << "thread pool includes " 
              << thread_count << " threads\n";

    Thread_Pool tx_pool;
    for (std::size_t i{}; i < 2 * thread_count; ++i)
      tx_pool.push(task_fn);

    std::this_thread::sleep_for(5s);

    // output -> 
    //  thread pool includes 12 threads
    //  thread id = 2 started running the task
    //  thread id = 4 started running the task
    //  thread id = 3 started running the task
    //  thread id = 7 started running the task
    //  thread id = 5 started running the task
    //  thread id = 6 started running the task
    //  thread id = 10 started running the task
    //  thread id = 9 started running the task
    //  thread id = 8 started running the task
    //  thread id = 11 started running the task
    //  thread id = 12 started running the task
    //  thread id = 13 started running the task
    //  thread id = 11 finished running the task
    //  thread id = 6 finished running the task
    //  thread id = 5 finished running the task
    //  thread id = 5 started running the task
    //  thread id = 10 finished running the task
    //  thread id = 4 finished running the task
    //  thread id = 4 started running the task
    //  thread id = 3 finished running the task
    //  thread id = 3 started running the task
    //  thread id = 12 finished running the task
    //  thread id = 12 started running the task
    //  thread id = 2 finished running the task
    //  thread id = 2 started running the task
    //  thread id = 9 finished running the task
    //  thread id = 7 finished running the task
    //  thread id = 7 started running the task
    //  thread id = 13 finished running the task
    //  thread id = 13 started running the task
    //  thread id = 8 finished running the task
    //  thread id = 6 started running the task
    //  thread id = 9 started running the task
    //  thread id = 10 started running the task
    //  thread id = 11 started running the task
    //  thread id = 8 started running the task
    //  thread id = 5 finished running the task
    //  thread id = 12 finished running the task
    //  thread id = 9 finished running the task
    //  thread id = 6 finished running the task
    //  thread id = 13 finished running the task
    //  thread id = 3 finished running the task
    //  thread id = 8 finished running the task
    //  thread id = 10 finished running the task
    //  thread id = 2 finished running the task
    //  thread id = 4 finished running the task
    //  thread id = 11 finished running the task
    //  thread id = 7 finished running the task
  }
*/

/*
                      -------------------
                      | <atomic> module | 
                      -------------------
*/

/*
  paylaşımlı kullanılan değişkenler için herhangi bir 
  senkronizasyon mekanizması olmadığında ve birden fazla 
  thread bu değişkenlere okuma amaçlı erişim sağlayıp, 
  en az 1 thread yazma amaçlı erişim sağlıyor ise 
  data race oluşur(tanımsız davranış).

                    ----------------------

  torn read : değişkeni ne eski değerinde ne de yeni değerinde 
              okumak(yazma işlemi sırasında okuma işlemi yapmak).

  torn write :  değişkene yazma işlemi yapılırken(henüz bitmemiş)
                tekrardan yazmak.

                    ----------------------

  değişkenin atomic olması, interleaving rsikini ortadan kaldırır.
  atomic değişken üzerinde, bir thread read-modify-write işlemleri 
  yaparken, başka bir thread herhangi bir işlem yapamaz.
  torn read ve torn write durumları oluşmaz.

                    ----------------------

  sequenced before : 
    (tek bir thread ile ilgili - Intrathread relation)

  bir threaddeki işlemlerin aralarındaki 
  sequence point ilişkileri.

  happens before : 
    (tek veya birden fazla thread ile ilgili olabilir.)
    visibility ile alakalı bir terim.

    thread_A            thread_B
    operation_1         operation_2

    operation_1 ve operation_2 arasında happens before ilişkisi
    operation_1'in oluşturduğu sonuç operation_2 tarafından
    gözlenebilir bir durumda olacak.

    zamansal olarak operation_1 in daha önce gerçekleşmiş 
    olması durumu, operation_2 esnasında operation_1'in 
    sonucunun gözlemlenebileceği anlamına gelmez.

    örneğin operation_1'deki değişiklik cache'te yapılıp henüz
    belleğe yazılmamıştır, bu esnada operation_2 başlar 
    ve operation_1'de değiştirilen fakat belleğe yazılmamış olan
    değeri okur. bu durumda operation_1'in sonucu operation_2
    tarafından gözlenemez.

                    ----------------------

    sequencial consistency : birden fazla thread olsa da,
    bu threadlerdeki işlemlerin global bir gerçekleşme sırası var
    ve her thread aynı sırayı izliyor(gözlüyor).

            <---- check sequencial_consistency.png ---->

    atomik değişkenlerdeki en yüksek garanti.

                    ----------------------
*/

/*
  #include <atomic>

  int main()
  {
    std::atomic_flag flag;
    // "std::atomic_flag" is guaranteed to be lock-free
    // there is not any mutex used when std::atomic_flag is used.
    // std::atomic_flag is a class type.

    std::atomic<int*> iptr;
    // "std::atomic" is a class template.
    // is NOT guaranteed to be lock-free.
  }
*/

/*
  // primary template for user-defined types and bool
  template <typename T>
  struct Atomic {};

  // explicit specialization for integral types
  template <> struct Atomic<int> {};
  template <> struct Atomic<long> {};
  template <> struct Atomic<char> {};

  // partial specialization for pointer types
  template <typename T>
  struct Atomic<T*> {};
*/

/*
  #include <atomic>

  struct AStruct {
    int m_a, m_b;
  };

  int main()
  {
    // -------------------------------------------------

    std::atomic<bool> a1;     // primary template
    std::atomic<AStruct> a2;  // primary template

    // -------------------------------------------------

    // integral types are different explicit specializations
    std::atomic<int> a3;      // explicit specialization
    std::atomic<long> a4;     // explicit specialization

    // -------------------------------------------------

    int i_arr[5]{};
    std::atomic<int*> a5{ i_arr };    
     // partial specialization

    // -------------------------------------------------
  }
*/

/*
  #include <atomic>   // std::atomic_flag, ATOMIC_FLAG_INIT

  int main()
  {
    std::cout << std::boolalpha;

    std::atomic_flag flag = ATOMIC_FLAG_INIT;

    // ----------------------------------------------------

    flag.clear();           
    // will clear the flag

    // ----------------------------------------------------

    bool b1 = flag.test_and_set();    
    // will set the flag and return the previous value  
    // if the flag was set before, it will return true
    // if the flag was not set before, it will return false

    std::cout << "b1 = " << b1 << '\n';
    // output -> b1 = false

    bool b2 = flag.test_and_set();
    std::cout << "b2 = " << b2 << '\n';
    // output -> b2 = true

    // ----------------------------------------------------
  }
*/

/*
  #include <atomic>

  int main()
  {
    std::cout << std::boolalpha;

    std::atomic_flag flag1;   // default initialization
    // default initialization after C++20 
    // will initialize flag's value with "false"

    std::cout << flag1.test() <<'\n';   // output -> false

    while (flag1.test_and_set())
      ; // null statement

    // if flag1 is set, this loop will be an infinite loop.
    // if any other thread will clear the flag1,
    // loop will be terminated.
  }
*/

/*
  #include <atomic>   // std::atomic_flag
  #include <vector>
  #include <thread>   // std::jthread

  class SpinLockMutex {
  private:
    std::atomic_flag m_flag;

  public:
    SpinLockMutex()
    {
      m_flag.clear();
    }

    void lock() 
    {
      while (m_flag.test_and_set())
        ; // null statement
    }

    void unlock() 
    {
      m_flag.clear();
    }
  };

  SpinLockMutex g_mtx;
  unsigned long long g_var{};     // shared variable

  void func_1()
  {
    for (int i = 0; i < 1'000'000; ++i) 
      ++g_var;
  }

  void func_2()
  {
    for (int i = 0; i < 1'000'000; ++i) 
    {
      g_mtx.lock();
      ++g_var;
      g_mtx.unlock();
    }
  }

  int main()
  {
    {
      std::jthread jt1(func_1);
      std::jthread jt2(func_1);
      std::jthread jt3(func_1);
      std::jthread jt4(func_1);
      std::jthread jt5(func_1);   
    }

    std::cout << "g_var = " << g_var << '\n';
    // output -> g_var = 1104273  ---> data race

    g_var = 0;

    {
      std::jthread jt1(func_2);
      std::jthread jt2(func_2);
      std::jthread jt3(func_2);
      std::jthread jt4(func_2);
      std::jthread jt5(func_2);   
    }

    std::cout << "g_var = " << g_var << '\n';
    // output -> g_var = 5000000
  }
*/

/*
  #include <atomic>
  #include <thread>   // std::jthread

  int main()
  {
    // ----------------------------------------------------

    std::atomic<int> a1 = 10;
    // a1's type is std::atomic_int(type alias)
    std::atomic a2      = 20;   // CTAD
    // a2's type is std::atomic_int

    // ----------------------------------------------------

    int ival = 0;

    auto fn1 = [&ival] {
      for (int i = 0; i < 100'000; ++i)
        ++ival;
    };

    {
      std::jthread jt1{ fn1 };
      std::jthread jt2{ fn1 };
      std::jthread jt3{ fn1 };
      std::jthread jt4{ fn1 };
    }

    std::cout << "ival = " << ival << '\n';
    // output -> ival = 136549

    // ----------------------------------------------------

    std::atomic_int a_ival = 0;

    auto fn2 = [&a_ival] {
      for (int i = 0; i < 100'000; ++i)
        ++a_ival;
    };

    {
      std::jthread jt1{ fn2 };
      std::jthread jt2{ fn2 };
      std::jthread jt3{ fn2 };
      std::jthread jt4{ fn2 };
    }

    std::cout << "a_ival = " << a_ival << '\n';
    // output -> a_ival = 400000

    // operator++() - pre-increment is an atomic operation

    // ----------------------------------------------------

    a_ival = 0;

      auto fn3 = [&a_ival] {
      for (int i = 0; i < 100'000; ++i)
        a_ival++;
    };

    {
      std::jthread jt1{ fn3 };
      std::jthread jt2{ fn3 };
      std::jthread jt3{ fn3 };
      std::jthread jt4{ fn3 };
    }

    std::cout << "a_ival = " << a_ival << '\n';
    // output -> a_ival = 400000

    // operator++(int) - post-increment is an atomic operation

    // ----------------------------------------------------

    a_ival = 0;

      auto fn4 = [&a_ival] {
      for (int i = 0; i < 100'000; ++i)
        a_ival += 1;
    };

    {
      std::jthread jt1{ fn4 };
      std::jthread jt2{ fn4 };
      std::jthread jt3{ fn4 };
      std::jthread jt4{ fn4 };
    }

    std::cout << "a_ival = " << a_ival << '\n';
    // output -> a_ival = 400000

    // operator+= is an atomic operation

    // ----------------------------------------------------

      a_ival = 0;

      auto fn5 = [&a_ival] {
      for (int i = 0; i < 100'000; ++i)
        a_ival = a_ival + 1;
        // a_ival.store(a_ival.load() + 1);
    };

    {
      std::jthread jt1{ fn5 };
      std::jthread jt2{ fn5 };
      std::jthread jt3{ fn5 };
      std::jthread jt4{ fn5 };
    }

    std::cout << "a_ival = " << a_ival << '\n';
    // output -> a_ival = 166707

    // 1. operator int() function is atomic (getting a_ival's value)
    // 2. operator+ for int is NOT an atomic operation
    // 3. setting a_ival's value is an atomic operation

    // ----------------------------------------------------
  }
*/

/*
  #include <atomic>

  int main()
  {
    // ------------------------------------------------------

    // assigment from non-atomic to atomic  VALID
    // assigment from atomic to non-atomic  VALID
    // asignment from atomic to atomic      SYNTAX ERROR

    // ------------------------------------------------------

    std::atomic a1 = 5;
    std::atomic a2 = 10;

    auto a3 = a1; // syntax error
    //  error: use of deleted function 
    //  'std::atomic<int>::atomic(const std::atomic<int>&)'
    // NOT copy constructible

    a1 = a2;  // syntax error (not )
    // error: use of deleted function 
    // 'std::atomic<int>& std::atomic<int>::operator=(
    //  const std::atomic<int>&)'
    // NOT copy assignable

    // ------------------------------------------------------

    std::atomic a4 = 10;
    std::atomic a5 = 20;

    a4 = a5.load();   // assingment from non-atomic to atomic
    int ival = a4;    // assigment from atomic to non-atomic   

    // ------------------------------------------------------
  }
*/

/*
  #include <atomic>
  #include <thread>   // std::jthread

  class Atomic_Counter {
  private:
    std::atomic<int> m_counter;

  public:
    Atomic_Counter() : m_counter(0) {}
    Atomic_Counter(int val) : m_counter(val) {}

    int operator++(){ return ++m_counter; }
    // ++expr is not LValue expression, it is RValue expression!
    // returning int, NOT Atomic_Counter&

    int operator--(){ return --m_counter; }
    // --expr is not LValue expression, it is RValue expression!
    // returning int, NOT Atomic_Counter&

    int operator++(int){ return m_counter++; }
    int operator--(int){ return m_counter--; }

    operator int() const { return m_counter.load(); }
    int get() const { return m_counter.load(); }
  };

  Atomic_Counter g_ac = 0;

  void foo()
  {
    for (int i = 0; i < 10'000; ++i)
      ++g_ac;
  }

  void bar()
  {
    for (int i = 0; i < 10'000; ++i)
      --g_ac;
  }

  int main()
  {
    {
      std::jthread jt1{ foo };
      std::jthread jt2{ bar };
    }

    std::cout << "g_ac = " << g_ac << '\n';
    // output -> g_ac = 0
  }
*/

/*
  // g++ -o prog main.cpp -std=c++23 -latomic

  #include <atomic>
  #include <memory>   // std::shared_ptr

  struct AStruct {
    int m_a, m_b, m_c;
  };

  int main()
  {
    std::cout << std::boolalpha;

    std::atomic<int> a1;
    std::cout << "a1.is_lock_free() = " 
              << a1.is_lock_free() << '\n';
    // output -> a1.is_lock_free() = true

    std::atomic<unsigned long long> a2;
    std::cout << "a2.is_lock_free() = " 
              << a2.is_lock_free() << '\n';
    // output -> a2.is_lock_free() = true

    std::atomic<unsigned long long*> a3;
    std::cout << "a3.is_lock_free() = " 
              << a3.is_lock_free() << '\n';
    // output -> a3.is_lock_free() = true

    std::atomic<AStruct> a4;
    std::cout << "a4.is_lock_free() = " 
              << a4.is_lock_free() << '\n';
    // output -> a4.is_lock_free() = false

    std::atomic<std::shared_ptr<int>> a5;
    std::cout << "a5.is_lock_free() = " 
              << a5.is_lock_free() << '\n';
    // output -> a5.is_lock_free() = false
  }
*/

/*
  #include <atomic>

  int main()
  {
    std::atomic a_int(10);

    auto ret = a_int.fetch_add(5);  
    // fetch functions(fetch_xxx) are atomic functions 
    // ret's type is `int`

    std::cout << "ret = " << ret << '\n';
    // output -> ret = 10
    std::cout << "x.load() = " << a_int.load() << '\n';
    // output -> x.load() = 15
  }
*/

/*
  #include <atomic>

  int main()
  {
    std::atomic<int> a_int = 5;
    int ival = a_int.exchange(11);

    std::cout << "ival = " << ival << '\n';
    // output -> ival = 5
    std::cout << "a_int.load() = " << a_int.load() << '\n';
    // output -> a_int.load() = 11
  }
*/

/*
  // CAS(Compare And Swap) operations

  // mental model for "compare_exchange_strong" operation
  // "compare_exchange_strong" is an atomic operation.

  bool Atomic::compare_exchange_strong(T& expected, T desired)
  {
    Lock lock;
    T temp = m_value;

    if (temp != expected) {
      expected = temp;
      return false;
    }

    m_value = desired;
    return true;
  }

  - Atomik değişkenlerin değeri multi-thread programlarda
    birden fazla thread tarafından değiştirilebilir.
    Yazılan kodun lojik yapısı açısından iki atomik işlem
    arasında gerçekleşen bu değişikliğin yakalanması gerekir.
    compare_exchange_strong bu değişikliği yakalamak için
    kullanılan bir fonksiyondur.
*/

/*
  #include <atomic>

  int main()
  {
    std::cout << std::boolalpha;

    // -------------------------------------------------

    std::atomic<int> a1(33);
    int ival = 44;

    bool ret = a1.compare_exchange_strong(ival, 99);

    std::cout << "ret = " << ret << '\n';
    // output -> ret = false
    std::cout << "ival = " << ival << '\n';
    // output -> ival = 33
    std::cout << "a1.load() = " << a1.load() << '\n';
    // output -> a1.load() = 33
    // -------------------------------------------------

    a1.store(33);
    ival = 33;

    ret = a1.compare_exchange_strong(ival, 99);

    std::cout << "ret = " << ret << '\n';
    // output -> ret = true
    std::cout << "ival = " << ival << '\n';
    // output -> ival = 33
    std::cout << "a1.load() = " << a1.load() << '\n';
    // output -> a1.load() = 99

    // -------------------------------------------------
  }
*/

/*
  #include <concepts>
  #include <atomic>

  template <std::integral T>
  void Atomic_Increment(std::atomic<T>& atomic_val)
  {
    T temp{ atomic_val };

    // other thread might change "atomic_val" value in this line!
    // if that change happens, 
    // "temp" and "atomic_val" won't be same
    // so "compare_exchange_weak" function will return false 
    // so "atomic_val"'s new value will be assign to "temp"

    // in the next iteration
    // if "atomic_val" is not changed by other thread
    // "temp" and "atomic_val" will be same
    // so "atomic_val" will be incremented by 1.

    while(!atomic_val.compare_exchange_weak(temp, temp + 1))
      ; // null statement
  }
*/

/*
  #include <atomic>

  template <typename T>
  void Fetch_Multiply(std::atomic<T>& atomic_val, T mul)
  {
    T old_value = atomic_val.load();

    while (!atomic_val.compare_exchange_weak( old_value, 
                                              old_value * mul))
      ; // null statement
  }
*/

/*
  #include <atomic>   
  // std::atomic_int, std::atomic_bool, std::atomic_long
  #include <concepts> // std::same_as

  int main()
  {
    constexpr auto b1 = 
      std::same_as<std::atomic<int>, std::atomic_int>;
    // b1 -> true

    constexpr auto b2 = 
      std::same_as<std::atomic<bool>, std::atomic_bool>;
    // b2 -> true

    constexpr auto b3 = 
      std::same_as<std::atomic<long>, std::atomic_long>;
    // b3 -> true
  }
*/

/*
  // is_lock_free        -> run-time
  // is_always_lock_free -> compile-time 

  #include <atomic>
  #include <memory>

  struct AStruct {
    int m_a, m_b, m_c;
  };

  struct BStruct {
    int m_a;
  };

  int main()
  {
    std::cout << std::boolalpha;

    // --------------------------------------------------

    std::atomic a_i1 = 5;
    std::cout << "a_i1.is_lock_free() = " 
              << a_i1.is_lock_free() << '\n';
    // output -> a_i1.is_lock_free() = true

    constexpr auto b1 = std::atomic_int::is_always_lock_free;
    // b1 -> true

    // --------------------------------------------------

    std::atomic a_ll1 = 555LL;
    std::cout << "a_ll1.is_lock_free() = " 
              << a_ll1.is_lock_free() << '\n';
    // output -> a_ll1.is_lock_free() = true

    constexpr auto b2 = std::atomic_llong::is_always_lock_free;
    // b2 -> true

    // --------------------------------------------------

    std::atomic<std::shared_ptr<int>> a_sp1;
    std::cout << "a_sp1.is_lock_free() = " 
              << a_sp1.is_lock_free() << '\n';
    // output -> a_sp1.is_lock_free() = false

    constexpr auto b3 = 
      std::atomic<std::shared_ptr<int>>::is_always_lock_free;
    // b3 -> false

    // --------------------------------------------------

    std::atomic<AStruct> a_as1;
    std::cout << "a_as1.is_lock_free() = " 
              << a_as1.is_lock_free() << '\n';
    // output -> a_as1.is_lock_free() = false
    //  implementation-defined

    constexpr auto b4 = 
      std::atomic<AStruct>::is_always_lock_free;
    // b4 -> false

    // --------------------------------------------------

    std::atomic<BStruct> a_bs1;
    std::cout << "a_bs1.is_lock_free() = " 
              << a_bs1.is_lock_free() << '\n';
    // output -> a_bs1.is_lock_free() = true
    //  implementation-defined

    constexpr auto b5 = 
      std::atomic<BStruct>::is_always_lock_free;
    // b5 -> true

    // --------------------------------------------------
  }
*/

/*
              <--- check atomic_operations.png --->
*/

/*
  #include <atomic>

  int main()
  {
    std::atomic a1 = 20;
    std::atomic a2 = 20;

    auto res1 = a1 += 2;            
    // returning new value (non-reference)
    auto res2 = a2.fetch_add(2);    
    // returning old value (non-reference)

    std::cout << "a1 = " << a1 << '\n';   
    // output -> a1 = 22
    std::cout << "a2 = " << a2 << '\n';   
    // output -> a2 = 22

    std::cout << "res1 = " << res1 << '\n'; 
    // output -> res1 = 22
    std::cout << "res2 = " << res2 << '\n';
    // output -> res2 = 20
  }
*/

/*
  #include <atomic>
  #include <vector>
  #include <thread>   // std::jthread

  std::atomic g_a1 = 0;
  std::atomic g_a2 = 0;
  std::atomic g_a3 = 0;
  std::atomic g_a4 = 0;
  std::atomic g_a5 = 0;

  void func()
  {
    for (int i = 0; i < 10'000; ++i)
      g_a1.exchange(g_a1 + 1);    // not indivisible
  }

  void func2()
  {
    for (int i = 0; i < 10'000; ++i)
      ++g_a2;                     // indivisible
  }

  void func3()
  {
    for (int i = 0; i < 10'000; ++i)
      g_a3++;                     // indivisible
  }

  void func4()
  {
    for (int i = 0; i < 10'000; ++i)
      g_a4.fetch_add(1);          // indivisible
  }

  void func5()
  {
    for (int i = 0; i < 10'000; ++i)
    {
      int expected = g_a5;
      while (!g_a5.compare_exchange_weak(expected, expected + 1))
        ; // null statement
    }
  }

  int main()
  {
    // -----------------------------------------

    {
      std::vector<std::jthread> tx_vec;

      for (int i = 0; i < 10; ++i)
        tx_vec.emplace_back(func);
    }

    std::cout << "g_a1 = " << g_a1 << '\n';
    // output -> g_a1 = 28469

    // -----------------------------------------

    {
      std::vector<std::jthread> tx_vec;

      for (int i = 0; i < 10; ++i)
        tx_vec.emplace_back(func2);
    }

    std::cout << "g_a2 = " << g_a2 << '\n';
    // output -> g_a2 = 100000

    // -----------------------------------------

    {
      std::vector<std::jthread> tx_vec;

      for (int i = 0; i < 10; ++i)
        tx_vec.emplace_back(func3);
    }

    std::cout << "g_a3 = " << g_a3 << '\n';
    // output -> g_a3 = 100000

    // -----------------------------------------

    {
      std::vector<std::jthread> tx_vec;

      for (int i = 0; i < 10; ++i)
        tx_vec.emplace_back(func4);
    }

    std::cout << "g_a4 = " << g_a4 << '\n';
    // output -> g_a4 = 100000

    // -----------------------------------------

    {
      std::vector<std::jthread> tx_vec;

      for (int i = 0; i < 10; ++i)
        tx_vec.emplace_back(func5);
    }

    std::cout << "g_a5 = " << g_a5 << '\n';
    // output -> g_a4 = 100000

    // -----------------------------------------
  }
*/

/*
  #include <atomic>

  int main()
  {
    std::atomic a1 = 10;

    ++a1;
    a1.operator++();
    // Those 2 lines are equivalent.

    a1++;
    a1.operator++(0);
    // Those 2 lines are equivalent.

    int val1 = a1;
    int val2 = a1.operator int();
    // Those 2 lines are equivalent.
  }
*/

/*
  #include <atomic>   // std::atomic_store, std::atomic_load

  int main()
  {
    std::atomic a1 = 10;

    a1.store(22);
    std::atomic_store(&a1, 22);
    // Those 2 lines are equivalent.

    int val1 = a1.load();
    int val2 = std::atomic_load(&a1);
    // Those 2 lines are equivalent.

    // std::atomic_stroe and std::atomic_load are global functions
    // that can be used with C style atomic operations.
  }
*/

/*
  #include <atomic>
  #include <thread>

  std::atomic_int g_a1 = 0;
  volatile int g_v1 = 0;

  int main()
  {
    auto fn_inc_atomic = []{
      for (int i = 0; i < 1'000'000; ++i)
        ++g_a1;
    };

    auto fn_inc_volatile = []{
      for (int i = 0; i < 1'000'000; ++i)
        ++g_v1;
    };

    auto fn_dec_atomic = []{
      for (int i = 0; i < 1'000'000; ++i)
        --g_a1;
    };

    auto fn_dec_volatile = []{
      for (int i = 0; i < 1'000'000; ++i)
        --g_v1;
    };
    
    // ----------------------------------------

    {
      std::jthread jt1{ fn_inc_atomic };
      std::jthread jt2{ fn_dec_atomic };
    }

    std::cout << "g_a1 = " << g_a1 << '\n';
    // output -> g_a1 = 0
    // operations are indivisible so no data race

    // ----------------------------------------

    {
      std::jthread jt1{ fn_inc_volatile };
      std::jthread jt2{ fn_dec_volatile };
    }

    //  warning: '--' expression of 
    //  'volatile'-qualified type is deprecated
    //  warning: '++' expression of 
    //  'volatile'-qualified type is deprecated 

    std::cout << "g_v1 = " << g_v1 << '\n';
    // output -> g_v1 = 390469

    // ----------------------------------------
  }
*/

/*
  ABA problem : 
    kırmızı ışıkta bekliyorsun, yeşil ışık yanıyor fakat farkında
    değilsin ve ışık tekrardan kırmızıya dönüyor. 
    senin tek bildiğin durum ışığın kırmızı olması.
*/

/*
  //  atomik değişkenin değeri 10(Senaryo-A), 
  //  fakat başka bir thread değişkeni 50 yapıyor(Senaryo-B), 
  //  "compare_exchange_weak" fonksiyonu değişkenin 
  //  "temp" değerini 50 ye çekiyor.
  //  Başka bir thread değişkenin tekrar 10 yapıyor(Senaryo-A).
  //  "compare_exchange_weak" fonksiyonu "temp"i 10 a çekiyor.
  //  temp ve değişken eşit olduğu için "compare_exchange_weak" 
  //  fonksiyonu değişkenin değerini 1 arttırıyor.

  #include <concepts>   // std::integral
  #include <atomic>

  template <std::integral T>
  void inc_atomic(std::atomic<T>& x)
  {
    T temp{ x };
    while(!x.compare_exchange_weak(temp, temp + 1))
      ; // null statement
  }
*/

/*
  #include <atomic>
  #include <string>
  #include <thread>
  #include <vector>

  std::atomic_bool g_is_ready{ false };
  std::atomic_bool g_is_done{ false };
  std::atomic_int  g_counter{ 0 };

  void lottery()
  {
    ++g_counter;

    while (!g_is_ready)
      ; // null statement

    for (volatile int i = 0; i < 30'000; ++i)
      ; // null statement 
    // this loop is for delaying the thread
    // volatile for compiler not to optimize this loop

    bool expected{ false };

    if (g_is_done.compare_exchange_strong(expected, true))
    {
      std::cout << std::this_thread::get_id() 
                << " is the winner\n";
    }
  }

  int main()
  {
    std::vector<std::jthread> tx_vec;

    const int thread_count = 10;

    for (int i = 0; i < thread_count; ++i)
      tx_vec.emplace_back(lottery);

    while (g_counter != thread_count)
      ; // null statement

    g_is_ready = true;

    // output -> 6 is the winner
  }
*/

/*
                        ---------------------
                        | std::memory_order |
                        ---------------------
*/

/*
  <--- check memory_order.png --->
  <--- check read_modify_write.png --->
  <--- check memory_order_2.png


  memory_order_relaxed : ordering ya da senkronizasyon konusunda
                          hiçbir garanti vermez.
*/

/*
  // in sequential consistency g_atom_ival value must be 1 
  // if 4 different threads will run those 4 functions.

  #include <atomic>
  #include <thread>
  #include <cassert>

  std::atomic_bool g_atom_xflag, g_atom_yflag;
  std::atomic_int  g_atom_ival;

  void set_x()
  {
    g_atom_xflag.store(true);
  }

  void set_y()
  {
    g_atom_yflag.store(true);
  }

  void read_x_then_y()
  {
    while (!g_atom_xflag.load())
      ; // null statement

    // g_atom_xflag guaranteed to be true here.
    // In global order, if g_atom_yflag stored before g_atom_xflag,
    // g_atom_yflag is also guaranteed to be true here.

    if (g_atom_yflag.load())
      ++g_atom_ival;
  }

  void read_y_then_x()
  {
    while (!g_atom_yflag.load())
      ; // null statement

    // g_atom_yflag guaranteed to be true here.
    // In global order, if g_atom_xflag stored before g_atom_yflag,
    // g_atom_xflag is also guaranteed to be true here.

    if (g_atom_xflag.load())
      ++g_atom_ival;
  }

  void func()
  {
    g_atom_xflag.store(false);
    g_atom_yflag.store(false);
    g_atom_ival.store(0);

    std::thread t1{ set_x };
    std::thread t2{ set_y };
    std::thread t3{ read_x_then_y };
    std::thread t4{ read_y_then_x };

    t1.join(); 
    t2.join(); 
    t3.join(); 
    t4.join();

    assert(g_atom_ival.load() != 0);
  }

  int main()
  {
    for (int i = 0; i < 10'000; ++i)
      func();
  }
*/

/*
  // sequential consistency, atomik değişken üzerinde 
  // senkronizasyon sağlar.

  #include <atomic>
  #include <thread>

  std::atomic_bool g_atom_flag{ false };
  int g_ival = 0;

  void producer()
  {
    g_ival = 111111;
    g_atom_flag = true;
  }

  void consumer()
  {
    while (!g_atom_flag.load())
      ; // null statement

    // g_atom_flag guaranteed to be true here.
    // so "g_ival" have a happens before(synchronization) relationship
    // it is guaranteed to be 111111.

    std::cout << "g_ival = " << g_ival << '\n';
  }

  int main()
  {
    {
      std::jthread jt1{ producer };
      std::jthread jt2{ consumer };
    }
    // output -> g_ival = 111111
  }
*/

/*
  // acquire_release semantics does not have a global order 
  // but there is a synchronization (happens before relationship).

  // release'den önceki kodlar, release sonrasına alınamazlar.
  // derleyici böyle bir optimizasyon yapamaz.
*/

/*
  #include <atomic>     // std::atomic_bool
  #include <thread>     // std::jthread
  #include <cassert>

  std::atomic_bool g_atom_flag{ false };
  int g_ival = 0;

  void producer()
  {
    g_ival = 111111;
    g_atom_flag.store(true, std::memory_order_release);
    // it is guaranteed that g_ival will be set to 111111
    // before the store operation(g_atom_flag is set to true).
    // compiler can not optimize the code in a way that
    // g_ival will be set to 111111 after the store operation
  }

  void consumer()
  {
    while (!g_atom_flag.load(std::memory_order_acquire))
      ; // null statement

    std::cout << "g_ival = " << g_ival << '\n';
    // it is guaranteed that g_ival will be printed 
    // after the load operation of g_atom_flag.
    // compiler can not optimize the code in a way that
    // g_ival will be printed before the load operation 
    // of g_atom_flag.
  }

  void func()
  {
    g_atom_flag = false;
    g_ival = 0;

    {
      std::jthread jt1{ producer };
      std::jthread jt2{ consumer };
    }

    assert(g_ival == 111111);
  }

  int main()
  {
    func();   // output -> g_ival = 111111
  }
*/

/*
  #include <atomic>
  #include <string>
  #include <cassert>
  #include <thread>

  std::atomic<std::string*> g_atom_pstr;
  int g_data;

  void producer()
  {
    std::string* p_str = new std::string("hello world");
    g_data = 44;

    g_atom_pstr.store(p_str, std::memory_order_release);
  }
  // aynı atomic değişken üzerinde(g_atom_pstr), 
  // bir başka threadde load işlemi "std::memory_order_acquire"
  // ile yapılırsa, store'dan önce yapılan işlemlerin sonuçlarını
  // gözleme garantisi vardır.

  void consumer()
  {
    std::string* p_str;
    while (!(p_str = g_atom_pstr.load(std::memory_order_acquire)))
      ; // null statement

    assert(*p_str == "hello world");
    assert(g_data == 44);
  }

  int main()
  {
    std::thread t1{ producer };
    std::thread t2{ consumer };

    t1.join();
    t2.join();
  }
*/

/*
  #include <atomic>   // std::atomic_bool
  #include <string>
  #include <thread>   // std::jthread
  #include <cassert>  // assert

  std::atomic_bool g_atom_flag1 = false;
  std::atomic_bool g_atom_flag2 = false;

  std::string g_str{};    // shared variable

  void func_1()
  {
    g_str = "world";
    g_atom_flag1.store(true, std::memory_order_release);
  }

  void func_2()
  {
    while (!g_atom_flag1.load(std::memory_order_acquire))
      ; // null statement

    g_str += " galaxy";
    g_atom_flag2.store(true, std::memory_order_release);
  }

  void func_3()
  {
    while (!g_atom_flag2.load(std::memory_order_acquire))
      ; // null statement

    g_str += " universe";
  }

  void test()
  {
    g_atom_flag1 = false;
    g_atom_flag2 = false;

    {
      std::jthread jt1{ func_1 };
      std::jthread jt2{ func_2 };
      std::jthread jt3{ func_3 };
    }

    assert(g_str == "world galaxy universe");
  }

  int main()
  {
    for (volatile int i = 0; i < 10'000; ++i)
      test();
  }
*/

/*
  #include <atomic>
  #include <thread>

  class SpinLockMutex {
  private:
    std::atomic_flag m_flag;

  public:
    SpinLockMutex() { m_flag.clear(); }

    void lock()
    {
      while (m_flag.test_and_set(std::memory_order_acquire))
        ; // null statement
    }

    void unlock()
    {
      m_flag.clear(std::memory_order_release);
    }
  };


  SpinLockMutex g_mtx;
  unsigned long long g_var{};     // shared variable

  void func_1()
  {
    for (int i = 0; i < 1'000'000; ++i) 
      ++g_var;
  }

  void func_2()
  {
    for (int i = 0; i < 1'000'000; ++i) 
    {
      g_mtx.lock();
      ++g_var;
      g_mtx.unlock();
    }
  }

  int main()
  {
    {
      std::jthread jt1(func_1);
      std::jthread jt2(func_1);
      std::jthread jt3(func_1);
      std::jthread jt4(func_1);
      std::jthread jt5(func_1);   
    }

    std::cout << "g_var = " << g_var << '\n';
    // output -> g_var = 1435046  ---> data race

    g_var = 0;

    {
      std::jthread jt1(func_2);
      std::jthread jt2(func_2);
      std::jthread jt3(func_2);
      std::jthread jt4(func_2);
      std::jthread jt5(func_2);   
    }

    std::cout << "g_var = " << g_var << '\n';
    // output -> g_var = 5000000
  }
*/

/*
  // std::memory_order_relaxed is NOT giving 
  // a synchronization guarantee

  #include <atomic>
  #include <cassert>
  #include <thread>

  std::atomic<bool> g_atom_x, g_atom_y, g_atom_z;

  void func_1()
  {
    g_atom_x.store(true, std::memory_order_relaxed);
  }

  void func_2()
  {
    g_atom_y.store(true, std::memory_order_relaxed);
  }

  void func_3()
  {
    while (!g_atom_y.load(std::memory_order_relaxed))
      ; // null statement

    if (g_atom_x.load(std::memory_order_relaxed))
      g_atom_z = true;

    assert(g_atom_z);
  }

  int main()
  {
    for (int i = 0; i < 100'000; ++i)
    {
      g_atom_x = false;
      g_atom_y = false;
      g_atom_z = false;

      std::jthread jt1{ func_1 };
      std::jthread jt2{ func_2 };
      std::jthread jt3{ func_3 };
    }
    // output -> 
    //  Assertion failed: g_atom_z, file main.cpp, line 8358
  }
*/

/*
  - global bir sayaç değişkeni var ve tek beklenti, threadlerin 
    bu değişkeni arttırması. 
    Herhangi bir global order sağlanmasına gerek yok.
    Senkronizasyon noktası oluşturulmasına gerek yok.
    Sadece atomicity garantisi verilmesi yeterli.
    dolayısıyla "std::memory_order_relaxed" semantiği kullanılır.
*/

/*
  // std::memory_order_relaxed is giving atomicity guarantee

  #include <atomic>
  #include <vector>
  #include <thread>

  std::atomic_int g_atom_counter = 0;

  void func_1()
  {
    // ++g_atom_counter;
    // g_atom_counter.fetch_add(1, std::memory_order_seq_cst);
    // Those 2 lines are equivalent. (sequential consistency)

    for (int i = 0; i < 100'000; ++i)
      g_atom_counter.fetch_add(1, std::memory_order_relaxed);
  }

  void func_2()
  {
    for (int i = 0; i < 100'000; ++i)
      g_atom_counter.fetch_add(1, std::memory_order_relaxed);
  }

  void func_3()
  {
    for (int i = 0; i < 100'000; ++i)
      g_atom_counter.fetch_add(1, std::memory_order_relaxed);
  }


  int main()
  {
    {
      std::jthread jt1{ func_1 };
      std::jthread jt2{ func_2 };
      std::jthread jt3{ func_3 };
    }

    std::cout << "g_atom_counter = " << g_atom_counter << '\n';
    // output -> g_atom_counter = 300000
  }
*/

/*
  // no global order, no synchronization, only atomicity

  #include <atomic>
  #include <syncstream>   // std::osyncstream
  #include <thread>       // std::jthread

  std::atomic<long> g_atom_data{ 0 };

  void do_work(int id)
  {
    std::osyncstream{ std::cout } << "thread id = " << id << " - "
      << g_atom_data.fetch_add(1, std::memory_order_relaxed) 
      << '\n';
  }

  int main()
  {
    std::jthread jt1{ do_work, 1 };
    std::jthread jt2{ do_work, 2 };
    std::jthread jt3{ do_work, 3 };
    std::jthread jt4{ do_work, 4 };
    std::jthread jt5{ do_work, 5 };
    std::jthread jt6{ do_work, 6 };
  }

  // output ->
  //  thread id = 1 - 2
  //  thread id = 4 - 1
  //  thread id = 2 - 0
  //  thread id = 5 - 3
  //  thread id = 3 - 4
  //  thread id = 6 - 5
*/

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

/*
  #include <memory>   // std::shared_ptr

  class Strategy {
  public:
    virtual void operator()() = 0;
    virtual ~Strategy() = default;
  };

  class Context {
  private:
    std::shared_ptr<Strategy> msp_strategy{};

  public:
    void set_strategy(std::shared_ptr<Strategy> sp_strategy)
    {
      msp_strategy = sp_strategy;
    }

    void do_stuff()
    {
      if (msp_strategy)
        (*msp_strategy)();
    }
  };
*/
