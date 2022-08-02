#pragma once

#include <cstdint>
#include <string>

#include "ChessBoard.h"
#include "MoveGen.h"
#include "Move.h"
#include "FakeEval.h"
#include "TT.h"

class Search
{
public:
    Search(ChessBoard* board, MoveGen* moveGen, FakeEval* fakeEval, TT* tt);
    ~Search() = default;
    std::pair<std::string, int16_t> search(uint8_t depth);

private:
    ChessBoard* mBoard;
    MoveGen* mMoveGen;
    FakeEval* mFakeEval;
    TT* mTT;

    int16_t alphaBetaWhite(int16_t alpha, int16_t beta, uint8_t depthleft, uint8_t depth);
    int16_t alphaBetaBlack(int16_t alpha, int16_t beta, uint8_t depthleft, uint8_t depth);
    int16_t alphaBeta(int16_t alpha, int16_t beta, uint8_t depthleft, uint8_t depth);
    int16_t qsearch(int16_t alpha, int16_t beta);
};