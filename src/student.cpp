#include "../headers/student.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory> // std::make_unique
#include <format>
#include <print>

using namespace std;

// Student_Impl nested type definition
class Student::S_Impl {
private:
  string m_name;
  string m_surname;
  vector<int> m_grades_vec{};

public:
  S_Impl(const char* p_name, const char* p_surname)
    : m_name{ p_name }, m_surname{ p_surname } {}

  // non-const member function
  void add_grade(int grade)   
  {
    m_grades_vec.push_back(grade);
  }

  // const member function
  void print() const
  {
    // std::print("name = {}, surname = {}, grades = {}\n", 
    //             m_name, m_surname, m_grades_vec);

    std::cout << "name = " << m_name
              << ", surname = " << m_surname
              << ", grades = ";

    for (const auto grade : m_grades_vec)
      std::cout << grade << ' ';

    std::cout << '\n';
  }
};

// -------------------------------------------------

Student::Student(const char* p_name, const char* p_surname) 
  : mp_SIMPL{ make_unique<Student::S_Impl>(p_name, p_surname) } {}

// -------------------------------------------------

Student::~Student() = default;

// -------------------------------------------------

Student::Student(const Student& other)
  : mp_SIMPL{ make_unique<Student::S_Impl>(*other.mp_SIMPL) } {}

// -------------------------------------------------

Student& Student::operator=(Student other)
{
  swap(other);
  return *this;
}
// for parameter variable, copy constructor will be called
// only way that copy assignment function 
// will throw an exception is in parameter variable's construction
// if it throws an exception, 
// then copy assignment function's body will not be executed
// if it does not throw an exception,
// swap function will be called(noexcept guarantee)
// so, strong exception guarantee is provided

// -------------------------------------------------

Student::Student(Student&&) noexcept = default;

// -------------------------------------------------

Student& Student::operator=(Student&&) noexcept = default;

// -------------------------------------------------

void Student::add_grade(int grade)
{
  get_simpl()->add_grade(grade);
}

// -------------------------------------------------

void Student::print() const
{
  get_simpl()->print();
}

// -------------------------------------------------