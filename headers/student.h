#ifndef STUDENT_H
#define STUDENT_H

#include <memory>     // std::unique_ptr
#include <algorithm>  // std::swap

class Student {
private:
  class S_Impl;  // forward declaration
  std::unique_ptr<S_Impl> mp_SIMPL;

  // for deep constness
  S_Impl* get_simpl() { return mp_SIMPL.get(); }
  const S_Impl* get_simpl() const { return mp_SIMPL.get(); }
  
public:
  // special member functions
  Student(const char* p_name, const char* p_surname);
  ~Student();   // destructor declaration

  Student(const Student&);
  Student& operator=(Student);

  Student(Student&&) noexcept;
  Student& operator=(Student&&) noexcept;
  
  // public member functions
  void add_grade(int grade);
  void print()const;

  void swap(Student& other) noexcept
  {
    std::swap(mp_SIMPL, other.mp_SIMPL);
  }
};

#endif // STUDENT_H

// -------------------------------------------------

// when destructor is not defined, compiler will
// generate public inline defaulted destructor 
// which will delete mp_SIMPL unique_ptr object
// but since mp_SIMPL is incomplete type, 
// it will cause syntax error.

// -------------------------------------------------
