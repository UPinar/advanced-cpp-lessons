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
