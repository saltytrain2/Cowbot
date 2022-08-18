#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>

#include "Attack.h"

using namespace Cowbot;

TEST_CASE("Attack Bitboards") {
    auto attackBoards = std::make_shared<Attack>();

    SUBCASE("Pawn Bitboards") {
        CHECK(attackBoards->getPawnAttacks(Square::A2, Color::White) == 0x20000);
        CHECK(attackBoards->getPawnAttacks(Square::A2, Color::Black) == 0x2);
        CHECK(attackBoards->getPawnAttacks(Square::D4, Color::White) == 0x1400000000);
        CHECK(attackBoards->getPawnAttacks(Square::D4, Color::Black) == 0x140000);
        CHECK(attackBoards->getPawnAttacks(Square::H5, Color::White) == 0x400000000000);
        CHECK(attackBoards->getPawnAttacks(Square::H5, Color::Black) == 0x40000000);
        CHECK(attackBoards->getPawnAttacks(Square::F7, Color::White) == 0x5000000000000000);
        CHECK(attackBoards->getPawnAttacks(Square::F7, Color::Black) == 0x500000000000);
        CHECK(attackBoards->getPawnAttacks(Square::E5, Color::White) == 0x280000000000);
    }

    SUBCASE("King Bitboards") {
        CHECK(attackBoards->getKingAttacks(Square::A1) == 0x302);
        CHECK(attackBoards->getKingAttacks(Square::A5) == 0x30203000000);
        CHECK(attackBoards->getKingAttacks(Square::A8) == 0x203000000000000);
        CHECK(attackBoards->getKingAttacks(Square::D5) == 0x1C141C000000);
        CHECK(attackBoards->getKingAttacks(Square::C3) == 0xE0A0E00);
        CHECK(attackBoards->getKingAttacks(Square::H1) == 0xC040);
        CHECK(attackBoards->getKingAttacks(Square::H5) == 0xC040C0000000);
        CHECK(attackBoards->getKingAttacks(Square::H8) == 0x40C0000000000000);
        CHECK(attackBoards->getKingAttacks(Square::E1) == 0x3828);
    }

    SUBCASE("Knight Bitboards") {
        CHECK(attackBoards->getKnightAttacks(Square::A1) == 0x20400);
        CHECK(attackBoards->getKnightAttacks(Square::H7) == 0x2000204000000000);
        CHECK(attackBoards->getKnightAttacks(Square::E1) == 0x284400);
        CHECK(attackBoards->getKnightAttacks(Square::G3) == 0xA0100010A0);
        CHECK(attackBoards->getKnightAttacks(Square::D6) == 0x1422002214000000);
    }

    SUBCASE("Bishop Bitboards") {
        CHECK(attackBoards->getBishopAttacks(Square::A1, 0xFC40000) == 0x40200);
        CHECK(attackBoards->getBishopAttacks(Square::A1, 0) == 0x8040201008040200);
        CHECK(attackBoards->getBishopAttacks(Square::D4, 0x67008001E01) == 0x21400142240); 
        CHECK(attackBoards->getBishopAttacks(Square::D4, 0x2467000001E41) == 0x21400142240);
        CHECK(attackBoards->getBishopAttacks(Square::C4, 0xBBE728005426E391) == 0x20110A000A1020);
        CHECK(attackBoards->getBishopAttacks(Square::B2, 0xBBE728005426E391) == 0x50005);
        CHECK(attackBoards->getBishopAttacks(Square::G4, 0xBBE728005426E391) == 0x40810A000A00000);
        CHECK(attackBoards->getBishopAttacks(Square::F8, 0xBBE728005426E391) == 0x50080000000000);
        CHECK((attackBoards->getBishopAttacks(Square::A6, 0x5962340000001100) & 0x20) == 0);
    }

    SUBCASE("Rook Bitboards") {
        CHECK(attackBoards->getRookAttacks(Square::A1, 0xBBE728005426E391) == 0x11E);
        CHECK(attackBoards->getRookAttacks(Square::H1, 0xBBE728005426E391) == 0x8070);
        CHECK(attackBoards->getRookAttacks(Square::H8, 0xBBE728005426E391) == 0x6080000000000000);
        CHECK(attackBoards->getRookAttacks(Square::A8, 0xBBE728005426E391) == 0x201000000000000);
    }

    SUBCASE("Queen Bitboards") {
        CHECK(attackBoards->getQueenAttacks(Square::B3, 0xBBE728005426E391) == 0x2020207050708);
        CHECK(attackBoards->getQueenAttacks(Square::D8, 0xBBE728005426E391) == 0x161C280000000000);
    }

    SUBCASE("In Between Bitboards") {
        CHECK(attackBoards->inBetween(Square::A1, Square::A3) == 0x100);
        CHECK(attackBoards->inBetween(Square::B4, Square::E1) == 0x40800);
        CHECK(attackBoards->inBetween(Square::C3, Square::F7) == 0);
        CHECK(attackBoards->inBetween(Square::D1, Square::D8) == 0x8080808080800);
        CHECK(attackBoards->inBetween(Square::D6, Square::H6) == 0x700000000000);
        CHECK(attackBoards->inBetween(Square::A7, Square::G1) == 0x20408102000);
        CHECK(attackBoards->inBetween(Square::H3, Square::C8) == 0x8102040000000);
        CHECK(attackBoards->inBetween(Square::E1, Square::H4) == 0x402000);
    }

    SUBCASE("Lined Bitboards") {
        CHECK(attackBoards->inLine(Square::A1, Square::A3) == 0x101010101010101);
        CHECK(attackBoards->inLine(Square::B4, Square::E1) == 0x102040810);
        CHECK(attackBoards->inLine(Square::C3, Square::F7) == 0);
        CHECK(attackBoards->inLine(Square::D1, Square::D8) == 0x808080808080808);
        CHECK(attackBoards->inLine(Square::D6, Square::H6) == 0xFF0000000000);
        CHECK(attackBoards->inLine(Square::A7, Square::G1) == 0x1020408102040);
        CHECK(attackBoards->inLine(Square::H3, Square::C8) == 0x408102040800000);
    }
}
