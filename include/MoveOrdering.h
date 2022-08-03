#pragma once

#include <vector>

#include "types.h"
#include "Move.h"
#include "Utils.h"
#include "ChessBoard.h"
#include "FakeEval.h"

class MoveOrdering
{
public:
    MoveOrdering(ChessBoard* board, FakeEval* fakeEval);
    std::vector<Move> deltaPruning(const std::vector<Move>& captures, int16_t standpat, int16_t alpha);
    std::vector<Move> staticExchangePruning(const std::vector<Move>& captures);
private:
    ChessBoard* mBoard;
    FakeEval* mFakeEval;
};