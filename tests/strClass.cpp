#include <iostream>

#include "catch.hpp"
#include "../pattern.h"

using namespace std;
using namespace pattern;

TEST_CASE("Pattern", "[Pattern]") {

  std::string expected;

  SECTION("Testing constructor") {
    string s("[a]bc[--]");

    Pattern sclass1, sclass2;;
    Pattern::iterator it;

    expected = "abc[--]";
    REQUIRE(Pattern("[a]bc[--]").str() == expected);

    expected = "abc[--]";
    REQUIRE(Pattern(s).str() == expected);

    expected = "abc[--]";
    REQUIRE_NOTHROW(sclass1 = Pattern("[a]bc[--]"));
    REQUIRE(sclass1.str() == expected);

    expected = "abc[--]";
    REQUIRE_NOTHROW(sclass2 = Pattern("[a]bc[--]"));
    REQUIRE(sclass2.str() == expected);
  }

  SECTION("Testing simple matches") {
    string s = "testando com afeto";

    size_t pos;
    bool b;

    REQUIRE(Pattern("testa").match(s, pos=0) == true);

    REQUIRE(Pattern("esta").match(s, pos=0) == false);

    REQUIRE(Pattern("esta").match(s, pos=1) == true);

    REQUIRE(Pattern("[ t]a").match(s, pos=3) == true);

    REQUIRE(Pattern("[ t]a").match(s, pos=12) == true);

    REQUIRE(Pattern("[^t]a").match(s, pos=12) == true);
  }

  SECTION ("Testing find() function") {
    string s = "testando com carinho!";

    size_t pos;
    size_t fpos;
    tWord str;

    REQUIRE_NOTHROW(str = Pattern("t").find(s, pos=0));
    REQUIRE(str == "t");
    REQUIRE(str.start() == 0);

    expected = "ta";
    REQUIRE_NOTHROW(str = Pattern("ta").find(s, pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == 3);

    expected = "nho!";
    REQUIRE_NOTHROW(str = Pattern("nho!").find(s, pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == 17);

    expected = "com"; fpos = 9;
    REQUIRE_NOTHROW(str = Pattern("[a-c][m-p][m-p]").find(s, pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = ""; fpos = 0;
    REQUIRE_NOTHROW(str = Pattern("").find(s, pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = "com]"; fpos = 4;
    REQUIRE_NOTHROW(str = Pattern("com]").find("0c2[com]", pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = "[com]"; fpos = 3;
    REQUIRE_NOTHROW(str = Pattern("\\[com]").find("0c2[com]", pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = "2[com]"; fpos = 2;
    REQUIRE_NOTHROW(str = Pattern("2\\[com]").find("0c2[com]", pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = "t"; fpos = 3;
    REQUIRE_NOTHROW(str = Pattern("t").find(s, pos=1));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = ""; fpos = 21;
    REQUIRE_NOTHROW(str = Pattern("com").find(s, pos=14));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);
  }

  SECTION("Testing the getClass() method") {
    Pattern sc;
    size_t start, pos, fpos;

    expected = "feijoada"; fpos = 8;
    REQUIRE_NOTHROW(sc = Pattern::getClass("feijoada", pos=0));
    REQUIRE(sc.str() == expected);
    REQUIRE(pos == fpos);

    expected = "ijoada"; fpos = 8;
    REQUIRE_NOTHROW(sc = Pattern::getClass("feijoada", pos=2));
    REQUIRE(sc.str() == expected);
    REQUIRE(pos == fpos);

    expected = "fei"; fpos = 3;
    REQUIRE_NOTHROW(sc = Pattern::getClass("fei(oada", pos=0));
    REQUIRE(sc.str() == expected);
    REQUIRE(pos == fpos);

    expected = "fei(ada)"; fpos = 9;
    REQUIRE_NOTHROW(sc = Pattern::getClass("fei\\(ada)", pos=0));
    REQUIRE(sc.str() == expected);
    REQUIRE(pos == fpos);

    expected = ""; fpos = 8;
    REQUIRE_NOTHROW(sc = Pattern::getClass("feijoada", pos=8));
    REQUIRE(sc.str() == expected);
    REQUIRE(pos == fpos);
  }

  SECTION("Testing error handling") {
    Pattern sc;
    size_t start, pos, fpos;

    REQUIRE_THROWS(Pattern::getClass("feijoada", pos=-1));

    REQUIRE_THROWS(Pattern::getClass("feijoada", pos=9));

    REQUIRE_NOTHROW(Pattern::getClass("feijoada\\", pos=0));
  }
}
