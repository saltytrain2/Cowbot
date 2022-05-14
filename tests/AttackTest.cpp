#include "doctest.h"
#include "Attack.h"

TEST_CASE("Attack Bitboards") {
    Attack attackBoards;

    SUBCASE("King Bitboards") {
        CHECK(attackBoards.getKingAttacks(a1) == 0x302);
        CHECK(attackBoards.getKingAttacks(a5) == 0x30203000000);
        CHECK(attackBoards.getKingAttacks(a8) == 0x203000000000000);
        CHECK(attackBoards.getKingAttacks(d5) == 0x1C141C000000);
        CHECK(attackBoards.getKingAttacks(c3) == 0xE0A0E00);
        CHECK(attackBoards.getKingAttacks(h1) == 0xC040);
        CHECK(attackBoards.getKingAttacks(h5) == 0xC040C0000000);
        CHECK(attackBoards.getKingAttacks(h8) == 0x40C0000000000000);
    }
}