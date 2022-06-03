#include "doctest.h"

#include "MoveGen.h"
#include "Attack.h"
#include "ChessBoard.h"
#include "Move.h"
#include <memory>

TEST_CASE("Move Generation") {
    auto attack = std::make_shared<Attack>();
    auto board = std::make_shared<ChessBoard>(attack);
    MoveGen moveGenerator(board, attack);
    SUBCASE("Starting Position") {
        board->updateChessBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        moveGenerator.generateLegalMoves();
        CHECK(moveGenerator.getMoves().size() == 20);
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
        moveGenerator.generateLegalMoves();
        CHECK(moveGenerator.getMoves().size() == 29);
        // for (auto i : moveGenerator.getMoves()) {
        //     board->makeMove(i);
        //     board->printSquareBoard();
        //     board->undoMove();
        // }
    }

    SUBCASE("Secondary Position") {
        board->updateChessBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        moveGenerator.generateLegalMoves();
        CHECK(moveGenerator.getMoves().size() == 48);
        
        board->updateChessBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");
        moveGenerator.generateLegalMoves();
        CHECK(moveGenerator.getMoves().size() == 14);

        board->updateChessBoard("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
        moveGenerator.generateLegalMoves();
        CHECK(moveGenerator.getMoves().size() == 6);

        board->updateChessBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
        moveGenerator.generateLegalMoves();
        CHECK(moveGenerator.getMoves().size() == 44);

        board->updateChessBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
        moveGenerator.generateLegalMoves();
        CHECK(moveGenerator.getMoves().size() == 46);
    }
}