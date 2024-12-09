#ifndef STREAM_H
#define STREAM_H

struct Stream {
  Stream();
  ~Stream();
};

// -----------------------------------------------------

extern Stream& s_cout;  // global stream object

// bu başlık dosyasını #include eden bütün modüller
// s_cout nesnesini kullanabilir.

// -----------------------------------------------------

struct Stream_Initializer {
  Stream_Initializer();
  ~Stream_Initializer();
};

// -----------------------------------------------------

static Stream_Initializer s_initializer{};
// global static object (internal linkage)
// static initializer for every translation unit

// bu başlık dosyasını #include eden bütün modüllerde
// ayrı bir Stream_Initializer nesnesi oluşacak.

// -----------------------------------------------------

#endif  // STREAM_H