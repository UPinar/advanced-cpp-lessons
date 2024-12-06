#include <iostream>

/*
                  ==========================
                  | `regex` module (C++11) |
                  ==========================
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

  /d+ -> 1 or more digits(greedy)

    1234567890              (1234567890)
    123456789012314123      (123456789012314123)

  --------------------------------------------------

  3\.\d+   

  3 digit               +
  literally '.'         +
  1 or more digits(greedy)

    3.14159                 (3.14159)
    4.132432                NO MATCH
    3.1415926535            (3.1415926535)

  --------------------------------------------------

  3\.\d+?

  3 digit               +
  literally '.'         +
  1 or more digits(lazy)

  3.14159                 (3.1)
  4.132432                NO MATCH
  3.1415926535            (3.1)

  --------------------------------------------------

  3\.\d*?

  3 digit               +
  literally '.'         +
  0 or more digits(lazy)

  3.14159                 (3.)
  4.132432                NO MATCH
  3.1415926535            (3.)

  --------------------------------------------------

  hello(world)?

  literally "hello"     +
  literally "world" or nothing  (greedy)


  helloworld              (helloworld)
  hello                   (hello)
  hello123                (hello)

  --------------------------------------------------

  hello(world)??

  literally "hello"     +
  literally "world" or nothing  (lazy)

  helloworld              (hello)
  hello                   (hello)
  hello123                (hello)

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

/*
                        ---------------
                        | alternation |
                        ---------------
*/

/*
  (car|tar)mal

  literally 'c' + 'a' + 'r' OR literally 'm' + 'a' + 'l'  +
  literally 'm' + 'a' + 'l'

  carmal         (carmal)
  tarmal         (tarmal)
*/

/*
  \b([1-9]|[12][0-9]|3[01])\b

  word boundary (LINE START)                      + 
  ( 1 digit from 1 to 9                           OR 
    1 or 2 and 1 digit from 0 to 9                OR
    3 and 0 or 1 )                                +
  word boundary (LINE END)

  (1) (2) (3)  ... (9)                       
  (10) (11)   ... (19) 
  (20) (21)   ... (29) 
  (30) (31)       

  matches days of month
*/

/*
  \b([1-9]|1[0-2])\b

  word boundary (LINE START)                      + 
  ( 1 digit from 1 to 9                           OR 
    1 and 0 or 1 or 2 )                           +

  (1) (2) (3)  ... (9) (10) (11) (12)
  
  matches months of year
*/

/*
  \b([0-9]|[1-9][0-9]|[1-9][0-9][0-9])\b
  \b(\d|[1-9]\d|[1-9]\d{2})\b

  word boundary (LINE START)                        + 
  ( 1 digit from 0 to 9                             OR 
    1 digit from 1 to 9 and 1 digit from 0 to 9     OR
    1 digit from 1 to 9 and 2 digits from 0 to 9 )  +
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

  literally "hello" +
  2 digits          +
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

  5 digits                +
  0 or 1 of (literally '-' + 4 digits) (non-capturing group)

  43215           (43215)
  43215-1293      (43215-1293)
*/

/*
  RGB color picker example: #000000 to #FFFFFF

  #(?:[\dA-Fa-f]{6}|[\dA-Fa-f]{3})

  literally '#'                                         +                      
  6 characters from (0 to 9 and hexadecimal character)  OR
  3 characters from (0 to 9 and hexadecimal character)  
  (non-capturing group)

  #AB1294     (#AB1294)
  #AB1        (#AB1)
  #124        (#124)
  #00a        (#00a)
  #87a        (#87a)                             
*/

/*
                      ---------------------
                      | C++ regex library |
                      ---------------------
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

    //  4 digits        + 
    //  3 whitespaces   + 
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
    // 4 to 7 lowercase characters from a to f  +
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

    // literally "co"         +
    // 0 or more characters   +
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
    // 4 digits               +
    // 3 lowercase characters +
    // 4 digits

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
    // 4 digits               +   (1st capture group)
    // 3 lowercase characters +   (2nd capture group)
    // 4 digits                   (3rd capture group)

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
    // 4 digits               +   (1st capture group)
    // 3 lowercase characters +   (2nd capture group)
    // 4 digits                   (3rd capture group)

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
    // 4 digits               +   (1st capture group)
    // 3 lowercase characters +   (2nd capture group)
    // 4 digits                   (3rd capture group)

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
    // 4 digits               +   (1st capture group)
    // 3 lowercase characters +   (2nd capture group)
    // 4 digits                   (3rd capture group)

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
  #include <vector>
  #include <string>
  #include <fstream>    // std::ifstream, std::ofstream
  #include <utility>    // std::move
  #include <regex>      // std::regex_search, std::smatch

  using namespace std;
  vector<string> sentence_file_to_vec(const string& file_name)
  {
    ifstream ifs{ file_name };
    if (!ifs) {
      cerr << "file open error\n";
      throw std::runtime_error{ "file open error" };
    }

    vector<string> svec;
    string sline;

    while (getline(ifs, sline))
      svec.push_back(std::move(sline));

    return svec; // Named Return Value Optimization(NRVO)
  }

  int main()
  {
    auto vec = sentence_file_to_vec("sentences.txt");

    std::ofstream ofs{ "output.txt" };
    if (!ofs) {
      std::cerr << "file open error\n";
      throw std::runtime_error{ "file open error" };
    }

    std::regex rgx{"\\d\\.\\d+"};
    // digit                    + 
    // literally '.'            + 
    // 1 or more digit (greedy)

    smatch results;

    for (const auto& sentence : vec) 
    {
      if (regex_search(sentence, results, rgx))
      {
        // continue lesson35
      }
    }
  }
*/
