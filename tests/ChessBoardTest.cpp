#include "doctest.h"

#include "ChessBoard.h"

TEST_CASE("validity of constructors") {
    ChessBoard board;

    SUBCASE("start position should be valid") {
        CHECK(board.getTurn() == white);
        CHECK(board.getWhiteCastleRights(kingside) == true);
        CHECK(board.getWhiteCastleRights(queenside) == true);
        CHECK(board.getBlackCastleRights(kingside) == true);
        CHECK(board.getBlackCastleRights(queenside) == true);
        CHECK(board.getWhitePawns() == uint64_t(0xFF00));
        CHECK(board.getWhiteKnights() == uint64_t(0x42));
        CHECK(board.getWhiteBishops() == uint64_t(0x24));
        CHECK(board.getWhiteRooks() == uint64_t(0x81));
        CHECK(board.getWhiteQueens() == uint64_t(0x8));
        CHECK(board.getWhiteKing() == uint64_t(0x10));
        CHECK(board.getBlackPawns() == uint64_t(0xFF000000000000));
        CHECK(board.getBlackKnights() == uint64_t(0x4200000000000000));
        CHECK(board.getBlackBishops() == uint64_t(0x2400000000000000));
        CHECK(board.getBlackRooks() == uint64_t(0x8100000000000000));
        CHECK(board.getBlackQueens() == uint64_t(0x800000000000000));
        CHECK(board.getBlackKing() == uint64_t(0x1000000000000000));
    }
}