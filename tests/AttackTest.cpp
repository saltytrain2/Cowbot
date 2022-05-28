#include "doctest.h"
#include "Attack.h"

TEST_CASE("Attack Bitboards") {
    Attack attackBoards;

    SUBCASE("King Bitboards") {
        CHECK(attackBoards.getKingAttacks(Square::A1) == 0x302);
        CHECK(attackBoards.getKingAttacks(Square::A5) == 0x30203000000);
        CHECK(attackBoards.getKingAttacks(Square::A8) == 0x203000000000000);
        CHECK(attackBoards.getKingAttacks(Square::D5) == 0x1C141C000000);
        CHECK(attackBoards.getKingAttacks(Square::C3) == 0xE0A0E00);
        CHECK(attackBoards.getKingAttacks(Square::H1) == 0xC040);
        CHECK(attackBoards.getKingAttacks(Square::H5) == 0xC040C0000000);
        CHECK(attackBoards.getKingAttacks(Square::H8) == 0x40C0000000000000);
        CHECK(attackBoards.getKingAttacks(Square::E1) == 0x3828);
    }

    SUBCASE("Knight Bitboards") {
        CHECK(attackBoards.getKnightAttacks(Square::A1) == 0x20400);
        CHECK(attackBoards.getKnightAttacks(Square::H7) == 0x2000204000000000);
        CHECK(attackBoards.getKnightAttacks(Square::E1) == 0x284400);
        CHECK(attackBoards.getKnightAttacks(Square::G3) == 0xA0100010A0);
        CHECK(attackBoards.getKnightAttacks(Square::D6) == 0x1422002214000000);
    }

    SUBCASE("Bishop Bitboards") {
        CHECK(attackBoards.getBishopAttacks(Square::A1, 0xFC40000) == 0x40200);
        CHECK(attackBoards.getBishopAttacks(Square::A1, 0) == 0x8040201008040200);
        CHECK(attackBoards.getBishopAttacks(Square::D4, 0x67008001E01) == 0x21400142240); 
        CHECK(attackBoards.getBishopAttacks(Square::D4, 0x2467000001E41) == 0x21400142240);
        CHECK(attackBoards.getBishopAttacks(Square::C4, 0xBBE728005426E391) == 0x20110A000A1020);
        CHECK(attackBoards.getBishopAttacks(Square::B2, 0xBBE728005426E391) == 0x50005);
        CHECK(attackBoards.getBishopAttacks(Square::G4, 0xBBE728005426E391) == 0x40810A000A00000);
        CHECK(attackBoards.getBishopAttacks(Square::F8, 0xBBE728005426E391) == 0x50080000000000);
    }

    SUBCASE("Rook Bitboards") {
        CHECK(attackBoards.getRookAttacks(Square::A1, 0xBBE728005426E391) == 0x11E);
        CHECK(attackBoards.getRookAttacks(Square::H1, 0xBBE728005426E391) == 0x8070);
        CHECK(attackBoards.getRookAttacks(Square::H8, 0xBBE728005426E391) == 0x6080000000000000);
        CHECK(attackBoards.getRookAttacks(Square::A8, 0xBBE728005426E391) == 0x201000000000000);
    }

    SUBCASE("Queen Bitboards") {
        CHECK(attackBoards.getQueenAttacks(Square::B3, 0xBBE728005426E391) == 0x2020207050708);
        CHECK(attackBoards.getQueenAttacks(Square::D8, 0xBBE728005426E391) == 0x161C280000000000);
    }
}