#include <iostream>

#include "catch.hpp"
#include "../pattern.h"

using namespace std;
using namespace pMatch;

TEST_CASE("strClass", "[strClass]") {

  std::string expected;

  SECTION("Testing constructor") {
    string s("[a]bc[--]");

    strClass sclass1, sclass2;;
    strClass::iterator it;

    expected = "abc[--]";
    REQUIRE(strClass("[a]bc[--]").str() == expected);

    expected = "abc[--]";
    REQUIRE(strClass(s).str() == expected);

    expected = "abc[--]";
    REQUIRE_NOTHROW(sclass1 = strClass("[a]bc[--]"));
    REQUIRE(sclass1.str() == expected);

    expected = "abc[--]";
    REQUIRE_NOTHROW(sclass2 = strClass("[a]bc[--]"));
    REQUIRE(sclass2.str() == expected);
  }

  SECTION("Testing simple matches") {
    string s = "testando com afeto";

    size_t pos;
    bool b;

    REQUIRE(strClass("testa").match(s, pos=0) == true);

    REQUIRE(strClass("esta").match(s, pos=0) == false);

    REQUIRE(strClass("esta").match(s, pos=1) == true);

    REQUIRE(strClass("[ t]a").match(s, pos=3) == true);

    REQUIRE(strClass("[ t]a").match(s, pos=12) == true);

    REQUIRE(strClass("[^t]a").match(s, pos=12) == true);
  }

  SECTION ("Testing find() function") {
    string s = "testando com carinho!";

    size_t pos;
    size_t fpos;
    tWord str;

    REQUIRE_NOTHROW(str = strClass("t").find(s, pos=0));
    REQUIRE(str == "t");
    REQUIRE(str.start() == 0);

    expected = "ta";
    REQUIRE_NOTHROW(str = strClass("ta").find(s, pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == 3);

    expected = "nho!";
    REQUIRE_NOTHROW(str = strClass("nho!").find(s, pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == 17);

    expected = "com"; fpos = 9;
    REQUIRE_NOTHROW(str = strClass("[a-c][m-p][m-p]").find(s, pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = ""; fpos = 0;
    REQUIRE_NOTHROW(str = strClass("").find(s, pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = "com]"; fpos = 4;
    REQUIRE_NOTHROW(str = strClass("com]").find("0c2[com]", pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = "[com]"; fpos = 3;
    REQUIRE_NOTHROW(str = strClass("\\[com]").find("0c2[com]", pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = "2[com]"; fpos = 2;
    REQUIRE_NOTHROW(str = strClass("2\\[com]").find("0c2[com]", pos=0));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = "t"; fpos = 3;
    REQUIRE_NOTHROW(str = strClass("t").find(s, pos=1));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);

    expected = ""; fpos = 21;
    REQUIRE_NOTHROW(str = strClass("com").find(s, pos=14));
    REQUIRE(str == expected);
    REQUIRE(str.start() == fpos);
  }

  SECTION("Testing the getClass() method") {
    strClass sc;
    size_t start, pos, fpos;

    expected = "feijoada"; fpos = 8;
    REQUIRE_NOTHROW(sc = strClass::getClass("feijoada", pos=0));
    REQUIRE(sc.str() == expected);
    REQUIRE(pos == fpos);

    expected = "ijoada"; fpos = 8;
    REQUIRE_NOTHROW(sc = strClass::getClass("feijoada", pos=2));
    REQUIRE(sc.str() == expected);
    REQUIRE(pos == fpos);

    expected = "fei"; fpos = 3;
    REQUIRE_NOTHROW(sc = strClass::getClass("fei(oada", pos=0));
    REQUIRE(sc.str() == expected);
    REQUIRE(pos == fpos);

    expected = "fei(ada)"; fpos = 9;
    REQUIRE_NOTHROW(sc = strClass::getClass("fei\\(ada)", pos=0));
    REQUIRE(sc.str() == expected);
    REQUIRE(pos == fpos);

    expected = ""; fpos = 8;
    REQUIRE_NOTHROW(sc = strClass::getClass("feijoada", pos=8));
    REQUIRE(sc.str() == expected);
    REQUIRE(pos == fpos);
  }

  SECTION("Testing error handling") {
    strClass sc;
    size_t start, pos, fpos;

    REQUIRE_THROWS(strClass::getClass("feijoada", pos=-1));

    REQUIRE_THROWS(strClass::getClass("feijoada", pos=9));

    REQUIRE_NOTHROW(strClass::getClass("feijoada\\", pos=0));
  }
}
