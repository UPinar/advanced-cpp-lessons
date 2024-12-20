<h1 align="center"> AdvancedCPPLessons </h1>

### [Value Categories](https://github.com/UPinar/AdvancedCPPLessons/tree/main/value_categories)
  - `decltype` specifier
  - `std::move` <algorithm> <utility>
  - universal(forwarding) reference
  - reference collapsing
  - special member functions
  - `noexcept` specifier |  `noexcept` operator

### [Copy Elision](https://github.com/UPinar/AdvancedCPPLessons/tree/main/copy_elision)
  - Mandatory Copy Elision
    - Unmaterialize object passing
    - (Unnamed) Return Value Optimization (RVO) 
  - Named Return Value Optimization (NRVO) [Optimization - Copy Elision]
  - No NRVO Scenarios

### [Move Semantics](https://github.com/UPinar/AdvancedCPPLessons/tree/main/move_semantics)
  - Rule of Zero
  - Bad Scenarios
  - `noexcept`
  - `override` contextual keyword(bağlamsal anahtar sözcük)
  - move sematics and `noexcept` specifier relationship
  - moved from state

### [Perfect Forwarding](https://github.com/UPinar/AdvancedCPPLessons/tree/main/perfect_forwarding)
  - universal(forwarding) reference
  - perfect forwarding
    - `auto&&`
    - `decltype(auto)`
  - reference qualifier
  
### [Move Semantics and Perfect Forwarding in STL](https://github.com/UPinar/AdvancedCPPLessons/tree/main/move_semantic_perfect_forwarding_in_standart_library)
  - move-only types
  - `std::initializer_list` class template
  - `std::remove` and `std::remove_if` algorithms
  - `std::move` and `std::move_backward` algorithms
  - `std::move_iterator` iterator adaptor
  
### [Literal Operator Functions](https://github.com/UPinar/AdvancedCPPLessons/tree/main/literal_operator_functions)
  - cooked literal operator functions
  - uncooked literal operator functions
  - raw string literal
  - `std::quoted`

### [`std::string_view` (C++17)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/string_view)


### [Vocabulary Types (C++17)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/vocabulary_types)
  - `std::optional` class template
  - `std::variant` variadic class template
    - `std::monostate` class
    - `std::visit` variadic function templace
    - overloader idiom
  - `std::any` class
    - `std::any_cast` function template
  
### [Class Template Argument Deduction(CTAD) (C++17)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/class_template_argument_deduction)
  - class template argument deduction
  - deduction guides

### [Structured Binding (C++17)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/structured_binding)

### [Three-way comparison(spaceship) operator (C++20)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/spaceship_operator)

### [Lambda expressions](https://github.com/UPinar/AdvancedCPPLessons/tree/main/lambda_expressions)
  - lambda expressions
  - lambda idioms
    - positive lambda idiom
    - immediately invoked funtion expression
    - overload resolution idiom
    - idiom that prevent static initialization order fiasco
  - lambda expressions and C++20
    - default ctor and copy assignment of stateless lambda expression
    - lambda expression inside unevaluated context
    - familiar template syntax

### [`<format>` module (C++20)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/format_module)
  - `std::format`
  - formatting user defined types 

### [Aggregate](https://github.com/UPinar/AdvancedCPPLessons/tree/main/aggregate)
  - aggregate types
  - designated initializer for aggregate types
  
### [`constexpr` `consteval` `constinit`](https://github.com/UPinar/AdvancedCPPLessons/tree/main/constexpr_consteval_constinit)
  - `constexpr` functions
  - `consteval`(immediate) function (C++20)
  - `constinit`

### [Attributes](https://github.com/UPinar/AdvancedCPPLessons/tree/main/attributes)
  - `[[nodiscard]]` attribute
  - `[[deprecated]]` attribute
  - `[[likely]]` and `[[unlikely]]` attributes 
  - `[[maybe_unused]]` attribute
  - `[[noreturn]] ` attribute
  - `[[fallthrough]]` attribute

### [`<concepts>` module (C++20)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/concepts_module)
  - `requires` clause
  - named constraints (`concept`)
  - `requires` expression
    - simple requirement
    - type requirement
    - compound requirement
    - nested requirement (`requires` clause)
  - subsumption
  - standart library concepts
  
### [C++20 Changes](https://github.com/UPinar/AdvancedCPPLessons/tree/main/cpp20_changes)
  - signed integral types right and left shift
  - `std::ssize` global function template
  - struct's bitfield members
  - `typename` keyword in dependent type
  - safe comparison in mixed types
  - conversion from `void*` to `bool` 
  - alias template deduction
  - `source_location` module 
  - `using` enum declaration
  - conditionally `explicit` constructor

### [C++23 Changes](https://github.com/UPinar/AdvancedCPPLessons/tree/main/cpp23_changes)
  - explicit object parameter

### [`<bit>` module (C++20)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/bit_module)

### [`<ranges>` module (C++20)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/ranges_module)
  - rangified algorithms
  - projection in ranges::algorithms
  - std::ranges::views
  - lazy evaluation
  - common
  - subrange
  - borrowed range
  - views
  - range adaptors and range factories
  - caching mechanism in views

### [`<span>` module (C++20)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/span_module)
  - `std::span`
  - `std::dynamic_extent`
  - `std::subspan` 
  
### [C++ Idioms](https://github.com/UPinar/AdvancedCPPLessons/tree/main/cpp_idioms)
  - exception tracker idiom
  - exception dispatcher idiom
  - polymorphic exception idiom
  - ADL + fallback idiom
  - hidden friend idioms
  - scope guard idiom
  - return type resolve idiom
  - non virtual interface idiom (NVI)
    - fragile base class problem
  - `std::tuple` idioms
  - PIMPL idiom(Pointer to Implementation)
  - Fast PIMPL idiom
  - copy-swap idiom
  - attorney-client idiom
  - nifty counter idiom
  
### [Generic Programming](https://github.com/UPinar/AdvancedCPPLessons/tree/main/generic_programming)
  - template argument deduction
  - template parameter types
    - template type parameter
    - non-type template parameter (NTTP)
    - template template parameter
  - variable template
  - default arguments in template parameters
  - template specializations
    - explicit(full) specialization
    - partial specialization
  - variadic templates (C++11)
    - pack expansion
    - fold expressions (C++17)
      - unary fold
      - binary fold
    - fold expressions idioms
  - static if (C++17)
  - template instantiation
    - point of instantiation (POI)
    - explicit instantiation
      - explicit instantiation directive
      - `extern` template declaration
  - CRTP (Curiously Recurring Template Pattern)
  - Mixin Pattern
  
### [Strong Types](https://github.com/UPinar/AdvancedCPPLessons/tree/main/strong_types)

### [Exception Handling](https://github.com/UPinar/AdvancedCPPLessons/tree/main/exception_handling)
  - `std::exception_ptr` (C++11)
    - `std::make_exception_ptr`
  - `std::nested_exception` (C++11)
    - `std::throw_with_nested`
    - `std::rethrow_if_nested`
  
### [Regex](https://github.com/UPinar/AdvancedCPPLessons/tree/main/regex)
  - quantifiers
  - lazy or greedy quantifiers
  - anchor
  - capture group
  - back reference
  - alternation
  - positive and negative look ahead
  - positive and negative look back
  - `<regex>` module (C++11)
    - `std::regex_match`
    - `std::smatch`, `std::cmatch`
    - `std::regex_search`
    - `std::regex_iterator`
    - `std::regex_token_iterator`
    - `std::regex_replace`
  
### [Concurrency](https://github.com/UPinar/AdvancedCPPLessons/tree/main/concurrency)
  - `std::thread`
  - `std::this_thread` namespace
    - `std::this_thread::sleep_for`
    - `std::this_thread::get_id`
    - `std::this_thread::sleep_until`
    - `std::this_thread::yield`
  - exception in threads
  - move semantics in threads
  - thread local storage class
  - `std::jthread` class
  - mutual exclusion(mutex)
    - `std::mutex` class
    - deadlock scenarios
    - `std::recursive_mutex` class
    - `std::timed_mutex` class
    - `std::lock_guard` class
    - `std::lock` global function template
    - `std::scoped_lock` class
    - `std::unique_lock` class
    - `std::shared_mutex` class (C++17)
  - `std::once_flag`, `std::call_once`
  - `std::future`, `std::promise`
  - `std::async`
    - `std::launch::async` policy
    - `std::launch::deferred` policy
  - `std::shared_future`
  - `std::packaged_task`
  - condition variable
  - concurrency in standart library algorithms
    - global execution policy objects
    - `std::reduce` algorithm
    - `std::transform_reduce` algorithm
    - `std::inclusive_scan`, `std::exclusive_scan` algorithms
  - `<atomic>` module
    - `std::atomic_flag` class
    - `std::atomic` class template

### [Not Related](https://github.com/UPinar/AdvancedCPPLessons/tree/main/not_related)
  - `template <auto N>` syntax
  - lazy initialization
  - refactoring functions
  - default declaration of special member functions
  - template argument deduction and `auto` type deduction
  - `std::type_identity`
  - `std::iota` algorithm
  - `namespace` alias
  - ADL and two step lookup
  - `std::invoke`
  - `std::exchange` in <utility>
  - `std::clamp` in <algorithm>
  - deep constness
  - file buffer to stream
  - `std::accumulate` algorithm
  - `std::transform` algorithm
  - `std::inner_product` algorithm
  - `std::partial_sum` algorithm