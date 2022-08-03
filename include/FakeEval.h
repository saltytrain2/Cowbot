#pragma once

#include <cstdint>


#include "ChessBoard.h"
#include "types.h"

/*
    Code Taken and adapted from the ChessProgramming Wiki
    This is not meant to be the actual evaluation function that the engine will use
    This serves as a placeholder until the NNUE is complete
*/

class FakeEval 
{
public:
    FakeEval();
    ~FakeEval() = default;
    // always return eval wrt white
    int16_t eval(const ChessBoard& board);

    int16_t getPieceValue(PieceSets piece);

private:
    int16_t mMGTable[12][64];
    int16_t mEGTable[12][64];

    void init_tables();
};