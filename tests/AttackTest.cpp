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
        CHECK(attackBoards.getKingAttacks(e1) == 0x3828);
    }

    SUBCASE("Knight Bitboards") {
        CHECK(attackBoards.getKnightAttacks(a1) == 0x20400);
        CHECK(attackBoards.getKnightAttacks(h7) == 0x2000204000000000);
        CHECK(attackBoards.getKnightAttacks(e1) == 0x284400);
        CHECK(attackBoards.getKnightAttacks(g3) == 0xA0100010A0);
        CHECK(attackBoards.getKnightAttacks(d6) == 0x1422002214000000);
    }

    SUBCASE("Bishop Bitboards") {
        Bitboard blockers = 0xFC40000;
        CHECK(attackBoards.getBishopAttacks(a1, blockers) == 0x40200);
        CHECK(attackBoards.getBishopMasks(a1) == 0x40201008040200);
    }
}