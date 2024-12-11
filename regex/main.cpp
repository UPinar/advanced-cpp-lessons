#include <iostream>

/*
                  ===============================
                  | Regex (Regular expressions) |
                  ===============================
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

  a   
  literally 'a' character

  --------------------------------------------------

  . (metacharacter)      
  any character but '\n'

    T           (T)
    h           (h) 
    e           (e)
    .           (.)    

  --------------------------------------------------

  \.     
  literally '.' character

  --------------------------------------------------

  [ (metacharacter)

                    ----------------
  
  \[      
  literally '[' character

  --------------------------------------------------

  \ (metacharacter)

                    ----------------
  
  \\      
  literally '\' character

  --------------------------------------------------

  d\.

  literally 'd'     +
  literally '.'

    backyard.       (d + .)
    published.      (d + .)

  --------------------------------------------------

  .d\.

  any character but '\n'  +
  literally 'd'           +
  literally '.'

    bed.            (e + d + .)

  --------------------------------------------------

  ta

  literally "ta"

    talen
    take

  --------------------------------------------------

  .a

  any character but '\n'   + 
  literally 'a'

    has             (h + a)
    just arrived    (space + a)
  
  --------------------------------------------------

  t.e   

  literally 't'           +
  any character but '\n'  +
  literally 'e'

    the                       (t + h + e)          
    therapist encouraged      (t + space + e)
    their                     (t + h + e)

  --------------------------------------------------

  c.r  

  literally 'c'           +
  any character but '\n'  +
  literally 'r'

    security                  (c + u + r)
    officer                   (c + e + r)
    car's                     (c + a + r)
    cards                     (c + a + r)

  --------------------------------------------------
*/

/*
  --------------------------------------------------

  [abc]

  literally 'a' or 'b' or 'c'

    test            (t)
    never           (e)
    pages           (a)
    party           (a) 
    has             (a)

  --------------------------------------------------

  [aer][trs]

  literally 'a' or 'e' or 'r' + 
  literally 't' or 'r' or 's'

    test            (e + s)
    never           (e + r)
    pages           (e + s)
    party           (a + r) 
    has             (a + s)

  --------------------------------------------------

  [aer][trs][aekm]

  literally 'a' or 'e' or 'r' +
  literally 't' or 'r' or 's' +
  literally 'a' or 'e' or 'k' or 'm'

    eaten          (a + t + e)
    cleared        (a + r + e)
    permits        (e + r + m)

  --------------------------------------------------
 */  

/*
  --------------------------------------------------

  [a-e]     

  literally ('a' to 'e')

    patience        (a)
    test            (e)

  --------------------------------------------------

  [a-e][e-k]

  literally ('a' to 'e') + 
  literally ('e' to 'k')

    deck            (d + e)(c + k)
    change          (c + h)
    be              (b + e)
    kitchen         (c + h)
    priocess        (c + e)

  --------------------------------------------------

  [0-9]

  literally ('0' to '9')

    50              (5)(0)
    300             (3)(0)(0)

  --------------------------------------------------

  [mkr0-9A-F]

  literally 'm' or 'k' or 'r' or ('0' to '9') or ('A' to 'F')

    redecorated     (r)(r)
    1.2             (1)(2)
    team            (m)
    Can             (C)

  --------------------------------------------------
*/

/*
  --------------------------------------------------

  [^abc]

  literally (NOT 'a') AND (NOT 'b') AND (NOT 'c')

    test            (t)(e)(s)(t)
    never           (n)(e)(v)(e)(r)
    pages           (p)(g)(e)(s)
    party           (p)(r)(t)(y)
    has             (h)(s)

  --------------------------------------------------
  
  [^0-9]

  literally (NOT '0' to '9')

    every           (e)(v)(e)(r)(y)  
    charachter      (c)(h)(a)(r)(a)(c)(h)(t)(e)(r)
    other           (o)(t)(h)(e)(r)
    than            (t)(h)(a)(n)
    numbers         (n)(u)(m)(b)(e)(r)(s)

  --------------------------------------------------

  [^0-9a-z]

  literally (NOT '0' to '9') AND (NOT 'a' to 'z')

    We have          (' ')
    VIP              (V)(I)(P)  
    []               ([)(])
    \add             (\)(a)(d)(d)

  --------------------------------------------------

  [\^]

  literally '^'

    ^hello^          (^)(^)

  --------------------------------------------------

  [\^mr].[\^ks]

  literally '^' or 'm' or 'r'   +
  any character but '\n'        +
  literally '^' or 'k' or 's'

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

  literally ('0' to '9')

    50              (5)(0)
    300             (3)(0)(0)

  --------------------------------------------------

  \d

  digit [literally ('0' to '9')]

    50              (5)(0)
    300             (3)(0)(0)

  --------------------------------------------------

  d\d

  literally 'd'   + 
  digit(literally ('0' to '9'))

    ard93           (d9)

  --------------------------------------------------
*/

/*
  --------------------------------------------------
  [^0-9] and \D(NON DIGIT character) are equivalent
  --------------------------------------------------

  [^0-9]

  literally (NOT '0' to '9')

    every           (e)(v)(e)(r)(y)  
    charachter      (c)(h)(a)(r)(a)(c)(h)(t)(e)(r)
    other           (o)(t)(h)(e)(r)
    than            (t)(h)(a)(n)
    numbers         (n)(u)(m)(b)(e)(r)(s)

  --------------------------------------------------

  \D

  non-digit [literally (NOT '0' to '9')]

    every           (e)(v)(e)(r)(y)  
    charachter      (c)(h)(a)(r)(a)(c)(h)(t)(e)(r)
    other           (o)(t)(h)(e)(r)
    than            (t)(h)(a)(n)
    numbers         (n)(u)(m)(b)(e)(r)(s)

  --------------------------------------------------

  \D\d

  non-digit  +
  digit

    $1.2           ($1)(.2)
    are 52         (" 5")
    ard93          (d9)
    9:00           (:0)
    30,000         (,0)

  --------------------------------------------------

  [\dA-Fa-f] -> hexadecimal characters

  digit or ('A' to 'F') or ('a' to 'f')

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
  \w -> alphanumeric characters + '_' (word characters)
  --------------------------------------------------
  \W -> non-alphanumeric characters - '_'
  --------------------------------------------------

  \d\w 

  digit             + 
  word character

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

  literally 'k'                     +
  literally (0 or 1) 'a' character  +
  literally 'r'

    kar             (kar)
    kr              (kr)

  --------------------------------------------------

  [kts]a?[rmn]

  literally 'k' or 't' or 's'          +
  literally (0 or 1) 'a' character     +
  literally 'r' or 'm' or 'n'


    kn              (kn)
    kan             (kan)

  --------------------------------------------------

  (ram)?\d

  literally (0 or 1) "ram"  + 
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

  literally (0 or more) 'r' character

    rrrrr         (rrrrr)     

  --------------------------------------------------

  ah*

  literally 'a' character  +
  literally (0 or more) 'h' character

    a             (a)
    ah            (ah)
    ahh           (ahh)
    ahhh          (ahhh) 

  --------------------------------------------------

  [krm]*

  literally (0 or more) 'k' or 'r' or 'm' character

    kmkra       (kmkr)  

  --------------------------------------------------

  [xy]*a

  literally (0 or more) 'x' or 'y' character  +
  literally 'a' character

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

  literally (1 or more) 'x' or 'y' character  +
  literally 'a' character

    a           NOT MATCH
    xa          (xa)
    xya         (xya)
    xyxya       (xyxya)

  --------------------------------------------------

  \d+

  (1 or more) digits

    50          (50)
    300         (300)

  --------------------------------------------------
*/

/*
  --------------------------------------------------

  \d{4}

  (exactly 4) digits

    1234          (1234)
    123           NOT MATCH
    12345         (1234)
    123456789     (1234)(5678)

  --------------------------------------------------

  \d{2,}

  (2 or more) digits

    1             NOT MATCH
    12            (12)
    123           (123)
    1234          (1234)
    12345         (12345)
    123456789     (123456789)

  --------------------------------------------------

  \d{3,4}

  (3 to 4) digits

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
                ------------------------------
                | lazy or greedy quantifiers |
                ------------------------------
*/

/*
  - quantifiers are default greedy(maximal munch)

  - lazy quantifiers(minimal munch) are used to match as 
    few characters as possible. 
    '?' token added to the end of quantifier.
*/

/*
  --------------------------------------------------

  /d+

  (1 or more) digits (greedy)

    1234567890              (1234567890)
    123456789012314123      (123456789012314123)

  --------------------------------------------------

  3\.\d+   

  literally '3'                 +
  literally '.'                 +
  (1 or more) digits (greedy)

    3.14159                 (3.14159)
    4.132432                NO MATCH
    3.1415926535            (3.1415926535)

  --------------------------------------------------

  3\.\d+?

  literally '3'                 +
  literally '.'                 +
  (1 or more) digits (lazy)

    3.14159                 (3.1)
    4.132432                NO MATCH
    3.1415926535            (3.1)

  --------------------------------------------------

  3\.\d*?

  literally '3'                 +
  literally '.'                 +
  (0 or more) digits (lazy)

    3.14159                 (3.)
    4.132432                NO MATCH
    3.1415926535            (3.)

  --------------------------------------------------

  hello(world)?

  literally "hello"           +
  literally (0 or 1) "world"


    helloworld              (helloworld)
    hello                   (hello)
    hello123                (hello)

  --------------------------------------------------

  hello(world)??

  literally "hello"                 +
  literally (0 or 1) "world" (lazy)

    helloworld              (hello)
    hello                   (hello)
    hello123                (hello)

  --------------------------------------------------

  <.*>

  literally '<'                                   +
  (0 or more) all characters but '\n' (greedy)    +
  literally '>'

    <h4> hello world </h4>         (<h4> hello world </h4>)

  --------------------------------------------------

  <.*?>

  literally '<'                               +
  (0 or more) all characters but '\n' (lazy)  +
  literally '>'

    <h4> hello world </h4>         (<h4>)

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

  literally "kar" at the beginning of the string

    |kar| kardelen reenkarnasyon kardes kardeslik sakar   ->  (kar)

  --------------------------------------------------

  kar$

  literally "kar" at the end of the string

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

  \stor

  whitespace        +
  literally "tor"

    |    tories         (tor)
    |  torqued        (tor)
    | torso          (tor)  

  --------------------------------------------------

  tor\s

  literally "tor"   +
  
    transistor |    (tor)
    generator    |  (tor)
    inventor |      (tor)

  --------------------------------------------------

  \btor

  word boundary(LINE START)  +
  literally "tor"

    tories         (tor)
    torqued        (tor)
    torso          (tor)

  --------------------------------------------------

  tor\b

  literally "tor"           +
  word boundary(LINE END)

    transistor     (tor)
    inventor       (tor)
    senator        (tor)

  --------------------------------------------------

  \Btor

  NON word boundary(LINE START)  +
  literally "tor"

    transistor     (tor)
    tutorial       (tor)
    generator      (tor)
    mentorship     (tor)

  --------------------------------------------------

  tor\B

  literally "tor"             +
  NON word boundary(LINE END)

    tutorial       (tor)
    tories         (tor)
    torqued        (tor)

  --------------------------------------------------

  \Btor\B

  NON word boundary(LINE START)  +
  literally "tor"                 +
  NON word boundary(LINE END)

    tutorial       (tor)
    restore        (tor)
    victory        (tor)
    factorial      (tor)

  --------------------------------------------------
*/

/*
  --------------------------------------------------

  \d{4}\.[A-F]{4}\.\d{4}

  (exactly 4) digits                  + 
  literally '.'                       + 
  (exactly 4) uppercase ('A' to 'F')  +
  literally '.'                       +
  (exactly 4) digits

    (1234.ABCD.5678)
    (4008.BDDF.6234)

  --------------------------------------------------

  \d{4}\.[A-F]{4}\.\d{4}\b

  (exactly 4) digits                  +
  literally '.'                       +
  (exactly 4) uppercase ('A' to 'F')  +
  literally '.'                       +
  (exactly 4) digits                  +
  word boundary(LINE END)

    T2p1234.ABCD.5678     (1234.ABCD.5678)
    K8yH4008.BDDF.6234    (4008.BDDF.6234)

  --------------------------------------------------

  \d+ 

  (1 or more) digits

  --------------------------------------------------

  a\d+

  literally 'a'       + 
  (1 or more) digits

    fZ8ha3941       (a3941)
    a8242           (a8242)

  --------------------------------------------------

  (a\d)+

  (1 or more) (literally 'a' + digit)  

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

  (exactly 4) digits                + (capture group 1)
  literally '.'                     +
  (exactly 4) ('A' to 'F')          + (capture group 2)
  literally '.'                     +
  (exactly 4) digits                  (capture group 3)

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

  (exactly 4) digits                + (non-capturing group)
  literally '.'                     +
  (exactly 4) ('A' to 'F')          + (capture group 1)
  literally '.'                     +
  (exactly 4) digits                  (capture group 2)

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
  
  (exactly 3) digits          + (capture group 1)
  literally "car"             +
  1st capture group

    match = (123car123)
    capture group 1 = (123)

    match = (456car456)
    capture group 1 = (456)

  --------------------------------------------------

  (\d{3})([A-F]{2})\1\2

  (exactly 3) digits                  + (capture group 1)
  (exactly 2) ('A' to 'F')            + (capture group 2)
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

  (0 or more) any character but '\n' (greedy)   +
  (exactly 3) ('a' to 'z')                      + (capture group 1)
  (0 or more) any character but '\n' (greedy)   +
  1st capture group

    Xanthinthique     
    match = (Xanthinthi)
    capture group 1 = (thi)

    Wunderkinder  
    match = (Wunderkinder)
    capture group 1 = (der)

  --------------------------------------------------

  .*([a-z]{3})[a-z]\1

  (0 or more) any character but '\n' (greedy)   +
  (exactly 3) ('a' to 'z')                      + (capture group 1)
  (exactly 1) ('a' to 'z')                      +
  1st capture group

    wretchlessness
    match = (wretchlessness)
    capture group 1 = (ess)

  --------------------------------------------------

  .*([a-z]{2}).*\1.*\1

  (0 or more) any character but '\n' (greedy)   +
  (exactly 2) ('a' to 'z')                      + (capture group 1)
  (0 or more) any character but '\n' (greedy)   +
  1st capture group                             +
  (0 or more) any character but '\n' (greedy)   +
  1st capture group

    word-for-word
    match = (word-for-wor)
    capture group 1 = (or)

    wine-drinking
    match = (wine-drinkin)
    capture group 1 = (in)

  --------------------------------------------------
*/

/*
                        ---------------
                        | alternation |
                        ---------------
*/

/*
  (car|tar)mal

  literally "car" OR literally "tar"  +
  literally "mal"

    carmal         (carmal)
    tarmal         (tarmal)
*/

/*
  \b([1-9]|[12][0-9]|3[01])\b

  word boundary (LINE START)                          + 
  (
    ((exactly 1) ('1' to '9'))                    
    OR
    (('1' or '2') AND (exactly 1)('0' to '9'))    
    OR
    ((literally '3') AND (exactly 1)('0' or '1'))       
  )                                                   +
  word boundary (LINE END)

    (1) (2) (3)  ... (9)                       
    (10) (11)   ... (19) 
    (20) (21)   ... (29) 
    (30) (31)       
    matches days of month
*/

/*
  \b([1-9]|1[0-2])\b

  word boundary (LINE START)                          + 
  ( 
    ((exactly 1) ('1' to '9'))                 
    OR
    (literally '1'AND (exactly 1) ('0' to '2')) 
  )                                                   +    
  word boundary (LINE END)                

    (1) (2) (3)  ... (9) (10) (11) (12)
    matches months of year
*/

/*
  \b([0-9]|[1-9][0-9]|[1-9][0-9][0-9])\b
  \b(\d|[1-9]\d|[1-9]\d{2})\b

  word boundary (LINE START)        + 
  ( 
    (exactly 1) ('0' to '9')                
    OR
    (
      (exactly 1) ('1' to '9')  
      AND 
      (exactly 1) ('0' to '9')
    )                                       
    OR
    (
      (exactly 1) ('1' to '9')  
      AND 
      (exactly 2) ('0' to '9') 
    )
  )                                 +
  word boundary (LINE END)

    matches from (0) to (999)
*/

/*
            ------------------------------------
            | positive and negative look ahead | 
            ------------------------------------
            | positive and negative look back  |
            ------------------------------------
  Not every regex grammar supports look ahead and look back.
*/

/*
  hello(?=world)      -> positive look ahead

  literally "hello"   +
  literally "world"   +
  (only match "hello" if it is followed by "world")

    aliens_helloworld       (hello)
    aliens_hellogalaxy      NO MATCH
    aliens_hellouniverse    NO MATCH
*/

/*
  hello(?=\d\d)     -> positive look ahead

  literally "hello"           +
  (exactly 2) digits          +
  (only match "hello" if it is followed by 2 digits)

    repeathello             NO MATCH
    repeathello1            NO MATCH
    repeathello12           (hello)
    repeathello123          (hello)
*/

/*
  hello(?!world)      -> negative look ahead

  literally "hello"   +
  literally "world"   +
  (only match "hello" if it is NOT followed by "world")

    hello                   (hello)
    aliens_helloworld       NO MATCH
    aliens_hellogalaxy      (hello)
    aliens_hellouniverse    (hello)
*/

/*
  (?<=hello)world     -> positive look back

  literally "hello"   +
  literally "world"   +
  (only match "world" if it is preceded by "hello")

    world                  NO MATCH
    helloworld             (world)
    hiworld                NO MATCH
*/

/*
  (?<=\d)hello      -> positive look back

  digit               +
  literally "hello"   +
  (only match "hello" if it is preceded by a digit)

  hello                 NO MATCH
  1hello                (hello)
  12hello               (hello)
*/

/*
  (?<!hello)world     -> negative look back

  literally "hello"   +
  literally "world"   +
  (only match "world" if it is NOT preceded by "hello")

    helloworld          NO MATCH
    bravenewworld       (world)
    1world              (world)
*/

/*
  postal code example: 43215-1293 or 43215 

  \d{5}(?:-\d{4})?

  (exactly 5) digits                              +
  (0 or 1) (literally '-' + (exactly 4) digits)   
    (non-capturing group)

    43215           (43215)
    43215-1293      (43215-1293)
*/

/*
  RGB color picker example: #000000 to #FFFFFF

  #(?:[\dA-Fa-f]{6}|[\dA-Fa-f]{3})
  literally '#'                                         +                      
  (
    (exactly 6) (digit OR ('A' to 'Z') OR ('a' to 'f'))  
    OR
    (exactly 3) (digit OR ('A' to 'Z') OR ('a' to 'f'))
  ) (non-capturing group)

  #AB1294     (#AB1294)
  #AB1        (#AB1)
  #124        (#124)
  #00a        (#00a)
  #87a        (#87a)                             
*/

/*
                    --------------------------
                    | <regex> module (C++11) |
                    --------------------------
*/

/*
  - farklı regex gramerleri seçilebilir.
  - regex stringi, regex sınıfı türünden 
    bir nesne ile ifade edilir.

  - std::regex'in türü:
    class std::basic_regex<char, std::regex_traits<char>>;
  - std::wregex'in türü:
    class std::basic_regex<wchar_t, std::regex_traits<wchar_t>>;
*/

/*
  #include <regex>

  int main()
  {
    // -----------------------------------------------------

    // \d{4}\s{3}arm

    //  (exactly 4) digits        + 
    //  (exactly 3) whitespaces   + 
    //  literally "arm"

    std::regex rgx1_1{ "\\d{4}\\s{3}arm" };
    std::regex rgx1_2{ R"(\d{4}\s{3}arm)" };

    // -----------------------------------------------------

    // \\hello\\world

    // literally '\'      + 
    // literally "hello"  + 
    // literally '\'      +
    // literally "world"

    std::regex rgx2_1{ "\\\\hello\\\\world" };
    std::regex rgx2_2{ R"(\\hello\\world)" };

    // -----------------------------------------------------
  }
*/

/*
  // "regex_match" function returns bool
  // 3rd parameter is default argument

  #include <regex>  // std::regex_match
  #include <string>

  int main()
  {
    std::regex rgx{ R"(\d{3}[a-z]{4}\d{3})" };

    // -----------------------------------------------------

    std::string str{ "123abcd456" };

    if (std::regex_match(str, rgx))
      std::cout << "regex matched\n";
    else
      std::cout << "regex not matched\n";
    // output -> regex matched

    // -----------------------------------------------------

    str.assign("1111world2222");

    if (std::regex_match(str, rgx))
      std::cout << "regex matched\n";
    else
      std::cout << "regex not matched\n";
    // output -> regex not matched

    // -----------------------------------------------------
  }
*/

/*
  #include <string>
  #include <vector>
  #include <fstream>    // std::ifstream, std::ofstream
  #include <iterator>
  #include <regex>
  #include <format>

  using namespace std;
  vector<string> file_to_vec(const string& file_name)
  {
    ifstream ifs{ file_name };
    if (!ifs) {
      cerr << "file open error\n";
      throw std::runtime_error{ "file open error" };
    }

    return vector<string>{ istream_iterator<string>{ifs}, {} };
    // returning temporary object (Mandatory Copy Elision)
  }

  int main()
  {
    auto vec = file_to_vec("dictionary.txt");
    cout << format("vec.size() = {}", vec.size()) << '\n';
    // output -> vec.size() = 466549

    std::ofstream ofs{ "output.txt" };
    if (!ofs) {
      std::cerr << "file open error\n";
      throw std::runtime_error{ "file open error" };
    }

    // -----------------------------------------------------

    regex rgx1{ "\\b[a-f]{4}tion" };

    // word boundary (LINE START)               +
    // (4 to 7) ('a' to 'f')                    +
    // literally "tion"

    for (const auto& word : vec) 
    {
      if (regex_match(word, rgx1))
        ofs << word << '\n';
    }
    // output.txt -> abaction

    // -----------------------------------------------------

    regex rgx2{ "\\b[a-f]{4,7}tion\\w+"};

    // word boundary (LINE START)                 +
    // 4 to 7 lowercase characters from a to f    +
    // literally "tion"                           +
    // 1 or more alphanumeric characters + '_'  (greedy)

    for (const auto& word : vec) 
    {
      if (regex_match(word, rgx2))
        ofs << word << '\n';
    }

    // output.txt ->
    //  affectional
    //  affectionally
    //  affectionate
    //  affectionately
    //  affectionateness
    //  affectioned
    //  affectionless
    //  affections
    //  defecations
    //  defectionist
    //  defections

    // -----------------------------------------------------

    regex rgx3{ "co.*ion" };

    // literally "co"                       +
    // (0 or more) any character but '\n'   +
    // literally "ion"

    for (const auto& word : vec) 
    {
      if (regex_match(word, rgx3))
        ofs << word << '\n';
    }

    // output.txt ->
    //  coacervation
    //  coaction
    //  coadaptation
    //  co-adaptation
    //  coadministration
    //  coadmiration
    //  coadunation
    //  coaffirmation
    //  coaggregation
    //  ...

    // -----------------------------------------------------
  }
*/

/*
  smatch   : search on std::string
  cmatch   : search on const char* (C-style string)

  - smatch is a container of sub_match objects.
*/

/*
  #include <regex>  // std::smatch
  #include <format>

  int main()
  {
    std::smatch sm;   // default initialized

    std::cout << std::format("{}\n", sm.empty());  
    // output -> true
  }
*/

/*
  #include <regex>  // std::smatch, std::regex_match
  #include <string>

  int main()
  {
    std::string str{ "1234hello5678" };

    std::regex rgx{ "\\d{4}[a-z]{5}\\d{4}" };
    // (exactly 4) digits         +
    // (exactly 5) ('a' to 'z')   +
    // (exactly 4) digits

    std::smatch sm;

    // function's 2nd parameter is std::smatch& type
    if (std::regex_match(str, sm, rgx)) 
    {
      using namespace std;
      cout << "regex matched\n";

      cout << "sm.size() = " << sm.size() << '\n';
      cout << "sm.length(0) = " << sm.length(0) << '\n';
      cout << "sm.position(0) = " << sm.position(0) << '\n';
    }
    else
      std::cout << "regex not matched\n";
    
    // output -> 
    //  regex matched
    //  sm.size() = 1
    //  sm.length(0) = 13
    //  sm.position(0) = 0
  }
*/

/*
  // in this scenario with 3 capture groups,
  // regex_search or regex_match functions 
  // will add 4 sub_match objects to smatch container
  //  1 : whole match
  //  2 : 1st capture group
  //  3 : 2nd capture group
  //  4 : 3rd capture group

  #include <regex>    // std::smatch, std::regex_match
  #include <string>
  #include <format>

  int main()
  {
    using namespace std;

    string str{ "1234hello5678" };

    regex rgx{ "(\\d{4})([a-z]{5})(\\d{4})" };
    // (exactly 4) digits         +   (1st capture group)
    // (exactly 5) ('a' to 'z')   +   (2nd capture group)
    // (exactly 4) digits             (3rd capture group)

    smatch sm;

    if (regex_match(str, sm, rgx)) 
    {
      cout << "sm.size() = " << sm.size() << '\n';

      for (size_t i{}; i < sm.size(); ++i) {
        cout << format("sm.str({}) = {}\n", i, sm.str(i));
        cout << format("sm.position({}) = {}\n", i, sm.position(i));
        cout << format("sm.length({}) = {}\n", i, sm.length(i));  
        cout << '\n';
      }
    }
    else
      std::cout << "regex not matched\n";
    
    // output -> 
    //  sm.size() = 4   --> 1 whole match + 3 capture groups

    //  sm.str(0) = 1234hello5678 --> whole match
    //  sm.position(0) = 0        --> whole match         INDEX
    //  sm.length(0) = 13         --> whole match         LENGTH

    //  sm.str(1) = 1234          --> 1st capture group 
    //  sm.position(1) = 0        --> 1st capture group   INDEX
    //  sm.length(1) = 4          --> 1st capture group   LENGTH

    //  sm.str(2) = hello         --> 2nd capture group 
    //  sm.position(2) = 4        --> 2nd capture group   INDEX
    //  sm.length(2) = 5          --> 2nd capture group   LENGTH

    //  sm.str(3) = 5678          --> 3rd capture group 
    //  sm.position(3) = 9        --> 3rd capture group   INDEX
    //  sm.length(3) = 4          --> 3rd capture group   LENGTH
  }
*/

/*
  #include <regex>  // std::smatch, std::regex_match
  #include <string>
  #include <format>

  int main()
  {
    using namespace std;
    cout << boolalpha;

    string str{ "1234hello5678" };

    regex rgx{ "(\\d{4})([a-z]{5})(\\d{4})" };
    // (exactly 4) digits         +   (1st capture group)
    // (exactly 5) ('a' to 'z')   +   (2nd capture group)
    // (exactly 4) digits             (3rd capture group)

    smatch sm;

    if (regex_match(str, sm, rgx)) 
    {
      for (size_t i{}; i < sm.size(); ++i) {
        cout << format("sm[{}] = ", i) << sm[i] << '\n';
        // operator<< overload for sub_match object
        cout << format("sm[{}].length() = {}\n", i, sm[i].length());
        cout << format("sm[{}].matched = {}\n", i, sm[i].matched);
      }
    }
    
    // output -> 
    //  sm[0] = 1234hello5678
    //  sm[0].length() = 13
    //  sm[0].matched = true
    //  sm[1] = 1234
    //  sm[1].length() = 4
    //  sm[1].matched = true
    //  sm[2] = hello
    //  sm[2].length() = 5
    //  sm[2].matched = true
    //  sm[3] = 5678
    //  sm[3].length() = 4
    //  sm[3].matched = true
  }
*/

/*
  ---------------------------------------------------------
  regex rgx{ "(\\d{4})([a-z]{5})(\\d{4})" };
  string str{ "galaxy1234hello5678universe" };
  ---------------------------------------------------------

  smatch sm;        --->  total 4 sub_match objects
  (1234hello5678)     -> whole match
  (1234)              -> 1st capture group
  (hello)             -> 2nd capture group
  (5678)              -> 3rd capture group

  all 4 sub_match object's "matched" bool member will be true.

  ---------------------------------------------------------

  (galaxy) and (universe) are NOT matched 
  but they are in the string

  so "regex_search" function will give 
  2 more sub_match object 
    - prefix sub_match object for (galaxy) 
    - suffix sub_match object for (universe)

  so prefix and suffix sub_match object's "matched" 
  bool member will be false.

  ---------------------------------------------------------
*/

/*
  #include <regex>    // std::smatch, std::regex_match
  #include <string>
  #include <format>

  int main()
  {
    using namespace std;
    cout << boolalpha;

    string str{ "1234hello5678" };

    regex rgx{ "(\\d{4})([a-z]{5})(\\d{4})" };
    // (exactly 4) digits         +   (1st capture group)
    // (exactly 5) ('a' to 'z')   +   (2nd capture group)
    // (exactly 4) digits             (3rd capture group)

    smatch sm;

    if (regex_match(str, sm, rgx)) 
    {
      for (const auto& sm_elem : sm) {
        cout << "sm_elem.length() = " << sm_elem.length() << '\n';
        cout << "sm_elem.str()    = " << sm_elem.str() << '\n';
        cout << "sm_elem.matched  = " << sm_elem.matched << '\n';
        cout << '\n';
      }
    }
    
    // output -> 
    //  sm_elem.length() = 13
    //  sm_elem.str()    = 1234hello5678
    //  sm_elem.matched  = true
    //  
    //  sm_elem.length() = 4
    //  sm_elem.str()    = 1234
    //  sm_elem.matched  = true
    //  
    //  sm_elem.length() = 5
    //  sm_elem.str()    = hello
    //  sm_elem.matched  = true
    //  
    //  sm_elem.length() = 4
    //  sm_elem.str()    = 5678
    //  sm_elem.matched  = true
  }
*/

/*
  #include <regex>    // std::smatch, std::regex_match
  #include <string>
  #include <format>

  int main()
  {
    using namespace std;
    cout << boolalpha;

    string str{ "1234hello5678" };

    regex rgx{ "(\\d{4})([a-z]{5})(\\d{4})" };
    // (exactly 4) digits         +   (1st capture group)
    // (exactly 5) ('a' to 'z')   +   (2nd capture group)
    // (exactly 4) digits             (3rd capture group)

    smatch sm;

    if (regex_match(str, sm, rgx)) 
    {
      for (auto iter = sm.cbegin(); iter != sm.cend(); ++iter) 
      {
        cout << "sm_elem.length() = " << iter->length() << '\n';
        cout << "sm_elem.str()    = " << iter->str() << '\n';
        cout << "sm_elem.matched  = " << iter->matched << '\n';
        cout << '\n';
      }
    }
    
    // output -> 
    //  sm_elem.length() = 13
    //  sm_elem.str()    = 1234hello5678
    //  sm_elem.matched  = true
    //  
    //  sm_elem.length() = 4
    //  sm_elem.str()    = 1234
    //  sm_elem.matched  = true
    //  
    //  sm_elem.length() = 5
    //  sm_elem.str()    = hello
    //  sm_elem.matched  = true
    //  
    //  sm_elem.length() = 4
    //  sm_elem.str()    = 5678
    //  sm_elem.matched  = true
  }
*/

/*
  #include <fstream>    // std::ifstream
  #include <string>
  #include <regex>      // std::regex_matc, std::smatch
  #include <format>

  int main()
  {
    using namespace std;

    ifstream ifs{ "floats.txt" };
    if (!ifs) {
      cerr << "file open error\n";
      throw std::runtime_error{ "file open error" };
    }

    string line;

    regex rgx{ "([-+]?\\d*)\\.?(\\d+)" };
    //  (
    //    (0 or 1) literally ('-' or '+') 
    //    AND  
    //    (0 or more) digits 
    //  )                                   +   (1st capture group)
    // (0 or 1) any character but '\n'      +
    // (1 or more) digits                   +   (2nd capture group)

    smatch sm;

    while (ifs >> line)
    {
      if (regex_match(line, sm, rgx)) {
        cout << format("{:<15} is VALID\n", line);
        cout << format("whole part = {:<10} |", sm.str(1));
        cout << format("fractional part = {:10}\n", sm.str(2));
        cout << '\n';
      }
      else
        cout << format("{:<15} is INVALID\n\n", line);
    }
    // output ->
    //  hello           is INVALID
    //  
    //  12.4            is VALID
    //  whole part = 12         |fractional part = 4
    //  
    //  +15.76985       is VALID
    //  whole part = +15        |fractional part = 76985
    //  
    //  -1.2            is VALID
    //  whole part = -1         |fractional part = 2
    //  
    //  world           is INVALID
    //  
    //  9.87348654      is VALID
    //  whole part = 9          |fractional part = 87348654
    //  
    //  999990.12       is VALID
    //  whole part = 999990     |fractional part = 12
    //  
    //  -1275634.333334 is VALID
    //  whole part = -1275634   |fractional part = 333334
    //  
    //  galaxy          is INVALID
    //  
    //  93.333          is VALID
    //  whole part = 93         |fractional part = 333
  }
*/

/*
  #include <regex>

  int main()
  {
    // -----------------------------------------------------

    std::regex rgx1{ "hello" };
    // literally "hello"

    std::cout << "rgx1.mark_count() = " 
              << rgx1.mark_count() << '\n';
    // output -> rgx1.mark_count() = 0

    // -----------------------------------------------------

    std::regex rgx2{ "(hello) (world)" };

    // literally "hello"    +     (1st capture group)
    // literally ' '        + 
    // literally "world"          (2nd capture group)

    std::cout << "rgx2.mark_count() = " 
              << rgx2.mark_count() << '\n';
    // output -> rgx2.mark_count() = 2

    // -----------------------------------------------------

    std::regex rgx3{ "(he(ll)o) (w(or)ld)" };

    // literally "hello"    +     (1st capture group)
    // literally ' '        +
    // literally "world"          (3rd capture group) 

    // literally "ll" inside 1st capture group --> 2nd capture group
    // literally "or" inside 3rd capture group --> 4th capture group

    std::cout << "rgx3.mark_count() = " 
              << rgx3.mark_count() << '\n';
    // output -> rgx3.mark_count() = 4

    // -----------------------------------------------------

    std::regex rgx4{ "\\(hello world\\)" };

    // literally '('          +
    // literally "hello"      +
    // literally ' '          +
    // literally "world"      +
    // literally ')'

    std::cout << "rgx4.mark_count() = " 
              << rgx4.mark_count() << '\n';
    // output -> rgx4.mark_count() = 0

    // -----------------------------------------------------

    std::regex rgx5{ "(?:hello) (world)" };
    // literally "hello"    +     (NO capture group)
    // literally ' '        +
    // literally "world"          (1st capture group)

    std::cout << "rgx5.mark_count() = " 
              << rgx5.mark_count() << '\n';
    // output -> rgx5.mark_count() = 1

    // -----------------------------------------------------

    std::regex rgx6{ "(hello) (world)", 
                      std::regex_constants::nosubs };

    // literally "hello"    +     (1st capture group)
    // literally ' '        +
    // literally "world"          (2nd capture group)

    std::cout << "rgx6.mark_count() = " 
              << rgx6.mark_count() << '\n';
    // output -> rgx6.mark_count() = 0

    // because of 2nd parameter is std::regex_constants::nosubs
    // there WON'T BE ANY capture group

    // -----------------------------------------------------
  }
*/

/*
  #include <vector>
  #include <string>
  #include <fstream>    // std::ifstream, std::ofstream
  #include <iterator>   // std::istream_iterator
  #include <algorithm>  // std::for_each
  #include <regex>      // std::smatch
  #include <format>

  using namespace std;

  vector<string> file_to_vec(const string& filename)
  {
    ifstream ifs{ filename };
    if (!ifs)
      throw std::runtime_error{ "file open error" };
    
    return vector<string>{ istream_iterator<string>{ ifs }, {} };
    // returning temporary object (Mandatory Copy Elision)
  }

  int main()
  {
    auto vec = file_to_vec("dictionary.txt");

    std::ofstream ofs{ "output.txt" };
    if (!ofs) 
      throw std::runtime_error{ "file open error" };

    regex rgx{ "([krmnt]+)(ar)(\\w*)(in)(\\w*)" };

    // (1 or more) ('k' OR 'r' OR 'm' OR 'n' OR 't')  + (1CG)
    // literally "ar"                                 + (2CG)
    // (0 or more) word characters                    + (3CG)
    // literally "in"                                 + (4CG)
    // (0 or more) word characters                    + (5CG)

    smatch sm;


    for_each(vec.cbegin(), vec.cend(), [&](const string& str){
      if (regex_match(str, sm, rgx))
      {
        ofs << format("({})({})({})({})({})\n", 
            sm.str(1), 
            sm.str(2), 
            sm.str(3),
            sm.str(4), 
            sm.str(5));
      }});

    // output.txt ->
    //  (k)(ar)(ab)(in)(er)
    //  (k)(ar)()(in)(ghota)
    //  (k)(ar)(yok)(in)(esis)
    //  (k)(ar)(yok)(in)(etic)
    //  (k)(ar)(yot)(in)()
    //  (k)(ar)(yot)(in)(s)
    //  (k)(ar)(t)(in)(g)
    //  ...
  }
*/

/*
  #include <vector>
  #include <string>
  #include <fstream>    // std::ifstream, std::ofstream
  #include <iterator>   // std::istream_iterator
  #include <regex>      // std::regex_search, std::smatch

  using namespace std;
  vector<string> file_to_vec(const string& file_name)
  {
    ifstream ifs{ file_name };
    if (!ifs) 
      throw std::runtime_error{ "file open error" };
    
    return vector<string>{ istream_iterator<string>{ ifs }, {} };
    // returning temporary object (Mandatory Copy Elision)
  }

  int main()
  {
    auto vec = file_to_vec("numbers.txt");

    std::ofstream ofs{ "output.txt" };
    if (!ofs) 
      throw std::runtime_error{ "file open error" };

    regex rgx{ "(\\d{4})\\.([A-F]{4})\\.(\\d{4})" };

    // (exactly 4) digits             +   (1st capture group)
    // literally '.'                  +
    // (exactly 4) ('A' to 'F')       +   (2nd capture group)
    // literally '.'                  +
    // (exactly 4) digits                 (3rd capture group)

    smatch sm;
    for (const auto& str : vec) {
      if (regex_search(str, sm, rgx)) {
        if (sm.str(2)[0] == 'F' && sm[3].str()[0] == '1') {
          ofs << format("{:<10}({})({})({}){:>10}\n",
                    sm.prefix().str(),  // sub_match object
                    sm.str(1),          // sub_match object
                    sm.str(2),          // sub_match object
                    sm.str(3),          // sub_match object
                    sm.suffix().str()); // sub_match object
        }
      }
    }

    // output.txt ->
    //  efgh      (5678)(FBDE)(1234)      qrst
    //  efgh      (5678)(FBDE)(1234)      opqr
  }
*/

/*
                  -----------------------
                  | std::regex_iterator |
                  -----------------------
*/

/*
  #include <string>
  #include <fstream>    // std::ifstream
  #include <iterator>   // std::istreambuf_iterator
  #include <regex>      // std::sregex_iterator

  using namespace std;

  string file_to_str(const string& filename)
  {
    ifstream ifs{ filename };
    if (!ifs)
      throw std::runtime_error{ "file open error" };

    return string{ istreambuf_iterator<char>{ ifs }, {} };
    // returning temporary object (Mandatory Copy Elision)
  }

  int main()
  {
    auto str = file_to_str("numbers.txt");

    regex rgx{ "(\\d{4})\\.([A-F]{4})\\.(\\d{4})" };

    // iterator to smatch containers begin position 
    //  std::sregex_iterator(beg, end, rgx)
    // iterator to smatch containers end position 
    //  std::sregex_iterator() -> default initialized

    for ( sregex_iterator iter{ str.begin(), str.end(), rgx}, end;
          iter != end; ++iter) 
    {
      cout << format("{}\n", iter->str(0)); 
      // will write all matches
      cout << format("{}\n", iter->str(1)); 
      // will write 1st capture group
      cout << format("{}\n", iter->str(2)); 
      // will write 2nd capture group
      cout << format("{}\n", iter->str(3)); 
      // will write 3rd capture group

      if (iter->str(2)[0] == 'F' && iter->str(3)[0] == '1')
        cout << format("{}\n", iter->str());
      // output ->
      //  5678.FBDE.1234
      //  5678.FBDE.1234
    }
  }
*/

/*
  #include <string>
  #include <fstream>    // std::ifstream
  #include <iterator>   // std::istreambuf_iterator
  #include <regex>      // std::sregex_iterator

  using namespace std;

  string file_to_str(const string& filename)
  {
    ifstream ifs{ filename };
    if (!ifs)
      throw std::runtime_error{ "file open error" };

    return string{ istreambuf_iterator<char>{ ifs }, {} };
    // returning temporary object (Mandatory Copy Elision)
  }

  int main()
  {
    auto str = file_to_str("sentences.txt");

    // -----------------------------------------------------

    regex rgx{ "\\lac\\w*" };

    // word boundary (LINE START)   +
    // literally "tor"              +
    // 0 or more word character (greedy)

    int cnt{};
    for ( sregex_iterator iter{ str.begin(), str.end(), rgx};
          iter != sregex_iterator{}; ++iter) 
    {
      cout << format("{:<4} {}\n", ++cnt, iter->str());
    }
    // output ->
    //  1    lacus
    //  2    lacerat
    //  3    lacus
    //  4    lacinia
    //  5    lacerat
    //  6    lacinia
    //  ...

    // -----------------------------------------------------

    cnt = 0;
    regex rgx2{ "\\w*lac\\w*" };

    // 0 or more word character (greedy) +
    // literally "lac"                   +
    // 0 or more word character (greedy)

    for ( sregex_iterator iter{ str.begin(), str.end(), rgx2};
          iter != sregex_iterator{}; ++iter) 
    {
      cout << format("{:<4} {}\n", ++cnt, iter->str());
    }
    // output ->
    //  1    lacus
    //  2    placerat
    //  3    lacus
    //  4    lacinia
    //  5    placerat
    //  6    lacinia
    //  ...

    // -----------------------------------------------------

    cnt = 0;
    regex rgx3{ "\\w*tor\\b" };

    // 0 or more word character (greedy) +
    // literally "tor"                   +
    // word boundary (LINE END)

    for ( sregex_iterator iter{ str.begin(), str.end(), rgx3};
          iter != sregex_iterator{}; ++iter) 
    {
      cout << format("{:<4} {}\n", ++cnt, iter->str());
    }

    // output ->
    //  1    auctor
    //  2    tortor
    //  3    porttitor
    //  4    auctor
    //  ...

    // -----------------------------------------------------
  }
*/

/*
                  -----------------------------
                  | std::regex_token_iterator |
                  -----------------------------
*/

/*
  #include <string>
  #include <fstream>    // std::ifstream
  #include <iterator>   // std::istreambuf_iterator
  #include <regex>      // std::sregex_token_iterator
  #include <format>

  using namespace std;

  string file_to_str(const string& filename)
  {
    ifstream ifs{ filename };
    if (!ifs)
      throw std::runtime_error{ "file open error" };

    return string{ istreambuf_iterator<char>{ ifs }, {} };
    // returning temporary object (Mandatory Copy Elision)
  }

  int main()
  {
    auto s = file_to_str("numbers.txt");

    regex rgx{ "(\\d{4})\\.([A-F]{4})\\.(\\d{4})" };

    // -----------------------------------------------------

    for ( sregex_token_iterator it{ s.begin(), s.end(), rgx, 0 };
          it != sregex_token_iterator{}; ++it) 
    {
      cout << format("{}\n", it->str());
    }
    // output ->
    //  1234.ABCD.5678
    //  5678.BCDE.1234
    //  2345.CDEF.6789
    //  3456.DFAB.7890

    // -----------------------------------------------------

    for ( sregex_token_iterator it{ s.begin(), s.end(), rgx, 1 };
          it != sregex_token_iterator{}; ++it) 
    {
      cout << format("{}\n", it->str());
    }

    // output ->
    //  1234
    //  5678
    //  2345
    //  3456

    // -----------------------------------------------------

    for ( sregex_token_iterator it{ s.begin(), s.end(), rgx, 2 };
          it != sregex_token_iterator{}; ++it) 
    {
      cout << format("{}\n", it->str());
    }

    // output ->
    //  ABCD
    //  BCDE
    //  CDEF
    //  DFAB

    // -----------------------------------------------------

    for ( sregex_token_iterator it{ s.begin(), s.end(), rgx, 3 };
          it != sregex_token_iterator{}; ++it) 
    {
      cout << format("{}\n", it->str());
    }

    // output ->
    //  5678
    //  1234
    //  6789
    //  7890

    // -----------------------------------------------------

    for ( sregex_token_iterator it{ s.begin(), s.end(), rgx, {1, 2} };
          it != sregex_token_iterator{}; ++it) 
    {
      cout << format("{}\n", it->str());
    }

    // output ->
    //  1234    (index 1)
    //  ABCD    (index 2)
    //  5678    (index 1)
    //  BCDE    (index 2)
    //  2345    (index 1)
    //  CDEF    (index 2)
    //  3456    (index 1)
    //  DFAB    (index 2)

    // -----------------------------------------------------

    // -1 index is for non matching parts

    for ( sregex_token_iterator it{ s.begin(), s.end(), rgx, -1 };
          it != sregex_token_iterator{}; ++it) 
    {
      cout << format("{}\n", it->str());
    }

    // output ->
    //  abcd
    //  efghi
    //  jklm
    //  nopqr
    //  stuv
    //  wxyz
    //  efgh
    //  ijklm
    //  ...

    // numbers.txt
    //  abcd1234.ABCD.5678efghi     ---> abcd AND efghi 
    //  jklm5678.BCDE.1234nopqr     ---> jklm AND nopqr
    //  stuv2345.CDEF.6789wxyz      ---> stuv AND wxyz
    //  efgh3456.DFAB.7890ijklm     ---> efgh AND ijklm

    // -----------------------------------------------------
  }
*/

/*
  // using regex_token_iterator for tokenizing a string

  #include <string>
  #include <regex>      // std::sregex_token_iterator
  #include <format>

  int main()
  {
    using namespace std;

    string s{ "Monday, Tuesday, Wednesday.. Thursday! Friday." };

    regex rgx{"[\\s,.!]+"};
    // (1 or more) (whitespace OR ',' OR '.' OR '!') (greedy)

    for ( sregex_token_iterator it{ s.begin(), s.end(), rgx, -1 };
          it != sregex_token_iterator{}; ++it) 
    {
      cout << format("{}\n", it->str());
    }
    // output ->
    //  Monday
    //  Tuesday
    //  Wednesday
    //  Thursday
    //  Friday
  }
*/

/*
                      ----------------------
                      | std::regex_replace |
                      ----------------------
*/

/*
  #include <string>
  #include <regex>    // std::regex_iterator, std::regex_replace
  #include <format>

  int main()
  {
    using namespace std;

    string str{ "zoosporangial outlive zoosporangium "
                "zoospore disemploys zoosporic sculpts "
                "zoosporocyst bowmaking zoosterol glacier" };

    regex rgx{"\\b(zoo)([^ ]*)"};

    // word boundary (LINE START)               +
    // literally "zoo"                          +   (1CG)
    // (0 or more) NOT space character (greedy)     (2CG)


    for (sregex_iterator iter{ str.begin(), str.end(), rgx };
        iter != sregex_iterator{}; ++iter) 
    {
      cout << format("{:<20} - {} ({})\n", 
                iter->str(), iter->str(1), iter->str(2));
    }
    // output ->
    //  zoosporangial        - zoo (sporangial)
    //  zoosporangium        - zoo (sporangium)
    //  zoospore             - zoo (spore)
    //  zoosporic            - zoo (sporic)
    //  zoosporocyst         - zoo (sporocyst)
    //  zoosterol            - zoo (sterol)

    // ---------------------------------------------------------

    auto s1 = regex_replace(str, rgx, "*");
    // regex_replace function returns std::string
    // putting '*' characters for each match

    cout << format("[{}]\n", s1);
    // output -> 
    //  [* outlive * * disemploys * sculpts * bowmaking * glacier]

    // ---------------------------------------------------------

    auto s2 = regex_replace(str, rgx, "($1) _$2_");
    // 1 index sub_match will have paranthesis
    // 2 index sub_match will have underscore
    // non matching parts will be as it is

    cout << format("[{}]\n", s2);
    // output ->
    //  [(zoo) _sporangial_ outlive (zoo) _sporangium_ (zoo) 
    //  _spore_ disemploys (zoo) _sporic_ sculpts (zoo) 
    //  _sporocyst_ bowmaking (zoo) _sterol_ glacier]

    // ---------------------------------------------------------

    auto s3 = regex_replace(str, rgx, "$2$1");
    
    cout << format("[{}]\n", s3);
    // output ->
    //  [sporangialzoo outlive sporangiumzoo sporezoo disemploys 
    //  sporiczoo sculpts sporocystzoo bowmaking sterolzoo glacier]

    // ---------------------------------------------------------

    auto s4 = regex_replace(str, rgx, "($`)");
    // $` is for prefix sub_match object

    cout << format("[{}]\n", s4);
    // output ->
    //  [() outlive ( outlive ) ( ) disemploys ( disemploys ) 
    //  sculpts ( sculpts ) bowmaking ( bowmaking ) glacier]

    // ---------------------------------------------------------

    auto s5 = regex_replace(str, rgx, "($&)");
    // $& is for whole matches

    cout << format("[{}]\n", s5);
    // output ->  
    //  [(zoosporangial) outlive (zoosporangium) (zoospore) 
    //  disemploys (zoosporic) sculpts (zoosporocyst) bowmaking 
    //  (zoosterol) glacier]

    // ---------------------------------------------------------
  }
*/

/*
  #include <string>
  #include <regex>

  int main()
  {
    std::string str{ "hello hello world world galaxy galaxy" };

    std::regex rgx{ "\\b(\\w+)\\s+\\1"};

    // word boundary (LINE START)             +
    // (1 or more) word character (greedy)    + (1st capture group)
    // (1 or more) whitespaces    (greedy)    +
    // 1st capture group

    std::cout << std::regex_replace(str, rgx, "$1");
    // output -> hello world galaxy

    // "hello hello" is a match, "hello" is 1st capture group
    // replacing match with 1st capture group
  }
*/