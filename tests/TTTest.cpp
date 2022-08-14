#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>

#include "TT.h"
#include "Move.h"
#include "Utils.h"

using namespace Cowbot;

TEST_CASE("Testing TT Entries") {
    std::cout << sizeof(TTEntry) << std::endl;
    std::cout << (uint16_t)to_int(Utils::flipSquare(Square::A1)) << " " << (uint16_t)to_int(Utils::flipSquare(Square::D3)) << std::endl;
}