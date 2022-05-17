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
        CHECK(attackBoards.getBishopAttacks(a1, 0xFC40000) == 0x40200);
        CHECK(attackBoards.getBishopAttacks(a1, 0) == 0x8040201008040200);
        CHECK(attackBoards.getBishopAttacks(d4, 0x67008001E01) == 0x21400142240); 
        CHECK(attackBoards.getBishopAttacks(d4, 0x2467000001E41) == 0x21400142240);
        CHECK(attackBoards.getBishopAttacks(c4, 0xBBE728005426E391) == 0x20110A000A1020);
        CHECK(attackBoards.getBishopAttacks(b2, 0xBBE728005426E391) == 0x50005);
        CHECK(attackBoards.getBishopAttacks(g4, 0xBBE728005426E391) == 0x40810A000A00000);
        CHECK(attackBoards.getBishopAttacks(f8, 0xBBE728005426E391) == 0x50080000000000);
    }

    SUBCASE("Rook Bitboards") {
        CHECK(attackBoards.getRookAttacks(a1, 0xBBE728005426E391) == 0x11E);
        CHECK(attackBoards.getRookAttacks(h1, 0xBBE728005426E391) == 0x8070);
        CHECK(attackBoards.getRookAttacks(h8, 0xBBE728005426E391) == 0x6080000000000000);
        CHECK(attackBoards.getRookAttacks(a8, 0xBBE728005426E391) == 0x201000000000000);
    }

    SUBCASE("Queen Bitboards") {
        CHECK(attackBoards.getQueenAttacks(b3, 0xBBE728005426E391) == 0x2020207050708);
        CHECK(attackBoards.getQueenAttacks(d8, 0xBBE728005426E391) == 0x161C280000000000);
    }
}