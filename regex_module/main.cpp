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

/*
  --------------------------------------------------

  [abc]       -> a or b or c (only 1 character)

  --------------------------------------------------

  [aer][trs]  -> (a or e or r) + (t or r or s)
    test            (e + s)
    never           (e + r)
    pages           (e + s)
    party           (a + r) 
    has             (a + s)

  --------------------------------------------------

  [aer][trs][aekm]
    eaten          (a + t + e)
    cleared        (a + r + e)
    permits        (e + r + m)

  --------------------------------------------------
 */  

/*
  --------------------------------------------------

  [a-e]      -> a or b or c or d or e (only 1 character)
    patience        (a)
    test            (e)

  --------------------------------------------------

  [a-e][e-k]
    deck            (d + e)(c + k)
    change          (c + h)
    be              (b + e)
    kitchen         (c + h)
    priocess        (c + e)

  --------------------------------------------------

  [0-9]
    50              (5)(0)
    300             (3)(0)(0)

  --------------------------------------------------

  [mkr0-9A-F]
    redecorated     (r)(r)
    1.2             (1)(2)
    team            (m)
    Can             (C)

  --------------------------------------------------
*/

/*
  --------------------------------------------------

  [^abc]     -> not one of the characters in the set(a or b or c)
    test            (t)(e)(s)(t)
    never           (n)(e)(v)(e)(r)
    pages           (p)(g)(e)(s)
    party           (p)(r)(t)(y)
    has             (h)(s)

  --------------------------------------------------
  
  [^0-9]
    every           (e)(v)(e)(r)(y)  
    charachter      (c)(h)(a)(r)(a)(c)(h)(t)(e)(r)
    other           (o)(t)(h)(e)(r)
    than            (t)(h)(a)(n)
    numbers         (n)(u)(m)(b)(e)(r)(s)

  --------------------------------------------------

  [^0-9a-z]
    We have          (' ')
    VIP              (V)(I)(P)  
    []               ([)(])
    \add             (\)(a)(d)(d)

  --------------------------------------------------

  [\^]
    ^hello^          (^)(^)

  --------------------------------------------------

  [\^mr].[\^ks]
    result            (res)
    cards             (rds)
    suprise           (ris) 
    broken            (rok) 

  --------------------------------------------------
*/

/*
  --------------------------------------------------
  [0-9] and \d(digit character) are equivalent
  --------------------------------------------------

  [0-9]
    50              (5)(0)
    300             (3)(0)(0)

  --------------------------------------------------

  \d
    50              (5)(0)
    300             (3)(0)(0)

  --------------------------------------------------

  d\d
    ard93           (d9)

  --------------------------------------------------
*/

/*
  --------------------------------------------------
  [^0-9] and \D(NON DIGIT character) are equivalent
  --------------------------------------------------

  [^0-9]
    every           (e)(v)(e)(r)(y)  
    charachter      (c)(h)(a)(r)(a)(c)(h)(t)(e)(r)
    other           (o)(t)(h)(e)(r)
    than            (t)(h)(a)(n)
    numbers         (n)(u)(m)(b)(e)(r)(s)

  --------------------------------------------------

  \D
    every           (e)(v)(e)(r)(y)  
    charachter      (c)(h)(a)(r)(a)(c)(h)(t)(e)(r)
    other           (o)(t)(h)(e)(r)
    than            (t)(h)(a)(n)
    numbers         (n)(u)(m)(b)(e)(r)(s)

  --------------------------------------------------

  \D\d
    $1.2           ($1)(.2)
    are 52         (" 5")
    ard93          (d9)
    9:00           (:0)
    30,000         (,0)

  --------------------------------------------------
  [\dA-Fa-f] or [0-9A-Fa-f] -> hexadecimal characters
  --------------------------------------------------
*/

/*
  --------------------------------------------------
  \s  -> whitespace characters
  --------------------------------------------------
  \S  -> non-whitespace characters
  --------------------------------------------------
*/

/*
  --------------------------------------------------
  \w -> alphanumeric characters + '_'
  --------------------------------------------------
  \W -> non-alphanumeric characters - '_'
  --------------------------------------------------

  \d\w 
    at9_8723jasd      (9_)(87)(23)

  --------------------------------------------------
*/

/*
                        ---------------
                        | quantifiers |
                        ---------------
*/

// Lesson_34 00:41:50