#include "Search.h"

#include <map>
#include <algorithm>

#include "Move.h"
#include "ChessBoard.h"
#include "MoveOrdering.h"

using namespace Cowbot;

Search::Search(ChessBoard* board, MoveGen* moveGen, FakeEval* fakeEval, TT* tt, MoveOrdering* moveOrdering)
    : mBoard(board), mMoveGen(moveGen), mFakeEval(fakeEval), mTT(tt), mMoveOrdering(moveOrdering)
{}

std::pair<Move, int16_t> Search::search(uint8_t depth)
{
    auto rootMoves = mMoveGen->generateLegalMoves(mBoard->getTurn());
    std::pair<Move, int16_t> bestMove;

    for (int i = 1; i <= depth; ++i) {
        for (auto move: rootMoves) {
            mBoard->makeMove(move);
            // int16_t score = toMove == Color::White ? alphaBetaBlack(INT16_MIN, INT16_MAX, depth-1, 1) : alphaBetaWhite(INT16_MIN, INT16_MAX, depth-1, 1);
            int16_t score = -alphaBeta(INT16_MIN + 1, INT16_MAX, i - 1, 1);
            if (score > bestMove.second) {
                bestMove.first = move;
                bestMove.second = score;
            }
            mBoard->undoMove();
        }
        for (auto& move: rootMoves) {
            if (move == bestMove.first) {
                std::swap(rootMoves[0], move);
            }
        }
        mMoveOrdering->orderMoves(rootMoves.begin() + 1, rootMoves.end());
    }
    mMoveOrdering->clearHistory();
    return bestMove;
}

int16_t Search::alphaBeta(int16_t alpha, int16_t beta, uint8_t depthleft, uint8_t depth)
{
    // attempt to search for this position in the transposition table
    int16_t origAlpha = alpha;
    ZobristHash posHash = mBoard->getHash();
    TTEntry entry = mTT->getEntry(posHash);
    if (mTT->keyExists(posHash) && entry.depth >= depthleft) {
        if (entry.node == NodeType::EXACT) {
            return entry.score;
        } else if (entry.node == NodeType::UPPERBOUND) {
            beta = std::min(beta, entry.score);
        } else {
            alpha = std::max(alpha, entry.score);
        }

        if (alpha >= beta) {
            return entry.score;
        }
    }

    auto legalMoves = mMoveGen->generateLegalMoves(mBoard->getTurn());
    if (legalMoves.empty()) {
        return mBoard->isKingUnderAttack() ? INT16_MIN + depth : 0;
    }

    if (depthleft == 0) {
        return qsearch(alpha, beta, 4);
    }

    Move bestMove;
    mMoveOrdering->orderMoves(legalMoves.begin(), legalMoves.end());
    for (auto move: legalMoves) {
        mBoard->makeMove(move);
        int16_t score = -alphaBeta(-beta, -alpha, depthleft - 1, depth + 1);
        mBoard->undoMove();
        if (score >= beta) {
            mTT->setEntry(posHash, TTEntry(posHash, move, score, depthleft, NodeType::LOWERBOUND));
            if (!mBoard->isCapture(move)) {
                mMoveOrdering->addHistory(move, depth);
            }
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }

    if (!bestMove) {
        bestMove = legalMoves[0];
    }
    NodeType node = alpha <= origAlpha ? NodeType::UPPERBOUND : NodeType::EXACT;
    mTT->setEntry(posHash, TTEntry(posHash, bestMove, alpha, depthleft, node));
    return alpha;
}

int16_t Search::qsearch(int16_t alpha, int16_t beta, int16_t depthleft)
{
    int16_t currentEval = mFakeEval->eval(*mBoard);
    if (currentEval >= beta) {
        return beta;
    }

    // int16_t maxMaterialSwing = 1025;
    // if (mBoard->containsPromotingPawns()) {
    //     maxMaterialSwing += 1025 - 82;
    // }

    // // hopeless situation, simply return alpha
    // if (currentEval < alpha - maxMaterialSwing) {
    //     return alpha;
    // }

    if (alpha < currentEval) {
        alpha = currentEval;
    }

    auto prunedCaptures = mMoveGen->generateLegalCaptures(mBoard->getTurn());
    mMoveOrdering->orderMovesQuiescent(prunedCaptures.begin(), prunedCaptures.end(), currentEval, alpha);

    for (auto capture : prunedCaptures) {
        if (depthleft != 0) {
            mBoard->makeMove(capture);
            int16_t score = -qsearch(alpha, beta, depthleft - 1);
            mBoard->undoMove();

            if (score >= beta) {
                return beta;
            }
            if (score > alpha) {
                alpha = score;
            }
        }
    }
    return alpha;
}