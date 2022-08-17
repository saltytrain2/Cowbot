#pragma once

#include <cstdint>
#include <string>

#include "ChessBoard.h"
#include "MoveGen.h"
#include "Move.h"
#include "FakeEval.h"
#include "TT.h"
#include "MoveOrdering.h"

namespace Cowbot {
class Search
{
public:
    Search(ChessBoard* board, MoveGen* moveGen, FakeEval* fakeEval, TT* tt, MoveOrdering* moveOrdering);
    ~Search() = default;
    std::pair<Move, int16_t> search(uint8_t depth);

private:
    ChessBoard* mBoard;
    MoveGen* mMoveGen;
    FakeEval* mFakeEval;
    TT* mTT;
    MoveOrdering* mMoveOrdering;

    int16_t alphaBeta(int16_t alpha, int16_t beta, uint8_t depthleft, uint8_t pliesFromRoot);
    int16_t qsearch(int16_t alpha, int16_t beta);
};
} // namespace Cowbot