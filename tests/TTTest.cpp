#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>

#include "TT.h"

TEST_CASE("Testing TT Entries") {
    std::cout << sizeof(TTEntry) << std::endl;
}