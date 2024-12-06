#include <iostream>

/*
                        ==================
                        | `regex` module |
                        ==================
*/

/*
  regex(regular expressions) - düzenli ifadeler :
    bir yazının uyması gereken pattern.

  regex engine, regex stringi alarak, faydalı hizmetler veriyor.
    - validation (geçerlilik kontrolü)
      bir yazının regex string kurallarına uygunluğu kontrolü.
    - search (arama)
    - replace (değiştirme)
*/

/*
  <---- check https://regex101.com/ ---->
*/

/*
  --------------------------------------------------

  a       -> all 'a' characters literrally

  --------------------------------------------------

  .       -> all characters but '\n' (metacharacter)
    T           (T)
    h           (h) 
    e           (e)
    .           (.)    

                    ----------------

  \.      -> '.' character literrally

  --------------------------------------------------

  [       -> not found (metacharacter)

                    ----------------
  
  \[      -> '[' character literrally

  --------------------------------------------------

  \      -> not found (metacharacter)

                    ----------------
  
  \\      -> '\' character literrally

  --------------------------------------------------

  d\.   -> d + '.' character
    backyard.       (d + .)
    published.      (d + .)

  --------------------------------------------------

  .d\.   -> d + '.' character
    bed.            (e + d + .)

  --------------------------------------------------

  ta    -> all `ta` characters (case sensitive)
    talen
    take

  --------------------------------------------------

  .a    -> any character but '\n' + a 
    has             (h + a)
    just arrived    (space + a)
  
  --------------------------------------------------

  t.e   -> t + any character but '\n' + e
    the                       (t + h + e)          
    therapist encouraged      (t + space + e)
    their                     (t + h + e)

  --------------------------------------------------

  c.r  -> c + any character but '\n' + r
    security                  (c + u + r)
    officer                   (c + e + r)
    car's                     (c + a + r)
    cards                     (c + a + r)

  --------------------------------------------------
*/

// Lesson_34 00:24:30