#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>
#include "ChessBoard.h"
#include "Attack.h"

using namespace Cowbot;

TEST_CASE("standard game") {
    auto attack = std::make_shared<Attack>();
    attack->initTables();
    ChessBoard board(attack.get());
    board.updateChessBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(board.getTurn() == Color::White);
    CHECK(board.getWhiteCastleRights(Castling::Kingside) == true);
    CHECK(board.getWhiteCastleRights(Castling::Queenside) == true);
    CHECK(board.getBlackCastleRights(Castling::Kingside) == true);
    CHECK(board.getBlackCastleRights(Castling::Queenside) == true);
    CHECK(board.getWhitePawns() == 0xFF00);
    CHECK(board.getWhiteKnights() == 0x42);
    CHECK(board.getWhiteBishops() == 0x24);
    CHECK(board.getWhiteRooks() == 0x81);
    CHECK(board.getWhiteQueens() == 0x8);
    CHECK(board.getWhiteKing() == 0x10);
    CHECK(board.getBlackPawns() == 0xFF000000000000);
    CHECK(board.getBlackKnights() == 0x4200000000000000);
    CHECK(board.getBlackBishops() == 0x2400000000000000);
    CHECK(board.getBlackRooks() == 0x8100000000000000);
    CHECK(board.getBlackQueens() == 0x800000000000000);
    CHECK(board.getBlackKing() == 0x1000000000000000);
    CHECK(board.getWhitePieces() == 0xFFFF);
    CHECK(board.getBlackPieces() == 0xFFFF000000000000);
    CHECK(board.getAllPieces() == 0xFFFF00000000FFFF);
    CHECK(board.getEmptySquares() == 0xFFFFFFFF0000);
    CHECK(board.getPinnedPieces() == 0);
    board.printSquareBoard();

    SUBCASE("making valid moves") {
        Move move("e2e4");
        board.makeMove(move);
        board.printSquareBoard();
        CHECK(board.getWhitePawns() == 0x1000EF00);
        Move move2("e7e5");
        move.updateMove("e7e5");
        board.makeMove(move);
        CHECK(board.getBlackPawns() == 0xEF001000000000);
        move.updateMove("g1f3");
        board.makeMove(move);
        move.updateMove("b8c6");
        board.makeMove(move);
        move.updateMove("f3e5");
        board.makeMove(move);
        move.updateMove("c6e5");
        board.makeMove(move);
        CHECK(board.getWhiteKnights() == 0x2);
        CHECK(board.getBlackPawns() == 0xEF000000000000);
        board.undoMove();
        board.printSquareBoard();
        board.undoMove();
        board.printSquareBoard();
    }
}
