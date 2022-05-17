#include "doctest.h"
#include "MoveGen.h"
#include "Attack.h"
#include "ChessBoard.h"
#include "Move.h"
#include <memory>

TEST_CASE("Move Generation") {
    auto attack = std::make_shared<Attack>();
    ChessBoard board;
    SUBCASE("Starting Position") {
        MoveGen moveGenerator(board, attack.get());
        moveGenerator.generatePseudoLegalMoves();
        CHECK(moveGenerator.getMoves().size() == 20);
    }

    SUBCASE("Moves Made") {
        Move move("e2e4");
        board.makeMove(move);
        move.updateMove("e7e5");
        board.makeMove(move);
        MoveGen moveGenerator(board, attack.get());
        moveGenerator.generatePseudoLegalMoves();
        CHECK(moveGenerator.getMoves().size() == 29);
    }
}