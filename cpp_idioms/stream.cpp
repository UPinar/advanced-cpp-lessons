#include "stream.h"
#include <new>          // placement new
#include <type_traits>  // std::aligned_storage

static int nifty_counter; // zero-initialized
// (internal linkage)

static typename std::aligned_storage<
          sizeof(Stream), alignof(Stream)>::type stream_buffer;
// storage for the "s_cout" object

Stream& s_cout = reinterpret_cast<Stream&>(stream_buffer);

Stream::Stream(){ /* initialize things*/ }
Stream::~Stream(){ /*clean-up*/ }

Stream_Initializer::Stream_Initializer()
{
  if (nifty_counter++ == 0)
    new(&s_cout)Stream(); // placement new
}

Stream_Initializer::~Stream_Initializer()
{
  if (--nifty_counter == 0)
    (&s_cout)->~Stream(); // explicit destructor call
}

/*
  - world.cpp       (first translation unit)
  - galaxy.cpp      (second translation unit)
  - universe.cpp    (third translation unit)
  files will include "stream.h" header file.

  inside "world.cpp" file, global s_initializer object's 
  constructor will be called and nifty_counter will be 
  incremented by 1 (0 ---> 1) and global static "s_cout" 
  object will be constructed using placement new.

  inside "galaxy.cpp" file, global s_initializer object's
  constructor will be called which will not do anything.

  inside "universe.cpp" file, global s_initializer object's
  constructor will be called which will not do anything.

  when the program ends, global s_initializer object's
  destructor will be called and nifty_counter will be
  decremented by 1 (1 ---> 0) and global static "s_cout"
  object will be destructed using explicit destructor call.
*/