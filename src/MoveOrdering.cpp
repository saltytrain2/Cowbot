#include "MoveOrdering.h"

#include <vector>
#include <algorithm>

#include "Move.h"
#include "types.h"

MoveOrdering::MoveOrdering(ChessBoard *board, FakeEval* fakeEval)
    : mBoard(board), mFakeEval(fakeEval)
{}

void MoveOrdering::orderMoves(moveVectorIter begin, moveVectorIter end) const
{
    auto captureEnd = orderCapturesFirst(begin, end);
    MvvLva(begin, captureEnd);
    orderHistory(captureEnd, end);
}
void MoveOrdering::orderMovesQuiescent(moveVectorIter begin, moveVectorIter end, int16_t standpat, int16_t alpha) const
{
    deltaPruning(begin, end, standpat, alpha);
}

moveVectorIter MoveOrdering::orderCapturesFirst(moveVectorIter begin, moveVectorIter end) const
{
    auto currentIter = begin;
    Color self = Color(to_int(mBoard->getPiece(begin->getStartingSquare())) & 1);
    Bitboard enemyPieces = mBoard->getPieces(!self);
    for (auto i = begin; i != end; ++i) {
        if (Utils::getBitboard(i->getEndingSquare()) & enemyPieces) {
            std::swap(*i, *currentIter);
            ++currentIter;
        }
    }
    return currentIter;
}

void MoveOrdering::deltaPruning(moveVectorIter begin, moveVectorIter end, int16_t standpat, int16_t alpha) const
{
    auto deltaThreshold = [this, standpat, alpha](auto x) {
        PieceSets capturedPiece = mBoard->getPiece(x.getEndingSquare());
        return standpat + mFakeEval->getPieceValue(capturedPiece) + 150 < alpha;
    };
    auto it = std::remove_if(begin, end, deltaThreshold);

    // sort by highest value capture, ties broken by lowest value capturer
    MvvLva(begin, it);
}

void MoveOrdering::addHistory(const Move& move, uint16_t depth)
{
    Square from = move.getStartingSquare(), to = move.getEndingSquare();
    PieceSets movedPiece = mBoard->getPiece(from);
    mHistory[to_int(movedPiece) & 1][to_int(from)][to_int(to)] += depth*depth;
}

uint32_t MoveOrdering::getHistory(const Move& move) const
{
    Square from = move.getStartingSquare(), to = move.getEndingSquare();
    PieceSets movedPiece = mBoard->getPiece(from);
    return mHistory[to_int(movedPiece) & 1][to_int(from)][to_int(to)];
}

void MoveOrdering::MvvLva(moveVectorIter begin, moveVectorIter end) const
{
    auto sortVictims = [this](Move x, Move y) {
        PieceSets xCapturedPiece = mBoard->getPiece(x.getEndingSquare());
        PieceSets yCapturedPiece = mBoard->getPiece(y.getEndingSquare());
        return xCapturedPiece < yCapturedPiece;
    };
    std::sort(begin, end, sortVictims);

    auto sortAggressors = [this](Move x, Move y) {
        PieceSets xPiece = mBoard->getPiece(x.getEndingSquare());
        PieceSets yPiece = mBoard->getPiece(y.getEndingSquare());
        return xPiece > yPiece;
    };
    auto startSortAggressors = begin;
    for (auto i = begin + 1, j = begin; i != end && begin != end; ++i, ++j) {
        PieceSets xCapturedPiece = mBoard->getPiece(i->getEndingSquare());
        PieceSets yCapturedPiece = mBoard->getPiece(j->getEndingSquare());
        if (xCapturedPiece != yCapturedPiece) {
            std::sort(startSortAggressors, i, sortAggressors);
            startSortAggressors = i;
        }
    }
}

void MoveOrdering::orderHistory(moveVectorIter begin, moveVectorIter end) const
{
    auto compare = [this](Move x, Move y) {
        uint32_t xHistory = getHistory(x);
        uint32_t yHistory = getHistory(y);
        return xHistory < yHistory;
    };
    std::sort(begin, end, compare);
}

void MoveOrdering::clearHistory()
{
    for (uint8_t i = 0; i < 2; ++i) {
        for (uint8_t j = 0; j < 64; ++j) {
            for (uint8_t k = 0; k < 64; ++k) {
                mHistory[i][j][k] = 0;
            }
        }
    }
}