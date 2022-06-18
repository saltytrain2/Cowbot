#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "MoveGen.h"
#include "Attack.h"
#include "ChessBoard.h"
#include "Move.h"
#include <memory>

TEST_CASE("Move Generation") {
    auto attack = std::make_shared<Attack>();
    attack->initTables();
    auto board = std::make_shared<ChessBoard>(attack.get());
    MoveGen moveGenerator(board.get(), attack.get());
    SUBCASE("Starting Position") {
        board->updateChessBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        auto moves = moveGenerator.generateLegalMoves();
        CHECK(moves.size() == 20);
        // for (auto i : moveGenerator.getMoves()) {
        //     board->makeMove(i);
        //     board->printSquareBoard();
        //     board->undoMove();
        // }
    }

    SUBCASE("Moves Made") {
        board->updateChessBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        Move move("e2e4");
        board->makeMove(move);
        move.updateMove("e7e5");
        board->makeMove(move);
        auto moves = moveGenerator.generateLegalMoves();
        CHECK(moves.size() == 29);
        // for (auto i : moveGenerator.getMoves()) {
        //     board->makeMove(i);
        //     board->printSquareBoard();
        //     board->undoMove();
        // }
    }

    SUBCASE("Secondary Position") {
        board->updateChessBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        auto moves = moveGenerator.generateLegalMoves();
        CHECK(moves.size() == 48);
        for (auto i : moves) {
            board->makeMove(i);
            //board->printSquareBoard();
            board->undoMove();
            CHECK(board->getWhitePawns() == 0x81000E700);
            CHECK(board->getWhiteKnights() == 0x1000040000);
            CHECK(board->getWhiteBishops() == 0x1800);
            CHECK(board->getWhiteRooks() == 0x81);
            CHECK(board->getWhiteQueens() == 0x200000);
            CHECK(board->getWhiteKing() == 0x10);
            CHECK(board->getBlackPawns() == 0x2D500002800000);
            CHECK(board->getBlackKnights() == 0x220000000000);
            CHECK(board->getBlackBishops() == 0x40010000000000);
            CHECK(board->getBlackRooks() == 0x8100000000000000);
            CHECK(board->getBlackQueens() == 0x10000000000000);
            CHECK(board->getBlackKing() == 0x1000000000000000);
        }

        board->updateChessBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");
        moves = moveGenerator.generateLegalMoves();
        CHECK(moves.size() == 14);

        board->updateChessBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
        moves = moveGenerator.generateLegalMoves();
        CHECK(moves.size() == 6);

        Move move(Square::G1, Square::H1, MoveType::Quiet);
        board->makeMove(move);
        moves = moveGenerator.generateLegalMoves();
        CHECK(moves.size() == 46);
        for (auto i : moves) {
            board->makeMove(i);
            board->printSquareBoard();
            board->undoMove();
        }
        board->undoMove();
        moves = moveGenerator.generateLegalMoves();
        CHECK(moves.size() == 6);

        board->updateChessBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
        moves = moveGenerator.generateLegalMoves();
        CHECK(moves.size() == 44);

        board->updateChessBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
        moves = moveGenerator.generateLegalMoves();
        CHECK(moves.size() == 46);
    }
}