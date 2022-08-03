#include "MoveOrdering.h"

#include <vector>

#include "Move.h"
#include "types.h"

MoveOrdering::MoveOrdering(ChessBoard *board, FakeEval* fakeEval)
    : mBoard(board), mFakeEval(fakeEval)
{}

std::vector<Move> MoveOrdering::deltaPruning(const std::vector<Move>& captures, int16_t standpat, int16_t alpha)
{
    std::vector<Move> pruned;

    for (auto capture: captures) {
        PieceSets capturedPiece = mBoard->getPiece(capture.getEndingSquare());

        if (standpat + mFakeEval->getPieceValue(capturedPiece) + 150 > alpha) {
            pruned.push_back(capture);
        }
    }
    return pruned;
}

std::vector<Move> MoveOrdering::staticExchangePruning(const std::vector<Move>& captures)
{
    std::vector<Move> pruned;

    for (auto capture: captures) {
        Square endingSquare = capture.getEndingSquare();

    }
    return pruned;
}

