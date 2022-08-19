#include "MoveOrdering.h"

#include <vector>
#include <algorithm>

#include "Move.h"
#include "types.h"

using namespace Cowbot;

MoveOrdering::MoveOrdering(ChessBoard *board, FakeEval* fakeEval)
    : mBoard(board), mFakeEval(fakeEval), mHistory{}
{}

void MoveOrdering::orderMoves(moveVectorIter begin, moveVectorIter end) const
{
    auto captureEnd = orderCapturesFirst(begin, end);
    MvvLva(begin, captureEnd);
    orderHistory(captureEnd, end);
}
void MoveOrdering::orderMovesQuiescent(moveVectorIter begin, moveVectorIter end) const
{
    MvvLva(begin, end);
}

moveVectorIter MoveOrdering::orderCapturesFirst(moveVectorIter begin, moveVectorIter end) const
{
    auto currentIter = begin;
    Color self = Color(to_int(mBoard->getPiece(begin->getStartingSquare())) & 1);
    Bitboard enemyPieces = mBoard->getPieces(!self);
    for (auto i = begin; i != end; ++i) {
        if (Utils::getBitboard(i->getEndingSquare()) & enemyPieces || i->getMoveType() == MoveType::Enpassant) {
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
    MvvLva(it, end);
}

void MoveOrdering::addHistory(Move move, uint16_t depth)
{
    Square to = move.getEndingSquare();
    PieceSets movedPiece = mBoard->getPiece(move.getStartingSquare());
    mHistory[to_int(movedPiece)][to_int(to)] += depth*depth;
}

uint32_t MoveOrdering::getHistory(Move move) const
{
    Square from = move.getStartingSquare(), to = move.getEndingSquare();
    PieceSets movedPiece = mBoard->getPiece(from);
    return mHistory[to_int(movedPiece)][to_int(to)];
}

void MoveOrdering::MvvLva(moveVectorIter begin, moveVectorIter end) const
{
    auto sortVictims = [this](Move x, Move y) {
        PieceSets xCapturedPiece = x.getMoveType() == MoveType::Enpassant ? PieceSets::WhitePawns : mBoard->getPiece(x.getEndingSquare());
        PieceSets yCapturedPiece = y.getMoveType() == MoveType::Enpassant ? PieceSets::WhitePawns : mBoard->getPiece(y.getEndingSquare());
        PieceSets xMovedPiece = mBoard->getPiece(x.getStartingSquare());
        PieceSets yMovedPiece = mBoard->getPiece(x.getStartingSquare());
        int16_t xEval = mFakeEval->getPieceValue(xCapturedPiece) - mFakeEval->getPieceValue(xMovedPiece);
        int16_t yEval = mFakeEval->getPieceValue(yCapturedPiece) - mFakeEval->getPieceValue(yMovedPiece);
        return xEval > yEval;
    };
    std::sort(begin, end, sortVictims);
}

void MoveOrdering::orderHistory(moveVectorIter begin, moveVectorIter end) const
{
    auto compare = [this](Move x, Move y) {
        uint32_t xHistory = getHistory(x);
        uint32_t yHistory = getHistory(y);
        return xHistory > yHistory;
    };
    std::sort(begin, end, compare);
}

void MoveOrdering::clearHistory()
{
    for (uint8_t i = 0; i < 12; ++i) {
        for (uint8_t j = 0; j < 64; ++j) {
            mHistory[i][j] = 0;
        }
    }
}