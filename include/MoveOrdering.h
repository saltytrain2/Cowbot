#pragma once

#include <vector>

#include "types.h"
#include "Move.h"
#include "Utils.h"
#include "ChessBoard.h"
#include "FakeEval.h"


typedef std::vector<Cowbot::Move>::iterator moveVectorIter;

namespace Cowbot {
class MoveOrdering
{
public:
    MoveOrdering(ChessBoard* board, FakeEval* fakeEval);
    void orderMoves(moveVectorIter begin, moveVectorIter end) const;
    void orderMovesQuiescent(moveVectorIter begin, moveVectorIter end) const;
    void addHistory(Move move, uint16_t depth);
    void clearHistory();
    uint32_t getHistory(Move move) const;

private:
    ChessBoard* mBoard;
    FakeEval* mFakeEval;
    // index as piece to
    uint32_t mHistory[12][64];

    moveVectorIter orderCapturesFirst(moveVectorIter begin, moveVectorIter end) const;
    void deltaPruning(moveVectorIter begin, moveVectorIter end, int16_t standpat, int16_t alpha) const;
    void MvvLva(moveVectorIter begin, moveVectorIter end) const;
    void orderHistory(moveVectorIter begin, moveVectorIter end) const;
};
} // namespace Cowbot