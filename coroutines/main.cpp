#include <iostream>

/*
                          ==============
                          | Coroutines |
                          ==============
*/

/*
        <---- check images/coroutine_taxonomy.png ---->
*/

/*
  ------------------------------------------------------

  - coroutine'ler, çalıştırılma süreci içerisinde, 
    çalışması askıya alınıp(durdurulup)(suspend), 
    tekrar devam ettirilebilen(resume) fonksiyonlardır.
    Bu durum bir veya birden fazla kere gerçekleşebilir.

      <---- check images/coroutine_execution.png ---->
      <---- check images/coroutine_subroutine.png ---->

  ------------------------------------------------------

  - oluşturulan custimization mekanizmaları ile, 
    çağıran kod ile coroutine arasında değer alışverişi 
    sağlanabilir.

  ------------------------------------------------------

  - neden suspension point oluşturulur ? 
    ->  üretilen ara değerin çağıran koda iletilmesi 
        gerekebilir.
    ->  coroutine'in işini gerçekleştirebilmesi için 
        bir event'in gerçekleşmesi gerekir.
    ->  çalışma önceliği daha yüksek olan bir işlemin 
        gerçekleşmesi için coroutine'in askıya alınması
        gerekebilir.

  ------------------------------------------------------

  - derleyicinin fonksiyonun koduna bakarak, coroutine 
    olduğunu anlaması için,
      - co_await 
      - co_yield
      - co_return
    anahtar sözcüklerininin en az birinin 
    bulunması gerekmektedir.

  ------------------------------------------------------

  - coroutine fonksiyonların geri dönüş değeri, 
    belirli koşulları sağlayan bir sınıf türündendir.
  
  ------------------------------------------------------

  - "main" fonksiyonu bir coroutine fonksiyonu olamaz.
  - variadic fonksiyonlar coroutine fonksiyonu olamaz.
  - sınıfların constructor ve destructor'ları coroutine 
    fonksiyonu olamaz.
  - constexpr fonksiyonlar coroutine fonksiyonu olamaz.
  - consteval fonksiyonlar coroutine fonksiyonu olamaz.
  - coroutine fonksiyonlar, 
    auto return type, concept placeholder type kullanamazlar
  - coroutine fonksiyonlarda "return" statement kullanılamaz.
    "co_return" statement kullanılmalıdır.
  
  ------------------------------------------------------

  - coroutine fonksiyonun geri dönüş değeri, 
    coroutine'i çağıracak kodun, coroutine fonksiyon ile 
    iletişimini sağlayacak bir nesnenin türüdür.
    (corotuine interface)
    
  ------------------------------------------------------
*/

/*
  #include <coroutine>

  class Coro {};

  Coro func()
  {
    std::cout << "hello world\n";

    co_return;  // syntax error
    // error: unable to find the promise type for this coroutine
  }
*/

/*
  #include <coroutine>  

  struct Coro {
    // nested type
    struct promise_type {};
  };

  Coro func()
  {
    std::cout << "hello world\n";

    co_return;
  }

  // syntax error 
  //  error: no member named 'return_void' in 
  //  error: no member named 'initial_suspend' in 
  //  error: no member named 'unhandled_exception' in
  //  error: no member named 'final_suspend' in 
  //  error: no member named 'get_return_object' in 
  //        'Coro::promise_type'
*/

/*
  #include <coroutine>  // std::suspend_never

  struct Coro {
    struct promise_type {
      Coro get_return_object() 
      {
        return Coro{};
      }

      std::suspend_never initial_suspend() 
      {
        return {};
      }

      std::suspend_never final_suspend() noexcept
      {
        return {};
      }

      void return_void() {}
      void unhandled_exception() {}
    };
  };

  Coro func()
  {
    std::cout << "hello world\n";
    co_return;
  }

  int main()
  {
    func();   // output -> hello world
  }
*/

/*
  --------------------------------------------------------
        <---- check images/coroutine_workflow.png ---->
  --------------------------------------------------------

  - bir coroutine başlatıldığında,
    fonksiyonun tekrar çalışmaya devam edebilmesi için(resume)
    bazı verilerin tutulması gerekir.
    - fonksiyonun parametre değişkenleri
    - fonksiyon içindeki yerel değişkenler(local variables)
    - fonksiyonun çalışmaya devam edeceği nokta.
    - aracı rolünde olan promise nesnesi 
      (coroutine'in geri dönüş değeri olan türün promise_type
      nested type'ı türünden bir nesne)
    bu veriler bir nesne ile temsil edilir ve bu nesneye
    coroutine state/frame denir.

  - coroutine state/frame genel olarak heap alanında oluşturulur
    (dynamic memory allocation),
    fakat derleyici koda bakarak bu bellek alanını stack'te 
    oluşturmayı tercih edebilir.

  --------------------------------------------------------

  - coroutine'in çalışmasının durdurulmasını(suspend) sağlayan 
    co_await, co_yield operatörleri, coroutine state/frame'in 
    içinde bulunan promise nesnesi ile iletişim sağlar.

  --------------------------------------------------------

  - coroutine'i çağıran kodun(caller), 
    coroutine suspend edildiği durumda 
    coroutine'i tekrar çalıştırabilmesi(resume) için, 
    coroutine state/frame'e erişmesi gerekir.

  - coroutine state opak bir yapıda olduğu için,
    doğrudan erişim sağlanamaz. 
    Pointer gibi davranan std::coroutine_handle sınıfı türünden 
    bir nesne aracılığı ile erişim sağlanır.

  --------------------------------------------------------

  - "co_await", "co_yield" operatörleri ile coroutine'in 
    davranışı customize edilebilir.
    -> coroutine suspend edilmeden önce yapılması 
      gereken işlemler ayarlanabilir.
    -> coroutine suspend edildikten sonra yapılması 
      gereken işlemler ayarlanabilir.
    -> coroutine resume edilmeden önce yapılması 
      gereken işlemler ayarlanabilir.
    -> coroutine resume edildikten sonra yapılması
      gereken işlemler ayarlanabilir.

  --------------------------------------------------------
*/

/*
                      ---------------------
                      | co_await operator |
                      ---------------------
*/

/*
  --------------------------------------------------------

  "co_await" is a unary prefix operator

  --------------------------------------------------------

  co_await expr;
    it must be an awaitable object generated
    from "co_await" operator's operand(expression)

  --------------------------------------------------------

  awaiter class has the following member functions:
    - await_ready
    - await_suspend
    - await_resume

  standart library provides the following awaiter types:
    - std::suspend_always
    - std::suspend_never

  <---- check images/standart_suspend_classes.png ---->

  --------------------------------------------------------

  co_await std::suspend_always{};
    - "co_await" operator's operand is an object of 
      std::suspend_always type.
    - suspension will be accepted.

  co_await std::suspend_never{};
    - "co_await" operator's operand is an object of 
      std::suspend_never type.
    - suspension will be rejected.

  --------------------------------------------------------
*/

/*
  --------------------------------------------------------

  1. Aşama

  - derleyici öncelikle "co_await" operatörünün operand'ı olan 
    ifadeden bir awaitable elde etmek zorunda.

  - derleyici, ilgili promise type'ın "await_transform" 
    isimli bir üye fonksiyonu var mı kontrolünü yapar.
    - var ise, promise nesnesinin "await_transform" 
      fonksiyonunu çağırır ve "co_await" operatörünün
      operand'ı olan ifadeyi bu fonksiyona argüman olarak
      gönderir. Fonksiyonun geri dönüş değerini awaitable türünden
      bir nesne olarak alır.
    - yok ise, awaitable olarak "co_await" operatörünün 
      operand'ı olan ifadeyi kullanır.

  <---- check get_awaitable.png ---->

  --------------------------------------------------------

  2. Aşama

  - derleyici, awaitable olarak elde edilen türün 
    "operator co_await" üye fonksiyonu var mı kontrolünü yapar.
    - var ise bu fonksiyonu çağırır ve geri dönüş değerini 
      awaiter türünden bir nesne olarak alır.
    - üye fonksiyon yok fakat global "operator co_await" 
      fonksiyonu var ise, bu fonksiyonu çağırır ve 
      geri dönüş değerini awaiter türünden bir nesne olarak alır.
    - ne üye fonksiyonu ne de global fonksiyon yok ise,
      awaitable'ın kendisini awaiter olarak kullanır.

  <---- check get_awaiter.png ---->

  - dolayısıyla bir sınıf hem awaitable hem de awaiter 
    olabilir.

  --------------------------------------------------------

  3. Aşama

  - derleyici, awaiter türünden nesnenin
    ("co_await" operatörünün operandı) "await_ready" 
    üye fonksiyonunu coroutine suspend edilmeden önce çağırır.
      - false dönerse, değer hazır değil, 
        suspension kabul edilir, "await_suspend" çağrılır.
      - true dönerse, değer hazır, suspension reddedilir.
        coroutine durdurulmadan devam eder.

    - "await_ready" çağrıldığında, henüz suspension 
      gerçekleşmemiştir dolayısıyla "await_resume" veya 
      "destroy" fonksiyonlarının çağrılmaması gerekir.

  --------------------------------------------------------

  4. Aşama

  - "await_ready" fonksiyonu false döndürür ise 
    "await_suspend" fonksiyonu çağrılır.

  void Awaiter::await_suspend(std::coroutine_handle<>);
    - "await_suspend" fonksiyonun geri dönüş değeri "void" ise,
      suspension gerçekleşir ve programın akışı 
      coroutine'i çağıran koda geri döner.

  bool Awaiter::await_suspend(std::coroutine_handle<>);
    - "await_suspend" fonksiyonun geri dönüş değeri "bool" ise,
      - true dönerse suspension gerçekleşir, programın akışı
        coroutine'i çağıran koda geri döner.
      - false dönerse suspension gerçekleşmez, coroutine
        çalışmasına devam eder.
    
  std::coroutine_handle 
    Awaiter::await_suspend(std::coroutine_handle<>);

  --------------------------------------------------------

  5. Aşama

  - "await_resume" caller'ın coroutine'in tekrar çalıştırılması 
    durumunda, coroutine'in çalışmaya başlamasından önce çağrılır.

  --------------------------------------------------------
*/

/*
  #include <coroutine>  
  // std::coroutine_handle, std::suspend_never, 
  // std::suspend_always

  class [[nodiscard]] Coro_Interface {
  public:
    struct promise_type;
    using Crt_Handle = std::coroutine_handle<promise_type>;
  private:
    Crt_Handle m_handle;

  public:
    struct promise_type {
      auto get_return_object()
      {
        return Crt_Handle::from_promise(*this);
      }

      auto initial_suspend() noexcept
      {
        return std::suspend_never{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void return_void() {}
      void unhandled_exception() 
      {
        std::terminate();
      }
    };
    
    Coro_Interface(auto h) : m_handle(h) {}
    ~Coro_Interface() 
    { 
      if (m_handle) 
        m_handle.destroy();
    }

    Coro_Interface(const Coro_Interface&) = delete;
    Coro_Interface& operator=(const Coro_Interface&) = delete;

    bool resume() const 
    {
      if (!m_handle || m_handle.done())
        return false;

      m_handle.resume();  
      return !m_handle.done();
    }
  };

  struct Awaiter {
    bool await_ready() const
    {  
      std::cout << "Awaiter::await_ready called.\n";
      return false;   
    }
    // because of "false" return type,
    // suspension will be accepted, 
    // await_suspend will be called.

    void await_suspend(std::coroutine_handle<>) const
    {
      std::cout << "Awaiter::await_suspend called.\n";
    }
    // because of "void" return type, 
    // suspension will be accepted and program flow
    // will return to the caller.

    void await_resume() const
    {
      std::cout << "Awaiter::await_resume called.\n";
    }
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await Awaiter{};
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    std::cout << "[0] - main started\n";
    auto cr_interface = coroutine_func();
    std::cout << "[1] - main continues\n";
    cr_interface.resume();
    std::cout << "[2] - main ends\n";
  }
  // output ->
  //  [0] - main started
  //  [0] - coroutine_func started
  //  Awaiter::await_ready called.
  //  Awaiter::await_suspend called.
  //  [1] - main continues
  //  Awaiter::await_resume called.
  //  [1] - coroutine_func ends
  //  [2] - main ends
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::suspend_never

  struct Coro_Interface {

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return {};
      }

      auto initial_suspend()
      {
        return std::suspend_always{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void unhandled_exception() {}
      void return_void() {}
    };
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await std::suspend_never{};
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    coroutine_func();  // output -> "" (empty)
  }
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::suspend_never

  struct Coro_Interface {

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return {};
      }

      auto initial_suspend()
      {
        return std::suspend_never{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void unhandled_exception() {}
      void return_void() {}
    };
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await std::suspend_never{};
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    coroutine_func();  
    // output -> 
    //  [0] - coroutine_func started
    //  [1] - coroutine_func ends
  }
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::coroutine_handle,
  // std::suspend_never

  struct Coro_Interface {

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return {};
      }

      auto initial_suspend()
      {
        return std::suspend_never{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void unhandled_exception() {}
      void return_void() {}
    };
  };

  struct Awaiter {
    bool await_ready() const
    {  
      std::cout << "Awaiter::await_ready called.\n";
      return false;   
    }
    void await_suspend(std::coroutine_handle<>) const
    {
      std::cout << "Awaiter::await_suspend called.\n";
    }
    void await_resume() const
    {
      std::cout << "Awaiter::await_resume called.\n";
    }
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await Awaiter{};
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    std::cout << "[0] - main started\n";
    coroutine_func();
    std::cout << "[1] - main ends\n";
  }
  // output ->
  //  [0] - main started
  //  [0] - coroutine_func started
  //  Awaiter::await_ready called.    ----> returns false
  //  Awaiter::await_suspend called.
  //  [1] - main ends
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::coroutine_handle,
  // std::suspend_never

  struct Coro_Interface {

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return {};
      }

      auto initial_suspend()
      {
        return std::suspend_never{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void unhandled_exception() {}
      void return_void() {}
    };
  };

  struct Awaiter {
    bool await_ready() const
    {  
      std::cout << "Awaiter::await_ready called.\n";
      return true;   
    }
    void await_suspend(std::coroutine_handle<>) const
    {
      std::cout << "Awaiter::await_suspend called.\n";
    }
    void await_resume() const
    {
      std::cout << "Awaiter::await_resume called.\n";
    }
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await Awaiter{};
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    std::cout << "[0] - main started\n";
    coroutine_func();
    std::cout << "[1] - main ends\n";
  }
  // output ->
  //  [0] - main started
  //  [0] - coroutine_func started
  //  Awaiter::await_ready called.      ----> returns true
  //  Awaiter::await_resume called.
  //  [1] - coroutine_func ends
  //  [1] - main ends
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::suspend_never

  struct Coro_Interface {

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return {};
      }

      auto initial_suspend()
      {
        return std::suspend_never{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void unhandled_exception() {}
      void return_void() {}
    };
  };

  Coro_Interface coroutine_func()
  {
    co_await 7;   // syntax error 
    //  error: awaitable type 'int' is not a structure
  }

  int main()
  {
    coroutine_func();
  }
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::suspend_never,
  // std::coroutine_handle

  struct Awaiter {
    bool await_ready() const
    {  
      std::cout << "Awaiter::await_ready called.\n";
      return false;   
    }
    void await_suspend(std::coroutine_handle<>) const
    {
      std::cout << "Awaiter::await_suspend called.\n";
    }
    void await_resume() const
    {
      std::cout << "Awaiter::await_resume called.\n";
    }
  };

  struct Coro_Interface {

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return {};
      }

      auto initial_suspend()
      {
        return std::suspend_never{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void unhandled_exception() {}
      void return_void() {}

      auto await_transform(int value) const
      {
        std::cout << 
          "promise_type::await_transform(int x) called, x = "
          << value << '\n';
        return Awaiter{};
      }
    };
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await 7;
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    std::cout << "[0] - main started\n";
    coroutine_func();
    std::cout << "[1] - main ends\n";
  }

  // output ->
  //  [0] - main started
  //  [0] - coroutine_func started
  //  promise_type::await_transform(int x) called, x = 7
  //  Awaiter::await_ready called.      ----> returns false
  //  Awaiter::await_suspend called.
  //  [1] - main ends


  // 1. Awaitable type does not have "operator co_await" 
  //  member function
  // 2. There is no global "operator co_await" function
  // 3. await_transform member function is called
  //  and return type is used as awaiter type
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::suspend_never

  struct Mystruct {};

  struct Coro_Interface {

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return {};
      }

      auto initial_suspend()
      {
        return std::suspend_never{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void unhandled_exception() {}
      void return_void() {}

      auto await_transform(int value) const
      {
        return Mystruct{};  // syntax error
        // error: no member named 'await_ready' in 'Mystruct'
      }
    };
  };

  Coro_Interface coroutine_func()
  {
    co_await 7;
  }

  int main()
  {
    coroutine_func();
  }
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::suspend_never,

  struct Awaiter {
    bool await_ready() const
    {  
      std::cout << "Awaiter::await_ready called.\n";
      return false;   
    }
    void await_suspend(std::coroutine_handle<>) const
    {
      std::cout << "Awaiter::await_suspend called.\n";
    }
    void await_resume() const
    {
      std::cout << "Awaiter::await_resume called.\n";
    }
  };

  struct Mystruct {
    auto operator co_await() const
    {
      std::cout << "Mystruct::operator co_await called\n";
      return Awaiter{};
    }
  };

  struct Coro_Interface {

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return {};
      }

      auto initial_suspend()
      {
        return std::suspend_never{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void unhandled_exception() {}
      void return_void() {}

      auto await_transform(int value) const
      {
        std::cout << 
          "promise_type::await_transform(int x) called, x = "
          << value << '\n';
        return Mystruct{};
      }
    };
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await 7;
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    std::cout << "[0] - main started\n";
    coroutine_func();
    std::cout << "[1] - main ends\n";
  }

  // output ->
  //  [0] - main started
  //  [0] - coroutine_func started
  //  promise_type::await_transform(int x) called, x = 7
  //  Mystruct::operator co_await called
  //  Awaiter::await_ready called.      ----> returns false
  //  Awaiter::await_suspend called.
  //  [1] - main ends
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::suspend_never,
  // std::coroutine_handle

  struct Awaiter {
    bool await_ready() const
    {  
      std::cout << "Awaiter::await_ready called.\n";
      return false;   
    }
    void await_suspend(std::coroutine_handle<>) const
    {
      std::cout << "Awaiter::await_suspend called.\n";
    }
    void await_resume() const
    {
      std::cout << "Awaiter::await_resume called.\n";
    }
  };

  struct Mystruct {};

  auto operator co_await(Mystruct)
  {
    std::cout << "::operator co_await(Mystruct) called\n";
    return Awaiter{};
  }

  struct Coro_Interface {

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return {};
      }

      auto initial_suspend()
      {
        return std::suspend_never{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void unhandled_exception() {}
      void return_void() {}

      auto await_transform(int value) const
      {
        std::cout << 
          "promise_type::await_transform(int x) called, x = "
          << value << '\n';
        return Mystruct{};
      }
    };
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await 7;
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    std::cout << "[0] - main started\n";
    coroutine_func();
    std::cout << "[1] - main ends\n";
  }

  // output ->
  //  [0] - main started
  //  [0] - coroutine_func started
  //  promise_type::await_transform(int x) called, x = 7
  //  ::operator co_await(Mystruct) called
  //  Awaiter::await_ready called.        ----> returns false
  //  Awaiter::await_suspend called.
  //  [1] - main ends
*/

/*
                      -------------------------
                      | std::coroutine_handle |
                      -------------------------
*/

/*
  <---- check coroutine_handle_void.png ---->

  couroutine_handle<Promise>'ın couroutine_handle<void>'a 
  dönüşmesi gereken her durumda, örtülü dönüşüm gerçekleşir.
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::suspend_never,
  // std::coroutine_handle

  class Coro_Interface {
  public:
    struct promise_type;  // forward declaration
    using CoroHandle_t = std::coroutine_handle<promise_type>;

    void resume()
    {
      m_coro_handle.resume();
    }

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return Coro_Interface(CoroHandle_t::from_promise(*this));
      }

      auto initial_suspend()
      {
        return std::suspend_never{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void return_void() {}
      void unhandled_exception() {}
    };

  private:
    Coro_Interface(CoroHandle_t ch) : m_coro_handle(ch) {}

    CoroHandle_t m_coro_handle;
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await std::suspend_always{};
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    std::cout << "[0] - main started\n";
    auto cr_interface = coroutine_func();
    std::cout << "[1] - main continues\n";
    cr_interface.resume();
    std::cout << "[2] - main ends\n";
  }
  // output ->
  //  [0] - main started
  //  [0] - coroutine_func started
  //  [1] - main continues
  //  [1] - coroutine_func ends
  //  [2] - main ends
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::coroutine_handle

  class Coro_Interface {
  public:
    struct promise_type;  // forward declaration
    using CoroHandle_t = std::coroutine_handle<promise_type>;

    void resume()
    {
      m_coro_handle.resume();
    }

    bool done() const
    {
      return m_coro_handle.done();
    }

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return Coro_Interface(CoroHandle_t::from_promise(*this));
      }

      auto initial_suspend()
      {
        return std::suspend_always{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void return_void() {}
      void unhandled_exception() {}
    };

  private:
    Coro_Interface(CoroHandle_t ch) : m_coro_handle(ch) {}

    CoroHandle_t m_coro_handle;
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await std::suspend_always{};
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    std::cout << std::boolalpha;

    // -------------------------------------------

    auto cr_interface = coroutine_func();
    // suspend inside "initial_suspend" function

    std::cout << "cr_interface.done() = " 
              << cr_interface.done() << '\n';
    // output -> cr_interface.done() = false

    // -------------------------------------------

    cr_interface.resume();
    // suspend inside "coroutine_func" function

    // output -> [0] - coroutine_func started

    std::cout << "cr_interface.done() = " 
              << cr_interface.done() << '\n';
    // output -> cr_interface.done() = false

    // -------------------------------------------

    cr_interface.resume();

    // output -> [1] - coroutine_func ends

    std::cout << "cr_interface.done() = " 
              << cr_interface.done() << '\n';
    // output -> cr_interface.done() = true

    // -------------------------------------------
  }
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::coroutine_handle

  class Coro_Interface {
  public:
    struct promise_type;  // forward declaration
    using CoroHandle_t = std::coroutine_handle<promise_type>;

    void resume()
    {
      m_coro_handle.resume();
    }

    bool done() const
    {
      return m_coro_handle.done();
    }

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return Coro_Interface(CoroHandle_t::from_promise(*this));
      }

      auto initial_suspend()
      {
        return std::suspend_always{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void return_void() {}
      void unhandled_exception() {}
    };

    CoroHandle_t m_coro_handle;

  private:
    Coro_Interface(CoroHandle_t ch) : m_coro_handle(ch) {}
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await std::suspend_always{};
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    std::cout << std::boolalpha;

    // -------------------------------------------

    Coro_Interface::CoroHandle_t handle_1;  
    // default initialization

    Coro_Interface::CoroHandle_t handle_2{};
    // value initialization

    Coro_Interface::CoroHandle_t handle_3{ nullptr };

    std::cout << (handle_1 == nullptr) << '\n';   
    // output -> true
    std::cout << (handle_2 == nullptr) << '\n';   
    // output -> true
    std::cout << (handle_3 == nullptr) << '\n';   
    // output -> true

    // -------------------------------------------

    // m_coro_handle is in public section 
    // for demonstration purpose

    auto cr_interface = coroutine_func();
    auto handle_4 = cr_interface.m_coro_handle;

    std::cout << (handle_4 == nullptr) << '\n';   
    // output -> false

    // because of handle is associated with coroutine
    // it is not nullptr

    if (handle_4)
      std::cout << "handle_4 is associated with a coroutine\n";
    // output -> handle_4 is associated with a coroutine

    handle_4 = nullptr;
    std::cout << (handle_4 == nullptr) << '\n';   
    // output -> true

    // -------------------------------------------
  }
*/

/*
  #include <coroutine>  
  // std::suspend_always, std::coroutine_handle

  class Coro_Interface {
  public:
    struct promise_type;  // forward declaration
    using CoroHandle_t = std::coroutine_handle<promise_type>;

    void resume()
    {
      m_coro_handle.resume();
    }

    bool done() const
    {
      return m_coro_handle.done();
    }

    struct promise_type {
      Coro_Interface get_return_object()
      {
        return Coro_Interface(CoroHandle_t::from_promise(*this));
      }

      auto initial_suspend()
      {
        return std::suspend_always{};
      }

      auto final_suspend() noexcept
      {
        return std::suspend_always{};
      }

      void return_void() {}
      void unhandled_exception() {}
    };

    CoroHandle_t m_coro_handle;

  private:
    Coro_Interface(CoroHandle_t ch) : m_coro_handle(ch) {}
  };

  Coro_Interface coroutine_func()
  {
    std::cout << "[0] - coroutine_func started\n";
    co_await std::suspend_always{};
    std::cout << "[1] - coroutine_func ends\n";
  }

  int main()
  {
    std::cout << std::boolalpha;

    // --------------------------------------------------

    auto cr_interface = coroutine_func();

    cr_interface.resume();
    cr_interface.m_coro_handle();
    // Those 2 lines are equivalent.

    // output ->
    //  [0] - coroutine_func started
    //  [1] - coroutine_func ends

    // --------------------------------------------------

    // if we have a promise object, 
    // we can reach coroutine_handle object 
    // by using "from_promise" static member function.

    // if we have a coroutine_handle object,
    // we can reach promise object
    // by using "promise" non-static member function.

    auto prom = cr_interface.m_coro_handle.promise();

    // --------------------------------------------------

    auto handle1 = cr_interface.m_coro_handle;
    void* vp = handle1.address();

    auto handle2 = decltype(handle1)::from_address(vp);

    std::cout << (handle1 == handle2) << '\n';
    // output -> true

    // --------------------------------------------------

    std::cout << handle1.done() << '\n';  // output -> true

    // --------------------------------------------------

    std::coroutine_handle<> v_handle = cr_interface.m_coro_handle;
    // assignment is valid because of implicit conversion

    // --------------------------------------------------
  }
*/