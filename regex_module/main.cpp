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

/*
  - bir token'ın arkasına geldiği zaman, o token'dan kaç 
    tane olabileceğini belirler.

  - token?      -> 0 or 1
  - token*      -> 0 or more
  - token+      -> 1 or more
  - token{n}    -> exactly n
  - token{n,}   -> n or more
  - token{n,m}  -> n to m
*/

/*
  --------------------------------------------------
  (?) -> 0 or 1
  --------------------------------------------------

  ka?r  

  literally 'k'             +
  literally 'a' or nothing  +
  literally 'r'

  kar             (kar)
  kr              (kr)

  --------------------------------------------------

  [kts]a?[rmn]

  one of the literally 'k' or 't' or 's'  +
  literally 'a' or nothing                +
  one of the literally 'r' or 'm' or 'n'

  kn              (kn)
  kan             (kan)

  --------------------------------------------------

  (ram)?\d      ->  (ram or nothing)(digit)

  literally "ram" or nothing  +
  digit

  756             (7)(5)(6)
  ram9            (ram9)

  --------------------------------------------------
*/

/*
  --------------------------------------------------
  (*) -> 0 or more
  --------------------------------------------------

  r* 
    rrrrr         (rrrrr)     

  --------------------------------------------------

  ah*
    a             (a)
    ah            (ah)
    ahh           (ahh)
    ahhh          (ahhh) 

  --------------------------------------------------

  [krm]*

    kmkra       (kmkr)  

  --------------------------------------------------

  [xy]*a
    a           (a)
    xya         (xya)
    xyxya       (xyxya)

  --------------------------------------------------
*/

/*
  --------------------------------------------------
  (+) -> 1 or more
  --------------------------------------------------

  [xy]+a
    a           NOT MATCH
    xa          (xa)
    xya         (xya)
    xyxya       (xyxya)

  --------------------------------------------------

  \d+
    50          (50)
    300         (300)

  --------------------------------------------------
*/

/*
  --------------------------------------------------

  \d{4} -> exactly 4 digits
    1234          (1234)
    123           NOT MATCH
    12345         (1234)
    123456789     (1234)(5678)

  --------------------------------------------------

  \d{2,} -> 2 or more digits
    1             NOT MATCH
    12            (12)
    123           (123)
    1234          (1234)
    12345         (12345)
    123456789     (123456789)

  --------------------------------------------------

  \d{3,4} -> 3 to 4 digits
    1             NOT MATCH
    12            NOT MATCH
    123           (123)
    1234          (1234)
    12345         (1234)
    123456789     (1234)(567)
    123456789     (1234)(5678)

  --------------------------------------------------
*/

/*
                          ----------
                          | anchor |
                          ----------
*/

/*
  ^   -> start of the string
  $   -> end of the string
  \b  -> word boundary      (beginning - end)
  \B  -> not word boundary  (beginning - end)
*/

/*
  --------------------------------------------------

  ^kar 
    |kar| kardelen reenkarnasyon kardes kardeslik sakar   ->  (kar)

  --------------------------------------------------

  kar$
    far kardelen reenkarnasyon kardes kardeslik sa|kar|   ->  (kar)

  --------------------------------------------------
*/

/*
  transistor tutorial tories generator inventor torqued 
  senator visitor torque editorial torso torrent doctor 
  director tortoise torment curator janitor factorial 
  torturous supervisor torch restore collector 
  torus torchlight actor tory distort investor constructor 
  narrator mentorship editor torpedoed tornado tormentor
  motorist contributors tornados mentor tormented tutor 
  monitor spector tortilla tornados torn torpedo victory
  editor laboratory decorator

  --------------------------------------------------

  \stor   -> (whitespace + tor)

  tories         (tor)
  torqued        (tor)
  torso          (tor)  

  --------------------------------------------------

  tor\s   -> (tor + whitespace)  
  transistor     (tor)
  generator      (tor)
  inventor       (tor)

  --------------------------------------------------

  \btor  (tor will be at the beginning of the word)

  tories         (tor)
  torqued        (tor)
  torso          (tor)

  --------------------------------------------------

  tor\b  (tor will be at the end of the word)

  transistor     (tor)
  inventor       (tor)
  senator        (tor)

  --------------------------------------------------

  \Btor  (tor will not be at the beginning of the word)

  transistor     (tor)
  tutorial       (tor)
  generator      (tor)
  mentorship     (tor)

  --------------------------------------------------

  tor\B  (tor will not be at the end of the word)

  tutorial       (tor)
  tories         (tor)
  torqued        (tor)

  --------------------------------------------------

  \Btor\B  (tor will not be at the beginning and end of the word)

  tutorial       (tor)
  restore        (tor)
  victory        (tor)
  factorial      (tor)

  --------------------------------------------------
*/

/*
  --------------------------------------------------

  \d{4}\.[A-F]{4}\.\d{4}

  4 digits                            + 
  literally '.'                       + 
  4 uppercase hexadecimal characters  +
  literally '.'                       +
  4 digits

  (1234.ABCD.5678)
  (4008.BDDF.6234)

  --------------------------------------------------

  \d{4}\.[A-F]{4}\.\d{4}\b

  4 digits                            +
  literally '.'                       +
  4 uppercase hexadecimal characters  +
  literally '.'                       +
  4 digits                            +
  word boundary(LINE END)

  T2p1234.ABCD.5678     (1234.ABCD.5678)
  K8yH4008.BDDF.6234    (4008.BDDF.6234)

  --------------------------------------------------

  \d+ 
  1 or more digits

  --------------------------------------------------

  a\d+
  a + 1 or more digits

  fZ8ha3941       (a3941)
  a8242           (a8242)

  --------------------------------------------------

  (a\d)+
  1 or more times (a + digit)

  8Ya3a110        (a3a1)
  jwa8242         (a8)  

  --------------------------------------------------
*/

/*
                      -----------------
                      | capture group |
                      -----------------
*/

/*
  --------------------------------------------------
  () -> priority increase and capturing grouping      
      (expensive for regex engine)
  --------------------------------------------------

  (\d{4})\.([A-F]{4})\.(\d{4})

  4 digits                            +
  literally '.'                       +
  4 uppercase hexadecimal characters  +
  literally '.'                       +
  4 digits

  match = (1234.ABCD.5678) 
  capture group 1 = (1234)
  capture group 2 = (ABCD)
  capture group 3 = (5678)

  match = (4008.BDDF.6234)
  capture group 1 = (4008)
  capture group 2 = (BDDF)
  capture group 3 = (6234)

  --------------------------------------------------
*/

/*
  --------------------------------------------------
  (:?) -> priority increase but non-capturing group
  --------------------------------------------------

  (:?\d{4})\.([A-F]{4})\.(\d{4})

  4 digits                            +
  literally '.'                       +
  4 uppercase hexadecimal characters  +
  literally '.'                       +
  4 digits

  match = (1234.ABCD.5678)
  capture group 1 = (ABCD)
  capture group 2 = (5678)

  match = (4008.BDDF.6234)
  capture group 1 = (BDDF)
  capture group 2 = (6234)

  --------------------------------------------------
*/

/*
                    ------------------
                    | back reference |
                    ------------------
*/

/*
  --------------------------------------------------
  backreference
  --------------------------------------------------

  (\d{3})car\1

  3 digits          +
  literally 'c'     +
  literally 'a'     +
  literally 'r'     +
  1st capture group

  match = (123car123)
  capture group 1 = (123)

  match = (456car456)
  capture group 1 = (456)

  --------------------------------------------------

  (\d{3})([A-F]{2})\1\2

  3 digits                            +
  2 hexadecimal upper case character  +
  1st capture group                   +
  2nd capture group

  match = (123AF123AF)
  capture group 1 = (123)
  capture group 2 = (AF)

  match = (456BC456BC)
  capture group 1 = (456)
  capture group 2 = (BC)

  --------------------------------------------------
*/

/*
  --------------------------------------------------

  .*([a-z]{3}).*\1

  n characters but not '\n'           +
  3 lowercase characters from a to z  +
  n characters but not '\n'           +
  1st capture group

  Xanthinthique     
  match = (Xanthinthi)
  capture group 1 = (thi)

  Wunderkinder  
  match = (Wunderkinder)
  capture group 1 = (der)

  --------------------------------------------------

  .*([a-z]{3})[a-z]\1

  n characters but not '\n'           +
  3 lowercase characters from a to z  +
  1 lowercase character from a to z   +
  1st capture group

  wretchlessness
  match = (wretchlessness)
  capture group 1 = (ess)

  --------------------------------------------------

  .*([a-z]{2}).*\1.*\1

  n characters but not '\n'           +
  2 lowercase characters from a to z  +
  n characters but not '\n'           +
  1st capture group                   +
  n characters but not '\n'           +
  1st capture group

  word-for-word
  match = (word-for-wor)
  capture group 1 = (or)

  wine-drinking
  match = (wine-drinkin)
  capture group 1 = (in)

  --------------------------------------------------
*/

// Lesson_34 01:32:21