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

### [Concepts](https://github.com/UPinar/AdvancedCPPLessons/tree/main/concepts)
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

### [`bit` module (C++20)](https://github.com/UPinar/AdvancedCPPLessons/tree/main/bit_module)

### [Not Related](https://github.com/UPinar/AdvancedCPPLessons/tree/main/not_related)
  - `template <auto N>` syntax
  - lazy initialization
  - refactoring functions
  - default declaration of special member functions
  - template argument deduction and `auto` type deduction
  - `std::type_identity`
  - `std::iota` algorithm